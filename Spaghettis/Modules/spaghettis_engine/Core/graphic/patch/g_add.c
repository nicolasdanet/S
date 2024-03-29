
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void   glist_undoDisable   (t_glist *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void glist_objectAddRaw (t_glist *glist, t_object *y, t_object *first, int prepend)
{
    y->g_next = NULL;
    
    if (first != NULL) { y->g_next = first->g_next; first->g_next = y; }
    else {
    //
    if (prepend || !glist->gl_graphics) {
        y->g_next = glist->gl_graphics; glist->gl_graphics = y;
    } else {
        t_object *t = NULL; for (t = glist->gl_graphics; t->g_next; t = t->g_next) { }
        t->g_next = y;
    }
    //
    }
}

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
    
    glist_objectAddRaw (glist, y, NULL, 0);
    
    instance_registerAdd (y);
    
    glist_objectAddUndoProceed (glist, y);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Inlets and outlets must be deleted from right to left to handle Undo/Redo properly. */

static void glist_objectRemoveWithCacheForInletsProceed (t_glist *glist, t_object *y, int isVinlet)
{
    int i, n = isVinlet ? vinlet_getIndex ((t_vinlet *)y) : voutlet_getIndex ((t_voutlet *)y);
    
    t_atom a;
    
    for (i = 0; i < buffer_getSize (glist->gl_sorterIndexes); i++) {
        if (buffer_getFloatAtIndex (glist->gl_sorterIndexes, i) < n) { break; }
    }
    
    SET_FLOAT (&a, n);  buffer_insertAtIndex (glist->gl_sorterIndexes, i, &a);
    SET_OBJECT (&a, y); buffer_insertAtIndex (glist->gl_sorterObjects, i, &a);
}

void glist_objectRemoveWithCacheForInlets (t_glist *glist, t_object *y)
{
    if (pd_class (y) == vinlet_class)       { glist_objectRemoveWithCacheForInletsProceed (glist, y, 1); }
    else if (pd_class (y) == voutlet_class) { glist_objectRemoveWithCacheForInletsProceed (glist, y, 0); }
    else {
        glist_objectRemove (glist, y);
    }
}

void glist_objectRemovePurgeCacheForInlets (t_glist *glist)
{
    int i;
    
    for (i = 0; i < buffer_getSize (glist->gl_sorterObjects); i++) {
        glist_objectRemove (glist, buffer_getObjectAt (glist->gl_sorterObjects, i));
    }
    
    buffer_clear (glist->gl_sorterIndexes);
    buffer_clear (glist->gl_sorterObjects);
}

void glist_objectRemoveRaw (t_glist *glist, t_object *y)
{
    if (glist->gl_graphics == y) { glist->gl_graphics = y->g_next; }
    else {
        t_object *t = NULL;
        for (t = glist->gl_graphics; t; t = t->g_next) {
            if (t->g_next == y) { t->g_next = y->g_next; break; }
        }
    }
}

static void glist_objectRemoveProceed (t_glist *glist, t_object *y)
{
    instance_registerRemove (y);
    
    glist_objectRemoveRaw (glist, y);
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

/* If needed the DSP is suspended to avoid multiple rebuilds of DSP graph. */

void glist_objectRemoveAll (t_glist *glist)
{
    t_object *t1 = NULL;
    t_object *t2 = NULL;
    
    int dspState = 0;
    int dspSuspended = 0;

    for (t1 = glist->gl_graphics; t1; t1 = t2) {
    //
    t2 = t1->g_next;
    
    if (!dspSuspended) {
        if (object_hasDsp (t1)) { dspState = dsp_suspend(); dspSuspended = 1; }
    }

    glist_objectRemoveWithCacheForInlets (glist, t1);
    //
    }
    
    glist_objectRemovePurgeCacheForInlets (glist);
    
    if (dspSuspended) { dsp_resume (dspState); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void glist_objectRemoveSelectedProceed (t_glist *glist)
{
    t_object *t1 = NULL;
    t_object *t2 = NULL;
    
    int dspState     = 0;
    int dspSuspended = 0;
    
    for (t1 = glist->gl_graphics; t1; t1 = t2) {
    //
    t2 = t1->g_next;
    
    if (glist_objectIsSelected (glist, t1)) {
        if (!dspSuspended) {
            if (object_hasDsp (t1)) { dspState = dsp_suspend(); dspSuspended = 1; }
        }

        glist_objectRemoveWithCacheForInlets (glist, t1);
    }
    //
    }

    glist_objectRemovePurgeCacheForInlets (glist);
    
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

int glist_objectGetNumberOf (t_glist *glist)
{
    return glist_objectGetIndexOf (glist, NULL);
}

int glist_objectGetNumberOfSelected (t_glist *glist)
{
    int n = 0; t_object *y = NULL;
    
    for (y = glist->gl_graphics; y; y = y->g_next) { n += glist_objectIsSelected (glist, y); }
    
    return n;
}

void glist_objectDeselectAll (t_glist *glist)
{
    t_object *y = NULL;
    
    for (y = glist->gl_graphics; y; y = y->g_next) {
        if (glist_objectIsSelected (glist, y)) { glist_objectDeselect (glist, y); }
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_APPLICATION )

std::vector<UniqueId> glist_objectGetAll (t_glist *glist)
{
    std::vector<UniqueId> v;
    
    t_object *y = NULL;
    
    for (y = glist->gl_graphics; y; y = y->g_next) { v.push_back (object_getUnique (y)); }
    
    return v;
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
