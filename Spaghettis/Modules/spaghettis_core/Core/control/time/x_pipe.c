
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../x_atomoutlet.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *pipe_class;                         /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _pipecallback {
    t_atom                  *h_atoms;
    t_clock                 *h_clock;
    struct _pipe            *h_owner;
    struct _pipecallback    *h_next;
    } t_pipecallback;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _pipe {
    t_object                x_obj;                  /* Must be the first. */
    t_float                 x_delay;
    int                     x_size;
    t_float                 x_unitValue;
    t_symbol                *x_unitName;
    t_atomoutlet            *x_vector;
    t_pipecallback          *x_callbacks;
    } t_pipe;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error clock_parseUnit (t_float, t_symbol *, t_float *, int *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static void callback_free (t_pipecallback *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void callback_ownership (t_pipecallback *h, t_pipe *x)
{
    h->h_owner = x;
}

static void callback_task (t_pipecallback *h)
{
    t_pipe *owner = h->h_owner;
    int i;
    
    if (owner->x_callbacks == h) { owner->x_callbacks = h->h_next; }
    else {
        t_pipecallback *m = NULL;
        t_pipecallback *n = NULL;
        for ((m = owner->x_callbacks); (n = m->h_next); (m = n)) { 
            if (n == h) { m->h_next = n->h_next; break; }
        }
    }
    
    for (i = owner->x_size - 1; i >= 0; i--) {
        t_error err = atomoutlet_broadcastIfTypeMatch (owner->x_vector + i, h->h_atoms + i);
        PD_UNUSED (err); PD_ASSERT (!err);
    }
    
    callback_free (h);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void callback_new (t_pipe *x, int argc, t_atom *argv)
{
    t_pipecallback *h = (t_pipecallback *)PD_MEMORY_GET (sizeof (t_pipecallback));
    int i;
       
    h->h_atoms = (t_atom *)PD_MEMORY_GET (x->x_size * sizeof (t_atom));
    h->h_clock = clock_newSingle ((void *)h, (t_method)callback_task);
    h->h_owner = x;
    h->h_next  = x->x_callbacks;
    
    if (x->x_unitName) {
        t_error err = clock_setUnitParsed (h->h_clock, x->x_unitValue, x->x_unitName);
        PD_UNUSED (err); PD_ASSERT (!err);
    }
    
    for (i = 0; i < x->x_size; i++) { atomoutlet_copyAtom (x->x_vector + i, h->h_atoms + i); }
    
    x->x_callbacks = h;

    clock_delay (h->h_clock, PD_MAX (x->x_delay, 0.0));
}

static void callback_free (t_pipecallback *h)
{
    clock_free (h->h_clock);
    PD_MEMORY_FREE (h->h_atoms);
    PD_MEMORY_FREE (h);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static int pipe_unitIsValid (t_pipe *x, t_float f, t_symbol *unitName, int verbose)
{
    if (f != 0.0 && unitName != &s_) {
    //
    int n; t_float t; t_error err = clock_parseUnit (f, unitName, &t, &n);
    
    if (err && verbose) { error_invalid (cast_object (x), sym_pipe, sym_unit); }
    
    return (!err);
    //
    }
    
    return 0;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void pipe_set (t_pipe *x, int argc, t_atom *argv)
{
    int i;
    
    for (i = 0; i < argc; i++) {
    //
    if (!atom_typesAreEquals (atomoutlet_getAtom (x->x_vector + i), argv + i)) {
        error_mismatch (cast_object (x), sym_pipe, sym_type);
        return;
    }
    //
    }

    for (i = 0; i < argc; i++) { atomoutlet_setAtom (x->x_vector + i, argv + i); }
}

static void pipe_list (t_pipe *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc > x->x_size) {
    //
    if (IS_FLOAT (argv + x->x_size)) { x->x_delay = GET_FLOAT (argv + x->x_size); }
    else {
        error_invalid (cast_object (x), sym_pipe, sym_delay);
        return;
    }
    //
    }
    
    argc = PD_MIN (argc, x->x_size);
    
    pipe_set (x, argc, argv); callback_new (x, argc, argv);
}

static void pipe_anything (t_pipe *x, t_symbol *s, int argc, t_atom *argv)
{
    utils_anythingToList (cast_pd (x), (t_listmethod)pipe_list, s, argc, argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void pipe_flush (t_pipe *x)      /* FIFO. */
{
    while (x->x_callbacks) {
    //
    t_pipecallback *m = NULL;
    t_pipecallback *n = NULL;
    
    for ((m = x->x_callbacks); (n = m->h_next); (m = n)) { }
    
    callback_task (m);
    //
    }
}

static void pipe_clear (t_pipe *x)
{
    t_pipecallback *h = NULL; while ((h = x->x_callbacks)) { x->x_callbacks = h->h_next; callback_free (h); }
}

/* Note that float arguments are always passed at last. */

static void pipe_unit (t_pipe *x, t_symbol *unitName, t_float f)
{
    if (pipe_unitIsValid (x, f, unitName, 1)) { x->x_unitValue = f; x->x_unitName = unitName; }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *pipe_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_pipe *x   = (t_pipe *)z;
    t_buffer *b = buffer_new();
    
    if (x->x_unitName) {
        buffer_appendSymbol (b, sym_unit);
        buffer_appendFloat (b,  x->x_unitValue);
        buffer_appendSymbol (b, x->x_unitName);
        buffer_appendComma (b);
    }
    
    buffer_appendSymbol (b, sym__restore);
    buffer_appendFloat (b, x->x_delay);
    
    int i;
    
    for (i = 0; i < x->x_size; i++) {
        buffer_appendAtom (b, atomoutlet_getAtom (x->x_vector + i));
    }
    
    return b;
    //
    }
    
    return NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void pipe_restoreOwnership (t_pipe *x)
{
    t_pipecallback *h = x->x_callbacks; while (h) { callback_ownership (h, x); h = h->h_next; }
}

static void pipe_restoreEncapsulation (t_pipe *x, t_pipe *old)
{
    int i;
    
    x->x_delay     = old->x_delay;
    x->x_unitValue = old->x_unitValue;
    x->x_unitName  = old->x_unitName;
    
    PD_ASSERT (x->x_size == old->x_size);
    
    for (i = 0; i < x->x_size; i++) {
        t_atom *a = atomoutlet_getAtom (old->x_vector + i); atomoutlet_setAtom (x->x_vector + i, a);
    }
    
    x->x_callbacks = old->x_callbacks; old->x_callbacks = NULL; pipe_restoreOwnership (x);
}

static void pipe_restore (t_pipe *x, t_symbol *s, int argc, t_atom *argv)
{
    t_pipe *old = (t_pipe *)instance_pendingFetch (cast_object (x));
    
    if (old) { pipe_restoreEncapsulation (x, old); }
    else {
    //
    x->x_delay = atom_getFloatAtIndex (0, argc, argv); if (argc > 1) { pipe_set (x, argc - 1, argv + 1); }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *pipe_new (t_symbol *s, int argc, t_atom *argv)
{
    t_pipe *x = (t_pipe *)pd_new (pipe_class);
    int i;
    
    x->x_delay      = 0.0;
    x->x_unitValue  = 0.0;
    x->x_unitName   = NULL;
    
    if (argc > 2) {
    //
    t_float f = atom_getFloat (argv + (argc - 2));
    t_symbol *unitName = atom_getSymbol (argv + (argc - 1));
    if (pipe_unitIsValid (x, f, unitName, 0)) {
        x->x_unitValue  = f;
        x->x_unitName   = unitName;
        argc -= 2;
    }
    //
    }
    
    if (argc) {
    //
    t_atom *a = argv + (argc - 1); if (IS_FLOAT (a)) { x->x_delay = GET_FLOAT (a); argc--; }
    //
    }

    x->x_size   = PD_MAX (1, argc);
    x->x_vector = (t_atomoutlet *)PD_MEMORY_GET (x->x_size * sizeof (t_atomoutlet));

    if (!argc) {
        atomoutlet_makeFloat (x->x_vector + 0, cast_object (x), ATOMOUTLET_OUTLET, 0.0);
        
    } else {

        for (i = 0; i < argc; i++) {
            int create = (i != 0) ? ATOMOUTLET_BOTH : ATOMOUTLET_OUTLET;
            atomoutlet_makeParsed (x->x_vector + i, cast_object (x), create, argv + i);
        }
    }
    
    inlet_newFloat (cast_object (x), &x->x_delay);
    
    x->x_callbacks = NULL;

    return x;
}

static void pipe_free (t_pipe *x)
{
    pipe_clear (x); PD_MEMORY_FREE (x->x_vector);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void pipe_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_pipe, 
            (t_newmethod)pipe_new,
            (t_method)pipe_free,
            sizeof (t_pipe),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
            
    class_addList (c, (t_method)pipe_list);
    class_addAnything (c, (t_method)pipe_anything);
    
    class_addMethod (c, (t_method)pipe_flush,   sym_flush,      A_NULL);
    class_addMethod (c, (t_method)pipe_clear,   sym_clear,      A_NULL);
    class_addMethod (c, (t_method)pipe_unit,    sym_unit,       A_FLOAT, A_SYMBOL, A_NULL);
    class_addMethod (c, (t_method)pipe_unit,    sym_tempo,      A_FLOAT, A_SYMBOL, A_NULL);

    class_addMethod (c, (t_method)pipe_restore, sym__restore,   A_GIMME, A_NULL);

    class_setDataFunction (c, pipe_functionData);
    class_requirePending (c);
    
    pipe_class = c;
}

void pipe_destroy (void)
{
    class_free (pipe_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
