
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define INSTANCE_TIME_CHAIN     PD_SECONDS_TO_MILLISECONDS (30)

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define INSTANCE_GRID_DEFAULT   12

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_pdinstance *pd_this;          /* Static. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void   canvas_new                      (void *, t_symbol *, int, t_atom *);
void   canvas_view                     (void *, t_symbol *, int, t_atom *);
void   legacy_start                    (void *, t_symbol *, int, t_atom *);
int    dspthread_isChainSafeToDelete   (t_dspthread *, t_chain *);
void   glist_setNext                   (t_glist *, t_glist *);
int    loader_load                     (t_glist *, t_symbol *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_glist *instance_contextGetCurrent (void)
{
    return cast_glist (instance_getBoundX());
}

t_dspcontext *instance_contextGetUgen (void)
{
    return instance_get()->pd_ugenContext;
}

void instance_contextSetCurrent (t_glist *glist)
{
    instance_setBoundX (cast_pd (glist));
}

void instance_contextSetUgen (t_dspcontext *context)
{
    instance_get()->pd_ugenContext = context;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_pd *instance_getMakerObject (void)
{
    return &(instance_get()->pd_objectMaker);
}

t_class *instance_getMakerObjectClass (void)
{
    return (instance_get()->pd_objectMaker);
}

int instance_isMakerObject (t_pd *x)
{
    return (x == instance_getMakerObject());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int instance_undoIsRecursive (void)
{
    return (instance_get()->pd_isUndoRecursive > 0);
}

void instance_undoSetRecursive (void)
{
    instance_get()->pd_isUndoRecursive++;
}

void instance_undoUnsetRecursive (void)
{
    instance_get()->pd_isUndoRecursive--;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int instance_hasPending (void)
{
    return (instance_get()->pd_hasPending != 0);
}

void instance_pendingBegin (void)
{
    instance_get()->pd_hasPending++;
}

void instance_pendingEnd (void)
{
    instance_get()->pd_hasPending--; if (instance_get()->pd_hasPending == 0) { instance_pendingRelease(); }
}

void instance_pendingRelease (void)
{
    t_object *y = instance_get()->pd_pending; instance_get()->pd_pending = NULL;
    
    while (y) { t_object *t = y; y = y->g_next; pd_free (cast_pd (t)); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_pd *instance_objectGetNewest (void)
{
    return instance_get()->pd_newest;
}

void instance_objectSetNewest (t_pd *x)
{
    instance_get()->pd_newest = x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void instance_rootsAdd (t_glist *glist)
{
    glist_setNext (glist, instance_get()->pd_roots); instance_get()->pd_roots = glist;
    
    instance_registerAdd (cast_object (glist));
}

void instance_rootsRemove (t_glist *glist)
{
    instance_registerRemove (cast_object (glist));
    
    if (glist == instance_get()->pd_roots) { instance_get()->pd_roots = glist_getNext (glist); }
    else {
        t_glist *z = NULL;
        for (z = instance_get()->pd_roots; glist_getNext (z) != glist; z = glist_getNext (z)) { }
        glist_setNext (z, glist_getNext (glist));
    }
}

void instance_rootsFreeAll (void)
{    
    while (1) {

        t_glist *glist = instance_get()->pd_roots;
        
        if (glist == NULL) { break; }
        else {
            glist_close (glist);
            if (glist == instance_get()->pd_roots) {    /* Not removed? */
                PD_BUG; break;
            }   
        }
    }
}

static t_glist *instance_rootsGet (void)
{
    return instance_get()->pd_roots;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void instance_registerAdd (t_object *o)
{
    register_add (instance_get()->pd_register, o);
    
    #if defined ( PD_BUILDING_APPLICATION )
    
    outputs_objectAdded (o);
    
    #endif
}

t_error instance_registerRemove (t_object *o)
{
    t_error err = PD_ERROR_NONE;
    
    #if defined ( PD_BUILDING_APPLICATION )
    
    outputs_objectRemoved (o);
    
    #endif
    
    err = register_remove (instance_get()->pd_register, object_getUnique (o));
    
    PD_ASSERT (!err);
    
    return err;
}

void instance_registerRename (t_object *o, t_id newUnique)
{
    #if defined ( PD_BUILDING_APPLICATION )
    
    outputs_objectRenamed (o, newUnique);
    
    #endif
    
    register_rename (instance_get()->pd_register, object_getUnique (o), newUnique);
}

int instance_registerContains (t_id u)
{
    return register_contains (instance_get()->pd_register, u);
}

t_object *instance_registerGetObject (t_id u)
{
    return register_getObject (instance_get()->pd_register, u);
}

t_glist *instance_registerGetOwner (t_id u)
{
    return register_getOwner (instance_get()->pd_register, u);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void instance_clocksAdd (t_clock *c)
{
    clocks_add (instance_get()->pd_clocks, c);
}

void instance_clocksRemove (t_clock *c)
{
    clocks_remove (instance_get()->pd_clocks, c);
}

void instance_clocksDestroy (t_clock *c)
{
    PD_ASSERT (sys_isControlThread());
    
    clocks_destroy (instance_get()->pd_clocks, c);
}

void instance_clocksTick (t_systime t)
{
    PD_ASSERT (sys_isControlThread());
    
    clocks_tick (instance_get()->pd_clocks, t);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_chain *instance_chainGetCurrent (void)
{
    return (t_chain *)atomic_pointerRead (&instance_get()->pd_chain);
}

static void instance_chainSetCurrent (t_chain *chain)
{
    t_chain *oldChain = (t_chain *)atomic_pointerSwap (&instance_get()->pd_chain, (void *)chain);
    
    if (oldChain) { chain_release (oldChain); }
}

t_chain *instance_chainGetTemporary (void)
{
    t_chain *chain = instance_get()->pd_build; PD_ASSERT (chain); return chain;
}

static void instance_chainStartTemporary (void)
{
    atomic_int32Write (&instance_get()->pd_chainRetain, 1);

    PD_ASSERT (instance_get()->pd_build == NULL); instance_get()->pd_build = chain_new();
}

static void instance_chainPushTemporary (void)
{
    instance_chainSetCurrent (instance_chainGetTemporary()); instance_get()->pd_build = NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int instance_isChainSafeToDelete (t_chain *chain)
{
    if (atomic_int32Read (&instance_get()->pd_chainRetain)) { return 0; }
    
    return dspthread_isChainSafeToDelete (instance_get()->pd_dsp, chain);
}

void instance_chainSetInitialized (void)
{
    atomic_int32Write (&instance_get()->pd_chainRetain, 0);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error instance_dspCreate (void)
{
    return dspthread_create (instance_get()->pd_dsp);
}

void instance_dspStart (void)
{
    t_glist *glist;

    PD_ASSERT (instance_contextGetUgen() == NULL);
    
    instance_chainStartTemporary();
    
    for (glist = instance_rootsGet(); glist; glist = glist_getNext (glist)) {
        canvas_dspProceed (glist, 1, NULL); 
    }
    
    instance_chainPushTemporary();
    
    dspthread_run (instance_get()->pd_dsp);
}

void instance_dspStop (void)
{
    dspthread_stop (instance_get()->pd_dsp);
}

void instance_dspClean (void)
{
    if (instance_chainGetCurrent()) {
    //
    t_systime t = dspthread_time (instance_get()->pd_dsp);
    
    if (t && scheduler_getMillisecondsSince (t) > INSTANCE_TIME_CHAIN) {
    //
    if (atomic_int32Read (&instance_get()->pd_chainRetain) == 0) {
    //
    instance_chainSetCurrent (NULL);
    //
    }
    //
    }
    //
    }
}

void instance_dspFree (void)
{
    instance_chainSetCurrent (NULL); dspthread_free (instance_get()->pd_dsp);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void instance_audioCloseTask (void *dummy)
{
    int old = dsp_getState(); dsp_setState (0); if (old) { error_unexpected (NULL, sym_audio, sym_shutdown); }
}

void instance_audioCloseWithError (void)
{
    clock_delay (instance_get()->pd_stop, 0.0);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error instance_overflowPush (t_object *y)
{
    int count   = ++instance_get()->pd_overflowCount;
    t_error err = (count >= INSTANCE_OVERFLOW);
    
    if (err && !instance_get()->pd_overflow) { instance_get()->pd_overflow = 1; error_stackOverflow (y); }
    
    err |= instance_get()->pd_overflow;
    
    return err;
}

void instance_overflowPop (void)
{
    int count = --instance_get()->pd_overflowCount; if (count == 0) { instance_get()->pd_overflow = 0; }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Called if no method of the maker object match. */

static void instance_factory (t_pd *x, t_symbol *s, int argc, t_atom *argv)
{
    instance_get()->pd_newest = NULL;
    
    /* Note that it can be a recursive call. */
    
    if (!instance_get()->pd_isLoadingExternal) {

        /* First search an external. */
        
        if (loader_load (instance_contextGetCurrent(), s)) {
            instance_get()->pd_isLoadingExternal = 1;
            pd_message (x, s, argc, argv);              /* Try again. */
            instance_get()->pd_isLoadingExternal = 0;
        
        /* Otherwise look for an abstraction. */
        
        } else {
            instance_loadAbstraction (s, argc, argv);
        }
        
    } else {
        error_canNotFind (NULL, sym_loader, sym_class);   /* External MUST provide a properly named class. */
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_pdinstance *instance_new()
{
    t_pdinstance *x = (t_pdinstance *)PD_MEMORY_GET (sizeof (t_pdinstance));
    
    x->pd_stack.s_stack = (t_stackelement *)PD_MEMORY_GET (INSTANCE_STACK * sizeof (t_stackelement));
    
    x->pd_environment.env_directory = &s_;
    x->pd_environment.env_fileName  = &s_;
    
    x->pd_objectMaker = class_new (sym_objectmaker, NULL, NULL, 0, CLASS_ABSTRACT, A_NULL);
    x->pd_canvasMaker = class_new (sym_canvasmaker, NULL, NULL, 0, CLASS_ABSTRACT, A_NULL);
    x->pd_clocks      = clocks_new();
    x->pd_pending     = NULL;
    x->pd_register    = register_new();
    x->pd_view        = buffer_new();
    x->pd_pool        = buffer_new();
    x->pd_dsp         = dspthread_new();
    x->pd_stop        = clock_newSafe ((void *)x, (t_method)instance_audioCloseTask);
    x->pd_hasGrid     = 1;
    x->pd_gridSize    = INSTANCE_GRID_DEFAULT;
    
    class_addAnything (x->pd_objectMaker, (t_method)instance_factory);
    
    class_addMethod (x->pd_canvasMaker, (t_method)canvas_new,   sym_canvas,     A_GIMME, A_NULL);
    class_addMethod (x->pd_canvasMaker, (t_method)canvas_view,  sym_view,       A_GIMME, A_NULL);
    class_addMethod (x->pd_canvasMaker, (t_method)legacy_start, sym_spaghettis, A_GIMME, A_NULL);
    
    return x;
}

static void instance_free (t_pdinstance *x)
{
    PD_ASSERT (x->pd_roots       == NULL);
    PD_ASSERT (x->pd_polling     == NULL);
    PD_ASSERT (x->pd_autorelease == NULL);
    PD_ASSERT (x->pd_pending     == NULL);
    
    PD_ASSERT (buffer_getSize (x->pd_pool) == x->pd_poolCount);
    
    clock_free (x->pd_stop);
    buffer_free (x->pd_pool);
    buffer_free (x->pd_view);
    register_free (x->pd_register);
    clocks_free (x->pd_clocks);
    class_free (x->pd_canvasMaker);
    class_free (x->pd_objectMaker);
    
    PD_ASSERT (x->pd_stack.s_stackIndex == 0);
    
    if (x->pd_environment.env_argv) { PD_MEMORY_FREE (x->pd_environment.env_argv); }
    
    PD_MEMORY_FREE (x->pd_stack.s_stack);
    PD_MEMORY_FREE (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void instance_initialize (void)
{
    pd_this = instance_new();
    
    instance_setBoundN (&(instance_get()->pd_canvasMaker));
}

void instance_release (void)
{
    instance_setBoundA (NULL);
    instance_setBoundN (NULL);
    instance_setBoundX (NULL);
    
    instance_free (pd_this);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
