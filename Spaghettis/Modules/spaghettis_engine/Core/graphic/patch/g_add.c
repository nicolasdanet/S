
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void glist_undoDisable (t_glist *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void glist_objectAddUndoProceed (t_glist *glist, t_object *y)
{
    if (glist_undoIsOk (glist)) {
    //
    t_undosnippet *snippet = undosnippet_new (y);
    
    if (glist_undoHasSeparatorAtLast (glist)) { glist_undoAppend (glist, undoadd_new()); }
    
    glist_undoAppend (glist, undocreate_new (y, snippet));
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void glist_objectAdd (t_glist *glist, t_object *y)
{
    PD_ASSERT (object_getOwner (y) == glist);
    
    glist_graphicsAppend (glist, y);
    
    instance_registerAdd (y);
    
    glist_objectAddUndoProceed (glist, y);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void glist_objectRemoveProceed (t_glist *glist, t_object *y)
{
    instance_registerRemove (y);
    
    glist_graphicsRemove (glist, y);
}

static void glist_objectRemoveFree (t_glist *glist, t_object *y)
{
    if (object_hasDsp (y) && !object_isCanvas (y)) {
        if (garbage_newObject (y)) { return; }
    }
    
    if (instance_pendingRequired (y)) { instance_pendingAdd (y); return; }

    pd_free (cast_pd (y));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void glist_objectRemove (t_glist *glist, t_object *y)
{
    int needToRebuild = object_hasDsp (y);
    int undoable      = glist_undoIsOk (glist);
    int state         = 0;
    
    t_undosnippet *snippet = NULL;
    
    if (object_isCanvas (y)) { glist_closebang (cast_glist (y)); }
    
    if (needToRebuild) { state = dsp_suspend(); }
    
    {
        if (undoable && glist_undoHasSeparatorAtLast (glist)) { glist_undoAppend (glist, undoremove_new()); }
    
        {
            if (undoable) { snippet = undosnippet_new (y); }    /* MUST be before call below. */
            
            glist_objectDeleteLines (glist, y);
            
            if (object_isCanvas (y)) {      /* MUST be done after call above. */
            //
            glist_undoDisable (cast_glist (y)); glist_objectRemoveAll (cast_glist (y));
            //
            }
            
            if (undoable) { glist_undoAppend (glist, undodelete_new (y, snippet)); }
        }
    
        glist_objectRemoveProceed (glist, y);
        glist_objectRemoveFree (glist, y);
    }
    
    if (needToRebuild) { dsp_resume (state); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Inlets and outlets must be deleted from right to left to handle Undo/Redo properly. */

static void glist_objectRemoveCachedOutlets (t_glist *glist, t_object *y, int n)
{
    int i, size = buffer_getSize (glist->gl_tempIndexes);
    
    for (i = 0; i < size; i++) {
        if (buffer_getFloatAtIndex (glist->gl_tempIndexes, i) < n) {
            break;
        }
    }
    
    buffer_insertFloatAtIndex (glist->gl_tempIndexes, i, n);
    buffer_insertObjectAtIndex (glist->gl_tempOutlets, i, y);
}

/* Cache objects to avoid container corruption during the traversal. */

static void glist_objectRemoveCached (t_glist *glist, t_object *y)
{
    if (pd_class (y) == vinlet_class) {
        glist_objectRemoveCachedOutlets (glist, y, vinlet_getIndex ((t_vinlet *)y));
    } else if (pd_class (y) == voutlet_class) {
        glist_objectRemoveCachedOutlets (glist, y, voutlet_getIndex ((t_voutlet *)y));
    } else {
        buffer_appendAsObject (glist->gl_tempObjects, y);
    }
}

static void glist_objectRemoveRelease (t_glist *glist)
{
    int i;
    
    for (i = 0; i < buffer_getSize (glist->gl_tempObjects); i++) {
        glist_objectRemove (glist, buffer_getObjectAt (glist->gl_tempObjects, i));
    }
    
    for (i = 0; i < buffer_getSize (glist->gl_tempOutlets); i++) {
        glist_objectRemove (glist, buffer_getObjectAt (glist->gl_tempOutlets, i));
    }
    
    buffer_clear (glist->gl_tempIndexes);
    buffer_clear (glist->gl_tempOutlets);
    buffer_clear (glist->gl_tempObjects);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* If needed the DSP is suspended to avoid multiple rebuilds of DSP graph. */

void glist_objectRemoveAll (t_glist *glist)
{
    int dspState = 0;
    int dspSuspended = 0;

    int i, n = glist_graphicsGetSize (glist);
    
    for (i = 0; i < n; i++) {
    //
    t_object *y = glist_graphicsGetObjectAt (glist, i);
    
    if (!dspSuspended) {
        if (object_hasDsp (y)) { dspState = dsp_suspend(); dspSuspended = 1; }
    }

    glist_objectRemoveCached (glist, y);
    //
    }
    
    glist_objectRemoveRelease (glist);
    
    if (dspSuspended) { dsp_resume (dspState); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void glist_objectRemoveSelectedProceed (t_glist *glist)
{
    int dspState     = 0;
    int dspSuspended = 0;
    
    int i, n = glist_graphicsGetSize (glist);
    
    for (i = 0; i < n; i++) {
    //
    t_object *y = glist_graphicsGetObjectAt (glist, i);
    
    if (glist_objectIsSelected (glist, y)) {
        if (!dspSuspended) {
            if (object_hasDsp (y)) { dspState = dsp_suspend(); dspSuspended = 1; }
        }

        glist_objectRemoveCached (glist, y);
    }
    //
    }

    glist_objectRemoveRelease (glist);
    
    glist_setDirty (glist, 1);
    
    if (dspSuspended) { dsp_resume (dspState); }
}

void glist_objectRemoveSelected (t_glist *glist)
{
    glist_objectRemoveSelectedProceed (glist);
    
    if (glist_undoIsOk (glist)) { glist_undoAppendSeparator (glist); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void glist_removeInletsAndOutlets (t_glist *glist)
{
    /* Note that DSP is (should be) already suspended. */
    
    int i, n = glist_graphicsGetSize (glist);

    for (i = 0; i < n; i++) {
    //
    t_object *y = glist_graphicsGetObjectAt (glist, i);
    t_class *c  = pd_class (y);
    
    if (c == vinlet_class || c == voutlet_class) { glist_objectRemoveCached (glist, y); }
    //
    }
    
    glist_objectRemoveRelease (glist);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
