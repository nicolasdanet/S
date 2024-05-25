
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define UNDOMANAGER_DELAY   227.0

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void   undoaction_releaseAllFrom       (t_undoaction *, t_undomanager *);
void   undomanager_collapse            (t_undomanager *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

int    undomanager_undoNeedToSuspend   (t_undomanager *);
int    undomanager_redoNeedToSuspend   (t_undomanager *);

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

t_undoaction *undomanager_getUndoAction (t_undomanager *x)
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

t_undoaction *undomanager_getRedoAction (t_undomanager *x)
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

#if defined ( PD_BUILDING_APPLICATION )

static juce::String undomanager_getLabel (t_undoaction *a)
{
    t_symbol *s = undoaction_getLabel (a);
    
    jassert (s);
    
    return makeString (symbol_getName (s));
}

juce::String undomanager_getUndoLabel (t_undomanager *x)
{
    t_undoaction *a = undomanager_getUndoAction (x);
    
    if (a) { return undomanager_getLabel (a); }
    
    return juce::String();
}

juce::String undomanager_getRedoLabel (t_undomanager *x)
{
    t_undoaction *a = undomanager_getRedoAction (x);
    
    if (a) { return undomanager_getLabel (a); }
    
    return juce::String();
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void undomanager_notify (t_undomanager *x)
{
    #if defined ( PD_BUILDING_APPLICATION )
    
    t_glist *g = x->um_owner;
    
    outputs_objectChanged (cast_object (g), Tags::attributes ( { Tag::Undo, Tag::Redo } ));
    
    #endif
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int undomanager_hasSeparatorAtLast (t_undomanager *x)
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

void undomanager_appendSeparator (t_undomanager *x)
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

void undomanager_append (t_undomanager *x, t_undoaction *a)
{
    undomanager_appendAction (x, a); undomanager_appendSeparatorLater (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Notice that DSP is suspended globally only to avoid unnecessary consecutive rebuilds. */

void undomanager_undo (t_undomanager *x)
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

void undomanager_redo (t_undomanager *x)
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

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_undomanager *undomanager_new (t_glist *owner)
{
    t_undomanager *x = (t_undomanager *)PD_MEMORY_GET (sizeof (t_undomanager));
    
    x->um_owner = owner;
    x->um_clock = clock_newSingle ((void *)x, (t_method)undomanager_task);

    undomanager_appendProceed (x, undoseparator_new());
    
    return x;
}

void undomanager_free (t_undomanager *x)
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
