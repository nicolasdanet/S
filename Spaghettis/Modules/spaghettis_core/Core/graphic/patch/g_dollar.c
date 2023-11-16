
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static int dollar_getDollarZero (t_glist *glist)
{
    t_environment *environment = NULL;
    
    glist       = (glist == NULL) ? instance_contextGetCurrent() : glist;
    environment = (glist != NULL) ? glist_getEnvironment (glist) : NULL;
    
    if (environment) { return environment_getDollarZero (environment); }
    else {
        return 0;
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static int dollar_expandSymbolProceed (const char *s,
    char *buffer,
    int size,
    int argc,
    t_atom *argv,
    t_glist *glist)
{
    /* Note that atol return zero for an invalid number. */
    /* Note that atol return 1234 for '1234Foo'. */
    
    int n = (int)atol (s);
    const char *ptr = s;
    char c = 0;
    int length = 0;
    t_error err = PD_ERROR_NONE;
    
    *buffer = 0;
    
    c = *ptr;
    
    while (c && (c >= '0') && (c <= '9')) { c = *(++ptr); length++; }

    /* Dollar number argument is out of bound. */
    /* Dollar expansion invalid (like "$bla"). */
    /* Dollar zero expansion. */
    /* Dollar number expansion. */
    
    if (n < 0 || n > argc) { return 0; }

    if (ptr == s) {                                       
        err = string_sprintf (buffer, size, "$");       /* Unsubstituted dollars are preserved. */
        PD_UNUSED (err); PD_ASSERT (!err);
        return 0;

    } else if (n == 0) {                                    
        t_atom a;
        SET_FLOAT (&a, dollar_getDollarZero (glist));
        err = atom_toString (&a, buffer, size);
        PD_ASSERT (length == 1);
        
    } else {                                                
        err = atom_toString (argv + (n - 1), buffer, size);
    }
    
    return (err ? -1 : length);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Dollar symbol expansion (e.g. '$1-foo' to 'bar-foo'). */
/* Note that 'foo$1' is valid and expanded to 'foobar' in that case. */

t_symbol *dollar_expandSymbolWithArguments (t_symbol *s, t_glist *glist, int argc, t_atom *argv)
{
    char t[PD_STRING] = { 0 };
    char result[PD_STRING] = { 0 };
    const char *str = s->s_name;
    const char *substr = NULL;
    int next = 0;
    t_error err = PD_ERROR_NONE;
    
    substr = strchr (str, '$');
    
    if (!substr) { return s; }
    else {
        err |= string_append (result, PD_STRING, str, (int)(substr - str));
        str = substr + 1;
    }

    while (!err && ((next = dollar_expandSymbolProceed (str, t, PD_STRING, argc, argv, glist)) >= 0)) {
    //
    if ((next == 0) && (*t == 0)) { return NULL; }      /* Dollar's number argument is out of bound. */

    err |= string_add (result, PD_STRING, t);
    str += next;
    
    substr = strchr (str, '$');
    
    if (substr) { err |= string_append (result, PD_STRING, str, (int)(substr - str)); str = substr + 1; }
    else {
        err |= string_add (result, PD_STRING, str);
        break;
    }
    //
    }
    
    if (err) { return NULL; } else { return gensym (result); }
}

t_symbol *dollar_expandSymbol (t_symbol *s, t_glist *glist)
{
    if (strchr (s->s_name, '$') == NULL) { return s; }
    else {
    //
    t_symbol *t = NULL;
    t_environment *e = NULL;
    
    if (glist) { e = glist_getEnvironment (glist); }

    if (!e) { t = dollar_expandSymbolWithArguments (s, glist, 0, NULL); }
    else {
    //
    t = dollar_expandSymbolWithArguments (s,
            glist,
            environment_getNumberOfArguments (e),
            environment_getArguments (e));
    //
    }
    
    return ((t != NULL) ? t : s);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Dollar number expansion (e.g. '$1' to 'foo'). */

int dollar_expandWithArguments (t_atom *dollar, t_atom *a, t_glist *glist, int argc, t_atom *argv)
{
    int n = GET_DOLLAR (dollar);
        
    PD_ASSERT (IS_DOLLAR (dollar));
    
    if (n > 0 && n <= argc) { *a = *(argv + n - 1); return 1; }
    else if (n == 0)        { SET_FLOAT (a, dollar_getDollarZero (glist)); return 1; }
    else {
        SET_FLOAT (a, 0.0);
    }
    
    return 0;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
