
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void buffer_appendAsClock (t_buffer *x, t_clock *c)
{
    t_atom a;
    SET_CLOCK (&a, c);
    buffer_append (x, 1, &a);
}

void buffer_appendAsObject (t_buffer *x, t_object *y)
{
    t_atom a;
    SET_OBJECT (&a, y);
    buffer_append (x, 1, &a);
}

void buffer_appendAsBuffer (t_buffer *x, t_buffer *b)
{
    t_atom a;
    SET_BUFFER (&a, b);
    buffer_append (x, 1, &a);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void buffer_prependAsObject (t_buffer *x, t_object *y)
{
    t_atom a;
    SET_OBJECT (&a, y);
    buffer_prepend (x, 1, &a);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void buffer_insertAsFloatAtIndex (t_buffer *b, int i, t_float f)
{
    t_atom a;
    SET_FLOAT (&a, f);
    buffer_insertAtIndex (b, i, &a);
}

void buffer_insertAsObjectAtIndex (t_buffer *b, int i, t_object *y)
{
    t_atom a;
    SET_OBJECT (&a, y);
    buffer_insertAtIndex (b, i, &a);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void *buffer_getVoidAt (t_buffer *x, int n)
{
    t_atom *a = buffer_getAtomAtIndex (x, n); PD_ASSERT (IS_VOID (a));   return GET_VOID (a);
}

t_object *buffer_getObjectAt (t_buffer *x, int n)
{
    t_atom *a = buffer_getAtomAtIndex (x, n); PD_ASSERT (IS_VOID (a));   return GET_OBJECT (a);
}

t_buffer *buffer_getBufferAt (t_buffer *x, int n)
{
    t_atom *a = buffer_getAtomAtIndex (x, n); PD_ASSERT (IS_VOID (a));   return GET_BUFFER (a);
}

t_clock *buffer_getClockAt (t_buffer *x, int n)
{
    t_atom *a = buffer_getAtomAtIndex (x, n); PD_ASSERT (IS_VOID (a));   return GET_CLOCK (a);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
