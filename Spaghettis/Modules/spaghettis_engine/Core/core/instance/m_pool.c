
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#define POOL_STRING     65

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_symbol *pool_generate (void)
{
    static int once = 0; static t_rand48 seed = 0;  /* Static. */
    
    if (!once) { once = 1; seed = PD_RAND48_SEED; }
    
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

t_symbol *pool_get (void)
{
    t_atom a; t_error err = buffer_pop (instance_get()->pd_pool, &a);
    
    if (!err) { return GET_SYMBOL (&a); } else { return pool_generate(); }
}

void pool_relinquish (t_symbol *s)
{
    pool_push (s);
}

/* Check if it looks like valid. */

int pool_check (t_symbol *s)
{
    const char *p = symbol_getName (s); return ((*p == '@') && (strlen (p) == POOL_STRING));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
