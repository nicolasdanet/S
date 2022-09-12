
/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "m_spaghettis.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Encapsulation safety for DSP object. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* A DSP object that will survive to encapsulation in live patching (minimal no-op case). */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *hello_class;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

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
    int n = (int)(w[3]);
    
    while (n--) { *out++ = *in++; }
    
    return (w + 4);
}

/* That function will be trigger just after swapping the DSP chains. */
/* It is called NON concurrently. */
/* Copy and/or swap internal states here (MUST be fast and lock-free). */

static void hello_initialize (void *lhs, void *rhs)
{
    // t_hello *x   = (t_hello *)lhs;
    // t_hello *old = (t_hello *)rhs;
}

/* Note that the dsp method could be called concurrently with the perform method. */
/* The DSP thread is run parallel to the main thread. */
/* Thus any method (apart from new and free) can be called concurrently. */
/* When the DSP chain is rebuilt, the old one IS still RUNNING. */
/* Once done the new chain is mount, and the old is pushed into the garbage. */

static void hello_dsp (t_hello *x, t_signal **s)
{
    if (spaghettis_objectDspNeedInitializer ((t_object *)x)) {    /* I.e. encapsulated. */
    //
    /* Catch the old object. */
    
    t_hello *old = (t_hello *)spaghettis_objectGetTemporary ((t_object *)x);
    
    if (old) {
    //
    /* Add a initializer with both references. */
    
    spaghettis_initializerNew (hello_initialize, x, old);
    
    /* Copy the signal values (i.e. constant message floats provided to inlets). */
    
    spaghettis_objectCopySignalValues ((t_object *)x, (t_object *)old);
    //
    }
    //
    }
    
    spaghettis_dspAdd3 (hello_perform,
        (t_int)spaghettis_signalGetVector (s[0]),
        (t_int)spaghettis_signalGetVector (s[1]),
        (t_int)spaghettis_signalGetVectorSize (s[0]));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* If initializer is not required. */

#if 0

static void hello_dsp (t_hello *x, t_signal **s)
{
    spaghettis_objectFetchAndCopySignalValuesIfRequired ((t_object *)x);
    
    spaghettis_dspAdd3 (hello_perform,
        (t_int)spaghettis_signalGetVector (s[0]),
        (t_int)spaghettis_signalGetVector (s[1]),
        (t_int)spaghettis_signalGetVectorSize (s[0]));
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *hello_functionData (t_object *z, int flags)
{
    if (spaghettis_objectFlagIsUndoOrEncaspulate (z, flags)) {
    //
    t_hello *x  = (t_hello *)z;
    t_buffer *b = spaghettis_bufferNew();
        
    /* Push the signal values (i.e. constant message floats provided to inlets). */
    /* Notice that so they will be automatically restored. */
    
    spaghettis_objectGetSignalValues ((t_object *)x, b);
    
    return b;
    //
    }
    
    return NULL;
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

PD_STUB void helloData_tilde_setup (void)
{
    t_class *c = NULL;
        
    c = spaghettis_classNew (spaghettis_symbol ("helloData~"),
            (t_newmethod)hello_new,
            NULL,
            sizeof (t_hello),
            CLASS_BOX | CLASS_SIGNAL);
    
    spaghettis_classAddDsp (c, (t_method)hello_dsp);

    spaghettis_classSetDataFunction (c, hello_functionData);
    
    hello_class = c;
}

PD_STUB void helloData_tilde_destroy (void)
{
    spaghettis_classFree (hello_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
