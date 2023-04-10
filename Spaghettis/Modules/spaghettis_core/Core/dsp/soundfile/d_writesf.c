
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "d_soundfile.h"
#include "d_sfthread.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Note that for now that object is reset with encapsulation. */

// TODO: Fetch states with pending?

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_class *writesf_tilde_class;                /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _writesf_tilde {
    t_object            sf_obj;                     /* Must be the first. */
    t_trylock           sf_mutex;
    int                 sf_numberOfChannels;
    int                 sf_bufferSize;
    int                 sf_run;
    int                 sf_dismissed;
    t_sfthread          *sf_thread;
    unsigned char       *sf_cached;
    } t_writesf_tilde;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void writesf_tilde_dismiss (t_writesf_tilde *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define WRITESF_BUFFER_SIZE     65536               /* Power of two. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void writesf_tilde_close (t_writesf_tilde *x)
{
    t_sfthread *sfthread = x->sf_thread;
    
    trylock_lock (&x->sf_mutex);
    
        x->sf_thread = NULL;
        x->sf_run    = 0;
    
    trylock_unlock (&x->sf_mutex);
    
    PD_MEMORY_BARRIER;
    
    sfthread_release (sfthread);
}

static void writesf_tilde_open (t_writesf_tilde *x, t_symbol *s, int argc, t_atom *argv)
{
    writesf_tilde_close (x);
    
    {
    //
    t_glist *owner = object_getOwner (cast_object (x));
    
    t_audioproperties p; soundfile_propertiesInit (&p);
    
    t_error err = soundfile_writeFileParse (owner,
                        sym_writesf__tilde__,
                        &argc,
                        &argv,
                        &p,
                        cast_object (x));
    
    p.ap_numberOfChannels = x->sf_numberOfChannels;
    
    if (!err) {
    //
    int f = soundfile_writeFileHeader (owner, &p, cast_object (x));
    
    err = (f < 0);
    
    if (!err) {
    
        t_sfthread *sfthread = sfthread_new (cast_object (x), SFTHREAD_WRITER, x->sf_bufferSize, f, &p);
        
        PD_MEMORY_BARRIER;
        
        trylock_lock (&x->sf_mutex);
        
            x->sf_thread = sfthread;
        
        trylock_unlock (&x->sf_mutex);
        
        PD_ASSERT (x->sf_thread);
    }
    //
    }
    
    if (err) { error_canNotOpen (cast_object (x), p.ap_fileName); }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void writesf_tilde_start (t_writesf_tilde *x)
{
    t_error err = PD_ERROR_NONE;
    
    trylock_lock (&x->sf_mutex);
    
        err = (x->sf_thread == NULL); if (!err) { x->sf_run = 1; }
    
    trylock_unlock (&x->sf_mutex);
    
    if (err) { error_unexpected (cast_object (x), sym_writesf__tilde__, sym_start); }
}

static void writesf_tilde_stop (t_writesf_tilde *x)
{
    writesf_tilde_close (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_int *writesf_tilde_perform (t_int *w)
{
    t_writesf_tilde *x = (t_writesf_tilde *)(w[1]);
    t_sfvectors *t     = (t_sfvectors *)(w[2]);
    int n = (int)(w[3]);
    
    if (trylock_trylock (&x->sf_mutex) == 0) {
    //
    if (x->sf_run && x->sf_thread && !sfthread_isEnd (x->sf_thread)) {
    //
    int isBigEndian      = sfthread_isBigEndian (x->sf_thread);
    int numberOfChannels = t->s_size;
    int bytesPerSample   = sfthread_getBytesPerSample (x->sf_thread);
    int bytesPerFrame    = numberOfChannels * bytesPerSample;
    int32_t required     = bytesPerFrame * n;
    
    if (required < x->sf_bufferSize) {
    //
    soundfile_encode32 (numberOfChannels,
        t->s_v,
        x->sf_cached,
        n,
        0,
        bytesPerSample,
        isBigEndian,
        (float)1.0);
    
    {
        int32_t written = ringbuffer_write (sfthread_getBuffer (x->sf_thread), x->sf_cached, required);
        if (written != required) {
            sfthread_setCorrupted (x->sf_thread);
        }
    }
    //
    }
    //
    }
    
    trylock_unlock (&x->sf_mutex);
    //
    }
    
    return (w + 4);
}

static void writesf_tilde_dsp (t_writesf_tilde *x, t_signal **sp)
{
    int i;

    t_sfvectors *t = sfvectors_new (cast_object (x)); t->s_size = x->sf_numberOfChannels;
    
    PD_ASSERT (t->s_size <= SOUNDFILE_CHANNELS);
    
    for (i = 0; i < t->s_size; i++) { t->s_v[i] = sp[i]->s_vector; }

    dsp_add3 (writesf_tilde_perform, x, t, sp[0]->s_vectorSize);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void writesf_tilde_functionDismiss (t_object *z)
{
    writesf_tilde_dismiss ((t_writesf_tilde *)z);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *writesf_tilde_new (t_float f1, t_float f2)
{
    int i, n = PD_CLAMP ((int)f1, 1, SOUNDFILE_CHANNELS);
    int size = PD_CLAMP ((int)f2, WRITESF_BUFFER_SIZE * 4 * n, WRITESF_BUFFER_SIZE * 256 * n);

    t_writesf_tilde *x = (t_writesf_tilde *)pd_new (writesf_tilde_class);
    
    trylock_init (&x->sf_mutex);
    
    if (!PD_IS_POWER_2 (size)) { size = (int)PD_NEXT_POWER_2 (size); }
    
    x->sf_numberOfChannels  = n;
    x->sf_bufferSize        = size;
    x->sf_cached            = (unsigned char *)PD_MEMORY_GET (x->sf_bufferSize);
    
    for (i = 1; i < x->sf_numberOfChannels; i++) { inlet_newSignal (cast_object (x)); }

    return x;
}

static void writesf_tilde_dismiss (t_writesf_tilde *x)
{
    if (!x->sf_dismissed) {
    //
    x->sf_dismissed = 1;
    
    writesf_tilde_close (x);
    //
    }
}

static void writesf_tilde_free (t_writesf_tilde *x)
{
    writesf_tilde_dismiss (x);
    
    PD_MEMORY_FREE (x->sf_cached);
    
    trylock_destroy (&x->sf_mutex);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void writesf_tilde_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_writesf__tilde__,
            (t_newmethod)writesf_tilde_new, 
            (t_method)writesf_tilde_free,
            sizeof (t_writesf_tilde),
            CLASS_DEFAULT | CLASS_SIGNAL,
            A_DEFFLOAT,
            A_DEFFLOAT,
            A_NULL);
        
    class_addDsp (c, (t_method)writesf_tilde_dsp);
        
    class_addMethod (c, (t_method)writesf_tilde_start,  sym_start,  A_NULL);
    class_addMethod (c, (t_method)writesf_tilde_stop,   sym_stop,   A_NULL);
    class_addMethod (c, (t_method)writesf_tilde_open,   sym_open,   A_GIMME, A_NULL);

    class_setDismissFunction (c, writesf_tilde_functionDismiss);

    writesf_tilde_class = c;
}

PD_LOCAL void writesf_tilde_destroy (void)
{
    class_free (writesf_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
