
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef M_ITERATOR_H_
#define M_ITERATOR_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _iterator {
    int     iter_argc;
    int     iter_index;
    t_atom  *iter_argv;
    } t_iterator;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_iterator *iterator_new   (int argc, t_atom *argv);

void   iterator_free       (t_iterator *x);
int    iterator_get        (t_iterator *x);
int    iterator_next       (t_iterator *x, t_atom **a);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // M_ITERATOR_H_