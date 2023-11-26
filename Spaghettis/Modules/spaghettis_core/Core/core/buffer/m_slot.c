
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int buffer_messagesIsLastProperlyEnded (t_buffer *x)
{
    int size = buffer_getSize (x);
    
    if (size) { t_atom *a = buffer_getAtomAtIndex (x, size - 1); return IS_SEMICOLON_OR_COMMA (a); }
    else {
        return 1;
    }
}

int buffer_messagesGetNumberOf (t_buffer *x)
{
    int i, count = 0;

    for (i = 0; i < buffer_getSize (x); i++) {
        t_atom *a = buffer_getAtomAtIndex (x, i); if (IS_SEMICOLON_OR_COMMA (a)) { count++; }
    }
    
    if (!buffer_messagesIsLastProperlyEnded (x)) { count++; }
    
    return count;
}

t_error buffer_messagesGetAt (t_buffer *x, int n, int *start, int *end)
{
    *start = 0; *end = 0;
    
    if (n >= 0) {
    //
    int i, k = 0;
    
    for (i = 0; i < buffer_getSize (x); i++) {
    //
    if (k != n) {
        if (IS_SEMICOLON_OR_COMMA (buffer_getAtomAtIndex (x, i))) { k++; }
        
    } else {
        int j = i;
        while (j < x->b_size && !IS_SEMICOLON_OR_COMMA (buffer_getAtomAtIndex (x, j))) { j++; }
        *start = i;
        *end   = j;
        return PD_ERROR_NONE;
    }
    //
    }
    //
    }
    
    return PD_ERROR;
}

t_error buffer_messagesGetAtWithTypeOfEnd (t_buffer *x,
    int n,
    int *start,
    int *end,
    t_atomtype *type)
{
    t_error err = buffer_messagesGetAt (x, n, start, end);
    
    if (!err) {
    //
    if (buffer_getAtomAtIndexChecked (x, *end)) { *type = atom_getType (buffer_getAtomAtIndex (x, *end)); }
    else {
        *type = A_NULL;
    }
    //
    }
    
    return err;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
