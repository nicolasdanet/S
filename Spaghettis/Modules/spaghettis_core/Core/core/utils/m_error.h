
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

PD_LOCAL void error__error                      (t_object *, const char *, const char *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL int  error__options                    (t_object *, t_symbol *s, int argc, t_atom *argv);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_FORCE void error_doesNotExist                (t_object *x, const char *filepath);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void error_dspLoop                     (t_object *);
PD_LOCAL void error_stackOverflow               (t_object *);
PD_LOCAL void error_recursiveCall               (t_object *);
PD_LOCAL void error_stubNotFound                (t_object *);
PD_LOCAL void error_searchPathOverflow          (t_object *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void error_recursiveInstantiation      (t_glist *, t_symbol *);

PD_LOCAL void error_alreadyExists               (t_object *, t_symbol *);
PD_LOCAL void error_canNotOpen                  (t_object *, t_symbol *);
PD_LOCAL void error_canNotCreate                (t_object *, t_symbol *);
PD_LOCAL void error_failsToRead                 (t_object *, t_symbol *);
PD_LOCAL void error_failsToWrite                (t_object *, t_symbol *);
PD_LOCAL void error_fileIsProtected             (t_object *, t_symbol *);
PD_LOCAL void error_ignored                     (t_object *, t_symbol *);
PD_LOCAL void error_failed                      (t_object *, t_symbol *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void error_noSuch                      (t_object *, t_symbol *, t_symbol *);
PD_LOCAL void error_canNotFind                  (t_object *, t_symbol *, t_symbol *);
PD_LOCAL void error_unknownMethod               (t_object *, t_symbol *, t_symbol *);
PD_LOCAL void error_unexpected                  (t_object *, t_symbol *, t_symbol *);
PD_LOCAL void error_invalid                     (t_object *, t_symbol *, t_symbol *);
PD_LOCAL void error_mismatch                    (t_object *, t_symbol *, t_symbol *);
PD_LOCAL void error_unspecified                 (t_object *, t_symbol *, t_symbol *);
PD_LOCAL void error_undefined                   (t_object *, t_symbol *, t_symbol *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void error_canNotMake                  (t_object *, int argc, t_atom *argv);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void error_invalidArguments            (t_object *, t_symbol *, int argc, t_atom *argv);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void warning_containsDuplicates        (t_object *);
PD_LOCAL void warning_missingInSearchPath       (t_object *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PD_LOCAL void warning_tooManyCharacters         (t_object *, t_symbol *);
PD_LOCAL void warning_multipleBinding           (t_object *, t_symbol *);
PD_LOCAL void warning_fileIsCorrupted           (t_object *, t_symbol *);
PD_LOCAL void warning_deprecatedObject          (t_object *, t_symbol *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PD_LOCAL void warning_invalid                   (t_object *, t_symbol *, t_symbol *);
PD_LOCAL void warning_empty                     (t_object *, t_symbol *, t_symbol *);
PD_LOCAL void warning_badType                   (t_object *, t_symbol *, t_symbol *);
PD_LOCAL void warning_unusedOption              (t_object *, t_symbol *, t_symbol *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PD_LOCAL void warning_unusedArguments           (t_object *, t_symbol *, int argc, t_atom *argv);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // M_ERROR_H_
