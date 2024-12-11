
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

// ====================================

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void error__error (t_object *x, const char *s1, const char *s2)
{
    post_error (x, "%s: %s", s1, s2);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static const char *error__empty (t_symbol *s)
{
    if (s == &s_) { return "\" \""; }
    
    return symbol_getName (s);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int error__options (t_object *x, t_symbol *s, int argc, t_atom *argv)
{
    int i, k = 0;
    
    for (i = 0; i < argc; i++) {
    //
    if (IS_SYMBOL (argv + i))  {
    //
    t_symbol *t = GET_SYMBOL (argv + i);
    
    /* Legacy options format. */
    
    if (t != sym___dash__ && string_startWith (symbol_getName (t), symbol_getName (sym___dash__))) {
        warning_invalidOption (x, s, t);
        k = 1;
    }
    
    /* New style. */
    
    if (t != sym___arrobe__ && string_startWith (symbol_getName (t), symbol_getName (sym___arrobe__))) {
        warning_unusedOption (x, s, t);
        k = 1;
    }
    //
    }
    //
    }
    
    return k;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void error_doesNotExist (t_object *x, const char *filepath)
{
    post_error (x, PD_TRANSLATE ("file: doesn't exist %s"), filepath);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void error_dspLoop (t_object *x)
{
    post_error (x, PD_TRANSLATE ("%s: DSP loop"), PD_NAME);
}

void error_stackOverflow (t_object *x)
{
    post_error (x, PD_TRANSLATE ("%s: stack overflow"), PD_NAME);
}

void error_stubNotFound (t_object *x)
{
    post_error (x, PD_TRANSLATE ("loader: stub not found"));
}

void error_searchPathOverflow (t_object *x)
{
    post_error (x, PD_TRANSLATE ("rescan: search path overflow"));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void error_recursiveInstantiation (t_glist *x, t_symbol *s)
{
    post_error (cast_object (x), PD_TRANSLATE ("%s: recursive instantiation"), symbol_getName (s));
}

void error_alreadyExists (t_object *x, t_symbol *s)
{
    post_error (x, PD_TRANSLATE ("%s: already exists"), symbol_getName (s));
}

void error_canNotOpen (t_object *x, t_symbol *s)
{
    post_error (x, PD_TRANSLATE ("%s: can't open"), symbol_getName (s));
}

void error_canNotCreate (t_object *x, t_symbol *s)
{
    post_error (x, PD_TRANSLATE ("%s: can't create"), symbol_getName (s));
}

void error_failsToRead (t_object *x, t_symbol *s)
{
    post_error (x, PD_TRANSLATE ("%s: fails to read"), symbol_getName (s));
}

void error_failsToWrite (t_object *x, t_symbol *s)
{
    post_error (x, PD_TRANSLATE ("%s: fails to write"), symbol_getName (s));
}

void error_fileIsProtected (t_object *x, t_symbol *s)
{
    post_error (x, PD_TRANSLATE ("%s: file is protected"), symbol_getName (s));
}

void error_ignored (t_object *x, t_symbol *s)
{
    post_error (x, PD_TRANSLATE ("%s: ignored"), symbol_getName (s));
}

void error_failed (t_object *x, t_symbol *s)
{   
    post_error (x, PD_TRANSLATE ("%s: failed"), symbol_getName (s));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void error_noSuch (t_object *x, t_symbol *s1, t_symbol *s2)
{
    post_error (x, PD_TRANSLATE ("%s: no such %s"), symbol_getName (s1), error__empty (s2));
}

void error_canNotFind (t_object *x, t_symbol *s1, t_symbol *s2)
{
    post_error (x, PD_TRANSLATE ("%s: can't find %s"), symbol_getName (s1), error__empty (s2));
}

void error_unknownMethod (t_object *x, t_symbol *s1, t_symbol *s2)
{
    post_error (x, PD_TRANSLATE ("%s: unknown method %s"), symbol_getName (s1), error__empty (s2));
}

void error_unexpected (t_object *x, t_symbol *s1, t_symbol *s2)
{
    post_error (x, PD_TRANSLATE ("%s: unexpected %s"), symbol_getName (s1), error__empty (s2));
}

void error_invalid (t_object *x, t_symbol *s1, t_symbol *s2)
{
    post_error (x, PD_TRANSLATE ("%s: invalid %s"), symbol_getName (s1), error__empty (s2));
}

void error_mismatch (t_object *x, t_symbol *s1, t_symbol *s2)
{
    post_error (x, PD_TRANSLATE ("%s: mismatch %s"), symbol_getName (s1), error__empty (s2));
}

void error_unspecified (t_object *x, t_symbol *s1, t_symbol *s2)
{
    post_error (x, PD_TRANSLATE ("%s: unspecified %s"), symbol_getName (s1), error__empty (s2));
}

void error_undefined (t_object *x, t_symbol *s1, t_symbol *s2)
{
    post_error (x, PD_TRANSLATE ("%s: undefined %s"), symbol_getName (s1), error__empty (s2));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void error_canNotMake (t_object *x, int argc, t_atom *argv)
{
    char *t = atom_atomsToString (argc, argv);
    
    post_error (x, PD_TRANSLATE ("%s: can't make [ %s ]"), PD_NAME, t);
    
    PD_MEMORY_FREE (t);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void error_invalidArguments (t_object *x, t_symbol *s, int argc, t_atom *argv)
{
    char *t = atom_atomsToString (argc, argv);
    
    post_error (x, PD_TRANSLATE ("%s: [ %s ] invalid argument(s)"), symbol_getName (s), t);
    
    PD_MEMORY_FREE (t);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void warning_canNotFindHelp (t_object *x, t_symbol *s)
{
    post_warning (x, PD_TRANSLATE ("file: can't find %s help"), symbol_getName (s));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void warning_containsDuplicates (t_object *x)
{
    post_warning (x, PD_TRANSLATE ("rescan: contains duplicates"));
}

void warning_missingInSearchPath (t_object *x)
{
    post_warning (x, PD_TRANSLATE ("%s: missing in search path"), PD_NAME);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void warning_tooManyCharacters (t_object *x, t_symbol *s)
{
    post_warning (x, PD_TRANSLATE ("%s: too many characters"), symbol_getName (s));
}

void warning_multipleBinding (t_object *x, t_symbol *s)
{
    post_warning (x, PD_TRANSLATE ("%s: multiple binding"), symbol_getName (s));
}

void warning_fileIsCorrupted (t_object *x, t_symbol *s)
{
    post_warning (x, PD_TRANSLATE ("%s: file is corrupted"), symbol_getName (s));
}

void warning_deprecatedObject (t_object *x, t_symbol *s)
{
    post_warning (x, PD_TRANSLATE ("%s: deprecated object"), symbol_getName (s));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void warning_invalid (t_object *x, t_symbol *s1, t_symbol *s2)
{
    post_warning (x, PD_TRANSLATE ("%s: invalid %s"), symbol_getName (s1), error__empty (s2));
}

void warning_empty (t_object *x, t_symbol *s1, t_symbol *s2)
{
    post_warning (x, PD_TRANSLATE ("%s: empty %s"), symbol_getName (s1), error__empty (s2));
}

void warning_badType (t_object *x, t_symbol *s1, t_symbol *s2)
{
    post_warning (x, PD_TRANSLATE ("%s: bad type %s"), symbol_getName (s1), error__empty (s2));
}

void warning_invalidOption (t_object *x, t_symbol *s1, t_symbol *s2)
{
    post_warning (x, PD_TRANSLATE ("%s: invalid option %s"), symbol_getName (s1), error__empty (s2));
}

void warning_unusedOption (t_object *x, t_symbol *s1, t_symbol *s2)
{
    post_warning (x, PD_TRANSLATE ("%s: unused option %s"), symbol_getName (s1), error__empty (s2));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void warning_unusedArguments (t_object *x, t_symbol *s, int argc, t_atom *argv)
{
    if (s != sym__inlet2) {     /* Avoid to moan for nothing (e.g. implicit list to float cast). */
    if (s != sym__inlet3) {
    //
    char *t = atom_atomsToString (argc, argv);
    
    post_warning (x, PD_TRANSLATE ("%s: [ %s ] unused argument(s)"), symbol_getName (s), t);
    
    PD_MEMORY_FREE (t);
    //
    }
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
