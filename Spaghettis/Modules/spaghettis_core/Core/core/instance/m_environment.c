
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static void instance_environmentSetFileProceed (t_symbol *name, t_symbol *directory)
{
    instance_get()->pd_environment.env_directory = directory;
    instance_get()->pd_environment.env_fileName  = name;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void instance_environmentSetFile (t_symbol *name, t_symbol *directory)
{
    PD_ASSERT (name && name != &s_);
    PD_ASSERT (directory && directory != &s_);
    
    instance_environmentSetFileProceed (name, directory);
}

void instance_environmentSetArguments (int argc, t_atom *argv)
{
    if (instance_get()->pd_environment.env_argv) { PD_MEMORY_FREE (instance_get()->pd_environment.env_argv); }
    
    PD_ASSERT (argc >= 0);

    instance_get()->pd_environment.env_argc = argc;
    instance_get()->pd_environment.env_argv = argc ? (t_atom *)PD_MEMORY_GET (argc * sizeof (t_atom)) : NULL;
    
    if (argc) { atom_copyAtoms (argv, argc, instance_get()->pd_environment.env_argv, argc); }
}

void instance_environmentResetFile (void)
{
    instance_environmentSetFileProceed (&s_, &s_);
}

void instance_environmentResetArguments (void)
{
    instance_environmentSetArguments (0, NULL);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Caller is responsible to free the environment returned. */

t_environment *instance_environmentFetchIfAny (void)
{
    static int dollarZero = 1000;   /* Static. */
    
    if (instance_get()->pd_environment.env_directory != &s_) {
    //
    t_environment *e = (t_environment *)PD_MEMORY_GET (sizeof (t_environment));
    
    t_atom *argv = instance_get()->pd_environment.env_argv;
    
    e->env_dollarZero   = dollarZero++;
    e->env_argc         = instance_get()->pd_environment.env_argc;
    e->env_argv         = argv ? argv : (t_atom *)PD_MEMORY_GET (0);
    e->env_directory    = instance_get()->pd_environment.env_directory;
    e->env_fileName     = instance_get()->pd_environment.env_fileName;

    instance_get()->pd_environment.env_argc      = 0;
    instance_get()->pd_environment.env_argv      = NULL;
    instance_get()->pd_environment.env_fileName  = &s_;
    instance_get()->pd_environment.env_directory = &s_;
    
    return e;
    //
    }
    
    return NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void environment_free (t_environment *e)
{
    if (e) {
        PD_MEMORY_FREE (e->env_argv);
        PD_MEMORY_FREE (e);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Always contains an extension. */

void environment_setFileName (t_environment *e, t_symbol *name)
{
    int hasExtension = 0;
    
    hasExtension |= string_endWith (name->s_name, PD_PATCH);
    hasExtension |= string_endWith (name->s_name, PD_HELP);
    
    if (hasExtension) { e->env_fileName = name; }
    else {
    //
    const char *t = e->env_fileName->s_name;        /* Previous. */
    
    if (string_endWith (t, PD_HELP)) { e->env_fileName = symbol_appendExtensionHelp (name); }
    else {
        e->env_fileName = symbol_appendExtensionPatch (name);
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
