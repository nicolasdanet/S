
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "x_array.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static void *array_makeObject (t_symbol *s, int argc, t_atom *argv)
{
    t_pd *newest = NULL;
    
    instance_objectSetNewest (NULL);
    
    if (!argc || !IS_SYMBOL (argv)) { newest = (t_pd *)arrayget_new (s,     argc, argv); }
    else {
    //
    t_symbol *t = atom_getSymbolAtIndex (0, argc, argv);
    
    if (t == sym_size)          { newest = (t_pd *)arraysize_new (s,        argc - 1, argv + 1); }
    else if (t == sym_sum)      { newest = (t_pd *)arraysum_new (s,         argc - 1, argv + 1); }
    else if (t == sym_get)      { newest = (t_pd *)arrayget_new (s,         argc - 1, argv + 1); }
    else if (t == sym_set)      { newest = (t_pd *)arrayset_new (s,         argc - 1, argv + 1); }
    else if (t == sym_quantile) { newest = (t_pd *)arrayquantile_new (s,    argc - 1, argv + 1); }
    else if (t == sym_random)   { newest = (t_pd *)arrayrandom_new (s,      argc - 1, argv + 1); }
    else if (t == sym_max)      { newest = (t_pd *)arraymax_new (s,         argc - 1, argv + 1); }
    else if (t == sym_min)      { newest = (t_pd *)arraymin_new (s,         argc - 1, argv + 1); }
    //
    }
    
    instance_objectSetNewest (newest);
    
    return newest;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void array_setup (void)
{
    class_addCreator ((t_newmethod)array_makeObject, sym_array, A_GIMME, A_NULL);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
