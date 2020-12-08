
/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "m_spaghettis.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Add closures to the DSP chain. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *hello_class;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _hello {
    t_object    x_obj;
    t_outlet    *x_outlet;
    } t_hello;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_int *hello_perform (t_int *w)
{
    t_sample *in  = (t_sample *)(w[1]);
    t_sample *out = (t_sample *)(w[2]);
    t_space *t    = (t_space *)(w[3]);          /* Fetch the space. */
    int n = (int)(w[4]);
    
    t_float f0 = space_getFloat0 (t);           /* Fetch the values. */
    t_float f1 = space_getFloat1 (t);
    t_float f2 = space_getFloat2 (t);
    t_float f3 = space_getFloat3 (t);
    
    while (n--) { *out++ = *in++; }
    
    space_setFloat0 (t, f0);                    /* Set the values. */
    space_setFloat1 (t, f1);
    space_setFloat2 (t, f2);
    space_setFloat3 (t, f3);
    
    return (w + 5);
}

static void hello_dsp (t_hello *x, t_signal **s)
{
    /* Get a space to manage variables in the perform method. */
    /* This space is owned by the DSP chain. */
    /* It will be freed at the same time the DSP chain is freed. */
    /* MUST be used only in the dsp method. */
    
    t_space *t = instance_objectGetNewSpace ((t_object *)x);
    
    /* Initialize the values. */
    
    space_setFloat0 (t, 3.14);
    space_setFloat1 (t, 3.14);
    space_setFloat2 (t, 3.14);
    space_setFloat3 (t, 3.14);
    
    /* Add the space to the DSP chain. */
    
    dsp_add4 (hello_perform,
        signal_getVector (s[0]),
        signal_getVector (s[1]),
        t,                                  /* Here (or at the place you want). */
        signal_getVectorSize (s[0]));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *hello_new (void)
{
    t_hello *x  = (t_hello *)pd_new (hello_class);
    
    x->x_outlet = outlet_newSignal ((t_object *)x);

    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_STUB void helloSpace_tilde_setup (void)
{
    t_class *c = NULL;
        
    c = class_new (gensym ("helloSpace~"),
            (t_newmethod)hello_new,
            NULL,
            sizeof (t_hello),
            CLASS_BOX | CLASS_SIGNAL,
            A_NULL);
    
    class_addDSP (c, (t_method)hello_dsp);

    hello_class = c;
}

PD_STUB void helloSpace_tilde_destroy (void)
{
    class_free (hello_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
