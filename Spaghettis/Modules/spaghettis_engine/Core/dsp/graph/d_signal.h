
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef D_SIGNAL_H_
#define D_SIGNAL_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_signal   *signal_newWithSignal       (t_signal *s);
t_signal   *signal_newWithContext      (t_dspcontext *c);

void       signal_borrow               (t_signal *s,  t_signal *toBeBorrowed);
int        signal_isCompatibleWith     (t_signal *s1, t_signal *s2);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // D_SIGNAL_H_