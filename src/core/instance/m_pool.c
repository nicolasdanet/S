
/* Copyright (c) 1997-2020 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#define POOL_STRING     65

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_symbol *pool_generate (void)
{
    static t_rand48 seed = PD_RAND48_SEED;  /* Static. */
    
    const char hex[] = "0123456789abcdef";
    
    char t[POOL_STRING + 1] = { 0 };
    
    int i = 1;
    
    t[0] = '@';
    
    while (i < POOL_STRING) {
    //
    uint32_t v = PD_RAND48_UINT32 (seed);
    do {
    t[i] = hex[v & 15];
    v >>= 4;
    i++;
    } while ((v != 0) && (i < POOL_STRING));
    //
    }
    
    instance_get()->pd_poolCount++;
    
    return gensym (t);
}

static void pool_push (t_symbol *s)
{
    PD_ASSERT (symbol_hasThingQuiet (s) == 0); buffer_appendSymbol (instance_get()->pd_pool, s);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_symbol *pool_get (void)
{
    t_atom a; t_error err = buffer_pop (instance_get()->pd_pool, &a);
    
    if (!err) { return GET_SYMBOL (&a); } else { return pool_generate(); }
}

PD_LOCAL void pool_relinquish (t_symbol *s)
{
    pool_push (s);
}

/* Check if it looks like valid. */

PD_LOCAL int pool_check (t_symbol *s)
{
    const char *p = s->s_name; return ((*p == '@') && (strlen (p) == POOL_STRING));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
