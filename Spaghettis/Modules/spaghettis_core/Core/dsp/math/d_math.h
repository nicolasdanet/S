
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef D_MATH_H_
#define D_MATH_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

struct _unop_tilde {
    t_object            x_obj;              /* Must be the first. */
    t_outlet            *x_outlet;
    };

struct _binop_tilde {
    t_object            x_obj;              /* Must be the first. */
    t_outlet            *x_outlet;
    };

struct _binopscalar_tilde {
    t_object            x_obj;              /* Must be the first. */
    t_float64Atomic     x_scalar;
    t_outlet            *x_outlet;
    };

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_buffer   *unop_tilde_functionData            (t_object *z, int flags);
t_buffer   *binop_tilde_functionData           (t_object *z, int flags);
t_buffer   *binopScalar_tilde_functionData     (t_object *z, int flags);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void       binopScalar_tilde_float             (struct _binopscalar_tilde *x, t_float f);
void       binopScalar_tilde_restore           (struct _binopscalar_tilde *x);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // D_MATH_H_
