
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* LLVM analyzer seems to report false positives. */

#ifndef __clang_analyzer__

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "x_mica.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if PD_WITH_BELLE

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *micaitem_class;        /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _micaitem {
    t_micabase  x_base;                 /* Must be the first. */
    t_symbol    *x_parsed;
    t_outlet    *x_outlet;
    } t_micaitem;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void micaitem_bang (t_micaitem *x)
{
    outlet_symbol (x->x_outlet, ((t_micabase *)x)->x_tag);
}

static void micaitem_list (t_micaitem *x, t_symbol *s, int argc, t_atom *argv)
{
    mica::Concept t;
    
    if (argc) {
    //
    mica::Concept a (concept_fetch (atom_getSymbolAtIndex (0, argc, argv)));
    mica::Concept b (concept_fetch (atom_getSymbolAtIndex (1, argc, argv)));
    mica::Concept c (concept_fetch (atom_getSymbolAtIndex (2, argc, argv)));
    
    if (x->x_parsed) {
        if (argc > 1) { t = mica::item (mica::Concept (concept_fetch (x->x_parsed)), a, b.getNumerator()); }
        else {
            t = mica::item (mica::Concept (concept_fetch (x->x_parsed)), a.getNumerator());
        }
    
    } else {
        if (argc > 2) { t = mica::item (a, b, c.getNumerator()); }
        else {
            t = mica::item (a, b.getNumerator());
        }
    }
    //
    }

    ((t_micabase *)x)->x_tag = concept_tag (t);
    
    micaitem_bang (x);
}

static void micaitem_anything (t_micaitem *x, t_symbol *s, int argc, t_atom *argv)
{
    utils_anythingToList (cast_pd (x), (t_listmethod)micaitem_list, s, argc, argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void *micaitem_new (t_symbol *s, int argc, t_atom *argv)
{
    t_micaitem *x = (t_micaitem *)pd_new (micaitem_class);
    
    ((t_micabase *)x)->x_tag = concept_tag (mica::Undefined);
    x->x_parsed = NULL;
    x->x_outlet = outlet_newSymbol (cast_object (x));
    
    if (argc) {
    //
    x->x_parsed = concept_tagParsed (argc, argv);
    
    if (concept_fetch (x->x_parsed).isUndefined()) { warning_invalid (sym_mica__space__item, sym_concept); }
    //
    }
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void micaitem_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_mica__space__item,
            (t_newmethod)micaitem_new,
            NULL,
            sizeof (t_micaitem),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
    
    class_addBang (c, (t_method)micaitem_bang);
    class_addList (c, (t_method)micaitem_list);
    class_addAnything (c, (t_method)micaitem_anything);
    
    class_addMethod (c, (t_method)micabase_restore, sym__restore, A_SYMBOL, A_NULL);

    class_setDataFunction (c, micabase_functionData);
    class_requirePending (c);
    
    class_setHelpName (c, sym_mica);
    
    micaitem_class = c;
}

PD_LOCAL void micaitem_destroy (void)
{
    class_free (micaitem_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#else

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void micaitem_setup (void)     { }
PD_LOCAL void micaitem_destroy (void)   { }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // PD_WITH_BELLE

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // __clang_analyzer__

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

