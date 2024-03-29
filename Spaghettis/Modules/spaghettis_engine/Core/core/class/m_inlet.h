
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef M_INLET_H_
#define M_INLET_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

struct _inlet {
    t_pd                    i_pd;           /* MUST be the first. */
    struct _inlet           *i_next;
    t_object                *i_owner;
    t_pd                    *i_receiver;
    t_symbol                *i_type;
    int                     i_hot;
    union {
        t_symbol            *i_method;
        t_float             *i_float;
        t_symbol            **i_symbol;
        t_float64Atomic     i_signal;
    } i_un;
    };

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_inlet *inlet_new (t_object *owner, t_pd *receiver, t_symbol *t, t_symbol *m);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void   inlet_free              (t_inlet *x);
void   inlet_setHot            (t_inlet *x);
void   inlet_moveFirst         (t_inlet *x);
int    inlet_isSignal          (t_inlet *x);
int    inlet_getIndex          (t_inlet *x);
int    inlet_getIndexAsSignal  (t_inlet *x);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static inline t_inlet *inlet_getNext (t_inlet *x)
{
    return x->i_next;
}

static inline t_object *inlet_getOwner (t_inlet *x)
{
    return x->i_owner;
}

static inline t_float64Atomic *inlet_getSignal (t_inlet *x)
{
    return &x->i_un.i_signal;
}

static inline t_symbol *inlet_getType (t_inlet *x)
{
    return (x->i_type == NULL) ? &s_anything : x->i_type;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // M_INLET_H_
