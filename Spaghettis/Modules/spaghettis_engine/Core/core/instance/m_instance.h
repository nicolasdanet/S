
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef M_INSTANCE_H_
#define M_INSTANCE_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* In the future, what should belongs to it and what should be kept global? */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _stackelement {
    t_glist         *s_context;
    t_symbol        *s_loadedAbstraction;
    } t_stackelement;

typedef struct _stack {
    int             s_stackIndex;
    t_stackelement  *s_stack;
    t_glist         *s_contextCached;
    t_glist         *s_contextPopped;
    } t_stack;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _pdinstance {
    t_stack         pd_stack;
    uint64_t        pd_pollingCount;
    uint64_t        pd_autoreleaseCount;
    int             pd_poolCount;
    int             pd_overflow;
    int             pd_overflowCount;
    int             pd_isLoadingExternal;
    int             pd_isUndoRecursive;
    int             pd_hasPending;
    int             pd_hasGrid;
    int             pd_gridSize;
    t_int32Atomic   pd_chainRetain;
    t_pointerAtomic pd_chain;
    t_chain         *pd_build;
    t_symbol        *pd_loadingAbstraction;
    t_dspcontext    *pd_ugenContext;
    t_clocks        *pd_clocks;
    t_glist         *pd_roots;
    t_environment   *pd_environment;
    t_clock         *pd_polling;
    t_clock         *pd_autorelease;
    t_clock         *pd_stop;
    t_pd            *pd_newest;
    t_class         *pd_objectMaker;
    t_class         *pd_canvasMaker;
    t_register      *pd_register;
    t_buffer        *pd_pending;
    t_buffer        *pd_view;
    t_buffer        *pd_pool;
    t_dspthread     *pd_dsp;
    } t_pdinstance;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define INSTANCE_STACK          1024        /* Arbitrary. */
#define INSTANCE_OVERFLOW       1000        /* Arbitrary. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

extern t_pdinstance *pd_this;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static inline t_pdinstance *instance_get (void)
{
    return pd_this;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* The #N symbol is bound to the patch factory. */
/* The #X symbol is bound to the context currently active. */
/* The #A symbol can be used to serialize things. */

static inline void instance_setBoundN (t_pd *x)
{
    s__N.s_thing = x;
}

static inline void instance_setBoundA (t_pd *x)
{
    s__A.s_thing = x;
}

static inline void instance_setBoundX (t_pd *x)
{
    s__X.s_thing = x;
}

static inline t_pd *instance_getBoundX (void)
{
    return s__X.s_thing;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void        instance_rootsAdd               (t_glist *glist);
void        instance_rootsRemove            (t_glist *glist);
void        instance_rootsFreeAll           (void);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void        instance_registerAdd            (t_object *o);
t_error     instance_registerRemove         (t_object *o);
void        instance_registerRename         (t_object *o, t_id newUnique);

int         instance_registerContains       (t_id u);

t_object    *instance_registerGetObject     (t_id u);
t_glist     *instance_registerGetOwner      (t_id u);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error     instance_dspCreate              (void);
void        instance_dspStart               (void);
void        instance_dspStop                (void);
void        instance_dspClean               (void);
void        instance_dspFree                (void);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void        instance_audioCloseWithError    (void);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_chain     *instance_chainGetCurrent       (void);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void        instance_clocksAdd              (t_clock *c);
void        instance_clocksRemove           (t_clock *c);
void        instance_clocksDestroy          (t_clock *c);
void        instance_clocksTick             (t_systime systime);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void        instance_pollingRun             (void);
void        instance_pollingStop            (void);
void        instance_pollingRegister        (t_pd *x);
void        instance_pollingUnregister      (t_pd *x);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void        instance_autoreleaseRun         (void);
void        instance_autoreleaseStop        (void);
void        instance_autoreleaseRegister    (t_pd *x);
void        instance_autoreleaseProceed     (t_pd *x);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void        instance_viewSet                (int argc, t_atom *argv);
void        instance_viewConsume            (t_glist *glist);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void        instance_patchNew               (t_symbol *name, t_symbol *directory);
void        instance_patchOpen              (t_symbol *name, t_symbol *directory);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void        instance_loadAbstraction        (t_symbol *name, int argc, t_atom *argv);
void        instance_loadSnippet            (t_glist *glist, t_buffer *b);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void        instance_stackPush              (t_glist *glist);
void        instance_stackPop               (t_glist *glist);
void        instance_stackPopPatch          (t_glist *glist);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_environment   *instance_environmentFetchIfAny     (void);

void        instance_environmentSetFile             (t_symbol *name, t_symbol *directory);
void        instance_environmentSetArguments        (int argc, t_atom *argv);
void        instance_environmentResetFile           (void);
void        instance_environmentResetArguments      (void);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error     instance_overflowPush           (t_object *y);
void        instance_overflowPop            (void);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_object    *instance_pendingFetch          (t_object *y);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

int         instance_hasPending             (void);

int         instance_pendingRequired        (t_object *y);
void        instance_pendingAdd             (t_object *y);

void        instance_pendingBegin           (void);
void        instance_pendingEnd             (void);
void        instance_pendingRelease         (void);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void        instance_snapSet                (int n);
void        instance_snapSetGrid            (int n);

int         instance_snapIsSet              (void);
int         instance_snapGetGrid            (void);
int         instance_snapGetSnapped         (int n);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_APPLICATION )

Point::Real    instance_snapped            (Point::Real pt);

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void        instance_contextSetCurrent      (t_glist *glist);
void        instance_contextSetUgen         (t_dspcontext *context);

t_dspcontext    *instance_contextGetUgen    (void);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_pd        *instance_getMakerObject        (void);
t_class     *instance_getMakerObjectClass   (void);

int         instance_isMakerObject          (t_pd *x);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_pd        *instance_objectGetNewest       (void);

void        instance_objectSetNewest        (t_pd *x);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int         instance_undoIsRecursive        (void);
void        instance_undoSetRecursive       (void);
void        instance_undoUnsetRecursive     (void);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // M_INSTANCE_H_
