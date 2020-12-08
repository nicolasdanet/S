
/* Copyright (c) 1997-2020 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

// ====================================

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void error__error (const char *s1, const char *s2)
{
    post_error ("%s: %s", s1, s2);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static const char *error__empty (t_symbol *s)
{
    if (s == &s_) { return "\" \""; }
    
    return s->s_name;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL int error__options (t_symbol *s, int argc, t_atom *argv)
{
    int i, k = 0;
    
    for (i = 0; i < argc; i++) {
    //
    if (IS_SYMBOL (argv + i))  {
    //
    t_symbol *t = GET_SYMBOL (argv + i);
    
    if (t != sym___dash__ && string_startWith (t->s_name, sym___dash__->s_name)) {
        warning_unusedOption (s, t);
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

PD_LOCAL void error_dspLoop (void)
{
    post_error (PD_TRANSLATE ("%s: DSP loop"), PD_NAME_LOWERCASE);
}

PD_LOCAL void error_stackOverflow (void)
{
    post_error (PD_TRANSLATE ("%s: stack overflow"), PD_NAME_LOWERCASE);
}

PD_LOCAL void error_recursiveCall (void)
{
    post_warning (PD_TRANSLATE ("%s: recursive call"), PD_NAME_LOWERCASE);
}

PD_LOCAL void error_stubNotFound (void)
{
    post_error (PD_TRANSLATE ("loader: stub not found"));
}

PD_LOCAL void error_searchPathOverflow (void)
{
    post_error (PD_TRANSLATE ("scan: search path overflow"));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void error_recursiveInstantiation (t_symbol *s)
{
    post_error (PD_TRANSLATE ("%s: recursive instantiation"), s->s_name);
}

PD_LOCAL void error_alreadyExists (t_symbol *s)
{
    post_error (PD_TRANSLATE ("%s: already exists"), s->s_name);
}

PD_LOCAL void error_canNotOpen (t_symbol *s)
{
    post_error (PD_TRANSLATE ("%s: can't open"), s->s_name);
}

PD_LOCAL void error_canNotCreate (t_symbol *s)
{
    post_error (PD_TRANSLATE ("%s: can't create"), s->s_name);
}

PD_LOCAL void error_failsToRead (t_symbol *s)
{
    post_error (PD_TRANSLATE ("%s: fails to read"), s->s_name);
}

PD_LOCAL void error_failsToWrite (t_symbol *s)
{
    post_error (PD_TRANSLATE ("%s: fails to write"), s->s_name);
}

PD_LOCAL void error_fileIsProtected (t_symbol *s)
{
    post_error (PD_TRANSLATE ("%s: file is protected"), s->s_name);
}

PD_LOCAL void error_ignored (t_symbol *s)
{
    post_error (PD_TRANSLATE ("%s: ignored"), s->s_name);
}

PD_LOCAL void error_failed (t_symbol *s)
{   
    post_error (PD_TRANSLATE ("%s: failed"), s->s_name);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void error_noSuch (t_symbol *s1, t_symbol *s2)
{
    post_error (PD_TRANSLATE ("%s: no such %s"), s1->s_name, error__empty (s2));
}

PD_LOCAL void error_canNotFind (t_symbol *s1, t_symbol *s2)
{
    post_error (PD_TRANSLATE ("%s: can't find %s"), s1->s_name, error__empty (s2));
}

PD_LOCAL void error_unknownMethod (t_symbol *s1, t_symbol *s2)
{
    post_error (PD_TRANSLATE ("%s: unknown method %s"), s1->s_name, error__empty (s2));
}

PD_LOCAL void error_unexpected (t_symbol *s1, t_symbol *s2)
{
    post_error (PD_TRANSLATE ("%s: unexpected %s"), s1->s_name, error__empty (s2));
}

PD_LOCAL void error_invalid (t_symbol *s1, t_symbol *s2)
{
    post_error (PD_TRANSLATE ("%s: invalid %s"), s1->s_name, error__empty (s2));
}

PD_LOCAL void error_mismatch (t_symbol *s1, t_symbol *s2)
{
    post_error (PD_TRANSLATE ("%s: mismatch %s"), s1->s_name, error__empty (s2));
}

PD_LOCAL void error_unspecified (t_symbol *s1, t_symbol *s2)
{
    post_error (PD_TRANSLATE ("%s: unspecified %s"), s1->s_name, error__empty (s2));
}

PD_LOCAL void error_undefined (t_symbol *s1, t_symbol *s2)
{
    post_error (PD_TRANSLATE ("%s: undefined %s"), s1->s_name, error__empty (s2));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void error_canNotMake (int argc, t_atom *argv)
{
    char *t = atom_atomsToString (argc, argv);
    
    post_error (PD_TRANSLATE ("%s: can't make [ %s ]"), PD_NAME_LOWERCASE, t);
    
    PD_MEMORY_FREE (t);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void error_invalidArguments (t_symbol *s, int argc, t_atom *argv)
{
    char *t = atom_atomsToString (argc, argv);
    
    post_error (PD_TRANSLATE ("%s: [ %s ] invalid argument(s)"), s->s_name, t);
    
    PD_MEMORY_FREE (t);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void warning_containsDuplicates (void)
{
    post_warning (PD_TRANSLATE ("scan: contains duplicates"));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void warning_tooManyCharacters (t_symbol *s)
{
    post_warning (PD_TRANSLATE ("%s: too many characters"), s->s_name);
}

PD_LOCAL void warning_multipleBinding (t_symbol *s)
{
    post_warning (PD_TRANSLATE ("%s: multiple binding"), s->s_name);
}

PD_LOCAL void warning_fileIsCorrupted (t_symbol *s)
{
    post_warning (PD_TRANSLATE ("%s: file is corrupted"), s->s_name);
}

PD_LOCAL void warning_deprecatedObject (t_symbol *s)
{
    post_warning (PD_TRANSLATE ("%s: deprecated object"), s->s_name);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void warning_invalid (t_symbol *s1, t_symbol *s2)
{
    post_warning (PD_TRANSLATE ("%s: invalid %s"), s1->s_name, error__empty (s2));
}

PD_LOCAL void warning_empty (t_symbol *s1, t_symbol *s2)
{
    post_warning (PD_TRANSLATE ("%s: empty %s"), s1->s_name, error__empty (s2));
}

PD_LOCAL void warning_badType (t_symbol *s1, t_symbol *s2)
{
    post_warning (PD_TRANSLATE ("%s: bad type %s"), s1->s_name, error__empty (s2));
}

PD_LOCAL void warning_unusedOption (t_symbol *s1, t_symbol *s2)
{
    post_warning (PD_TRANSLATE ("%s: unused option %s"), s1->s_name, error__empty (s2));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void warning_unusedArguments (t_symbol *s, int argc, t_atom *argv)
{
    if (s != sym__inlet2) {     /* Avoid to moan for nothing (e.g. implicit list to float cast). */
    if (s != sym__inlet3) {
    //
    char *t = atom_atomsToString (argc, argv);
    
    post_warning (PD_TRANSLATE ("%s: [ %s ] unused argument(s)"), s->s_name, t);
    
    PD_MEMORY_FREE (t);
    //
    }
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
