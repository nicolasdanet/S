
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void symbol_hasThingError (t_symbol *s, t_object *o)
{  
    if (s) {
    if (s != sym___hash__A) {
        error_noSuch (o, s, sym_object);
    }
    }
}

static int symbol_hasThingProceed (t_symbol *s, int withError, t_object *o)
{
    int k = 0;
    
    if (s && s->s_thing) {
    //
    if (pd_class (s->s_thing) == bindlist_class) { k = !bindlist_isEmpty ((t_bindlist *)s->s_thing); }
    else {
        k = 1;
    }
    //
    }
    
    if (withError && !k) { symbol_hasThingError (s, o); }
    
    return k;
}

int symbol_hasThing (t_symbol *s, t_object *o)
{
    return symbol_hasThingProceed (s, 1, o);
}

int symbol_hasThingQuiet (t_symbol *s)
{
    return symbol_hasThingProceed (s, 0, NULL);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_pd *symbol_getThingByClass (t_symbol *s, t_class *c)
{
    t_pd *x = NULL;
    
    if (!s->s_thing) { return NULL; }
    if (pd_class (s->s_thing) == c) { return s->s_thing; }
    
    if (pd_class (s->s_thing) == bindlist_class) {
        t_bindlist *b = (t_bindlist *)s->s_thing;
        t_bindelement *e = NULL;
        
        for (e = b->b_list; e; e = e->e_next) {
            if (*e->e_what == c) {
                if (x != NULL) { warning_multipleBinding (NULL, s); }
                x = e->e_what;
            }
        }
    }
    
    return x;
}

t_pd *symbol_getThing (t_symbol *s)
{
    if (symbol_hasThingQuiet (s)) { return s->s_thing; }
    else {
        return NULL;
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

const char *symbol_getName (t_symbol *s)
{
    return s->s_name;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
