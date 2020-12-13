
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef D_DSPTHREAD_H_
#define D_DSPTHREAD_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_dspthread    *dspthread_new      (void);

PD_LOCAL t_error        dspthread_create    (t_dspthread *x);
PD_LOCAL void           dspthread_free      (t_dspthread *x);

PD_LOCAL void           dspthread_run       (t_dspthread *x);
PD_LOCAL void           dspthread_stop      (t_dspthread *x);
PD_LOCAL t_systime      dspthread_time      (t_dspthread *x);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // D_DSPTHREAD_H_
