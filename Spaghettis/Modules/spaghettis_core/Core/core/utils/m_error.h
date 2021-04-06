
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef M_ERROR_H_
#define M_ERROR_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void error__error                      (const char *, const char *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL int  error__options                    (t_symbol *s, int argc, t_atom *argv);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void error_dspLoop                     (void);
PD_LOCAL void error_stackOverflow               (void);
PD_LOCAL void error_recursiveCall               (void);
PD_LOCAL void error_stubNotFound                (void);
PD_LOCAL void error_searchPathOverflow          (void);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void error_recursiveInstantiation      (t_symbol *);
PD_LOCAL void error_alreadyExists               (t_symbol *);
PD_LOCAL void error_canNotOpen                  (t_symbol *);
PD_LOCAL void error_canNotCreate                (t_symbol *);
PD_LOCAL void error_failsToRead                 (t_symbol *);
PD_LOCAL void error_failsToWrite                (t_symbol *);
PD_LOCAL void error_fileIsProtected             (t_symbol *);
PD_LOCAL void error_ignored                     (t_symbol *);
PD_LOCAL void error_failed                      (t_symbol *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void error_noSuch                      (t_symbol *, t_symbol *);
PD_LOCAL void error_canNotFind                  (t_symbol *, t_symbol *);
PD_LOCAL void error_unknownMethod               (t_symbol *, t_symbol *);
PD_LOCAL void error_unexpected                  (t_symbol *, t_symbol *);
PD_LOCAL void error_invalid                     (t_symbol *, t_symbol *);
PD_LOCAL void error_mismatch                    (t_symbol *, t_symbol *);
PD_LOCAL void error_unspecified                 (t_symbol *, t_symbol *);
PD_LOCAL void error_undefined                   (t_symbol *, t_symbol *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void error_canNotMake                  (int argc, t_atom *argv);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void error_invalidArguments            (t_symbol *, int argc, t_atom *argv);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void warning_containsDuplicates        (void);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PD_LOCAL void warning_tooManyCharacters         (t_symbol *);
PD_LOCAL void warning_multipleBinding           (t_symbol *);
PD_LOCAL void warning_fileIsCorrupted           (t_symbol *);
PD_LOCAL void warning_deprecatedObject          (t_symbol *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PD_LOCAL void warning_invalid                   (t_symbol *, t_symbol *);
PD_LOCAL void warning_empty                     (t_symbol *, t_symbol *);
PD_LOCAL void warning_badType                   (t_symbol *, t_symbol *);
PD_LOCAL void warning_unusedOption              (t_symbol *, t_symbol *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PD_LOCAL void warning_unusedArguments           (t_symbol *, int argc, t_atom *argv);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // M_ERROR_H_
