
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../m_spaghettis.h"
#include "../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define UNDOMANAGER_DELAY   227.0

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void   undoaction_releaseAllFrom       (t_undoaction *, t_undomanager *);
PD_LOCAL void   undomanager_collapse            (t_undomanager *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if defined ( PD_BUILDING_APPLICATION )

PD_LOCAL int    undomanager_undoNeedToSuspend   (t_undomanager *);
PD_LOCAL int    undomanager_redoNeedToSuspend   (t_undomanager *);

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void undomanager_task (t_undomanager *x)
{
    undomanager_appendSeparator (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_APPLICATION )

PD_LOCAL t_undoaction *undomanager_getUndoAction (t_undomanager *x)
{
    t_undoaction *a = x->um_tail;
    
    while (a && a->ua_previous) {
    //
    if (undoaction_getType (a->ua_previous) == UNDO_SEPARATOR) { return a; }
    
    a = a->ua_previous;
    //
    }
    
    return NULL;
}

PD_LOCAL t_undoaction *undomanager_getRedoAction (t_undomanager *x)
{
    t_undoaction *a = x->um_tail;
    
    while (a) {
    //
    if (undoaction_getType (a) != UNDO_SEPARATOR) { return a; }
    
    a = a->ua_next;
    //
    }
    
    return NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_symbol *undomanager_getUndoLabel (t_undomanager *x)
{
    t_undoaction *a = undomanager_getUndoAction (x);
    
    if (a) { return undoaction_getLabel (a); }
    
    return NULL;
}

static t_symbol *undomanager_getRedoLabel (t_undomanager *x)
{
    t_undoaction *a = undomanager_getRedoAction (x);
    
    if (a) { return undoaction_getLabel (a); }
    
    return NULL;
}

static void undomanager_notify (t_undomanager *x)
{
    t_symbol *undoLabel = undomanager_getUndoLabel (x);
    t_symbol *redoLabel = undomanager_getRedoLabel (x);
    
    if (undoLabel) { DBG (juce::String ("UNDO: ") + symbol_getName (undoLabel)); }
    if (redoLabel) { DBG (juce::String ("REDO: ") + symbol_getName (redoLabel)); }
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL int undomanager_hasSeparatorAtLast (t_undomanager *x)
{
    return (x->um_tail && (undoaction_getType (x->um_tail) == UNDO_SEPARATOR));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void undomanager_appendProceed (t_undomanager *x, t_undoaction *a)
{
    t_undoaction *t = x->um_tail;
    
    a->ua_next      = NULL;
    a->ua_previous  = NULL;
    
    if (t == NULL) { x->um_head = x->um_tail = a; }
    else {
    //
    undoaction_releaseAllFrom (t->ua_next, x);
        
    x->um_tail = a; t->ua_next = a; a->ua_previous = t;
    //
    }
    
    x->um_count++;
}

static void undomanager_appendAction (t_undomanager *x, t_undoaction *a)
{
    clock_unset (x->um_clock);
    
    if (!instance_undoIsRecursive()) { undomanager_appendProceed (x, a); }
    else {
        undoaction_releaseAllFrom (a, NULL);
    }
}

/* Filter consecutive separators. */

static void undomanager_appendSeparatorLater (t_undomanager *x)
{
    clock_delay (x->um_clock, UNDOMANAGER_DELAY);
}

PD_LOCAL void undomanager_appendSeparator (t_undomanager *x)
{
    if (!undomanager_hasSeparatorAtLast (x)) {
    //
    undomanager_appendAction (x, undoseparator_new()); undomanager_collapse (x); undomanager_notify (x);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void undomanager_append (t_undomanager *x, t_undoaction *a)
{
    undomanager_appendAction (x, a); undomanager_appendSeparatorLater (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_APPLICATION )

/* Notice that DSP is suspended globally only to avoid unnecessary consecutive rebuilds. */

PD_LOCAL void undomanager_undo (t_undomanager *x)
{
    clock_unset (x->um_clock);
    
    if (!instance_undoIsRecursive()) {
    //
    int dspState, dspSuspended = undomanager_undoNeedToSuspend (x);
    
    if (dspSuspended) { dspState = dsp_suspend(); }
    
    instance_undoSetRecursive();

    {
    //
    instance_pendingBegin();
    
    t_undoaction *a = x->um_tail;
    
    while (a && a->ua_previous) {
    
        if (class_hasMethod (pd_class (a), sym_undo)) { pd_message (cast_pd (a), sym_undo, 0, NULL); }
        
        a = a->ua_previous;
        
        if (undoaction_getType (a) == UNDO_SEPARATOR) { break; }
    }
    
    x->um_tail = a;
    
    instance_pendingEnd();
    //
    }
    
    instance_undoUnsetRecursive();
    
    if (dspSuspended) { dsp_resume (dspState); }
    
    undomanager_notify (x);
    //
    }
}

PD_LOCAL void undomanager_redo (t_undomanager *x)
{
    clock_unset (x->um_clock);
    
    if (!instance_undoIsRecursive()) {
    //
    int dspState, dspSuspended = undomanager_redoNeedToSuspend (x);
    
    if (dspSuspended) { dspState = dsp_suspend(); }
    
    instance_undoSetRecursive();

    {
    //
    instance_pendingBegin();
    
    t_undoaction *a = x->um_tail;
    
    while (a) {
    
        if (class_hasMethod (pd_class (a), sym_redo)) { pd_message (cast_pd (a), sym_redo, 0, NULL); }
        
        if (a->ua_next == NULL) { break; } else { a = a->ua_next; }
        
        if (undoaction_getType (a) == UNDO_SEPARATOR) { break; }
    }
    
    x->um_tail = a;
    
    instance_pendingEnd();
    //
    }
    
    instance_undoUnsetRecursive();
    
    if (dspSuspended) { dsp_resume (dspState); }
    
    undomanager_notify (x);
    //
    }
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_undomanager *undomanager_new (t_glist *owner)
{
    t_undomanager *x = (t_undomanager *)PD_MEMORY_GET (sizeof (t_undomanager));
    
    x->um_clock = clock_new ((void *)x, (t_method)undomanager_task);

    undomanager_appendProceed (x, undoseparator_new());
    
    return x;
}

PD_LOCAL void undomanager_free (t_undomanager *x)
{
    if (x) {
    //
    clock_unset (x->um_clock);
    undoaction_releaseAllFrom (x->um_head, x);
    PD_ASSERT (x->um_count == 0);
    clock_free (x->um_clock);
    PD_MEMORY_FREE (x);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
