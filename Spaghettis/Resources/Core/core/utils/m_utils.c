
/* Copyright (c) 1997-2020 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_error utils_version (char *dest, size_t size)
{
    t_error err = string_sprintf (dest, size, "%s %s / %s / %s", 
                    PD_NAME, 
                    PD_VERSION, 
                    midi_nameNative(), 
                    audio_nameNative());
    
    #if PD_32BIT
        err |= string_add (dest, size, " / 32-bit");
    #endif
    
    #if PD_64BIT
        err |= string_add (dest, size, " / 64-bit");
    #endif
    
    #if PD_WITH_DEBUG
        err |= string_add (dest, size, " / Debug");
    #endif
    
    return err;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void utils_anythingToList (t_pd *x, t_listmethod fn, t_symbol *s, int argc, t_atom *argv)
{
    t_atom *t = NULL;
    PD_ATOMS_ALLOCA (t, argc + 1);
    atom_copyAtoms (argv, argc, t + 1, argc);
    SET_SYMBOL (t, s);
    (*fn) (x, &s_anything, argc + 1, t);
    PD_ATOMS_FREEA (t, argc + 1);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_id utils_unique (void)
{
    static uint64_t t = 0; ++t; return t;
}

PD_LOCAL t_error utils_uniqueWithAtoms (int argc, t_atom *argv, t_id *u)
{
    uint64_t t; t_error err = utils_uInt64Deserialize (argc, argv, &t);
    
    (*u) = t;
    
    return err;
}

PD_LOCAL void utils_appendUnique (t_buffer *b, t_id u)
{
    t_atom a[16]; uint64_t t = u; t_error err = utils_uInt64Serialize (16, a, &t);
    
    PD_ASSERT (!err); PD_UNUSED (err);
    
    buffer_append (b, 16, a);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_symbol *utils_4BitsToUInt64Element (unsigned char b)
{
    switch (b) {
        case 0  : return sym___arrobe__0;
        case 1  : return sym___arrobe__1;
        case 2  : return sym___arrobe__2;
        case 3  : return sym___arrobe__3;
        case 4  : return sym___arrobe__4;
        case 5  : return sym___arrobe__5;
        case 6  : return sym___arrobe__6;
        case 7  : return sym___arrobe__7;
        case 8  : return sym___arrobe__8;
        case 9  : return sym___arrobe__9;
        case 10 : return sym___arrobe__a;
        case 11 : return sym___arrobe__b;
        case 12 : return sym___arrobe__c;
        case 13 : return sym___arrobe__d;
        case 14 : return sym___arrobe__e;
        default : return sym___arrobe__f;
    }
}

static unsigned char utils_uInt64ElementTo4Bits (t_symbol *s)
{
    if (s == sym___arrobe__0)       { return 0x00; }
    else if (s == sym___arrobe__1)  { return 0x01; }
    else if (s == sym___arrobe__2)  { return 0x02; }
    else if (s == sym___arrobe__3)  { return 0x03; }
    else if (s == sym___arrobe__4)  { return 0x04; }
    else if (s == sym___arrobe__5)  { return 0x05; }
    else if (s == sym___arrobe__6)  { return 0x06; }
    else if (s == sym___arrobe__7)  { return 0x07; }
    else if (s == sym___arrobe__8)  { return 0x08; }
    else if (s == sym___arrobe__9)  { return 0x09; }
    else if (s == sym___arrobe__a)  { return 0x0a; }
    else if (s == sym___arrobe__b)  { return 0x0b; }
    else if (s == sym___arrobe__c)  { return 0x0c; }
    else if (s == sym___arrobe__d)  { return 0x0d; }
    else if (s == sym___arrobe__e)  { return 0x0e; }
    else if (s == sym___arrobe__f)  { return 0x0f; }
    
    return 0xff;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Unsigned 64-bit long as hexadecimal string (splitted in element to avoid to flood the symbol table). */

PD_LOCAL int utils_uInt64IsElement (t_symbol *s)
{
    return (utils_uInt64ElementTo4Bits (s) != 0xff);
}

PD_LOCAL t_error utils_uInt64Serialize (int argc, t_atom *argv, uint64_t *n)
{
    if (argc >= 16) {
    //
    int i;
    
    for (i = 0; i < 16; i++) {
    //
    unsigned char t = ((*n) >> (i * 4)) & 0x0f;
    SET_SYMBOL (argv + (16 - 1 - i), utils_4BitsToUInt64Element (t));
    //
    }
    
    return PD_ERROR_NONE;
    //
    }
    
    return PD_ERROR;
}

PD_LOCAL t_error utils_uInt64Deserialize (int argc, t_atom *argv, uint64_t *n)
{
    if (argc >= 16) {
    //
    t_error err = PD_ERROR_NONE;
    uint64_t t = 0;
    int i;
    
    for (i = 0; i < 16; i++) {
    //
    unsigned char b = utils_uInt64ElementTo4Bits (atom_getSymbol (argv + i));
    err |= (b == 0xff); t |= b; if (i != (16 - 1)) { t <<= 4; }
    //
    }
    
    if (!err) { (*n) = t; return PD_ERROR_NONE; }
    //
    }
    
    (*n) = 0; return PD_ERROR;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
