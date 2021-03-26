
/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "m_spaghettis.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Biquad filter anatomized. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < https://ccrma.stanford.edu/~jos/filters/Direct_Form_II.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Include pthread header for mutexes. */

#include <pthread.h>

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Note that it is not required to link to the pthread library (the application already is). */
/* < http://www.informit.com/articles/article.aspx?p=22435 > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *biquad_class;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _biquad {
    t_object            x_obj;
    pthread_mutex_t     x_mutex;        /* Required to manage multi-thread safety. */
    t_sample            x_real1;        /* Used only in DSP thread or non concurrently. */
    t_sample            x_real2;        /* Used only in DSP thread or non concurrently. */
    t_float             x_a1;
    t_float             x_a2;
    t_float             x_b0;
    t_float             x_b1;
    t_float             x_b2;
    int                 x_set;
    t_outlet            *x_outlet;
    } t_biquad;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void biquad_set (t_biquad *x, t_float a1, t_float a2, t_float b0, t_float b1, t_float b2)
{
    /* Need to be locked since it can be read concurrently by the DSP thread. */
    /* What is matter to be protected is the state of the set flag. */
    /* Thus publish in the main thread need to be locked. */
    /* Thus consume in the DSP perform need to be locked. */
    
    pthread_mutex_lock (&x->x_mutex);
    
        x->x_a1  = a1;
        x->x_a2  = a2;
        x->x_b0  = b0;
        x->x_b1  = b1;
        x->x_b2  = b2;
        x->x_set = 1;
    
    pthread_mutex_unlock (&x->x_mutex);
}

static void biquad_list (t_biquad *x, t_symbol *s, int argc, t_atom *argv)
{
    t_float a1 = spaghettis_atomGetFloatAtIndex (0, argc, argv);
    t_float a2 = spaghettis_atomGetFloatAtIndex (1, argc, argv);
    t_float b0 = spaghettis_atomGetFloatAtIndex (2, argc, argv);
    t_float b1 = spaghettis_atomGetFloatAtIndex (3, argc, argv);
    t_float b2 = spaghettis_atomGetFloatAtIndex (4, argc, argv);
    
    biquad_set (x, a1, a2, b0, b1, b2);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Values of the space will be used only by the DSP thread. */
/* Notice that the DSP chain is run after the dsp method call. */
/* Thus it doesn't require to protect access in this dsp method. */

static void biquad_space (t_space *t, t_float a1, t_float a2, t_float b0, t_float b1, t_float b2)
{
    spaghettis_spaceSetFloat0 (t, a1);
    spaghettis_spaceSetFloat1 (t, a2);
    spaghettis_spaceSetFloat2 (t, b0);
    spaghettis_spaceSetFloat3 (t, b1);
    spaghettis_spaceSetFloat4 (t, b2);
}

static t_int *biquad_perform (t_int *w)
{
    t_biquad *x   = (t_biquad *)(w[1]);
    t_sample *in  = (t_sample *)(w[2]);
    t_sample *out = (t_sample *)(w[3]);
    t_space *t    = (t_space *)(w[4]);
    int n = (int)(w[5]);
    
    /* It is not allowed to block the DSP thread (we don't want to glitch). */
    /* So the biquad parameter are refreshed only if it is possible without to wait. */
    /* In practice collision is very improbable. */
    /* Benchmarking show that pthread_mutex_trylock is fast. */
    /* TBH, i don't see any other safe method without this trick. */
    
    if (pthread_mutex_trylock (&x->x_mutex) == 0) {
    //
    if (x->x_set) { biquad_space (t, x->x_a1, x->x_a2, x->x_b0, x->x_b1, x->x_b2); x->x_set = 0; }
    
    pthread_mutex_unlock (&x->x_mutex);
    //
    }

    /* Get the biquad paramaters (could/should be inlined in the future). */
    
    t_sample a1 = (t_sample)spaghettis_spaceGetFloat0 (t);
    t_sample a2 = (t_sample)spaghettis_spaceGetFloat1 (t);
    t_sample b0 = (t_sample)spaghettis_spaceGetFloat2 (t);
    t_sample b1 = (t_sample)spaghettis_spaceGetFloat3 (t);
    t_sample b2 = (t_sample)spaghettis_spaceGetFloat4 (t);
    
    /* Biquad stuff. */
    
    t_sample last1 = x->x_real1;
    t_sample last2 = x->x_real2;
    
    while (n--) {
    //
    t_sample f = (*in++) + a1 * last1 + a2 * last2; 
    *out++ = b0 * f + b1 * last1 + b2 * last2;
    last2  = last1;
    last1  = f;
    //
    }
    
    x->x_real1 = last1;
    x->x_real2 = last2;
    
    return (w + 6);
}

/* No race condition is possible in that function. */

static void biquad_initialize (void *lhs, void *rhs)
{
    t_biquad *x   = (t_biquad *)lhs;
    t_biquad *old = (t_biquad *)rhs;
    
    x->x_real1 = old->x_real1;
    x->x_real2 = old->x_real2;
}

/* Only the perform method could be run concurrently. */
/* All others (in the main thread) are sequentials. */
/* But TBH it is enough for headaches. */

static void biquad_dsp (t_biquad *x, t_signal **sp)
{
    /* Note that the old DSP thread is still running, while building the new one. */
        
    if (spaghettis_objectDspNeedInitializer ((t_object *)x)) {
    //
    /* In case of encapsulation, fetch the old object. */

    t_biquad *old = (t_biquad *)spaghettis_objectGetTemporary ((t_object *)x);
    
    /* Note that at this point the old object is still used by the old DSP thread. */
    
    if (old) {
    //
    /* Add an initializer in order to later copy/swap internal states before swapping the DSP chains. */

    spaghettis_initializerNew (biquad_initialize, x, old);
    
    /* Member variables is write only by the main thread. */
    /* Thus it is not required to lock the read in the main thread. */
    
    biquad_set (x, old->x_a1, old->x_a2, old->x_b0, old->x_b1, old->x_b2);
    
    /* Ditto for constant signal values. */
    
    spaghettis_objectCopySignalValues ((t_object *)x, (t_object *)old);
    //
    }
    //
    }
    
    {
    //
    t_space *t = spaghettis_objectGetNewSpace ((t_object *)x);
    
    /* Initialize values of the space. */
    
    biquad_space (t, x->x_a1, x->x_a2, x->x_b0, x->x_b1, x->x_b2);
    
    spaghettis_dspAdd5 (biquad_perform,
                (t_int)x,
                (t_int)spaghettis_signalGetVector (sp[0]),
                (t_int)spaghettis_signalGetVector (sp[1]),
                (t_int)t,
                (t_int)spaghettis_signalGetVectorSize (sp[0]));
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *biquad_functionData (t_object *z, int flags)
{
    /* Serialize biquad parameters (for undo/redo). */
    
    if (spaghettis_objectFlagIsUndoOrEncaspulate (z, flags)) {
    //
    t_biquad *x = (t_biquad *)z;
    t_buffer *b = spaghettis_bufferNew();
    t_float a1  = x->x_a1;
    t_float a2  = x->x_a2;
    t_float b0  = x->x_b0;
    t_float b1  = x->x_b1;
    t_float b2  = x->x_b2;
    
    spaghettis_bufferAppendSymbol (b, spaghettis_symbol ("list"));
    spaghettis_bufferAppendFloat (b,  a1);
    spaghettis_bufferAppendFloat (b,  a2);
    spaghettis_bufferAppendFloat (b,  b0);
    spaghettis_bufferAppendFloat (b,  b1);
    spaghettis_bufferAppendFloat (b,  b2);
    spaghettis_bufferAppendComma (b);
    spaghettis_objectGetSignalValues ((t_object *)x, b);
    
    return b;
    //
    }
    
    return NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *biquad_new (t_symbol *s, int argc, t_atom *argv)
{
    t_biquad *x = (t_biquad *)spaghettis_objectNew (biquad_class);
    
    pthread_mutex_init (&x->x_mutex, NULL);
    
    x->x_outlet = spaghettis_objectOutletNewSignal ((t_object *)x);
    
    biquad_list (x, s, argc, argv);

    return x;
}

static void biquad_free (t_biquad *x)
{
    pthread_mutex_destroy (&x->x_mutex);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_STUB void helloBiquad_tilde_setup (void)
{
    t_class *c = NULL;

    c = spaghettis_classNewWithArguments (spaghettis_symbol ("helloBiquad~"),
            (t_newmethod)biquad_new,
            (t_method)biquad_free,
            sizeof (t_biquad),
            CLASS_BOX | CLASS_SIGNAL);
            
    spaghettis_classAddDsp (c, (t_method)biquad_dsp);
    spaghettis_classAddList (c, (t_method)biquad_list);
    
    spaghettis_classSetDataFunction (c, biquad_functionData);
    
    biquad_class = c;
}

PD_STUB void helloBiquad_tilde_destroy (void)
{
    spaghettis_classFree (biquad_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
