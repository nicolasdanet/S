
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef M_UTILS_H_
#define M_UTILS_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error     utils_version               (char *dest, size_t size);

void        utils_anythingToList        (t_pd *x,
                                            t_listmethod fn,
                                            t_symbol *s,
                                            int argc,
                                            t_atom *argv);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_id        utils_unique                (void);
t_error     utils_uniqueWithAtoms       (int argc, t_atom *argv, t_id *u);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void        utils_appendUnique          (t_buffer *b, t_id u);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // M_UTILS_H_
