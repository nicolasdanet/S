
/* Copyright (c) 1997 Miller Puckette and others. */

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

void   environment_free            (t_environment *e);
void   environment_setFileName     (t_environment *e, t_symbol *name);

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
    return symbol_getName (e->env_directory);
}

static inline t_symbol *environment_getFileName (t_environment *e)
{
    return (e ? e->env_fileName : sym_Untitled);
}

static inline const char *environment_getFileNameAsString (t_environment *e)
{
    return symbol_getName (environment_getFileName (e));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static inline void environment_setDirectory (t_environment *e, t_symbol *directory)
{
    e->env_directory = directory;
}

static inline void environment_setDollarZero (t_environment *e, int n)
{
    e->env_dollarZero = n;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // M_ENVIRONMENT_H_
