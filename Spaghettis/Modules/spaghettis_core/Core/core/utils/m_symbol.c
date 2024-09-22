
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Notice that this function may introduce unescaped whitespaces. */
/* Extra care is required at the risk to trouble the parser. */

t_symbol *symbol_withAtoms (int argc, t_atom *argv)
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

int symbol_containsWhitespace (t_symbol *s)
{
    return string_contains (symbol_getName (s), " ");
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Merge two dollars in just one. */

t_symbol *symbol_replaceDoubleDollar (t_symbol *s)
{
    size_t size = strlen (symbol_getName (s));
    
    if (size >= PD_STRING) { PD_BUG; return s; }
    else {
        char t[PD_STRING + 1] = { 0 };
        const char *p = symbol_getName (s);
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

int symbol_hasExtension (t_symbol *s, const char *e)
{
    PD_ASSERT (e && *e == '.');
    
    return string_endWith (symbol_getName (s), e);
}

t_symbol *symbol_removeExtension (t_symbol *s)
{
    PD_ASSERT (s);
    
    if (s != &s_) {
    //
    char t[PD_STRING] = { 0 };
    int n = string_indexOfFirstOccurrenceFromEnd (symbol_getName (s), ".");
    t_error err = string_copy (t, PD_STRING, symbol_getName (s));
    PD_ASSERT (!err);
    if (!err && n >= 0) { t[n] = 0; return gensym (t); } 
    //
    }
    
    return s;
}

t_symbol *symbol_appendExtensionPatch (t_symbol *s)
{
    return symbol_addSuffix (s, gensym (PD_PATCH));
}

t_symbol *symbol_appendExtensionHelp (t_symbol *s)
{
    return symbol_addSuffix (s, gensym (PD_HELP));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_symbol *symbol_addPrefix (t_symbol *s, t_symbol *prefix)
{
    if (prefix == &s_) { return s; }
    else if (s == &s_) { return prefix; }
    else {
        t_error err = PD_ERROR_NONE;
        char t[PD_STRING] = { 0 };
        PD_ASSERT (s);
        PD_ASSERT (prefix);
        err = string_sprintf (t, PD_STRING, "%s%s", symbol_getName (prefix), symbol_getName (s));
        PD_UNUSED (err); PD_ASSERT (!err);
        return gensym (t);
    }
}

t_symbol *symbol_addSuffix (t_symbol *s, t_symbol *suffix)
{
    return symbol_addPrefix (suffix, s);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_symbol *symbol_appendCopySuffix (t_symbol *s)
{
    PD_ASSERT (s);
    
    if (string_endWith (symbol_getName (s), symbol_getName (sym___dash____asterisk__))) { return s; }

    return symbol_addSuffix (s, sym___dash____asterisk__);
}

t_symbol *symbol_removeCopySuffix (t_symbol *s)
{
    PD_ASSERT (s);
    
    if (string_endWith (symbol_getName (s), symbol_getName (sym___dash____asterisk__))) {
    //
    char t[PD_STRING] = { 0 };
    t_error err = string_copy (t, PD_STRING, symbol_getName (s));
    PD_ASSERT (!err);
    if (!err) {
        int n = string_indexOfFirstOccurrenceFromEnd (symbol_getName (s), "-");
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
