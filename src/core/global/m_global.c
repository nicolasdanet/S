
/* Copyright (c) 1997-2020 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void main_exit             (void);
PD_LOCAL void metadata_setParsed    (int, t_atom *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *global_class;       /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void global_open (void *dummy, t_symbol *name, t_symbol *directory)
{
    instance_patchOpen (name, directory);
}

static void global_scan (void *dummy, t_symbol *s, int argc, t_atom *argv)
{
    t_error err = searchpath_scan();
    
    if (argc && atom_getSymbol (argv) == sym_logged) { searchpath_report(); }
    
    if (searchpath_hasDuplicates()) { warning_containsDuplicates(); }
    if (err) { error_searchPathOverflow(); post ("scan: failed"); }  // --
    else {
        post ("scan: done");  // --
    }
}

static void global_dsp (void *dummy, t_symbol *s, int argc, t_atom *argv)
{
    if (argc) { dsp_setState ((int)atom_getFloatAtIndex (0, argc, argv)); }
}

static void global_metadata (void *dummy, t_symbol *s, int argc, t_atom *argv)
{
    if (argc) { metadata_setParsed (argc, argv); }
}

static void global_quit (void *dummy)
{
    main_exit();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void global_default (void *dummy, t_symbol *s, int argc, t_atom *argv)
{
    error_unknownMethod (sym_pd, s);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void global_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_pd, 
            NULL,
            NULL,
            0,
            CLASS_ABSTRACT,
            A_NULL);

    class_addMethod (c, (t_method)global_open,      sym_open,       A_SYMBOL, A_SYMBOL, A_NULL);
    class_addMethod (c, (t_method)global_scan,      sym_scan,       A_GIMME, A_NULL);
    class_addMethod (c, (t_method)global_dsp,       sym_dsp,        A_GIMME, A_NULL);
    class_addMethod (c, (t_method)global_metadata,  sym_metadata,   A_GIMME, A_NULL);
    class_addMethod (c, (t_method)global_quit,      sym_quit,       A_NULL);
    
    class_addAnything (c, (t_method)global_default);
    
    global_class = c;
        
    pd_bind (&global_class, sym_pd);        /* Fake binding the abstract class. */
}

PD_LOCAL void global_destroy (void)
{
    pd_unbind (&global_class, sym_pd);
    
    class_free (global_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
