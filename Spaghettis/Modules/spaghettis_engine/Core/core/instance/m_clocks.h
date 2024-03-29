
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef M_CLOCKS_H_
#define M_CLOCKS_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* A simple non-blocking bounded collection of pointers to manage clocks. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_clocks   *clocks_new     (void);

void       clocks_free     (t_clocks *x);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void       clocks_add      (t_clocks *x, t_clock *c);
void       clocks_remove   (t_clocks *x, t_clock *c);
void       clocks_destroy  (t_clocks *x, t_clock *c);
void       clocks_tick     (t_clocks *x, t_systime systime);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // M_CLOCKS_H_
