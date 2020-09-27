
/* Copyright (c) 1997-2020 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_EXPORT t_buffer *buffer_new (void)
{
    t_buffer *x = (t_buffer *)PD_MEMORY_GET (sizeof (t_buffer));
    
    x->b_allocated = 0;
    x->b_size      = 0;
    x->b_vector    = (t_atom *)PD_MEMORY_GET (0);
    
    return x;
}

PD_EXPORT t_buffer *buffer_newCopy (t_buffer *toCopy)
{
    t_buffer *x = buffer_new(); if (toCopy) { buffer_appendBuffer (x, toCopy); } return x;
}

PD_EXPORT void buffer_free (t_buffer *x)
{
    PD_MEMORY_FREE (x->b_vector);
    PD_MEMORY_FREE (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_EXPORT int buffer_getSize (t_buffer *x)
{
    return x->b_size;
}

PD_EXPORT t_atom *buffer_getAtoms (t_buffer *x)
{
    return x->b_vector;
}

PD_EXPORT t_atom *buffer_getAtomAtIndex (t_buffer *x, int n)
{
    return x->b_vector + n;
}

PD_EXPORT t_atom *buffer_getAtomAtIndexChecked (t_buffer *x, int n)
{
    if (n >= 0 && n < x->b_size) { return x->b_vector + n; }
    
    return NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_object *buffer_getObjectAt (t_buffer *x, int n)
{
    t_atom *a = buffer_getAtomAtIndex (x, n); PD_ASSERT (IS_VOID (a));   return GET_OBJECT (a);
}

PD_LOCAL t_buffer *buffer_getBufferAt (t_buffer *x, int n)
{
    t_atom *a = buffer_getAtomAtIndex (x, n); PD_ASSERT (IS_VOID (a));   return GET_BUFFER (a);
}

PD_LOCAL t_clock *buffer_getClockAt (t_buffer *x, int n)
{
    t_atom *a = buffer_getAtomAtIndex (x, n); PD_ASSERT (IS_VOID (a));   return GET_CLOCK (a);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_EXPORT t_error buffer_getAtIndex (t_buffer *x, int n, t_atom *a)
{
    t_atom *t = buffer_getAtomAtIndexChecked (x, n); if (t && a) { *a = *t; return PD_ERROR_NONE; }
    
    return PD_ERROR;
}

PD_EXPORT t_error buffer_setAtIndex (t_buffer *x, int n, t_atom *a)
{
    t_atom *t = buffer_getAtomAtIndexChecked (x, n); if (t) { *t = *a; return PD_ERROR_NONE; }
    
    return PD_ERROR;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_EXPORT t_error buffer_setFloatAtIndex (t_buffer *x, int n, t_float f)
{
    t_atom a; SET_FLOAT (&a, f); return buffer_setAtIndex (x, n, &a);
}

PD_EXPORT t_error buffer_setSymbolAtIndex (t_buffer *x, int n, t_symbol *s)
{
    t_atom a; SET_SYMBOL (&a, s); return buffer_setAtIndex (x, n, &a);
}

PD_EXPORT t_float buffer_getFloatAtIndex (t_buffer *x, int n)
{
    return atom_getFloatAtIndex (n, buffer_getSize (x), buffer_getAtoms (x));
}

PD_EXPORT t_symbol *buffer_getSymbolAtIndex (t_buffer *x, int n)
{
    return atom_getSymbolAtIndex (n, buffer_getSize (x), buffer_getAtoms (x));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_EXPORT void buffer_clear (t_buffer *x)
{
    x->b_size = 0;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void buffer_allocate (t_buffer *x, int n)
{
    size_t oldSize = sizeof (t_atom) * x->b_allocated;
    size_t newSize = sizeof (t_atom) * n;
    
    PD_ASSERT (n >= 0);
    
    if (oldSize != newSize) {
    //
    x->b_allocated = n;
    x->b_vector    = (t_atom *)PD_MEMORY_RESIZE (x->b_vector, oldSize, newSize);
    //
    }
}

PD_EXPORT void buffer_reserve (t_buffer *x, int n)
{
    if (n > x->b_allocated) { buffer_allocate (x, n); }
}

PD_EXPORT void buffer_resize (t_buffer *x, int n)
{
    PD_ASSERT (n >= 0); n = PD_MAX (n, 0); buffer_allocate (x, n); x->b_size = n;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
