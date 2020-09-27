
/* Copyright (c) 1997-2020 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

struct _chain {
    t_pd            x_pd;               /* Must be the first. */
    t_id            x_identifier;
    int64_t         x_phase;
    int             x_quantum;
    int             x_size;
    t_int           *x_chain;
    t_signal        *x_signals;
    t_closure       *x_closures;
    t_initializer   *x_initializers;
    };

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL int    instance_isChainSafeToDelete    (t_chain *);
PD_LOCAL void   instance_chainSetInitialized    (void);
PD_LOCAL void   initializer_proceed             (t_initializer *x);
PD_LOCAL void   initializer_free                (t_initializer *x);
PD_LOCAL void   closure_free                    (t_closure *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_class *chain_class;        /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_int chain_done (t_int *dummy)
{
    return 0;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_id chain_getIdentifier (t_chain *x)
{
    return x->x_identifier;
}

PD_LOCAL int chain_getSize (t_chain *x)
{
    return x->x_size;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_EXPORT void chain_append (t_chain *x, t_perform f, int n, ...)
{
    int size = x->x_size + n + 1;
    
    PD_ASSERT (x->x_chain != NULL);
    
    size_t newSize = sizeof (t_int) * size;
    size_t oldSize = sizeof (t_int) * x->x_size;
    
    x->x_chain = (t_int *)PD_MEMORY_RESIZE (x->x_chain, oldSize, newSize);
    
    {
    //
    int i;
    va_list ap;

    x->x_chain[x->x_size - 1] = (t_int)f;

    va_start (ap, n);
    
    for (i = 0; i < n; i++) { x->x_chain[x->x_size + i] = va_arg (ap, t_int); }
    
    va_end (ap);
    //
    }
    
    x->x_chain[size - 1] = (t_int)chain_done; x->x_size = size;
}

static void chain_initialize (t_chain *x)
{
    t_initializer *i = x->x_initializers; while (i) { initializer_proceed (i); i = i->s_next; }
}

PD_LOCAL void chain_tick (t_chain *x)
{
    t_int *t = x->x_chain;

    if (x->x_phase == 0) { chain_initialize (x); instance_chainSetInitialized(); }
    
    if (t) {
    //
    // PD_LOG ("#");
    // PD_LOG_NUMBER (x->x_phase);
    // PD_LOG ("#");
    
    while (t) { t = (*(t_perform)(*t))(t); }
    //
    }
    
    x->x_phase++;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void chain_addSignal (t_chain *x, t_signal *s)
{
    s->s_next = x->x_signals; x->x_signals = s;
}

PD_LOCAL void chain_addClosure (t_chain *x, t_closure *s)
{
    s->s_next = x->x_closures; x->x_closures = s;
}

PD_LOCAL void chain_addInitializer (t_chain *x, t_initializer *s)
{
    s->s_next = x->x_initializers; x->x_initializers = s;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_closure *chain_fetchClosure (t_chain *x, t_object *o)
{
    t_closure *c = x->x_closures;
    t_id u = object_getUnique (o);
    t_id s = object_getSource (o);
    
    while (c) {
    //
    if (c->s_id == u || c->s_src == u || c->s_id == s || c->s_src == s) { return c; } else { c = c->s_next; }
    //
    }
    
    return NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void chain_setQuantum (t_chain *x, int n)
{
    if (n >= INTERNAL_BLOCKSIZE) {
    //
    int t = n / INTERNAL_BLOCKSIZE; PD_ASSERT (PD_IS_POWER_2 (n));
    
    x->x_quantum = PD_MAX (x->x_quantum, t);
    //
    }
}

PD_LOCAL int chain_hasQuantumRemaining (t_chain *x)
{
    int k = ((x->x_phase % x->x_quantum) != 0); return k;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_chain *chain_new (void)
{
    t_chain *x = (t_chain *)pd_new (chain_class);
    
    x->x_identifier = utils_unique();
    x->x_size       = 1;
    x->x_chain      = (t_int *)PD_MEMORY_GET (sizeof (t_int));
    x->x_chain[0]   = (t_int)chain_done;
    
    chain_setQuantum (x, INTERNAL_BLOCKSIZE);
    
    return x;
}

static void chain_free (t_chain *x)
{
    t_signal *signal           = NULL;
    t_closure *closure         = NULL;
    t_initializer *initializer = NULL;
    
    while ((signal = x->x_signals)) {
    //
    x->x_signals = signal->s_next;
    
    if (!signal->s_hasBorrowed) { PD_MEMORY_FREE (signal->s_vector); }
    else {
        PD_MEMORY_FREE (signal->s_unused);
    }
    
    PD_MEMORY_FREE (signal);
    //
    }
    
    while ((closure = x->x_closures)) {
        x->x_closures = closure->s_next; closure_free (closure);
    }
    
    while ((initializer = x->x_initializers)) {
        x->x_initializers = initializer->s_next; initializer_free (initializer);
    }
    
    PD_MEMORY_FREE (x->x_chain);
}

PD_LOCAL void chain_release (t_chain *x)
{
    instance_autoreleaseRegister (cast_pd (x));
}

static void chain_autorelease (t_chain *x)
{
    if (instance_isChainSafeToDelete (x)) { instance_autoreleaseProceed (cast_pd (x)); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void chain_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_chain,
            NULL,
            (t_method)chain_free,
            sizeof (t_chain),
            CLASS_INVISIBLE,
            A_NULL);
    
    class_addAutorelease (c, (t_method)chain_autorelease);
    
    chain_class = c;
}

PD_LOCAL void chain_destroy (void)
{
    class_free (chain_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
