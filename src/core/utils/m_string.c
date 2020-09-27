
/* Copyright (c) 1997-2020 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_error string_appendProceed (char *dest, size_t size, const char *src, int n)
{
    size_t d = strlen (dest);
    size_t k = (size - 1) - d;
    size_t s = 0;
    
    PD_ASSERT (n >= -1);
    PD_ASSERT (size > d);
    
    if (n < 0) { s = strlen (src); }
    else {
        const char *t = src; while (*t && s < (size_t)n) { s++; t++; }
    }
    
    {
        size_t t = PD_MIN (s, k); if (t) { memmove (dest + d, src, t); } dest[d + t] = 0;
    }
    
    if (s <= k) { return PD_ERROR_NONE; }
    else {
        return PD_ERROR;
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_error string_copy (char *dest, size_t size, const char *src)
{
    size_t s = strlen (src);
    size_t n = PD_MIN (s, size - 1);
    
    PD_ASSERT (size > 0);
    
    if (n > 0) { memmove (dest, src, n); } dest[n] = 0;
    
    if (s < size) { return PD_ERROR_NONE; }
    else {
        return PD_ERROR;
    }
}

PD_LOCAL t_error string_add (char *dest, size_t size, const char *src)
{
    return string_appendProceed (dest, size, src, -1);
}

PD_LOCAL t_error string_append (char *dest, size_t size, const char *src, int n)
{
    if (n > 0) { return string_appendProceed (dest, size, src, n); }
    else if (n < 0) {
        PD_BUG; return PD_ERROR;
    }
    
    return PD_ERROR_NONE;
}

PD_LOCAL t_error string_sprintf (char *dest, size_t size, const char *format, ...)
{
    int t;
    va_list args;
    
    va_start (args, format);
    t = vsnprintf (dest, size, format, args);
    va_end (args);
    
    if (t >= 0 && (size_t)t < size) { return PD_ERROR_NONE; }
    else {
        return PD_ERROR;
    }
}

#if ( PD_WITH_DEBUG ) || defined ( PD_BUILDING_TESTS )

PD_LOCAL t_error string_addSprintf (char *dest, size_t size, const char *format, ...)
{
    int t;
    va_list args;
    size_t d = strlen (dest);
    
    PD_ASSERT (size > d);
    
    va_start (args, format);
    t = vsnprintf (dest + d, size - d, format, args);
    va_end (args);
    
    if (t >= 0 && (size_t)t < (size - d)) { return PD_ERROR_NONE; }
    else {
        return PD_ERROR;
    }
}

#endif

PD_LOCAL t_error string_addAtom (char *dest, size_t size, t_atom *a)
{
    t_error err = PD_ERROR_NONE;
    
    char *t = (char *)PD_MEMORY_GET (size);
    
    err |= atom_toString (a, t, (int)size);
    err |= string_add (dest, size, t);

    PD_MEMORY_FREE (t);
    
    return err;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if defined ( PD_BUILDING_TESTS )

PD_LOCAL void string_clear (char *dest, size_t size)
{
    (void)size; dest[0] = 0;
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* < http://www.cse.yorku.ca/~oz/hash.html > */

PD_LOCAL uint64_t string_hash (const char *s)
{
    uint64_t hash = 5381;
    int c;
    
    while ((c = *s++)) { hash = ((hash << 5) + hash) + c; }
    
    return hash;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL int string_startWith (const char *s, const char *isStartWith)
{
    size_t n = strlen (isStartWith);
    
    if (strlen (s) >= n) { return (strncmp (s, isStartWith, n) == 0); }
    
    return 0;
}

PD_LOCAL int string_endWith (const char *s, const char *isEndWith)
{
    size_t n = strlen (isEndWith);
    size_t m = strlen (s);
    
    if (m >= n) { return (strncmp (s + (m - n), isEndWith, n) == 0); }
    
    return 0;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL int string_containsOccurrenceAtStart (const char *s, const char *chars)
{
    return (strchr (chars, *s) != NULL);
}

PD_LOCAL int string_contains (const char *s, const char *isContained)
{
    return (strstr (s, isContained) != NULL);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static int string_indexOfFirstCharacterFrom (const char *s, char c, int n)
{
    const char *s2 = s + n;
    
    while (s2 != s) { 
        s2--;
        n--;
        if (*s2 == c) { return n; }
    }
    
    return -1;
}

static int string_indexOfFirstOccurrenceFrom (const char *s, const char *c, int n)
{
    int k = -1;
    int t = 0;
    
    while (*c != 0) { 
        t = string_indexOfFirstCharacterFrom (s, *c, n);
        if (t > k) { k = t; }
        c++; 
    }
    
    return k;
}

PD_LOCAL int string_indexOfFirstOccurrenceFromEnd (const char *s, const char *c)
{
    return string_indexOfFirstOccurrenceFrom (s, c, (int)strlen (s));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void string_replaceCharacter (char *s, char toBeReplaced, char c)
{
    PD_ASSERT (c && toBeReplaced);
    
    while (*s) { if (*s == toBeReplaced) { *s = c; } s++; }
}

/* The string must NOT be a static constant. */

PD_LOCAL void string_removeCharacter (char *s, char toBeRemoved)
{
    PD_ASSERT (toBeRemoved);
    
    char *r = s;
    char *w = s;
    
    while (*r) { *w = *r; r++; w += (*w != toBeRemoved); }
    
    *w = 0;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* True if the string start with a dollar following by zero or more numbers. */

PD_LOCAL int string_isDollarNumber (const char *s)
{
    if (*s != '$') { return 0; } while (*(++s)) { if (*s < '0' || *s > '9') { return 0; } }
    
    return 1;
}

/* True if the string start with a dollar following by a number. */

PD_LOCAL int string_startWithDollarNumber (const char *s)
{
    PD_ASSERT (s[0] != 0);
    
    if (s[0] != '$' || s[1] < '0' || s[1] > '9') { return 0; }
    
    return 1;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL int char_isSemicolonOrComma (char c)
{
    return (c == ',' || c == ';');
}

PD_LOCAL int char_isBackslash (char c)
{
    return (c == '\\');
}

PD_LOCAL int char_isWhitespaceOrNewlineOrTabulation (char c)
{
    return (c == ' ' || c == '\n' || c == '\r' || c == '\t');
}

PD_LOCAL int char_isNumericOrDot (char c)
{
    return ((c >= '0' && c <= '9') || (c == '.'));
}

PD_LOCAL int char_isAlphabeticLowercase (char c)
{
    return (c >= 'a' && c <= 'z');
}

PD_LOCAL int char_isAlphanumericOrUnderscore (char c)
{
    return ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c == '_'));
}

PD_LOCAL int char_isAlphanumericLowercaseOrUnderscore (char c)
{
    return ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c == '_'));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
