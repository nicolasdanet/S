
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

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
    
    if (symbol_hasExtension (name, PD_LEGACY)) { name = symbol_removeExtension (name); }
    else {
        hasExtension |= symbol_hasExtension (name, PD_PATCH);
        hasExtension |= symbol_hasExtension (name, PD_HELP);
    }
    
    if (hasExtension) { e->env_fileName = name; }
    else {
    //
    const char *t = symbol_getName (e->env_fileName);   /* Previous. */
    
    if (string_endWith (t, PD_HELP)) { e->env_fileName = symbol_appendExtensionHelp (name); }
    else {
        e->env_fileName = symbol_appendExtensionPatch (name);
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
