
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Machinery to reconnect inlets (in parent patch) when undoing their deletion for instance. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_undoaction *undomanager_getUndoAction (t_undomanager *);
t_undoaction *undomanager_getRedoAction (t_undomanager *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int undodisconnect_match           (t_undoaction *, t_id, t_items *, t_items *);
int undoaction_getInletsAndOutlets (t_undoaction *, t_items *, t_items *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static int undomanager_undoContainsDelete (t_undomanager *x, t_items *i, t_items *o)
{
    int k = 0;
    
    t_undoaction *a = x->um_tail;

    while (a && a->ua_previous) {
    
        if (undoaction_getType (a) == UNDO_DELETE) { k |= undoaction_getInletsAndOutlets (a, i, o); }
        
        a = a->ua_previous;
        
        if (undoaction_getType (a) == UNDO_SEPARATOR) { break; }
    }
    
    return k;
}

int undomanager_undoNeedToTriggerParent (t_undomanager *x, t_items *i, t_items *o)
{
    t_undoaction *a = undomanager_getUndoAction (x);
    
    if (a) {
    //
    t_undotype type = undoaction_getType (a);
        
    if (type == UNDO_ENCAPSULATE || type == UNDO_REMOVE) { return undomanager_undoContainsDelete (x, i, o); }
    //
    }
    
    return 0;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static int undomanager_redoContainsCreate (t_undomanager *x, t_items *i, t_items *o)
{
    int k = 0;
    
    t_undoaction *a = x->um_tail;
    
    while (a) {
    
        if (undoaction_getType (a) == UNDO_CREATE) { k |= undoaction_getInletsAndOutlets (a, i, o); }
        
        if (a->ua_next == NULL) { break; } else { a = a->ua_next; }
        
        if (undoaction_getType (a) == UNDO_SEPARATOR) { break; }
    }
    
    return k;
}

int undomanager_redoNeedToTriggerParent (t_undomanager *x, t_items *i, t_items *o)
{
    t_undoaction *a = undomanager_getRedoAction (x);
    
    if (a) {
    //
    t_undotype type = undoaction_getType (a);
    
    if (type == UNDO_ENCAPSULATE || type == UNDO_ADD) { return undomanager_redoContainsCreate (x, i, o); }
    //
    }
    
    return 0;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static int undomanager_undoContainsDisconnect (t_undomanager *x, t_glist *glist, t_items *i, t_items *o)
{
    t_id u = object_getUnique (cast_object (glist));
    
    t_undoaction *a = x->um_tail;

    while (a && a->ua_previous) {
    
        if (undodisconnect_match (a, u, i, o)) { } else if (undoaction_getType (a) != UNDO_SEPARATOR) {
            return 0;
        }
        
        a = a->ua_previous;
        
        if (undoaction_getType (a) == UNDO_SEPARATOR) { break; }
    }
    
    return 1;
}

int undomanager_triggerParentIsPossible (t_glist *glist, t_items *i, t_items *o)
{
    if (glist_hasParent (glist)) {
    //
    t_glist *parent = glist_getParent (glist);
    
    if (undomanager_undoContainsDisconnect (glist_getUndoManager (parent), glist, i, o)) {
        return 1;
    }
    //
    }
    
    return 0;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
