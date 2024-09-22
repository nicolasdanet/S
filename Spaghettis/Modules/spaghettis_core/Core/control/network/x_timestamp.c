
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *timestamp_class;        /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _timestamp {
    t_object    x_obj;                  /* Must be the first. */
    int         x_mode;
    t_outlet    *x_outlet;
    } t_timestamp;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define TIMESTAMP_IMMEDIATELY           0
#define TIMESTAMP_DISCARD               1
#define TIMESTAMP_ELAPSED               2

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void timestamp_float (t_timestamp *x, t_float f)
{
    t_atom a[STAMP_TAGS_SIZE]; t_stamp t;
    
    t_nano ns = PD_MILLISECONDS_TO_NANOSECONDS (PD_MAX (0.0, f));
    
    stamp_set (&t);
    
    if (ns) { stamp_addNanoseconds (&t, ns); }
    
    if (!stamp_setAsTags (STAMP_TAGS_SIZE, a, &t)) { outlet_list (x->x_outlet, STAMP_TAGS_SIZE, a); }
}

static void timestamp_bang (t_timestamp *x)
{
    timestamp_float (x, 0.0);
}

static void timestamp_list (t_timestamp *x, t_symbol *s, int argc, t_atom *argv)
{
    t_stamp t; t_error err = stamp_getWithTags (argc, argv, &t);
    
    if (err) { error_invalid (cast_object (x), sym_timestamp, sym_stamp); }
    else {
    //
    t_stamp now; t_nano ns;
    stamp_set (&now);
    err = stamp_elapsedNanoseconds (&now, &t, &ns);
    if (x->x_mode == TIMESTAMP_IMMEDIATELY) {
        outlet_float (x->x_outlet, PD_NANOSECONDS_TO_MILLISECONDS (ns));
    } else if (x->x_mode == TIMESTAMP_DISCARD) {
        if (!err) { outlet_float (x->x_outlet, PD_NANOSECONDS_TO_MILLISECONDS (ns)); }
    } else {
        if (!err) { outlet_float (x->x_outlet, PD_NANOSECONDS_TO_MILLISECONDS (ns)); }
        else {
            err = stamp_elapsedNanoseconds (&t, &now, &ns);
            PD_ASSERT (!err); PD_UNUSED (err);
            outlet_float (x->x_outlet, PD_NANOSECONDS_TO_MILLISECONDS (ns) * (-1.0));
        }
    }
    //
    }
}

static void timestamp_anything (t_timestamp *x, t_symbol *s, int argc, t_atom *argv)
{
    utils_anythingToList (cast_pd (x), (t_listmethod)timestamp_list, s, argc, argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void timestamp_discard (t_timestamp *x, t_float f)
{
    x->x_mode = (f == 0.0 ? TIMESTAMP_IMMEDIATELY : TIMESTAMP_DISCARD);
}

static void timestamp_elapsed (t_timestamp *x, t_float f)
{
    x->x_mode = (f == 0.0 ? TIMESTAMP_IMMEDIATELY : TIMESTAMP_ELAPSED);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *timestamp_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_timestamp *x = (t_timestamp *)z;
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, sym__restore);
    buffer_appendFloat (b, x->x_mode);
    
    return b;
    //
    }
    
    return NULL;
}

static void timestamp_restore (t_timestamp *x, t_float f)
{
    t_timestamp *old = (t_timestamp *)instance_pendingFetch (cast_object (x));

    x->x_mode = old ? old->x_mode : PD_CLAMP ((int)f, TIMESTAMP_IMMEDIATELY, TIMESTAMP_ELAPSED);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *timestamp_new (t_symbol *s, int argc, t_atom *argv)
{
    t_timestamp *x = (t_timestamp *)pd_new (timestamp_class);
    
    x->x_outlet = outlet_newMixed (cast_object (x));
    
    while (argc > 0) {
    //
    t_symbol *t = atom_getSymbolAtIndex (0, argc, argv);

    if (t == sym___dash__discard) {
        timestamp_discard (x, 1.0);
        argc--;
        argv++;
        
    } else if (t == sym___dash__elapsed) {
        timestamp_elapsed (x, 1.0);
        argc--;
        argv++;
        
    } else {
        break;
    }
    //
    }

    error__options (cast_object (x), s, argc, argv);
    
    if (argc) { warning_unusedArguments (cast_object (x), s, argc, argv); }
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void timestamp_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_timestamp,
            (t_newmethod)timestamp_new,
            NULL,
            sizeof (t_timestamp),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);

    class_addBang (c, (t_method)timestamp_bang);
    class_addFloat (c, (t_method)timestamp_float);
    class_addList (c, (t_method)timestamp_list);
    class_addAnything (c, (t_method)timestamp_anything);
    
    class_addMethod (c, (t_method)timestamp_discard, sym_discard,   A_FLOAT, A_NULL);
    class_addMethod (c, (t_method)timestamp_elapsed, sym_elapsed,   A_FLOAT, A_NULL);
    class_addMethod (c, (t_method)timestamp_restore, sym__restore,  A_FLOAT, A_NULL);

    class_setDataFunction (c, timestamp_functionData);
    class_requirePending (c);

    timestamp_class = c;
}

void timestamp_destroy (void)
{
    class_free (timestamp_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
