
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Check if undo or redo requires to rebuild the DSP graph. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int undomanager_undoNeedToSuspend (t_undomanager *x)
{
    t_undoaction *a = x->um_tail;

    while (a && a->ua_previous) {
    
        if (undoaction_suspend (a)) { return 1; }
        
        a = a->ua_previous;
        
        if (undoaction_getType (a) == UNDO_SEPARATOR) { break; }
    }
    
    return 0;
}

int undomanager_redoNeedToSuspend (t_undomanager *x)
{
    t_undoaction *a = x->um_tail;
    
    while (a) {
    
        if (undoaction_suspend (a)) { return 1; }
        
        if (a->ua_next == NULL) { break; } else { a = a->ua_next; }
        
        if (undoaction_getType (a) == UNDO_SEPARATOR) { break; }
    }
    
    return 0;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
