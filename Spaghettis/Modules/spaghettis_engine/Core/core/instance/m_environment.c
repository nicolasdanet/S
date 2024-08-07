
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

int environment_getDollarZero (t_environment *e)
{
    return e->env_dollarZero;
}

int environment_argc (t_environment *e)
{
    return buffer_getSize (e->env_arguments);
}

t_atom *environment_argv (t_environment *e)
{
    return buffer_getAtoms (e->env_arguments);
}

t_symbol *environment_getDirectory (t_environment *e)
{
    return e->env_directory;
}

const char *environment_getDirectoryAsString (t_environment *e)
{
    return symbol_getName (e->env_directory);
}

t_symbol *environment_getFileName (t_environment *e)
{
    return e->env_fileName;
}

const char *environment_getFileNameAsString (t_environment *e)
{
    return symbol_getName (e->env_fileName);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void environment_setDollarZero (t_environment *e, int n)
{
    e->env_dollarZero = n;
}

void environment_setArguments (t_environment *e, int argc, t_atom *argv)
{
    buffer_clear (e->env_arguments); buffer_append (e->env_arguments, argc, argv);
}

void environment_setDirectory (t_environment *e, t_symbol *directory)
{
    e->env_directory = directory;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Use old extension if possible. */

static void environment_setFileNameWithExtension (t_environment *e, t_symbol *s)
{
    int wasHelp = symbol_hasExtension (e->env_fileName, PD_HELP);
    
    if (symbol_hasExtension (s, PD_LEGACY)) { s = symbol_removeExtension (s); }
    
    if (wasHelp) { e->env_fileName = symbol_appendExtensionHelp (s); }
    else {
        e->env_fileName = symbol_appendExtensionPatch (s);
    }
}

/* Always ending a valid extension. */

void environment_setFileName (t_environment *e, t_symbol *s)
{
    if (symbol_hasExtension (s, PD_PATCH) || symbol_hasExtension (s, PD_HELP)) { e->env_fileName = s; }
    else {
        environment_setFileNameWithExtension (e, s);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_environment *environment_new (void)
{
    t_environment *e  = (t_environment *)PD_MEMORY_GET (sizeof (t_environment));
    
    e->env_dollarZero = 0;
    e->env_arguments  = buffer_new();
    e->env_fileName   = &s_;
    e->env_directory  = &s_;
    
    return e;
}

t_environment *environment_newConsume (t_environment *consumed)
{
    static int dollarZero = 1000;   /* Static. */
    
    if (consumed->env_directory != &s_) {
    //
    t_environment *e  = environment_new();
    
    e->env_dollarZero = dollarZero++;
    e->env_directory  = consumed->env_directory;
    e->env_fileName   = consumed->env_fileName;
    
    buffer_swap (e->env_arguments, consumed->env_arguments);

    consumed->env_fileName  = &s_;
    consumed->env_directory = &s_;
    
    return e;
    //
    }
    
    return NULL;
}

void environment_free (t_environment *e)
{
    if (e) {
        buffer_free (e->env_arguments);
        PD_MEMORY_FREE (e);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
