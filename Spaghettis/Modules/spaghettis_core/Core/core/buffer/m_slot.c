
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if PD_WITH_DEBUG

static int buffer_containsSemicolonOrComma (t_buffer *x)
{
    int i;
    
    for (i = 0; i < buffer_getSize (x); i++) {
        if (IS_SEMICOLON_OR_COMMA (buffer_getAtomAtIndex (x, i))) { return 1; }
    }
    
    return 0;
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL int buffer_messagesIsLastProperlyEnded (t_buffer *x)
{
    int size = buffer_getSize (x);
    
    if (size) { t_atom *a = buffer_getAtomAtIndex (x, size - 1); return IS_SEMICOLON_OR_COMMA (a); }
    else {
        return 1;
    }
}

PD_LOCAL int buffer_messagesGetNumberOf (t_buffer *x)
{
    int i, count = 0;

    for (i = 0; i < buffer_getSize (x); i++) {
        t_atom *a = buffer_getAtomAtIndex (x, i); if (IS_SEMICOLON_OR_COMMA (a)) { count++; }
    }
    
    if (!buffer_messagesIsLastProperlyEnded (x)) { count++; }
    
    return count;
}

PD_LOCAL t_error buffer_messagesGetAt (t_buffer *x, int n, int *start, int *end)
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

PD_LOCAL t_error buffer_messagesGetAtWithTypeOfEnd (t_buffer *x,
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
// MARK: -

/* Semicolon separated simple container for list of atoms. */

PD_LOCAL void buffer_slotsClear (t_buffer *x)
{
    buffer_clear (x);
}

PD_LOCAL int buffer_slotsSize (t_buffer *x)
{
    return buffer_messagesGetNumberOf (x);
}

PD_LOCAL int buffer_slotsIsEmpty (t_buffer *x)
{
    return (buffer_getSize (x) == 0);
}

PD_LOCAL t_error buffer_slotsGet (t_buffer *x, int n, t_buffer *b)
{
    t_atomtype type;
    int start   = 0;
    int end     = 0;
    t_error err = buffer_messagesGetAtWithTypeOfEnd (x, n, &start, &end, &type);
    
    if (!err) {
    //
    int count = end - start;
    
    PD_ASSERT (type == A_SEMICOLON);
    
    if (count > 0) {
    //
    buffer_append (b, count, buffer_getAtoms (x) + start); return PD_ERROR_NONE;
    //
    }
    //
    }
    
    return PD_ERROR;
}

PD_LOCAL void buffer_slotsAppend (t_buffer *x, t_buffer *b)
{
    PD_ASSERT (!buffer_containsSemicolonOrComma (b));
    
    buffer_appendBuffer (x, b); buffer_appendSemicolon (x);
}

PD_LOCAL t_error buffer_slotsSet (t_buffer *x, int n, t_buffer *b)
{
    t_atomtype type;
    int start   = 0;
    int end     = 0;
    t_error err = buffer_messagesGetAtWithTypeOfEnd (x, n, &start, &end, &type);
    
    PD_ASSERT (!buffer_containsSemicolonOrComma (b));
    
    if (!err) {
    //
    PD_ASSERT (end - start > 0);
    PD_ASSERT (type == A_SEMICOLON);
    
    buffer_replace (x, start, end, buffer_getSize (b), buffer_getAtoms (b));
    
    return PD_ERROR_NONE;
    //
    }
    
    return PD_ERROR;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
