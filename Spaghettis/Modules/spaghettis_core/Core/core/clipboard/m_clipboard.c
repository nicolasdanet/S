
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if defined ( PD_BUILDING_APPLICATION )

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_buffer *clipboard_bufferCopyPaste;     /* Static. */
static t_buffer *clipboard_bufferDuplicate;     /* Static. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PD_LOCAL t_rectangle glist_getBoundingBoxOfSelected (t_glist *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PD_LOCAL void snippet_renameArrays            (t_buffer *x, t_glist *glist);
PD_LOCAL void snippet_addOffsetToLines        (t_buffer *x, int i);
PD_LOCAL void snippet_substractOffsetToLines  (t_buffer *x, int i);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_buffer *clipboard_copyProceed (t_glist *glist, int copyAll, int isEncapsulate)
{
    t_buffer *b = buffer_new();

    t_object *y = NULL;
    t_traverser t;
    int flags = isEncapsulate ? SAVE_ENCAPSULATE : SAVE_COPY;
    
    for (y = glist->gl_graphics; y; y = y->g_next) {
    //
    if (copyAll || glist_objectIsSelected (glist, y)) { object_save (y, b, flags); }
    //
    }
    
    traverser_start (&t, glist);
    
    while (traverser_next (&t)) {
    //
    t_object *o = traverser_getSource (&t);
    t_object *d = traverser_getDestination (&t);
    int m = copyAll || glist_objectIsSelected (glist, o);
    int n = copyAll || glist_objectIsSelected (glist, d);
    
    if (m && n) {
    //
    int i = copyAll ? glist_objectGetIndexOf (glist, o) : glist_objectGetIndexOfAmongSelected (glist, o);
    int j = copyAll ? glist_objectGetIndexOf (glist, d) : glist_objectGetIndexOfAmongSelected (glist, d);
    
    buffer_appendSymbol (b, sym___hash__X);
    buffer_appendSymbol (b, sym_connect);
    buffer_appendFloat (b,  i);
    buffer_appendFloat (b,  traverser_getIndexOfOutlet (&t));
    buffer_appendFloat (b,  j);
    buffer_appendFloat (b,  traverser_getIndexOfInlet (&t));
    buffer_appendSemicolon (b);
    //
    }
    //
    }
    
    return b;
}

static void clipboard_copyRaw (t_glist *glist, int isDuplicate)
{
    if (glist_objectGetNumberOfSelected (glist)) {
    //
    t_buffer *b = clipboard_copyProceed (glist, 0, 0);
    
    if (isDuplicate) { buffer_free (clipboard_bufferDuplicate); clipboard_bufferDuplicate = b; }
    else {
        buffer_free (clipboard_bufferCopyPaste); clipboard_bufferCopyPaste = b;
    }
    //
    }
}

PD_LOCAL void clipboard_copyDuplicate (t_glist *glist)
{
    clipboard_copyRaw (glist, 1);
}

PD_LOCAL void clipboard_copy (t_glist *glist)
{
    clipboard_copyRaw (glist, 0);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void clipboard_pasteProceedDisplace (t_glist *glist, t_point *pt, int alreadyThere)
{
    t_object *y = NULL;
    int i = 0;

    t_rectangle r; rectangle_setNothing (&r);
    
    for (y = glist->gl_graphics; y; y = y->g_next) {
        if (i >= alreadyThere) {
            t_point t = object_getPoint (y); rectangle_addPoint (&r, &t);
        }
        i++;
    }
    
    if (!rectangle_isNothing (&r)) {
    //
    int a = point_getX (pt);
    int b = point_getY (pt);
    int m = rectangle_getTopLeftX (&r);
    int n = rectangle_getTopLeftY (&r);
    int deltaX = a - m;
    int deltaY = b - n;

    i = 0;
    
    for (y = glist->gl_graphics; y; y = y->g_next) {
        if (i >= alreadyThere) {
            unique_objectDisplace (object_getUnique (y), deltaX, deltaY);
        }
        i++;
    }
    //
    }
}

static void clipboard_pasteProceedLoadbang (t_glist *glist, int alreadyThere)
{
    t_object *y = NULL;
    int i = 0;

    for (y = glist->gl_graphics; y; y = y->g_next) {
        if (i >= alreadyThere) {
            if (object_isCanvas (y)) { glist_loadbang (cast_glist (y)); }
        }
        i++;
    }
}

int clipboard_pasteProceedSelect (t_glist *glist, int alreadyThere)
{
    t_object *y = NULL;
    int i = 0;
    int isDirty = 0;

    for (y = glist->gl_graphics; y; y = y->g_next) {
        if (i >= alreadyThere) {
            glist_objectSelect (glist, y); isDirty = 1;
        }
        i++;
    }
    
    return isDirty;
}

PD_LOCAL int clipboard_pasteProceed (t_glist *glist, t_buffer *b, t_point *pt, int renameArrays)
{
    int alreadyThere = glist_objectGetNumberOf (glist);

    glist_objectDeselectAll (glist);
    
    snippet_addOffsetToLines (b, alreadyThere);
    
    if (renameArrays) { snippet_renameArrays (b, glist); }
    
        instance_loadSnippet (glist, b);
    
    snippet_substractOffsetToLines (b, alreadyThere);
    
    clipboard_pasteProceedDisplace (glist, pt, alreadyThere);
    clipboard_pasteProceedLoadbang (glist, alreadyThere);
    
    return clipboard_pasteProceedSelect (glist, alreadyThere);
}

/*
t_point clipboard_pasteRawGetPoint (t_glist *glist)
{
    int n         = snap_getStep() * 2;
    t_rectangle r = glist_objectGetBoundingBoxOfSelected (glist);
    int nothing   = rectangle_isNothing (&r);
    
    t_point pt;
    
    if (!nothing) { point_set (&pt, rectangle_getTopLeftX (&r) + n, rectangle_getTopLeftY (&r) + n); }
    
    if (nothing || !rectangle_containsPoint (glist_getPatchGeometry (glist), &pt)) {

        point_set (&pt, instance_getDefaultX (glist), instance_getDefaultY (glist));
    }
    
    return pt;
}
*/

static t_point clipboard_pasteRawGetPoint (t_glist *glist)
{
    int n         = instance_snapGetGrid() * 2;
    t_rectangle r = glist_getBoundingBoxOfSelected (glist);
    int nothing   = rectangle_isNothing (&r);
    t_point pt    = point_make (0, 0);
    
    if (!nothing) { point_set (&pt, rectangle_getTopLeftX (&r) + n, rectangle_getTopLeftY (&r) + n); }
    else {
        DBG ("???");
    }
    
    return pt;
}

static void clipboard_pasteRaw (t_glist *glist, int isDuplicate)
{
    t_buffer *b = isDuplicate ? clipboard_bufferDuplicate : clipboard_bufferCopyPaste;
    
    if (buffer_getSize (b)) {
    //
    int isDirty  = 0;
    int undoable = glist_undoIsOk (glist);
    int state    = dsp_suspend();
    t_point pt   = clipboard_pasteRawGetPoint (glist);
    
    if (undoable) { glist_undoAppend (glist, isDuplicate ? undoduplicate_new() : undopaste_new()); }
    
    isDirty = clipboard_pasteProceed (glist, b, &pt, 1);
    
    dsp_resume (state);
    
    if (isDirty) { glist_setDirty (glist, 1); }
    if (isDirty && undoable) { glist_undoAppendSeparator (glist); }
    //
    }
    
    if (isDuplicate) { buffer_clear (clipboard_bufferDuplicate); }
}

PD_LOCAL void clipboard_pasteDuplicate (t_glist *glist)
{
    clipboard_pasteRaw (glist, 1);
}

PD_LOCAL void clipboard_paste (t_glist *glist)
{
    clipboard_pasteRaw (glist, 0);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void clipboard_initialize (void)
{
    clipboard_bufferCopyPaste = buffer_new();
    clipboard_bufferDuplicate = buffer_new();
}

PD_LOCAL void clipboard_release (void)
{
    if (clipboard_bufferCopyPaste) { buffer_free (clipboard_bufferCopyPaste); }
    if (clipboard_bufferDuplicate) { buffer_free (clipboard_bufferDuplicate); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#else

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PD_LOCAL void clipboard_initialize (void)
{
}

PD_LOCAL void clipboard_release (void)
{
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
