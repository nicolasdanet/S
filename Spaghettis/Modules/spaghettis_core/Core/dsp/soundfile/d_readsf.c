
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

static t_class *readsf_tilde_class;                                 /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _readsf_tilde {
    t_object            sf_obj;                                     /* Must be the first. */
    t_trylock           sf_mutex;
    int                 sf_numberOfChannels;
    int                 sf_bufferSize;
    int                 sf_run;
    int                 sf_dismissed;
    t_sfthread          *sf_thread;
    unsigned char       *sf_cached;
    t_clock             *sf_clock;
    t_outlet            *sf_audioOutlets[SOUNDFILE_CHANNELS];
    t_outlet            *sf_outletTopRight;
    } t_readsf_tilde;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void readsf_tilde_dismiss (t_readsf_tilde *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define READSF_BUFFER_SIZE      65536                               /* Power of two. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void readsf_tilde_close (t_readsf_tilde *x)
{
    t_sfthread *sfthread = x->sf_thread;
    
    trylock_lock (&x->sf_mutex);
    
        x->sf_thread = NULL;
        x->sf_run    = 0;
    
    trylock_unlock (&x->sf_mutex);
    
    PD_MEMORY_BARRIER;
    
    sfthread_release (sfthread);
}

static void readsf_tilde_open (t_readsf_tilde *x, t_symbol *s, int argc, t_atom *argv)
{
    readsf_tilde_close (x);
    
    {
    //
    t_glist *owner = object_getOwner (cast_object (x));
    
    t_audioproperties p; soundfile_propertiesInit (&p);
    
    t_error err = soundfile_readFileParse (owner,
                        sym_readsf__tilde__,
                        &argc,
                        &argv,
                        &p,
                        cast_object (x));
    
    if (!err) {
    //
    t_fileproperties t; fileproperties_init (&t);
    
    err = !(glist_fileExist (owner, p.ap_fileName->s_name, p.ap_fileExtension->s_name, &t));
    
    if (!err) {
    //
    int f = soundfile_readFileHeader (owner, &p);
    
    err = (f < 0);
    
    if (!err) {
    
        t_sfthread *sfthread = sfthread_new (cast_object (x), SFTHREAD_READER, x->sf_bufferSize, f, &p);
        
        PD_MEMORY_BARRIER;
        
        trylock_lock (&x->sf_mutex);
        
            x->sf_thread = sfthread;
        
        trylock_unlock (&x->sf_mutex);
        
        PD_ASSERT (sfthread);
    }
    //
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

static void readsf_tilde_start (t_readsf_tilde *x)
{
    t_error err = PD_ERROR_NONE;
    
    trylock_lock (&x->sf_mutex);
    
        err = (x->sf_thread == NULL); if (!err) { x->sf_run = 1; }
    
    trylock_unlock (&x->sf_mutex);
    
    if (err) { error_unexpected (cast_object (x), sym_readsf__tilde__, sym_start); }
}

static void readsf_tilde_stop (t_readsf_tilde *x)
{
    readsf_tilde_close (x);
}

static void readsf_tilde_float (t_readsf_tilde *x, t_float f)
{
    if (f != 0.0) { readsf_tilde_start (x); }
    else {
        readsf_tilde_stop (x);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_int *readsf_tilde_perform (t_int *w)
{
    t_readsf_tilde *x = (t_readsf_tilde *)(w[1]);
    t_sfvectors *t    = (t_sfvectors *)(w[2]);
    int n = (int)(w[3]);
    
    int numberOfFramesRead = 0;
    int isEnd = 0;
    
    if (trylock_trylock (&x->sf_mutex)) {
    //
    if (x->sf_run && x->sf_thread) {
    //
    int isBigEndian      = sfthread_isBigEndian (x->sf_thread);
    int numberOfChannels = sfthread_getNumberOfChannels (x->sf_thread);
    int bytesPerSample   = sfthread_getBytesPerSample (x->sf_thread);
    int bytesPerFrame    = numberOfChannels * bytesPerSample;
    int32_t required     = bytesPerFrame * n;
    int32_t loaded       = ringbuffer_read (sfthread_getBuffer (x->sf_thread), x->sf_cached, required);
    
    if (loaded == 0 && sfthread_isEnd (x->sf_thread)) { x->sf_run = 0; isEnd = 1; }
    else {
        
        numberOfFramesRead = (int)(loaded / bytesPerFrame);
        
        soundfile_decode32 (numberOfChannels,
            t->s_v,
            x->sf_cached,
            numberOfFramesRead,
            0,
            bytesPerSample,
            isBigEndian,
            t->s_size);
    }
    //
    }
    
    trylock_unlock (&x->sf_mutex);
    //
    }
    
    {
    //
    int i, j;
    
    for (i = 0; i < t->s_size; i++) {
        PD_RESTRICTED out = t->s_v[i]; for (j = numberOfFramesRead; j < n; j++) { out[j] = 0.0; }
    }
    //
    }
    
    if (isEnd) { clock_delay (x->sf_clock, 0.0); }
    
    return (w + 4);
}

static void readsf_tilde_dsp (t_readsf_tilde *x, t_signal **sp)
{
    int i;
    
    t_sfvectors *t = sfvectors_new (cast_object (x)); t->s_size = x->sf_numberOfChannels;
    
    PD_ASSERT (t->s_size <= SOUNDFILE_CHANNELS);
    
    for (i = 0; i < t->s_size; i++) { t->s_v[i] = sp[i]->s_vector; }
    
    dsp_add3 (readsf_tilde_perform, x, t, sp[0]->s_vectorSize);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void readsf_tilde_task (t_readsf_tilde *x)
{
    if (!x->sf_dismissed) { readsf_tilde_stop (x); outlet_bang (x->sf_outletTopRight); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void readsf_tilde_functionDismiss (t_object *z)
{
    readsf_tilde_dismiss ((t_readsf_tilde *)z);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *readsf_tilde_new (t_float f1, t_float f2)
{
    int i, n = PD_CLAMP ((int)f1, 1, SOUNDFILE_CHANNELS);
    int size = PD_CLAMP ((int)f2, READSF_BUFFER_SIZE * 4 * n, READSF_BUFFER_SIZE * 256 * n);
    
    t_readsf_tilde *x = (t_readsf_tilde *)pd_new (readsf_tilde_class);
    
    if (!PD_IS_POWER_2 (size)) { size = (int)PD_NEXT_POWER_2 (size); }
    
    trylock_init (&x->sf_mutex);
    
    x->sf_numberOfChannels  = n;
    x->sf_bufferSize        = size;
    x->sf_cached            = (unsigned char *)PD_MEMORY_GET (x->sf_bufferSize);
    x->sf_clock             = clock_new ((void *)x, (t_method)readsf_tilde_task);
    
    for (i = 0; i < n; i++) { x->sf_audioOutlets[i] = outlet_newSignal (cast_object (x)); }
    
    x->sf_outletTopRight = outlet_newBang (cast_object (x));
    
    return x;
}

static void readsf_tilde_dismiss (t_readsf_tilde *x)
{
    if (!x->sf_dismissed) {
    //
    x->sf_dismissed = 1;
    
    readsf_tilde_close (x);
    //
    }
}

static void readsf_tilde_free (t_readsf_tilde *x)
{
    readsf_tilde_dismiss (x);
    
    clock_free (x->sf_clock);
    
    PD_MEMORY_FREE (x->sf_cached);
    
    trylock_destroy (&x->sf_mutex);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void readsf_tilde_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_readsf__tilde__,
            (t_newmethod)readsf_tilde_new, 
            (t_method)readsf_tilde_free,
            sizeof (t_readsf_tilde),
            CLASS_DEFAULT, 
            A_DEFFLOAT, 
            A_DEFFLOAT,
            A_NULL);
    
    class_addDsp (c, (t_method)readsf_tilde_dsp);
    class_addFloat (c, (t_method)readsf_tilde_float);
    
    class_addMethod (c, (t_method)readsf_tilde_start,   sym_start,  A_NULL);
    class_addMethod (c, (t_method)readsf_tilde_stop,    sym_stop,   A_NULL);
    class_addMethod (c, (t_method)readsf_tilde_open,    sym_open,   A_GIMME, A_NULL);
    
    class_setDismissFunction (c, readsf_tilde_functionDismiss);

    readsf_tilde_class = c;
}

void readsf_tilde_destroy (void)
{
    class_free (readsf_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
