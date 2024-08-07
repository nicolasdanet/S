
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static void glist_serializeView (t_glist *glist, t_buffer *b)
{
    buffer_appendSymbol (b, sym___hash__N);
    buffer_appendSymbol (b, sym_view);
    buffer_appendFloat (b,  glist_getOffsetX (glist));
    buffer_appendFloat (b,  glist_getOffsetY (glist));
    buffer_appendFloat (b,  glist_getZoom (glist));
    buffer_appendFloat (b,  glist_hasInspector (glist));
    buffer_appendFloat (b,  glist_getInspectorWidth (glist));
    
    buffer_appendSemicolon (b);
}

static void glist_serializeHeader (t_glist *glist, t_buffer *b)
{
    t_rectangle *t = glist_getEditView (glist);
    
    buffer_appendSymbol (b, sym___hash__N);
    buffer_appendSymbol (b, sym_canvas);
    buffer_appendFloat (b,  rectangle_getTopLeftX (t));
    buffer_appendFloat (b,  rectangle_getTopLeftY (t));
    buffer_appendFloat (b,  rectangle_getWidth (t));
    buffer_appendFloat (b,  rectangle_getHeight (t));
    
    if (glist_isSubpatch (glist)) {
    //
    t_symbol *s = glist_getUnexpandedName (glist); buffer_appendSymbol (b, (s != &s_ ? s : sym_Untitled));
    //
    }
    
    buffer_appendSemicolon (b);
}

static void glist_serializeObjects (t_glist *glist, t_buffer *b, int flags)
{
    int i, n = glist_graphicsGetSize (glist);
    
    for (i = 0; i < n; i++) { object_save (glist_graphicsGetObjectAt (glist, i), b, flags); }
}

static void glist_serializeLines (t_glist *glist, t_buffer *b)
{
    t_traverser t; traverser_start (&t, glist);
    
    while (traverser_next (&t)) {
    //
    buffer_appendSymbol (b, sym___hash__X);
    buffer_appendSymbol (b, sym_connect);
    buffer_appendFloat (b,  traverser_getIndexOfSource (&t));
    buffer_appendFloat (b,  traverser_getIndexOfOutlet (&t));
    buffer_appendFloat (b,  traverser_getIndexOfDestination (&t));
    buffer_appendFloat (b,  traverser_getIndexOfInlet (&t));
    buffer_appendSemicolon (b);
    //
    }
}

static void glist_serializeTag (t_glist *glist, t_buffer *b, int flags)
{
    if (flags & SAVE_UNDO) { object_serializeUnique (cast_object (glist), sym__tagcanvas, b); }
}

static void glist_serializeDollarZero (t_glist *glist, t_buffer *b)
{
    int n = environment_getDollarZero (glist_getEnvironment (glist));
    
    buffer_appendSymbol (b, sym___hash__X);
    buffer_appendSymbol (b, sym__tagdollarzero);
    buffer_appendFloat (b, n);
    buffer_appendSemicolon (b);
}

static void glist_serializeFooter (t_glist *glist, t_buffer *b)
{
    if (glist_isSubpatch (glist)) {
    //
    buffer_appendSymbol (b, sym___hash__X);
    buffer_appendSymbol (b, sym_restore);
    buffer_appendFloat (b, object_getX (cast_object (glist)));
    buffer_appendFloat (b, object_getY (cast_object (glist)));
    buffer_serialize (b, object_getBuffer (cast_object (glist)));
    buffer_appendSemicolon (b);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void legacy_version (t_buffer *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void glist_serialize (t_glist *glist, t_buffer *b, int flags, int isAbstraction)
{
    if (isAbstraction) {    /* Encapsulation. */
    //
    glist_serializeView (glist, b);
    glist_serializeHeader (glist, b);
    glist_serializeDollarZero (glist, b);
    glist_serializeObjects (glist, b, flags);
    glist_serializeLines (glist, b);
    //
    } else {
    //
    glist_serializeView (glist, b);
    glist_serializeHeader (glist, b);
    glist_serializeObjects (glist, b, flags);
    glist_serializeLines (glist, b);
    glist_serializeTag (glist, b, flags);
    glist_serializeFooter (glist, b);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void glist_saveProceed (t_glist *glist, t_symbol *name, t_symbol *directory)
{
    t_buffer *b = buffer_new();
    
    legacy_version (b); glist_serialize (glist, b, SAVE_DEFAULT, 0);
    
    if (buffer_fileWrite (b, name, directory)) { error_failsToWrite (cast_object (glist), name); }
    else {
        post_system (cast_object (glist),
            PD_TRANSLATE ("file: saved to %s/%s"),
            symbol_getName (directory),
            symbol_getName (name));
        environment_setDirectory (glist_getEnvironment (glist), directory);
        glist_setDirty (glist, 0);
    }
    
    buffer_free (b);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void glist_save (t_glist *glist)
{
    t_glist *root  = glist_getTop (glist);
    t_symbol *name = environment_getFileName (glist_getEnvironment (root));
    
    if (glist_isFrozen (glist)) { error_fileIsProtected (cast_object (glist), name); }
    else if (name != &s_) {
        glist_saveProceed (root, name, environment_getDirectory (glist_getEnvironment (root)));
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
