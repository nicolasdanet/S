
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int message_isPrivateSymbol (t_symbol *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static int symbol_hasThingProceed (t_symbol *s, int withError, t_object *o)
{
    int k = 0;
    
    if (s && s->s_thing) {
    //
    if (message_isPrivateSymbol (s)) { k = 1; }
    else {
        k = bindlist_hasThing (s);
    }
    //
    }
    
    if (withError && !k && s && !message_isPrivateSymbol (s)) { error_noSuch (o, s, sym_object); }
    
    return k;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

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
    if (s->s_thing) { return bindlist_getThingByClass (s, c); }
    
    return NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_pd *symbol_getThing (t_symbol *s)
{
    return s->s_thing;
}

const char *symbol_getName (t_symbol *s)
{
    return s->s_name;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------