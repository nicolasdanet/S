
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define BUFFER_GROWTH           8
#define BUFFER_PREALLOCATED     8

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void buffer_append (t_buffer *x, int argc, t_atom *argv)
{
    if (argc > 0) {
    //
    int required = x->b_size + argc;

    while (required > x->b_allocated) {
    //
    int n = x->b_allocated * BUFFER_GROWTH; buffer_reserve (x, PD_MAX (BUFFER_PREALLOCATED, n));
    //
    }
    
    {
    //
    t_atom *a = NULL;

    for (a = x->b_vector + x->b_size; argc--; a++) { *a = *(argv++); }
    
    x->b_size = required;
    //
    }
    //
    }
}

void buffer_appendAtom (t_buffer *x, t_atom *a)
{
    buffer_append (x, 1, a);
}

void buffer_appendBuffer (t_buffer *x, t_buffer *y)
{
    buffer_append (x, buffer_getSize (y), buffer_getAtoms (y));
}

void buffer_appendFloat (t_buffer *x, t_float f)
{
    t_atom a;
    SET_FLOAT (&a, f);
    buffer_append (x, 1, &a);
}

void buffer_appendSymbol (t_buffer *x, t_symbol *s)
{
    t_atom a;
    SET_SYMBOL (&a, s);
    buffer_append (x, 1, &a);
}

void buffer_appendDollar (t_buffer *x, int n)
{
    t_atom a;
    SET_DOLLAR (&a, n);
    buffer_append (x, 1, &a);
}

void buffer_appendDollarSymbol (t_buffer *x, t_symbol *s)
{
    t_atom a;
    SET_DOLLARSYMBOL (&a, s);
    buffer_append (x, 1, &a);
}

void buffer_appendSemicolon (t_buffer *x)
{
    t_atom a;
    SET_SEMICOLON (&a);
    buffer_append (x, 1, &a);
}

void buffer_appendComma (t_buffer *x)
{
    t_atom a;
    SET_COMMA (&a);
    buffer_append (x, 1, &a);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
