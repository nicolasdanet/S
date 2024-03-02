
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* A root has no parent and an environment. */
/* An abstraction has a parent and an environment. */
/* A subpatch has a parent also but no environment. */
/* A top patch is either a root or an abstraction. */
/* Dirty and frozen bits are always owned by the top patch. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

t_glist *glist_getRoot (t_glist *glist)
{
    if (glist_isRoot (glist)) { return glist; } else { return glist_getRoot (glist_getParent (glist)); }
}

t_glist *glist_getTop (t_glist *glist)
{
    if (glist_isTop (glist)) { return glist; } else { return glist_getTop (glist_getParent (glist)); }
}

t_glist *glist_getParent (t_glist *g)
{
    return object_getOwner (cast_object (g));
}

t_glist *glist_getNext (t_glist *g)
{
    return g->gl_next;
}

t_symbol *glist_getName (t_glist *g)
{
    return g->gl_name;
}

t_undomanager *glist_getUndoManager (t_glist *glist)
{
    return glist->gl_undomanager;
}

t_symbol *glist_getUnexpandedName (t_glist *glist)
{
    return object_getUnexpandedNameAt (cast_object (glist), 1);
}

t_environment *glist_getEnvironment (t_glist *glist)
{
    return (glist_getTop (glist)->gl_environment);
}

t_abstractions *glist_getAbstractions (t_glist *glist)
{
    return glist_getRoot (glist)->gl_abstractions;
}

t_rectangle *glist_getEditView (t_glist *glist)
{
    return &glist->gl_editView;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int glist_getOffsetX (t_glist *g)
{
    return g->gl_offsetX;
}

int glist_getOffsetY (t_glist *g)
{
    return g->gl_offsetY;
}

int glist_getZoom (t_glist *g)
{
    return g->gl_zoom;
}

int glist_getInspectorWidth (t_glist *g)
{
    return g->gl_inspectorWidth;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void glist_setName (t_glist *glist, t_symbol *name)
{
    if (name == &s_) { name = sym_Untitled; }
    
    if (name != glist->gl_name) {
    //
    glist->gl_name = name;
    
    if (glist_isTop (glist)) { environment_setFileName (glist_getEnvironment (glist), name); }
    //
    }
}

void glist_setDirty (t_glist *glist, int n)
{
    int isDirty       = (n != 0);
    t_glist *y        = glist_getTop (glist);
    int isAbstraction = glist_isAbstraction (y);
    
    if (!isAbstraction) {
    //
    if (y->gl_isDirty != isDirty) {
    //
    y->gl_isDirty = isDirty;
    
    #if defined ( PD_BUILDING_APPLICATION )
    
    outputs_patchDirty (y, isDirty);
    
    #endif
    //
    }
    //
    }
}

void glist_setFrozen (t_glist *glist, int n)
{
    glist_getTop (glist)->gl_isFrozen = (n != 0);
}

void glist_setEditView (t_glist *glist, t_rectangle *r, int notify)
{
    rectangle_setCopy (glist_getEditView (glist), r);
    
    #if defined ( PD_BUILDING_APPLICATION )
    
    if (notify) {
    //
    outputs_objectChanged (cast_object (glist), Tags::attributes (Tag::EditView));
    //
    }
    
    #endif
}

void glist_setNext (t_glist *g, t_glist *next)
{
    g->gl_next = next;
}

void glist_setUniqueAndSource (t_glist *glist, int argc, t_atom *argv)
{
    t_id u; t_error err = utils_uniqueWithAtoms (argc, argv, &u);
    
    PD_ASSERT (!err); PD_UNUSED (err);
    
    object_changeUnique (cast_object (glist), u);
    object_changeSource (cast_object (glist), u);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int glist_isRoot (t_glist *glist)
{
    return (!glist_hasParent (glist));
}

int glist_isTop (t_glist *glist)
{
    int k = (glist_isRoot (glist) || glist_isAbstraction (glist));
    
    return k;
}

int glist_isAbstraction (t_glist *glist)
{
    return (glist_hasParent (glist) && (glist->gl_environment != NULL));
}

int glist_isSubpatch (t_glist *glist)
{
    return (!glist_isTop (glist));
}

#if 0

int glist_isDirty (t_glist *glist)
{
    return (glist_getTop (glist)->gl_isDirty != 0);
}

#endif

int glist_isAbstractionOrInside (t_glist *glist)
{
    return (glist_isAbstraction (glist_getTop (glist)));
}

int glist_isFrozen (t_glist *glist)
{
    return (glist_getTop (glist)->gl_isFrozen != 0);
}

int glist_isLoading (t_glist *g)
{
    return g->gl_isLoading;
}

int glist_isPasting (t_glist *g)
{
    return g->gl_isPasting;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int glist_hasParent (t_glist *g)
{
    return (glist_getParent (g) != NULL);
}

int glist_hasInspector (t_glist *g)
{
    return g->gl_hasInspector;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void glist_loadBegin (t_glist *g)
{
    g->gl_isLoading = 1;
}

void glist_loadEnd (t_glist *g)
{
    g->gl_isLoading = 0;
}

void glist_pasteBegin (t_glist *g)
{
    g->gl_isPasting = 1;
}

void glist_pasteEnd (t_glist *g)
{
    g->gl_isPasting = 0;
}

void glist_undoEnable (t_glist *g)
{
    g->gl_undoEnabled = 1;
}

void glist_undoDisable (t_glist *g)
{
    g->gl_undoEnabled = 0;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
