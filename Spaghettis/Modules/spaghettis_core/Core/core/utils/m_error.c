
/* Copyright (c) 1997 Miller Puckette and others. */

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

PD_LOCAL void error__error (t_object *x, const char *s1, const char *s2)
{
    post_error (x, "%s: %s", s1, s2);
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

PD_LOCAL int error__options (t_object *x, t_symbol *s, int argc, t_atom *argv)
{
    int i, k = 0;
    
    for (i = 0; i < argc; i++) {
    //
    if (IS_SYMBOL (argv + i))  {
    //
    t_symbol *t = GET_SYMBOL (argv + i);
    
    if (t != sym___dash__ && string_startWith (t->s_name, sym___dash__->s_name)) {
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

PD_LOCAL void error_doesNotExist (t_object *x, const char *filepath)
{
    post_error (x, PD_TRANSLATE ("file: doesn't exist %s"), filepath);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void error_dspLoop (t_object *x)
{
    post_error (x, PD_TRANSLATE ("%s: DSP loop"), PD_NAME_LOWERCASE);
}

PD_LOCAL void error_stackOverflow (t_object *x)
{
    post_error (x, PD_TRANSLATE ("%s: stack overflow"), PD_NAME_LOWERCASE);
}

PD_LOCAL void error_recursiveCall (t_object *x)
{
    post_error (x, PD_TRANSLATE ("%s: recursive call"), PD_NAME_LOWERCASE);
}

PD_LOCAL void error_stubNotFound (t_object *x)
{
    post_error (x, PD_TRANSLATE ("loader: stub not found"));
}

PD_LOCAL void error_searchPathOverflow (t_object *x)
{
    post_error (x, PD_TRANSLATE ("rescan: search path overflow"));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void error_recursiveInstantiation (t_object *x, t_symbol *s)
{
    post_error (x, PD_TRANSLATE ("%s: recursive instantiation"), s->s_name);
}

PD_LOCAL void error_alreadyExists (t_object *x, t_symbol *s)
{
    post_error (x, PD_TRANSLATE ("%s: already exists"), s->s_name);
}

PD_LOCAL void error_canNotOpen (t_object *x, t_symbol *s)
{
    post_error (x, PD_TRANSLATE ("%s: can't open"), s->s_name);
}

PD_LOCAL void error_canNotCreate (t_object *x, t_symbol *s)
{
    post_error (x, PD_TRANSLATE ("%s: can't create"), s->s_name);
}

PD_LOCAL void error_failsToRead (t_object *x, t_symbol *s)
{
    post_error (x, PD_TRANSLATE ("%s: fails to read"), s->s_name);
}

PD_LOCAL void error_failsToWrite (t_object *x, t_symbol *s)
{
    post_error (x, PD_TRANSLATE ("%s: fails to write"), s->s_name);
}

PD_LOCAL void error_fileIsProtected (t_object *x, t_symbol *s)
{
    post_error (x, PD_TRANSLATE ("%s: file is protected"), s->s_name);
}

PD_LOCAL void error_ignored (t_object *x, t_symbol *s)
{
    post_error (x, PD_TRANSLATE ("%s: ignored"), s->s_name);
}

PD_LOCAL void error_failed (t_object *x, t_symbol *s)
{   
    post_error (x, PD_TRANSLATE ("%s: failed"), s->s_name);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void error_noSuch (t_object *x, t_symbol *s1, t_symbol *s2)
{
    post_error (x, PD_TRANSLATE ("%s: no such %s"), s1->s_name, error__empty (s2));
}

PD_LOCAL void error_canNotFind (t_object *x, t_symbol *s1, t_symbol *s2)
{
    post_error (x, PD_TRANSLATE ("%s: can't find %s"), s1->s_name, error__empty (s2));
}

PD_LOCAL void error_unknownMethod (t_object *x, t_symbol *s1, t_symbol *s2)
{
    post_error (x, PD_TRANSLATE ("%s: unknown method %s"), s1->s_name, error__empty (s2));
}

PD_LOCAL void error_unexpected (t_object *x, t_symbol *s1, t_symbol *s2)
{
    post_error (x, PD_TRANSLATE ("%s: unexpected %s"), s1->s_name, error__empty (s2));
}

PD_LOCAL void error_invalid (t_object *x, t_symbol *s1, t_symbol *s2)
{
    post_error (x, PD_TRANSLATE ("%s: invalid %s"), s1->s_name, error__empty (s2));
}

PD_LOCAL void error_mismatch (t_object *x, t_symbol *s1, t_symbol *s2)
{
    post_error (x, PD_TRANSLATE ("%s: mismatch %s"), s1->s_name, error__empty (s2));
}

PD_LOCAL void error_unspecified (t_object *x, t_symbol *s1, t_symbol *s2)
{
    post_error (x, PD_TRANSLATE ("%s: unspecified %s"), s1->s_name, error__empty (s2));
}

PD_LOCAL void error_undefined (t_object *x, t_symbol *s1, t_symbol *s2)
{
    post_error (x, PD_TRANSLATE ("%s: undefined %s"), s1->s_name, error__empty (s2));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void error_canNotMake (t_object *x, int argc, t_atom *argv)
{
    char *t = atom_atomsToString (argc, argv);
    
    post_error (x, PD_TRANSLATE ("%s: can't make [ %s ]"), PD_NAME_LOWERCASE, t);
    
    PD_MEMORY_FREE (t);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void error_invalidArguments (t_object *x, t_symbol *s, int argc, t_atom *argv)
{
    char *t = atom_atomsToString (argc, argv);
    
    post_error (x, PD_TRANSLATE ("%s: [ %s ] invalid argument(s)"), s->s_name, t);
    
    PD_MEMORY_FREE (t);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void warning_containsDuplicates (t_object *x)
{
    post_warning (x, PD_TRANSLATE ("rescan: contains duplicates"));
}

PD_LOCAL void warning_invalidSearchPath (t_object *x)
{
    post_warning (x, PD_TRANSLATE ("%s: invalid searchpath"), PD_NAME_LOWERCASE);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void warning_tooManyCharacters (t_object *x, t_symbol *s)
{
    post_warning (x, PD_TRANSLATE ("%s: too many characters"), s->s_name);
}

PD_LOCAL void warning_multipleBinding (t_object *x, t_symbol *s)
{
    post_warning (x, PD_TRANSLATE ("%s: multiple binding"), s->s_name);
}

PD_LOCAL void warning_fileIsCorrupted (t_object *x, t_symbol *s)
{
    post_warning (x, PD_TRANSLATE ("%s: file is corrupted"), s->s_name);
}

PD_LOCAL void warning_deprecatedObject (t_object *x, t_symbol *s)
{
    post_warning (x, PD_TRANSLATE ("%s: deprecated object"), s->s_name);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void warning_invalid (t_object *x, t_symbol *s1, t_symbol *s2)
{
    post_warning (x, PD_TRANSLATE ("%s: invalid %s"), s1->s_name, error__empty (s2));
}

PD_LOCAL void warning_empty (t_object *x, t_symbol *s1, t_symbol *s2)
{
    post_warning (x, PD_TRANSLATE ("%s: empty %s"), s1->s_name, error__empty (s2));
}

PD_LOCAL void warning_badType (t_object *x, t_symbol *s1, t_symbol *s2)
{
    post_warning (x, PD_TRANSLATE ("%s: bad type %s"), s1->s_name, error__empty (s2));
}

PD_LOCAL void warning_unusedOption (t_object *x, t_symbol *s1, t_symbol *s2)
{
    post_warning (x, PD_TRANSLATE ("%s: unused option %s"), s1->s_name, error__empty (s2));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void warning_unusedArguments (t_object *x, t_symbol *s, int argc, t_atom *argv)
{
    if (s != sym__inlet2) {     /* Avoid to moan for nothing (e.g. implicit list to float cast). */
    if (s != sym__inlet3) {
    //
    char *t = atom_atomsToString (argc, argv);
    
    post_warning (x, PD_TRANSLATE ("%s: [ %s ] unused argument(s)"), s->s_name, t);
    
    PD_MEMORY_FREE (t);
    //
    }
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
