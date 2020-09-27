
/* Copyright (c) 1997-2020 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef M_ENVIRONMENT_H_
#define M_ENVIRONMENT_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _environment {
    int         env_dollarZero;
    int         env_argc;
    t_atom      *env_argv;
    t_symbol    *env_directory;
    t_symbol    *env_fileName;
    } t_environment;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void   environment_free            (t_environment *e);
PD_LOCAL void   environment_setFileName     (t_environment *e, t_symbol *name);
PD_LOCAL void   environment_setDirectory    (t_environment *e, t_symbol *directory);
PD_LOCAL void   environment_setDollarZero   (t_environment *e, int n);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static inline int environment_getDollarZero (t_environment *e)
{
    return e->env_dollarZero;
}

static inline int environment_getNumberOfArguments (t_environment *e)
{
    return e->env_argc;
}

static inline t_atom *environment_getArguments (t_environment *e)
{
    return e->env_argv;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static inline t_symbol *environment_getDirectory (t_environment *e)
{
    return e->env_directory;
}

static inline const char *environment_getDirectoryAsString (t_environment *e)
{
    return e->env_directory->s_name;
}

static inline t_symbol *environment_getFileName (t_environment *e)
{
    return (e ? e->env_fileName : sym_Untitled);
}

static inline const char *environment_getFileNameAsString (t_environment *e)
{
    return environment_getFileName (e)->s_name;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // M_ENVIRONMENT_H_
