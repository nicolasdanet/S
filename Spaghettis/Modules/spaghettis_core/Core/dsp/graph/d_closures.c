
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../d_fft.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void       chain_addClosure    (t_chain *, t_closure *);
t_closure  *chain_fetchClosure (t_chain *, t_object *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

enum {
    CLOSURES_SPACE      = 0,
    CLOSURES_VECTORS,
    CLOSURES_FFT,
    CLOSURES_RAW,
    CLOSURES_OBJECT,
    CLOSURES_BLOCK,
    CLOSURES_VINLET,
    CLOSURES_VOUTLET
    };

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void fft_stateRelease (t_FFTState *x)
{
    if (x->s_cache) { PD_MEMORY_FREE (x->s_cache); } x->s_size = 0;
}

void fft_stateInitialize (t_FFTState *x, int n)
{
    PD_ASSERT (n >= FFT_MINIMUM && n <= FFT_MAXIMUM);
    
    if (n > x->s_size) {
    //
    fft_stateRelease (x);

    x->s_cache = (double *)PD_MEMORY_GET (n * 2 * sizeof (double));
    x->s_size  = n;
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Return if initialization is required (i.e. encapsulation). */

int object_dspNeedInitializer (t_object *x)
{
    t_id u = object_getUnique (x);
    t_id s = object_getSource (x);
    t_id n = object_getNative (x);
    
    if (n != u || s != u) { return 1; }
    
    return 0;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_closure *closure_fetch (t_object *owner, int type)
{
    t_chain *chain = instance_chainGetCurrent();
    
    if (chain) {
    //
    t_closure *c = chain_fetchClosure (chain, owner);
    
    if (c && c->s_type == type) { return c; }
    //
    }
    
    return NULL;
}

#if 0

t_space *space_fetch (t_object *o)
{
    return (t_space *)closure_fetch (o, CLOSURES_SPACE);
}

#endif

t_object *garbage_fetch (t_object *o)
{
    t_garbage *g = (t_garbage *)closure_fetch (o, CLOSURES_OBJECT);
    
    return g ? cast_object (g->s_ptr) : NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_space *space_new (t_object *owner)
{
    t_space *x = (t_space *)PD_MEMORY_GET (sizeof (t_space));
    
    ((t_closure *)x)->s_id   = object_getUnique (owner);
    ((t_closure *)x)->s_src  = object_getSource (owner);
    ((t_closure *)x)->s_type = CLOSURES_SPACE;
    
    chain_addClosure (instance_chainGetTemporary(), (t_closure *)x);
    
    return x;
}

t_sfvectors *sfvectors_new (t_object *owner)
{
    t_sfvectors *x = (t_sfvectors *)PD_MEMORY_GET (sizeof (t_sfvectors));
    
    ((t_closure *)x)->s_id   = object_getUnique (owner);
    ((t_closure *)x)->s_src  = object_getSource (owner);
    ((t_closure *)x)->s_type = CLOSURES_VECTORS;
    
    chain_addClosure (instance_chainGetTemporary(), (t_closure *)x);
    
    return x;
}

t_FFTState *fftstate_new (t_object *owner, int n)
{
    t_FFTState *x = (t_FFTState *)PD_MEMORY_GET (sizeof (t_FFTState));
    
    ((t_closure *)x)->s_id   = object_getUnique (owner);
    ((t_closure *)x)->s_src  = object_getSource (owner);
    ((t_closure *)x)->s_type = CLOSURES_FFT;
    
    fft_stateInitialize (x, n);
    
    chain_addClosure (instance_chainGetTemporary(), (t_closure *)x);
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_blockclosure *block_newClosure (t_object *owner)
{
    t_blockclosure *x = (t_blockclosure *)PD_MEMORY_GET (sizeof (t_blockclosure));
    
    ((t_closure *)x)->s_id   = object_getUnique (owner);
    ((t_closure *)x)->s_src  = object_getSource (owner);
    ((t_closure *)x)->s_type = CLOSURES_BLOCK;
    
    chain_addClosure (instance_chainGetTemporary(), (t_closure *)x);
    
    return x;
}

t_vinletclosure *vinlet_newClosure (t_object *owner)
{
    t_vinletclosure *x = (t_vinletclosure *)PD_MEMORY_GET (sizeof (t_vinletclosure));
    
    ((t_closure *)x)->s_id   = object_getUnique (owner);
    ((t_closure *)x)->s_src  = object_getSource (owner);
    ((t_closure *)x)->s_type = CLOSURES_VINLET;
    
    chain_addClosure (instance_chainGetTemporary(), (t_closure *)x);
    
    return x;
}

t_voutletclosure *voutlet_newClosure (t_object *owner)
{
    t_voutletclosure *x = (t_voutletclosure *)PD_MEMORY_GET (sizeof (t_voutletclosure));
    
    ((t_closure *)x)->s_id   = object_getUnique (owner);
    ((t_closure *)x)->s_src  = object_getSource (owner);
    ((t_closure *)x)->s_type = CLOSURES_VOUTLET;
    
    chain_addClosure (instance_chainGetTemporary(), (t_closure *)x);
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void garbage_newRaw (void *m)
{
    t_chain *chain = instance_chainGetCurrent();
    
    PD_ASSERT (m);
    
    if (!chain) { PD_MEMORY_FREE (m); }
    else {
    //
    t_garbage *x = (t_garbage *)PD_MEMORY_GET (sizeof (t_garbage));
    
    ((t_closure *)x)->s_id   = 0;
    ((t_closure *)x)->s_type = CLOSURES_RAW;
    
    x->s_ptr = m;
    
    chain_addClosure (chain, (t_closure *)x);
    //
    }
}

int garbage_newObject (t_object *o)
{
    t_chain *chain = instance_chainGetCurrent();
    
    PD_ASSERT (o);
    
    if (chain) {
    //
    t_garbage *x = (t_garbage *)PD_MEMORY_GET (sizeof (t_garbage));
    
    ((t_closure *)x)->s_id   = object_getUnique (o);
    ((t_closure *)x)->s_src  = object_getSource (o);
    ((t_closure *)x)->s_type = CLOSURES_OBJECT;
    
    x->s_ptr = (void *)o;
    
    if (class_hasDismissFunction (pd_class (o))) { (*class_getDismissFunction (pd_class (o))) (o); }
    
    chain_addClosure (chain, (t_closure *)x);
    
    return 1;
    //
    }
    
    return 0;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void closure_free (t_closure *x)
{
    if (x->s_type == CLOSURES_FFT) { fft_stateRelease ((t_FFTState *)x); }
    
    if (x->s_type == CLOSURES_RAW) {
        void *t = ((t_garbage *)x)->s_ptr; if (t) { PD_MEMORY_FREE (t); }
    }
    
    if (x->s_type == CLOSURES_OBJECT) {
        void *t = ((t_garbage *)x)->s_ptr; if (t) { pd_free (cast_pd (t)); }
    }
    
    PD_MEMORY_FREE (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
