
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void instance_environmentSetFile (t_symbol *name, t_symbol *directory)
{
    PD_ASSERT (name && name != &s_);
    PD_ASSERT (directory && directory != &s_);
    
    environment_setFileName (instance_get()->pd_environment, name);
    environment_setDirectory (instance_get()->pd_environment, directory);
}

void instance_environmentSetArguments (int argc, t_atom *argv)
{
    environment_setArguments (instance_get()->pd_environment, argc, argv);
}

void instance_environmentResetFile (void)
{
    environment_setFileName (instance_get()->pd_environment, &s_);
    environment_setDirectory (instance_get()->pd_environment, &s_);
}

void instance_environmentResetArguments (void)
{
    environment_setArguments (instance_get()->pd_environment, 0, NULL);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Caller is responsible to free the environment returned. */

t_environment *instance_environmentFetchIfAny (void)
{
    return environment_newConsume (instance_get()->pd_environment);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
