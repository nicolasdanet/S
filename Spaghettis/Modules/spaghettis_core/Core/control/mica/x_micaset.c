
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "x_mica.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if PD_WITH_BELLE

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *micaset_class;          /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _micaset {
    t_micabase  x_base;                 /* Must be the first. */
    t_symbol    *x_parsed;
    t_outlet    *x_outlet;
    } t_micaset;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void micaset_bang (t_micaset *x)
{
    if (!x->x_parsed) { outlet_symbol (x->x_outlet, ((t_micabase *)x)->x_tag); }
    else {
        outlet_symbol (x->x_outlet, x->x_parsed);
    }
}

static void micaset_float (t_micaset *x, t_float f)
{
    if (!x->x_parsed) { t_atom a; SET_FLOAT (&a, f); ((t_micabase *)x)->x_tag = concept_tagParsed (1, &a); }
    
    micaset_bang (x);
}

static void micaset_list (t_micaset *x, t_symbol *s, int argc, t_atom *argv)
{
    if (!x->x_parsed) { ((t_micabase *)x)->x_tag = concept_tagParsed (argc, argv); }
    
    micaset_bang (x);
}

static void micaset_anything (t_micaset *x, t_symbol *s, int argc, t_atom *argv)
{
    utils_anythingToList (cast_pd (x), (t_listmethod)micaset_list, s, argc, argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_buffer *micabase_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_micabase *x = (t_micabase *)z;
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, sym__restore);
    buffer_appendSymbol (b, x->x_tag);
    
    return b;
    //
    }
    
    return NULL;
}

PD_LOCAL void micabase_restore (t_micabase *x, t_symbol *s)
{
    t_micabase *old = (t_micabase *)instance_pendingFetch (cast_object (x));

    x->x_tag = old ? old->x_tag : s;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void *micaset_new (t_symbol *s, int argc, t_atom *argv)
{
    t_micaset *x = (t_micaset *)pd_new (micaset_class);
    
    ((t_micabase *)x)->x_tag = concept_tag (mica::Undefined);
    x->x_parsed = NULL;
    x->x_outlet = outlet_newSymbol (cast_object (x));
    
    if (argc) {
    //
    x->x_parsed = concept_tagParsed (argc, argv);
    
    if (concept_fetch (x->x_parsed).isUndefined()) { warning_invalid (sym_mica__space__set, sym_concept); }
    //
    }
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void micaset_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_mica__space__set,
            (t_newmethod)micaset_new,
            NULL,
            sizeof (t_micaset),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
    
    class_addBang (c, (t_method)micaset_bang);
    class_addFloat (c, (t_method)micaset_float);
    class_addList (c, (t_method)micaset_list);
    class_addAnything (c, (t_method)micaset_anything);
    
    class_addMethod (c, (t_method)micabase_restore, sym__restore, A_SYMBOL, A_NULL);

    class_setDataFunction (c, micabase_functionData);
    class_requirePending (c);

    class_setHelpName (c, sym_mica);
    
    micaset_class = c;
}

PD_LOCAL void micaset_destroy (void)
{
    class_free (micaset_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#else

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void micaset_setup (void)      { }
PD_LOCAL void micaset_destroy (void)    { }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // PD_WITH_BELLE

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

