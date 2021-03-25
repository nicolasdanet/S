
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
    t_space *t    = (t_space *)(w[3]);                  /* Fetch the space. */
    int n = (int)(w[4]);
    
    t_float f0 = spaghettis_spaceGetFloat0 (t);         /* Fetch the values. */
    t_float f1 = spaghettis_spaceGetFloat1 (t);
    t_float f2 = spaghettis_spaceGetFloat2 (t);
    t_float f3 = spaghettis_spaceGetFloat3 (t);
    
    while (n--) { *out++ = *in++; }
    
    spaghettis_spaceSetFloat0 (t, f0);                  /* Set the values. */
    spaghettis_spaceSetFloat1 (t, f1);
    spaghettis_spaceSetFloat2 (t, f2);
    spaghettis_spaceSetFloat3 (t, f3);
    
    return (w + 5);
}

static void hello_dsp (t_hello *x, t_signal **s)
{
    /* Get a space to manage variables in the perform method. */
    /* This space is owned by the DSP chain. */
    /* It will be freed at the same time the DSP chain is freed. */
    /* MUST be used only in the dsp method. */
    
    t_space *t = spaghettis_objectGetNewSpace ((t_object *)x);
    
    /* Initialize the values. */
    
    spaghettis_spaceSetFloat0 (t, 3.14);
    spaghettis_spaceSetFloat1 (t, 3.14);
    spaghettis_spaceSetFloat2 (t, 3.14);
    spaghettis_spaceSetFloat3 (t, 3.14);
    
    /* Add the space to the DSP chain. */
    
    spaghettis_dspAdd4 (hello_perform,
        (t_int)spaghettis_signalGetVector (s[0]),
        (t_int)spaghettis_signalGetVector (s[1]),
        (t_int)t,                                           /* Here (or at the place you want). */
        (t_int)spaghettis_signalGetVectorSize (s[0]));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *hello_new (void)
{
    t_hello *x  = (t_hello *)spaghettis_objectNew (hello_class);
    
    x->x_outlet = spaghettis_objectOutletNewSignal ((t_object *)x);

    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_STUB void helloSpace_tilde_setup (void)
{
    t_class *c = NULL;
        
    c = spaghettis_classNew (spaghettis_symbol ("helloSpace~"),
            (t_newmethod)hello_new,
            NULL,
            sizeof (t_hello),
            CLASS_BOX | CLASS_SIGNAL);
    
    spaghettis_classAddDSP (c, (t_method)hello_dsp);

    hello_class = c;
}

PD_STUB void helloSpace_tilde_destroy (void)
{
    spaghettis_classFree (hello_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
