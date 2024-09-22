
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define UNDOMANAGER_LIMIT   65536   /* Arbitrary. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void undoresize_collapse       (t_undoaction *, t_undoaction *);
void undomotion_collapse       (t_undoaction *, t_undoaction *);
void undoaction_releaseAllFrom (t_undoaction *, t_undomanager *);
void undoaction_release        (t_undoaction *, t_undomanager *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Collapse consecutive resize actions. */

static void undomanager_collapseResizeProceed (t_undomanager *x, t_undoaction *a)
{
    t_undoaction *previous = a->ua_previous;
    
    while (previous) {
    //
    t_undoaction *t = previous;
    t_undotype type = undoaction_getType (t);
    
    previous = previous->ua_previous;
    
    if (type == UNDO_SEPARATOR) { break; }
    else if (type == UNDO_RESIZE) {
        if (undoaction_getUnique (t) == undoaction_getUnique (a)) {
            undoresize_collapse (a, t);
            undoaction_release (t, x);
        }
    }
    //
    }
}

/* Collapse consecutive motion actions. */

static void undomanager_collapseMotionProceed (t_undomanager *x, t_undoaction *a)
{
    t_undoaction *previous = a->ua_previous;
    
    while (previous) {
    //
    t_undoaction *t = previous;
    t_undotype type = undoaction_getType (t);
    
    previous = previous->ua_previous;
    
    if (type == UNDO_SEPARATOR) { break; }
    else if (type == UNDO_MOTION) {
        if (undoaction_getUnique (t) == undoaction_getUnique (a)) {
            undomotion_collapse (a, t);
            undoaction_release (t, x);
        }
    }
    //
    }
}

static void undomanager_collapseMotionAndResize (t_undomanager *x)
{
    t_undoaction *current = x->um_tail;
    
    while (current && current->ua_previous) {
    //
    t_undoaction *previous = current->ua_previous;
    t_undotype type = undoaction_getType (previous);
    
    if (type == UNDO_SEPARATOR)   { break; }
    else if (type == UNDO_MOTION) { undomanager_collapseMotionProceed (x, previous); }
    else if (type == UNDO_RESIZE) { undomanager_collapseResizeProceed (x, previous); }
    
    current = previous;
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Remove if creation is following by a deletion. */

static int undomanager_collapseCreationProceed (t_undomanager *x, t_undoaction *a)
{
    t_undoaction *previous = a->ua_previous;
    
    while (previous) {
    //
    t_undoaction *t = previous;
    t_undotype type = undoaction_getType (t);
    
    previous = previous->ua_previous;
    
    if (type == UNDO_SEPARATOR) { break; }
    else if (type == UNDO_CREATE) {
        if (undoaction_getUnique (t) == undoaction_getUnique (a)) {
            undoaction_release (t, x);
            undoaction_release (a, x);
            return 1;
        }
    }
    //
    }
    
    return 0;
}

static void undomanager_collapseCreation (t_undomanager *x)
{
    t_undoaction *current = x->um_tail;
    
    while (current && current->ua_previous) {
    //
    t_undoaction *previous = current->ua_previous;
    t_undotype type = undoaction_getType (previous);
    int removed = 0;
    
    if (type == UNDO_SEPARATOR) { break; }
    else if (type == UNDO_DELETE) {
        removed = undomanager_collapseCreationProceed (x, previous);
    }
    
    if (!removed) { current = previous; }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void undomanager_collapseSizeProceed (t_undomanager *x)
{
    t_undoaction *t = x->um_head;
    t_undoaction *a = NULL;
    
    while (t && t->ua_next) {
    //
    t_undotype type = undoaction_getType (t->ua_next);
    
    if (type == UNDO_SEPARATOR) { a = t->ua_next; break; }
    
    t = t->ua_next;
    //
    }
    
    if (t && a) {
    //
    t->ua_next = NULL; a->ua_previous = NULL; undoaction_releaseAllFrom (x->um_head, x); x->um_head = a;
    //
    } else {
        PD_BUG;
    }
}

/* Delete old actions if the stack exceeds an arbitrary limit. */

static void undomanager_collapseSize (t_undomanager *x)
{
    if (x->um_count > UNDOMANAGER_LIMIT) { undomanager_collapseSizeProceed (x); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void undomanager_collapsePost (t_undomanager *x)
{
    #if 0
    
    t_undoaction *t = x->um_tail;
    t_undoaction *a = NULL;
    
    while (t && t->ua_previous) {
    //
    t = t->ua_previous; if (undoaction_getType (t) == UNDO_SEPARATOR) { a = t; break; }
    //
    }
    
    if (a) {
    //
    a = a->ua_next;
    
    while (a) {
    //
    if (undoaction_getUnique (a)) {
        post_log ("UNDO / %s / %ld", symbol_getName (undoaction_getLabel (a)), undoaction_getUnique (a));
    } else {
        post_log ("UNDO / %s", symbol_getName (undoaction_getLabel (a)));
    }
    
    a = a->ua_next;
    //
    }
    //
    }
    
    #endif
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

// TODO: Collapse consecutive separators (e.g. after actions has been removed)?

// TODO: Remove superfluous actions (e.g. labels)?

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void undomanager_collapse (t_undomanager *x)
{
    PD_ASSERT (undomanager_hasSeparatorAtLast (x));
    
    undomanager_collapseMotionAndResize (x);
    undomanager_collapseCreation (x);
    undomanager_collapsePost (x);
    undomanager_collapseSize (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
