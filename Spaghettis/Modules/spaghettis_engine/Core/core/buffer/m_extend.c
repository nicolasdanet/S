
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Ending element is not included (can be the buffer size). */

t_error buffer_extend (t_buffer *x, int start, int end, int n)
{
    PD_ASSERT (n >= 0); n = PD_MAX (n, 0);
    
    if ((start < 0) || (end > x->b_size) || (start > end)) { return PD_ERROR; }
    else {
    //
    int count    = n - (end - start);
    int oldSize  = x->b_size;
    int newSize  = oldSize + count;
    int tailSize = oldSize - end;
    
    if (newSize > oldSize) { buffer_resize (x, newSize); }
    memmove ((void *)(x->b_vector + start + n), (void *)(x->b_vector + end), sizeof (t_atom) * tailSize);
    if (count > 0) { memset ((void *)(x->b_vector + end), 0, sizeof (t_atom) * count); }
    if (newSize < oldSize) { buffer_resize (x, newSize); }
    //
    }
    
    return PD_ERROR_NONE;
}

/* Ending element is not included (can be the buffer size). */

t_error buffer_replace (t_buffer *x, int start, int end, int argc, t_atom *argv)
{
    t_error err = PD_ERROR;
    
    if (argc >= 0) {
    //
    err = buffer_extend (x, start, end, argc);
    
    if (!err && argc && argv) { atom_copyAtoms (argv, argc, x->b_vector + start, argc); }
    //
    }
    
    return err;
}

void buffer_prepend (t_buffer *x, int argc, t_atom *argv)
{
    if (argc && x->b_size) { buffer_extend (x, 0, 0, argc); atom_copyAtoms (argv, argc, x->b_vector, argc); }
    else {
        buffer_append (x, argc, argv);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error buffer_insertAtIndex (t_buffer *x, int n, t_atom *a)
{
    if (n >= 0 && n <= x->b_size) {
    //
    if (n == 0)         { buffer_prepend (x, 1, a); return PD_ERROR_NONE; }
    if (n == x->b_size) { buffer_append (x, 1, a);  return PD_ERROR_NONE; }
    
    buffer_extend (x, n, n, 1);
    
    return buffer_setAtIndex (x, n, a);
    //
    }
    
    return PD_ERROR;
}

t_error buffer_removeAtIndex (t_buffer *x, int n)
{
    return buffer_extend (x, n, n + 1, 0);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void buffer_removeClock (t_buffer *x, t_clock *c)
{
    int i, n = buffer_getSize (x);
    
    int k = -1;
    
    for (i = 0; i < n; i++) {
        if (buffer_getClockAt (x, i) == c) { k = i; break; }
    }
    
    if (k > 0) {
        buffer_removeAtIndex (x, k);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void buffer_shuffle (t_buffer *x)
{
    atom_shuffle (buffer_getSize (x), buffer_getAtoms (x));
}

t_error buffer_pop (t_buffer *x, t_atom *a)
{
    int n = buffer_getSize (x);
    
    if (n > 0) {
    //
    atom_copyAtom (buffer_getAtomAtIndex (x, n - 1), a);
    buffer_resize (x, n - 1);
    return PD_ERROR_NONE;
    //
    }
    
    return PD_ERROR;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
