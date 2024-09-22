
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_buffer *clipboard_bufferCopyPaste;     /* Static. */
static t_buffer *clipboard_bufferDuplicate;     /* Static. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void glist_pasteBegin  (t_glist *);
void glist_pasteEnd    (t_glist *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

t_rectangle glist_getBoundingBoxOfSelected (t_glist *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void snippet_renameArrays            (t_buffer *x, t_glist *glist);
void snippet_addOffsetToLines        (t_buffer *x, int i);
void snippet_substractOffsetToLines  (t_buffer *x, int i);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void clipboard_copyProceedObjects (t_glist *glist, t_buffer *b, int copyAll, int isEncapsulate)
{
    int i, n = glist_graphicsGetSize (glist);
    
    for (i = 0; i < n; i++) {
    //
    t_object *y = glist_graphicsGetObjectAt (glist, i);
    
    int flags = isEncapsulate ? SAVE_ENCAPSULATE : SAVE_COPY;

    if (copyAll || glist_objectIsSelected (glist, y)) { object_save (y, b, flags); }
    //
    }
}

static void clipboard_copyProceedLines (t_glist *glist, t_buffer *b, int copyAll, int isEncapsulate)
{
    t_traverser t;
    
    traverser_start (&t, glist);
    
    while (traverser_next (&t)) {
    //
    t_object *o = traverser_getSource (&t);
    t_object *d = traverser_getDestination (&t);
    int m = copyAll || glist_objectIsSelected (glist, o);
    int n = copyAll || glist_objectIsSelected (glist, d);
    
    if (m && n) {
    //
    int i = copyAll ? traverser_getIndexOfSource (&t)      : glist_graphicsGetIndexOfAmongSelected (glist, o);
    int j = copyAll ? traverser_getIndexOfDestination (&t) : glist_graphicsGetIndexOfAmongSelected (glist, d);
    
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
}

t_buffer *clipboard_copyProceed (t_glist *glist, int copyAll, int isEncapsulate)
{
    t_buffer *b = buffer_new();

    clipboard_copyProceedObjects (glist, b, copyAll, isEncapsulate);
    clipboard_copyProceedLines (glist, b, copyAll, isEncapsulate);
    
    return b;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

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

void clipboard_copyDuplicate (t_glist *glist)
{
    clipboard_copyRaw (glist, 1);
}

void clipboard_copy (t_glist *glist)
{
    clipboard_copyRaw (glist, 0);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* While pasting, objects are invisibles at creation. */
/* Make them visible once moved at the right place. */

static void clipboard_pasteProceedShow (t_glist *glist, int alreadyThere)
{
    #if defined ( PD_BUILDING_APPLICATION )
    
    int i, n = glist_graphicsGetSize (glist);

    for (i = alreadyThere; i < n; i++) {
        outputs_objectChanged (glist_graphicsGetObjectAt (glist, i), Tags::attributes (Tag::Visible));
    }
    
    #endif
}

static void clipboard_pasteProceedDisplace (t_glist *glist, t_point *pt, int alreadyThere)
{
    int i, n = glist_graphicsGetSize (glist);

    t_rectangle r; rectangle_setNothing (&r);
    
    for (i = alreadyThere; i < n; i++) {
        t_point t = object_getPosition (glist_graphicsGetObjectAt (glist, i));
        rectangle_addPoint (&r, &t);
    }
    
    if (!rectangle_isNothing (&r)) {
    //
    int a = point_getX (pt);
    int b = point_getY (pt);
    int c = rectangle_getTopLeftX (&r);
    int d = rectangle_getTopLeftY (&r);
    int deltaX = a - c;
    int deltaY = b - d;

    for (i = alreadyThere; i < n; i++) {
        unique_objectDisplace (object_getUnique (glist_graphicsGetObjectAt (glist, i)), deltaX, deltaY);
    }
    //
    }
}

static void clipboard_pasteProceedLoadbang (t_glist *glist, int alreadyThere)
{
    int i, n = glist_graphicsGetSize (glist);

    for (i = alreadyThere; i < n; i++) {
    //
    t_object *y = glist_graphicsGetObjectAt (glist, i);
    
    if (object_isCanvas (y)) { glist_loadbang (cast_glist (y)); }
    //
    }
}

int clipboard_pasteProceedSelect (t_glist *glist, int alreadyThere)
{
    int i, n = glist_graphicsGetSize (glist);
        
    int isDirty = 0;

    for (i = alreadyThere; i < n; i++) {
        glist_objectSelect (glist, glist_graphicsGetObjectAt (glist, i)); isDirty = 1;
    }
    
    return isDirty;
}

int clipboard_pasteProceed (t_glist *glist, t_buffer *b, t_point *pt, int renameArrays)
{
    int alreadyThere = glist_objectGetNumberOf (glist);

    glist_objectDeselectAll (glist);
    
    snippet_addOffsetToLines (b, alreadyThere);
    
    if (renameArrays) { snippet_renameArrays (b, glist); }
    
    glist_pasteBegin (glist);
    
        instance_loadSnippet (glist, b);
    
    glist_pasteEnd (glist);
    
    clipboard_pasteProceedDisplace (glist, pt, alreadyThere);
    clipboard_pasteProceedShow (glist, alreadyThere);
    clipboard_pasteProceedLoadbang (glist, alreadyThere);
    
    snippet_substractOffsetToLines (b, alreadyThere);
    
    return clipboard_pasteProceedSelect (glist, alreadyThere);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_point clipboard_pasteRawGetPointNearSelection (t_glist *glist)
{
    t_rectangle r = glist_getBoundingBoxOfSelected (glist);
    
    if (!rectangle_isNothing (&r)) {
    //
    int offset = instance_snapGetGrid() * 2;
    int x      = rectangle_getTopLeftX (&r) + offset;
    int y      = rectangle_getTopLeftY (&r) + offset;
    
    return point_make (x, y);
    //
    }
    
    PD_BUG;
    
    return point_make (0, 0);
}

static void clipboard_pasteRaw (t_glist *glist, t_point *m)
{
    int isDuplicate = (m == NULL);
    t_buffer *b     = isDuplicate ? clipboard_bufferDuplicate : clipboard_bufferCopyPaste;
    
    if (buffer_getSize (b)) {
    //
    int isDirty  = 0;
    int undoable = glist_undoIsOk (glist);
    int state    = dsp_suspend();
    t_point pt   = isDuplicate ? clipboard_pasteRawGetPointNearSelection (glist) : *m;
    
    if (undoable) { glist_undoAppend (glist, isDuplicate ? undoduplicate_new() : undopaste_new()); }
    
    isDirty = clipboard_pasteProceed (glist, b, &pt, 1);
    
    dsp_resume (state);
    
    if (isDirty) { glist_setDirty (glist, 1); }
    if (isDirty && undoable) { glist_undoAppendSeparator (glist); }
    //
    }
    
    if (isDuplicate) { buffer_clear (clipboard_bufferDuplicate); }
}

void clipboard_pasteDuplicate (t_glist *glist)
{
    clipboard_pasteRaw (glist, NULL);
}

void clipboard_paste (t_glist *glist, t_point *m)
{
    clipboard_pasteRaw (glist, m);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void clipboard_initialize (void)
{
    clipboard_bufferCopyPaste = buffer_new();
    clipboard_bufferDuplicate = buffer_new();
}

void clipboard_release (void)
{
    if (clipboard_bufferCopyPaste) { buffer_free (clipboard_bufferCopyPaste); }
    if (clipboard_bufferDuplicate) { buffer_free (clipboard_bufferDuplicate); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
