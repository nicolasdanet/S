
/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "m_spaghettis.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* A no-op DSP object. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *hello_class;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _hello {
    t_object    x_obj;          /* MUST be the first. */
    t_outlet    *x_outlet;
    } t_hello;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_int *hello_perform (t_int *w)
{
    /* Fetch the parameters. */
    /* Note that the first (w[0]) is the adress of the function. */
    
    t_sample *in  = (t_sample *)(w[1]);
    t_sample *out = (t_sample *)(w[2]);
    int n = (int)(w[3]);
    
    while (n--) {
        t_sample f = *in; *out = f; in++; out++;
        // *out++ = *in++;
    }
    
    return (w + 4);     /* Extra care. It is the number of parameters required PLUS ONE. */
}

/* Note that the dsp method could be called concurrently with the perform method. */

static void hello_dsp (t_hello *x, t_signal **sp)
{
    /* Add a function to the DSP chain callable with three parameters. */
    /* First will be the vector in. */
    /* Secondth will be the vector out. */
    /* Third will be the vector size. */
    
    spaghettis_dspAdd3 (hello_perform,
        (t_int)spaghettis_signalGetVector (sp[0]),
        (t_int)spaghettis_signalGetVector (sp[1]),
        (t_int)spaghettis_signalGetVectorSize (sp[0]));
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

PD_STUB void hello_tilde_setup (void)       /* The "~" symbol is replaced by "_tilde". */
{
    t_class *c = NULL;
    
    /* The CLASS_SIGNAL flag makes the first inlet to be signal type. */
    /* In that case a float entry set the signal as a constant valued vector. */
    
    c = spaghettis_classNew (spaghettis_symbol ("hello~"),
            (t_newmethod)hello_new,
            NULL,
            sizeof (t_hello),
            CLASS_BOX | CLASS_SIGNAL);
    
    /* Set the function called while constructing the DSP graph. */
    
    spaghettis_classAddDsp (c, (t_method)hello_dsp);
    
    hello_class = c;
}

PD_STUB void hello_tilde_destroy (void)
{
    spaghettis_classFree (hello_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
