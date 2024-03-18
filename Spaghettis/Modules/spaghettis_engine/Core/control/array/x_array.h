
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef X_ARRAY_H_
#define X_ARRAY_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _arrayclient {
    t_object        ac_obj;                 /* Must be the first. */
    t_symbol        *ac_name;
    } t_arrayclient;

typedef struct _arrayrange {
    t_arrayclient   ar_arrayclient;         /* Must be the first. */
    t_error         ar_error;
    t_float         ar_onset;
    t_float         ar_size;
    } t_arrayrange;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define ARRAYCLIENT_ADDRESS_NAME(x)         &((x)->ac_name)

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define ARRAYRANGE_GOOD(x)                  (((t_arrayrange *)(x))->ar_error == PD_ERROR_NONE)

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error    arrayclient_init        (t_arrayclient *x, int *argc, t_atom **argv);
void       arrayclient_free        (t_arrayclient *x);
void       arrayclient_setName     (t_arrayclient *x, t_symbol *s);

t_symbol   *arrayclient_getName            (t_arrayclient *x);
t_garray   *arrayclient_fetchGraphicArray  (t_arrayclient *x);

void       arrayclient_restore     (t_arrayclient *x, t_arrayclient *old);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void       *arrayrange_new         (t_class *c, int argc, t_atom *argv, int makeOnset, int makeSize);
t_garray   *arrayrange_getRange    (t_arrayrange *x, int *i, int *n);

void       arrayrange_setOnset     (t_arrayrange *x, t_float f);
void       arrayrange_setSize      (t_arrayrange *x, t_float f);
t_float    arrayrange_getOnset     (t_arrayrange *x);
t_float    arrayrange_getSize      (t_arrayrange *x);
t_error    arrayrange_quantile     (t_arrayrange *x, t_float f, t_float *v);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

t_buffer   *arrayrange_functionData    (t_object *z, int flags);

void       arrayrange_restore          (t_arrayrange *x, t_symbol *s, int argc, t_atom *argv);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void       *arraysize_new          (t_symbol *s, int argc, t_atom *argv);
void       *arraysum_new           (t_symbol *s, int argc, t_atom *argv);
void       *arrayget_new           (t_symbol *s, int argc, t_atom *argv);
void       *arrayset_new           (t_symbol *s, int argc, t_atom *argv);
void       *arrayquantile_new      (t_symbol *s, int argc, t_atom *argv);
void       *arrayrandom_new        (t_symbol *s, int argc, t_atom *argv);
void       *arraymax_new           (t_symbol *s, int argc, t_atom *argv);
void       *arraymin_new           (t_symbol *s, int argc, t_atom *argv);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // X_ARRAY_H_
