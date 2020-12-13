
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef D_FILTERS_H_
#define D_FILTERS_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

struct _real_raw_tilde {
    t_object    x_obj;                  /* Must be the first. */
    t_sample    x_real;
    t_outlet    *x_outlet;
    };

struct _complex_raw_tilde {
    t_object    x_obj;                  /* Must be the first. */
    t_sample    x_real;
    t_sample    x_imaginary;
    t_outlet    *x_outletLeft;
    t_outlet    *x_outletRight;
    };

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_buffer   *real_raw_functionData      (t_object *z, int flags);
PD_LOCAL t_buffer   *complex_raw_functionData   (t_object *z, int flags);

PD_LOCAL void       real_raw_initializer        (t_object *x);
PD_LOCAL void       complex_raw_initializer     (t_object *x);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // D_FILTERS_H_
