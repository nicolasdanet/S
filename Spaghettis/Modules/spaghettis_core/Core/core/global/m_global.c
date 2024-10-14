
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_class *global_class;       /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static void global_open (void *dummy, t_symbol *name, t_symbol *directory)
{
    // TODO: check if file exists first.
    
    // instance_patchOpen (name, directory);
}

static void global_rescan (void *dummy, t_symbol *s, int argc, t_atom *argv)
{
    int logged = (argc && atom_getSymbol (argv) == sym_logged) ? 1 : 0;
    
    searchpath_rescan (logged);
}

static void global_dsp (void *dummy, t_symbol *s, int argc, t_atom *argv)
{
    if (argc) { dsp_setState ((int)atom_getFloatAtIndex (0, argc, argv)); }
}

static void global_quit (void *dummy)
{
    outputs_quit();
}

static void global_clear (void *dummy)
{
    outputs_clearConsole();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void global_default (void *dummy, t_symbol *s, int argc, t_atom *argv)
{
    error_unknownMethod (NULL, sym_pd, s);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void global_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_pd, 
            NULL,
            NULL,
            0,
            CLASS_ABSTRACT,
            A_NULL);

    class_addMethod (c, (t_method)global_open,      sym_open,       A_SYMBOL, A_SYMBOL, A_NULL);
    class_addMethod (c, (t_method)global_rescan,    sym_rescan,     A_GIMME, A_NULL);
    class_addMethod (c, (t_method)global_dsp,       sym_dsp,        A_GIMME, A_NULL);
    class_addMethod (c, (t_method)global_quit,      sym_quit,       A_NULL);
    class_addMethod (c, (t_method)global_clear,     sym_clear,      A_NULL);
    
    class_addAnything (c, (t_method)global_default);
    
    global_class = c;
        
    pd_bind (&global_class, sym_pd);        /* Fake binding the abstract class. */
}

void global_destroy (void)
{
    pd_unbind (&global_class, sym_pd);
    
    class_free (global_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
