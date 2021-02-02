
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL int iterator_next (t_iterator *x, t_atom **a)
{
    if (x->iter_argc > 0) {
    //
    if (x->iter_index < x->iter_argc) {
    //
    int i, n, next;
    for (i = x->iter_index; i < x->iter_argc && !IS_SEMICOLON_OR_COMMA (x->iter_argv + i); i++) { }
    n = i - x->iter_index;
    next = PD_MIN (i + 1, x->iter_argc);
    (*a) = x->iter_argv + x->iter_index; 
    x->iter_index = next;
    if (i < x->iter_argc) { return n + 1; }
    else {
        return n;
    }
    //
    }
    //
    }
    
    return 0;
}

PD_LOCAL int iterator_get (t_iterator *x)
{
    return x->iter_index;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_iterator *iterator_new (int argc, t_atom *argv)
{
    t_iterator *x = (t_iterator *)PD_MEMORY_GET (sizeof (t_iterator));
    
    x->iter_argc  = argc;
    x->iter_index = 0;
    x->iter_argv  = argv;
    
    return x;
}

PD_LOCAL void iterator_free (t_iterator *x)
{
    PD_MEMORY_FREE (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
