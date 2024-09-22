
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
    t_buffer    *env_arguments;
    t_symbol    *env_fileName;
    t_symbol    *env_directory;
    } t_environment;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int         environment_getDollarZero           (t_environment *e);

int         environment_argc                    (t_environment *e);
t_atom      *environment_argv                   (t_environment *e);

t_symbol    *environment_getFileName            (t_environment *e);
const char  *environment_getFileNameAsString    (t_environment *e);

t_symbol    *environment_getDirectory           (t_environment *e);
const char  *environment_getDirectoryAsString   (t_environment *e);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void        environment_setDollarZero           (t_environment *e, int n);
void        environment_setArguments            (t_environment *e, int argc, t_atom *argv);
void        environment_setFileName             (t_environment *e, t_symbol *name);
void        environment_setDirectory            (t_environment *e, t_symbol *directory);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_environment   *environment_newConsume         (t_environment *consumed);
t_environment   *environment_new                (void);

void            environment_free                (t_environment *e);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // M_ENVIRONMENT_H_
