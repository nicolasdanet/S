
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL char *heapstring_freeBorrowUnzeroed (t_heapstring *);      /* Caller acquires ownership. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

enum {
    FLOAT_STATE_ERROR                   = -1,
    FLOAT_STATE_START                   = 0,
    FLOAT_STATE_MINUS                   = 1,
    FLOAT_STATE_INTEGER_DIGIT,
    FLOAT_STATE_LEADING_DOT,
    FLOAT_STATE_DOT,
    FLOAT_STATE_FRACTIONAL_DIGIT,
    FLOAT_STATE_EXPONENTIAL,
    FLOAT_STATE_EXPONENTIAL_SIGN,
    FLOAT_STATE_EXPONENTIAL_DIGIT
    };
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static int buffer_parseIsMalformed (const char *s, int size)
{
    if (size < 0) { return 1; }
    else {
        int i; for (i = 0; i < size; i++) { if (s[i] == 0) { return 1; } }
    }
    
    return 0;
}

static int buffer_parseIsValidCharacter (char c)
{
    return (!char_isWhitespaceOrNewlineOrTabulation (c) && !char_isSemicolonOrComma (c));
}

static int buffer_parseIsValidState (int floatState)
{
    return (floatState == FLOAT_STATE_INTEGER_DIGIT
            || floatState == FLOAT_STATE_DOT
            || floatState == FLOAT_STATE_FRACTIONAL_DIGIT
            || floatState == FLOAT_STATE_EXPONENTIAL_DIGIT);
}

static int buffer_parseNextFloatState (int floatState, char c)
{
    int digit       = (c >= '0' && c <= '9');
    int dot         = (c == '.');
    int minus       = (c == '-');
    int plus        = (c == '+');
    int exponential = (c == 'e' || c == 'E');
    
    int k = floatState;
    
    PD_ASSERT (k != FLOAT_STATE_ERROR);
    
    if (floatState == FLOAT_STATE_START)                    {
        if (minus)                                          { k = FLOAT_STATE_MINUS; }
        else if (digit)                                     { k = FLOAT_STATE_INTEGER_DIGIT; }
        else if (dot)                                       { k = FLOAT_STATE_LEADING_DOT; }
        else {
            k = FLOAT_STATE_ERROR;
        }
        
    } else if (floatState == FLOAT_STATE_MINUS)             {
        if (digit)                                          { k = FLOAT_STATE_INTEGER_DIGIT; }
        else if (dot)                                       { k = FLOAT_STATE_LEADING_DOT; }
        else { 
            k = FLOAT_STATE_ERROR;
        }
        
    } else if (floatState == FLOAT_STATE_INTEGER_DIGIT)     {
        if (dot)                                            { k = FLOAT_STATE_DOT; }
        else if (exponential)                               { k = FLOAT_STATE_EXPONENTIAL; }
        else if (!digit) {
            k = FLOAT_STATE_ERROR;
        }
        
    } else if (floatState == FLOAT_STATE_LEADING_DOT)       {
        if (digit)                                          { k = FLOAT_STATE_FRACTIONAL_DIGIT; }
        else {
            k = FLOAT_STATE_ERROR;
        }
        
    } else if (floatState == FLOAT_STATE_DOT)               {
        if (digit)                                          { k = FLOAT_STATE_FRACTIONAL_DIGIT; }
        else if (exponential)                               { k = FLOAT_STATE_EXPONENTIAL; }
        else {
            k = FLOAT_STATE_ERROR;
        }
        
    } else if (floatState == FLOAT_STATE_FRACTIONAL_DIGIT)  {
        if (exponential)                                    { k = FLOAT_STATE_EXPONENTIAL; }
        else if (!digit) {
            k = FLOAT_STATE_ERROR;
        }
        
    } else if (floatState == FLOAT_STATE_EXPONENTIAL)       {
        if (plus || minus)                                  { k = FLOAT_STATE_EXPONENTIAL_SIGN; }
        else if (digit)                                     { k = FLOAT_STATE_EXPONENTIAL_DIGIT; }
        else {
            k = FLOAT_STATE_ERROR;
        }
        
    } else if (floatState == FLOAT_STATE_EXPONENTIAL_SIGN)  {
        if (digit)                                          { k = FLOAT_STATE_EXPONENTIAL_DIGIT; }
        else {
            k = FLOAT_STATE_ERROR;
        }
        
    } else if (floatState == FLOAT_STATE_EXPONENTIAL_DIGIT) {
        if (!digit) {
            k = FLOAT_STATE_ERROR;
        }
    }

    return k;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void buffer_withStringUnzeroed (t_buffer *x, const char *s, int size)
{
    const char *text = s;
    const char *tBound = s + size;

    buffer_clear (x);
    
    if (buffer_parseIsMalformed (s, size)) { PD_BUG; return; }
    
    while (1) {
    //
    /* Skip whitespaces. */
    
    while (char_isWhitespaceOrNewlineOrTabulation (*text) && (text != tBound)) { text++; }
    
    if (text == tBound)    { break; }
    else if (*text == ';') { buffer_appendSemicolon (x); text++; }
    else if (*text == ',') { buffer_appendComma (x);     text++; }
    else {
        
        char buffer[PD_STRING + 1] = { 0 };
        char *p = buffer;
        char *pBound = buffer + PD_STRING;
        
        int floatState = 0;
        int slash = 0;
        int lastSlash = 0;
        int dollar = 0;
        
        do {
        //
        char c = *p = *text++;
        
        lastSlash = slash; slash = char_isBackslash (c);

        if (floatState >= 0) { floatState = buffer_parseNextFloatState (floatState, c); }
        if (!lastSlash && text != tBound && string_startWithDollarNumber (text - 1)) { dollar = 1; }
        
        if (!slash)         { p++; }
        else if (lastSlash) { p++; slash = 0; }
        //
        } while (text != tBound && p != pBound && (slash || (buffer_parseIsValidCharacter (*text))));
                
        *p = 0;

        if (buffer_parseIsValidState (floatState)) {
            buffer_appendFloat (x, (t_float)atof (buffer));
                        
        } else if (dollar) {
            if (string_isDollarNumber (buffer)) {
                buffer_appendDollar (x, atoi (buffer + 1));
            } else {
                buffer_appendDollarSymbol (x, gensym (buffer));
            }
            
        } else {
            buffer_appendSymbol (x, gensym (buffer));
        }
    }
    //
    }
}

/* Caller acquires string ownership. */

PD_LOCAL void buffer_toStringUnzeroed (t_buffer *x, char **s, int *size)
{
    t_heapstring *h = heapstring_new (0);
    
    int i;

    for (i = 0; i < buffer_getSize (x); i++) {
    //
    t_atom *a = buffer_getAtomAtIndex (x, i);
    
    char t[PD_STRING] = { 0 }; t_error err = atom_toString (a, t, PD_STRING);
    
    PD_UNUSED (err); PD_ASSERT (!err);
    
    if (IS_SEMICOLON_OR_COMMA (a)) { heapstring_removeCharacterIfContainsAtEnd (h, ' '); }
    
    heapstring_add (h, t);
    
    if (IS_SEMICOLON (a)) { heapstring_add (h, "\n"); }
    else {
        heapstring_add (h, " ");
    }
    //
    }
    
    /* Cosmetic. */
    
    heapstring_removeCharacterIfContainsAtEnd (h, ' ');
    
    *size = heapstring_getSize (h); *s = heapstring_freeBorrowUnzeroed (h);
}

PD_LOCAL char *buffer_toString (t_buffer *x)
{
    char *s = NULL;
    int n, length = 0;
    
    buffer_toStringUnzeroed (x, &s, &length);
    n = length + 1; 
    s = (char *)PD_MEMORY_RESIZE (s, length, n);
    s[n - 1] = 0;
    
    return s;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void buffer_reparseProceed (t_buffer *x)
{
    char *s = NULL;
    int size = 0;
    
    buffer_toStringUnzeroed (x, &s, &size);
    buffer_withStringUnzeroed (x, s, size);
    
    PD_MEMORY_FREE (s);
}

PD_LOCAL void buffer_reparseIfContainsWhitespace (t_buffer *x)
{
    int i, size = buffer_getSize (x);
    int reparse = 0;
    
    for (i = 0; i < size; i++) {
    //
    t_atom *a = buffer_getAtomAtIndex (x, i);
    
    if (IS_SYMBOL (a) && symbol_containsWhitespace (GET_SYMBOL (a))) {
        reparse = 1; break;
    }
    //
    }
    
    if (reparse) { buffer_reparseProceed (x); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void buffer_serialize (t_buffer *x, t_buffer *y)
{
    t_buffer *copy = buffer_newCopy (y);
    int i;
    
    for (i = 0; i < buffer_getSize (copy); i++) {
    //
    t_atom *a = buffer_getAtomAtIndex (copy, i);
        
    if (!IS_FLOAT (a)) {
        char t[PD_STRING] = { 0 };
        t_error err = atom_toString (a, t, PD_STRING);
        PD_UNUSED (err); PD_ASSERT (!err);
        SET_SYMBOL (a, gensym (t));
    }
    //
    }
    
    buffer_appendBuffer (x, copy);
    buffer_free (copy);
}

PD_LOCAL void buffer_deserialize (t_buffer *x, int argc, t_atom *argv)
{
    int i;
    
    for (i = 0; i < argc; i++) {
    //
    if (!IS_SYMBOL (argv + i)) { buffer_appendAtom (x, argv + i); }
    else {
        t_atom a;
        const char *s = GET_SYMBOL (argv + i)->s_name;
        t_error err = atom_withStringUnzeroed (&a, s, (int)strlen (s));
        PD_UNUSED (err); PD_ASSERT (!err);
        buffer_appendAtom (x, &a);
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
