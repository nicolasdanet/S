
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* A root has no parent and an environment. */
/* An abstraction has a parent and an environment. */
/* A subpatch has a parent also but no environment. */
/* A top patch is either a root or an abstraction. */
/* Dirty and frozen bits are always owned by the top patch. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PD_LOCAL t_glist *glist_getRoot (t_glist *glist)
{
    if (glist_isRoot (glist)) { return glist; } else { return glist_getRoot (glist_getParent (glist)); }
}

PD_LOCAL t_glist *glist_getTop (t_glist *glist)
{
    if (glist_isTop (glist)) { return glist; } else { return glist_getTop (glist_getParent (glist)); }
}

PD_LOCAL t_glist *glist_getParent (t_glist *g)
{
    return g->gl_parent;
}

PD_LOCAL t_glist *glist_getNext (t_glist *g)
{
    return g->gl_next;
}

PD_FORCE t_symbol *glist_getName (t_glist *g)
{
    return g->gl_name;
}

PD_LOCAL t_undomanager *glist_getUndoManager (t_glist *glist)
{
    return glist->gl_undomanager;
}

PD_LOCAL t_symbol *glist_getUnexpandedName (t_glist *glist)
{
    return object_getUnexpandedNameAt (cast_object (glist), 1);
}

PD_LOCAL t_environment *glist_getEnvironment (t_glist *glist)
{
    return (glist_getTop (glist)->gl_environment);
}

PD_LOCAL t_abstractions *glist_getAbstractions (t_glist *glist)
{
    return glist_getRoot (glist)->gl_abstractions;
}

PD_LOCAL t_rectangle *glist_getEditView (t_glist *glist)
{
    return &glist->gl_editView;
}

PD_LOCAL t_rectangle *glist_getRunView (t_glist *glist)
{
    return &glist_getRoot (glist)->gl_runView;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void glist_setName (t_glist *glist, t_symbol *name)
{
    if (name == &s_) { name = sym_Untitled; }
    
    if (name != glist->gl_name) {
    //
    glist->gl_name = name;
    
    if (glist_isTop (glist)) { environment_setFileName (glist_getEnvironment (glist), name); }
    //
    }
}

PD_LOCAL void glist_setDirty (t_glist *glist, int n)
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

PD_LOCAL void glist_setFrozen (t_glist *glist, int n)
{
    glist_getTop (glist)->gl_isFrozen = (n != 0);
}

PD_LOCAL void glist_setEditView (t_glist *glist, t_rectangle *r, int notify)
{
    rectangle_setCopy (glist_getEditView (glist), r);
    
    #if defined ( PD_BUILDING_APPLICATION )
    
    if (notify) { outputs_objectUpdated (cast_object (glist), glist_getParent (glist), Tags::attributes()); }
    
    #endif
}

PD_LOCAL void glist_setRunView (t_glist *glist, t_rectangle *r, int notify)
{
    rectangle_setCopy (glist_getRunView (glist), r);
    
    #if defined ( PD_BUILDING_APPLICATION )
    
    if (notify) { outputs_objectUpdated (cast_object (glist_getRoot (glist)), NULL, Tags::attributes()); }
    
    #endif
}

PD_LOCAL void glist_setNext (t_glist *g, t_glist *next)
{
    g->gl_next = next;
}

PD_LOCAL void glist_setUniqueAndSource (t_glist *glist, int argc, t_atom *argv)
{
    t_id u; t_error err = utils_uniqueWithAtoms (argc, argv, &u);
    
    PD_ASSERT (!err); PD_UNUSED (err);
    
    object_changeUnique (cast_object (glist), u);
    object_changeSource (cast_object (glist), u);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL int glist_isRoot (t_glist *glist)
{
    return (!glist_hasParent (glist));
}

PD_LOCAL int glist_isTop (t_glist *glist)
{
    int k = (glist_isRoot (glist) || glist_isAbstraction (glist));
    
    return k;
}

PD_LOCAL int glist_isAbstraction (t_glist *glist)
{
    return (glist_hasParent (glist) && (glist->gl_environment != NULL));
}

PD_LOCAL int glist_isSubpatch (t_glist *glist)
{
    return (!glist_isTop (glist));
}

#if 0

PD_LOCAL int glist_isDirty (t_glist *glist)
{
    return (glist_getTop (glist)->gl_isDirty != 0);
}

#endif

PD_LOCAL int glist_isAbstractionOrInside (t_glist *glist)
{
    return (glist_isAbstraction (glist_getTop (glist)));
}

PD_LOCAL int glist_isFrozen (t_glist *glist)
{
    return (glist_getTop (glist)->gl_isFrozen != 0);
}

PD_LOCAL int glist_isLoading (t_glist *g)
{
    return g->gl_isLoading;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL int glist_hasParent (t_glist *g)
{
    return (g->gl_parent != NULL);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void glist_loadBegin (t_glist *g)
{
    g->gl_isLoading = 1;
}

PD_LOCAL void glist_loadEnd (t_glist *g)
{
    g->gl_isLoading = 0;
}

PD_LOCAL void glist_undoEnable (t_glist *g)
{
    g->gl_undoEnabled = 1;
}

PD_LOCAL void glist_undoDisable (t_glist *g)
{
    g->gl_undoEnabled = 0;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
