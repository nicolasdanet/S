
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Notice that this function may introduce unescaped whitespaces. */
/* Extra care is required at the risk to trouble the parser. */

PD_LOCAL t_symbol *symbol_withAtoms (int argc, t_atom *argv)
{
    t_symbol *s = &s_;
        
    if (argc == 1 && IS_SYMBOL (argv)) { s = GET_SYMBOL (argv); }
    else if (argc) {
        char *t = atom_atomsToString (argc, argv);
        s = gensym (t);
        PD_MEMORY_FREE (t);
    }
    
    return s;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL int symbol_containsWhitespace (t_symbol *s)
{
    return string_contains (s->s_name, " ");
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Merge two dollars in just one. */

PD_LOCAL t_symbol *symbol_replaceDoubleDollar (t_symbol *s)
{
    size_t size = strlen (s->s_name);
    
    if (size >= PD_STRING) { PD_BUG; return s; }
    else {
        char t[PD_STRING + 1] = { 0 };
        const char *p = s->s_name;
        int i, j = 0;
        for (i = 0; i < (int)size; i++, j++) {
            t[j] = p[i];
            if (p[i] == '$' && p[i + 1] == '$') { i++; }
        }
        return gensym (t);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_symbol *symbol_removeExtension (t_symbol *s)
{
    PD_ASSERT (s);
    
    if (s != &s_) {
    //
    char t[PD_STRING] = { 0 };
    int n = string_indexOfFirstOccurrenceFromEnd (s->s_name, ".");
    t_error err = string_copy (t, PD_STRING, s->s_name);
    PD_ASSERT (!err);
    if (!err && n >= 0) { t[n] = 0; return gensym (t); } 
    //
    }
    
    return s;
}

PD_LOCAL t_symbol *symbol_appendExtensionPatch (t_symbol *s)
{
    return symbol_addSuffix (s, gensym (PD_PATCH));
}

PD_LOCAL t_symbol *symbol_appendExtensionHelp (t_symbol *s)
{
    return symbol_addSuffix (s, gensym (PD_HELP));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_symbol *symbol_addPrefix (t_symbol *s, t_symbol *prefix)
{
    if (prefix == &s_) { return s; }
    else if (s == &s_) { return prefix; }
    else {
        t_error err = PD_ERROR_NONE;
        char t[PD_STRING] = { 0 };
        PD_ASSERT (s);
        PD_ASSERT (prefix);
        err = string_sprintf (t, PD_STRING, "%s%s", prefix->s_name, s->s_name);
        PD_UNUSED (err); PD_ASSERT (!err);
        return gensym (t);
    }
}

PD_LOCAL t_symbol *symbol_addSuffix (t_symbol *s, t_symbol *suffix)
{
    return symbol_addPrefix (suffix, s);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_symbol *symbol_appendCopySuffix (t_symbol *s)
{
    PD_ASSERT (s);
    
    if (string_endWith (s->s_name, sym___dash____asterisk__->s_name)) { return s; }

    return symbol_addSuffix (s, sym___dash____asterisk__);
}

PD_LOCAL t_symbol *symbol_removeCopySuffix (t_symbol *s)
{
    PD_ASSERT (s);
    
    if (string_endWith (s->s_name, sym___dash____asterisk__->s_name)) {
    //
    char t[PD_STRING] = { 0 };
    t_error err = string_copy (t, PD_STRING, s->s_name);
    PD_ASSERT (!err);
    if (!err) {
        int n = string_indexOfFirstOccurrenceFromEnd (s->s_name, "-");
        n = PD_CLAMP (n, 0, PD_STRING - 1);
        t[n] = 0;
        return gensym (t);
    }
    //
    }
    
    return s;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
