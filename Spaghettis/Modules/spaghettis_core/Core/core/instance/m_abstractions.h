
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef M_ABSTRACTIONS_H_
#define M_ABSTRACTIONS_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Used to keep states of abstractions during encapsulation (live coding). */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Takes exclusive ownership of the buffer. */

PD_LOCAL t_abstractions *abstractions_new           (void);

PD_LOCAL t_symbol       *abstractions_cache         (t_abstractions *x,
                                                        t_glist *glist,
                                                        t_symbol *name,
                                                        t_buffer *b);
                                                    
PD_LOCAL t_symbol       *abstractions_getName       (t_abstractions *x, t_symbol *key);
PD_LOCAL t_buffer       *abstractions_getSnippet    (t_abstractions *x,
                                                        t_symbol *key,
                                                        t_symbol **filename,
                                                        t_symbol **directory);

PD_LOCAL void           abstractions_free           (t_abstractions *x);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // M_ABSTRACTIONS_H_
