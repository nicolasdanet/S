
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

static t_class *micaindex_class;        /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _micaindex {
    t_micabase  x_base;                 /* Must be the first. */
    t_symbol    *x_parsed;
    t_outlet    *x_outlet;
    } t_micaindex;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void micaindex_bang (t_micaindex *x)
{
    outlet_symbol (x->x_outlet, ((t_micabase *)x)->x_tag);
}

static void micaindex_list (t_micaindex *x, t_symbol *s, int argc, t_atom *argv)
{
    mica::Concept t;
    
    if (argc) {
    //
    mica::Concept a (concept_fetch (atom_getSymbolAtIndex (0, argc, argv)));
    mica::Concept b (concept_fetch (atom_getSymbolAtIndex (1, argc, argv)));
    mica::Concept c (concept_fetch (atom_getSymbolAtIndex (2, argc, argv)));
    
    if (x->x_parsed) {
        if (argc > 1) { t = mica::index (mica::Concept (concept_fetch (x->x_parsed)), a, b); }
        else {
            t = mica::index (mica::Concept (concept_fetch (x->x_parsed)), a);
        }
        
    } else {
        if (argc > 2) { t = mica::index (a, b, c); }
        else {
            t = mica::index (a, b);
        }
    }
    //
    }

    ((t_micabase *)x)->x_tag = concept_tag (t);
    
    micaindex_bang (x);
}

static void micaindex_anything (t_micaindex *x, t_symbol *s, int argc, t_atom *argv)
{
    utils_anythingToList (cast_pd (x), (t_listmethod)micaindex_list, s, argc, argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void *micaindex_new (t_symbol *s, int argc, t_atom *argv)
{
    t_micaindex *x = (t_micaindex *)pd_new (micaindex_class);
    
    ((t_micabase *)x)->x_tag = concept_tag (mica::Undefined);
    x->x_parsed = NULL;
    x->x_outlet = outlet_newSymbol (cast_object (x));
    
    if (argc) {
    //
    x->x_parsed = concept_tagParsed (argc, argv);
    
    if (concept_fetch (x->x_parsed).isUndefined()) { warning_invalid (sym_mica__space__index, sym_concept); }
    //
    }
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void micaindex_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_mica__space__index,
            (t_newmethod)micaindex_new,
            NULL,
            sizeof (t_micaindex),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
    
    class_addBang (c, (t_method)micaindex_bang);
    class_addList (c, (t_method)micaindex_list);
    class_addAnything (c, (t_method)micaindex_anything);
    
    class_addMethod (c, (t_method)micabase_restore, sym__restore, A_SYMBOL, A_NULL);

    class_setDataFunction (c, micabase_functionData);
    class_requirePending (c);
    
    class_setHelpName (c, sym_mica);
    
    micaindex_class = c;
}

PD_LOCAL void micaindex_destroy (void)
{
    class_free (micaindex_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#else

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void micaindex_setup (void)    { }
PD_LOCAL void micaindex_destroy (void)  { }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // PD_WITH_BELLE

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

