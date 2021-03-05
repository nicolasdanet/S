
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

static t_class *micamap_class;          /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _micamap {
    t_micabase  x_base;                 /* Must be the first. */
    t_symbol    *x_parsed;
    t_outlet    *x_outlet;
    } t_micamap;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void micamap_bang (t_micamap *x)
{
    outlet_symbol (x->x_outlet, ((t_micabase *)x)->x_tag);
}

static void micamap_list (t_micamap *x, t_symbol *s, int argc, t_atom *argv)
{
    mica::Concept t;
    
    if (argc) {
    //
    mica::Concept a (concept_fetch (atom_getSymbolAtIndex (0, argc, argv)));
    mica::Concept b (concept_fetch (atom_getSymbolAtIndex (1, argc, argv)));
    mica::Concept c (concept_fetch (atom_getSymbolAtIndex (2, argc, argv)));
    
    if (x->x_parsed) {
        if (argc > 1) { t = mica::map (mica::Concept (concept_fetch (x->x_parsed)), a, b); }
        else {
            t = mica::map (mica::Concept (concept_fetch (x->x_parsed)), a);
        }
    
    } else {
        if (argc > 2) { t = mica::map (a, b, c); }
        else {
            t = mica::map (a, b);
        }
    }
    //
    }
    
    ((t_micabase *)x)->x_tag = concept_tag (t);
    
    micamap_bang (x);
}

static void micamap_anything (t_micamap *x, t_symbol *s, int argc, t_atom *argv)
{
    utils_anythingToList (cast_pd (x), (t_listmethod)micamap_list, s, argc, argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void *micamap_new (t_symbol *s, int argc, t_atom *argv)
{
    t_micamap *x = (t_micamap *)pd_new (micamap_class);
    
    ((t_micabase *)x)->x_tag = concept_tag (mica::Undefined);
    x->x_parsed = NULL;
    x->x_outlet = outlet_newSymbol (cast_object (x));
    
    if (argc) {
    //
    x->x_parsed = concept_tagParsed (argc, argv);
    
    if (concept_fetch (x->x_parsed).isUndefined()) { warning_invalid (sym_mica__space__map, sym_concept); }
    //
    }
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void micamap_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_mica__space__map,
            (t_newmethod)micamap_new,
            NULL,
            sizeof (t_micamap),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
    
    class_addBang (c, (t_method)micamap_bang);
    class_addList (c, (t_method)micamap_list);
    class_addAnything (c, (t_method)micamap_anything);
    
    class_addMethod (c, (t_method)micabase_restore, sym__restore, A_SYMBOL, A_NULL);

    class_setDataFunction (c, micabase_functionData);
    class_requirePending (c);
    
    class_setHelpName (c, sym_mica);
    
    micamap_class = c;
}

PD_LOCAL void micamap_destroy (void)
{
    class_free (micamap_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#else

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void micamap_setup (void)      { }
PD_LOCAL void micamap_destroy (void)    { }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // PD_WITH_BELLE

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

