
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "x_list.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Called by the t_listmethod of the object maker class. */

static void *list_makeObject (t_pd *dummy, t_symbol *s, int argc, t_atom *argv)
{
    t_pd *newest = NULL;
    
    instance_objectSetNewest (NULL);
    
    if (!argc || !IS_SYMBOL (argv)) { newest = (t_pd *)listappend_new (s,       argc, argv); }
    else {
    //
    t_symbol *t = atom_getSymbol (argv);
    
    if (t == sym_append)            { newest = (t_pd *)listappend_new (s,       argc - 1, argv + 1); }
    else if (t == sym_change)       { newest = (t_pd *)listchange_new (s,       argc - 1, argv + 1); }
    else if (t == sym_equal)        { newest = (t_pd *)listequal_new (s,        argc - 1, argv + 1); }
    else if (t == sym_fromsymbol)   { newest = (t_pd *)listfromsymbol_new (s,   argc - 1, argv + 1); }
    else if (t == sym_group)        { newest = (t_pd *)listgroup_new (s,        argc - 1, argv + 1); }
    else if (t == sym_iterate)      { newest = (t_pd *)listiterate_new (s,      argc - 1, argv + 1); }
    else if (t == sym_length)       { newest = (t_pd *)listlength_new (s,       argc - 1, argv + 1); }
    else if (t == sym_prepend)      { newest = (t_pd *)listprepend_new (s,      argc - 1, argv + 1); }
    else if (t == sym_reverse)      { newest = (t_pd *)listreverse_new (s,      argc - 1, argv + 1); }
    else if (t == sym_rotate)       { newest = (t_pd *)listrotate_new (s,       argc - 1, argv + 1); }
    else if (t == sym_scramble)     { newest = (t_pd *)listscramble_new (s,     argc - 1, argv + 1); }
    else if (t == sym_sort)         { newest = (t_pd *)listsort_new (s,         argc - 1, argv + 1); }
    else if (t == sym_split)        { newest = (t_pd *)listsplit_new (s,        argc - 1, argv + 1); }
    else if (t == sym_store)        { newest = (t_pd *)liststore_new (s,        argc - 1, argv + 1); }
    else if (t == sym_stream)       { newest = (t_pd *)liststream_new (s,       argc - 1, argv + 1); }
    else if (t == sym_sum)          { newest = (t_pd *)listsum_new (s,          argc - 1, argv + 1); }
    else if (t == sym_tosymbol)     { newest = (t_pd *)listtosymbol_new (s,     argc - 1, argv + 1); }
    else if (t == sym_trim)         { newest = (t_pd *)listtrim_new (s,         argc - 1, argv + 1); }
    else {
        error_unexpected (&s_list, t);
    }
    //
    }
    
    instance_objectSetNewest (newest);
    
    return newest;  /* Unused but kept due to t_newmethod signature. */
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void list_setup (void)
{
    class_addCreator ((t_newmethod)list_makeObject, &s_list, A_GIMME, A_NULL);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

