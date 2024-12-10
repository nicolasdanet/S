
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < http://opensoundcontrol.org/introduction-osc > */ 

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Note that this object is supposed to be compliant to OSC 1.1 Specification. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "x_osc.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Note that for now that object is reset with encapsulation. */

// TODO: Fetch states with pending?

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_class *oscformat_class;        /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _oscformat {
    t_object    x_obj;                  /* Must be the first. */
    char        *x_path;
    t_buffer    *x_pathBuffer;
    t_symbol    *x_format;
    t_outlet    *x_outlet;
    } t_oscformat;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static char oscformat_getTypeFromSymbol (t_symbol *s)
{
    if (stamp_isNTP (s))     { return 't'; }
    else if (s == sym_true)  { return 'T'; }
    else if (s == sym_false) { return 'F'; }
    else if (s == sym_null)  { return 'N'; }
    else if (s == &s_bang)   { return 'I'; }
    else {
        return 's';
    }
}

static void oscformat_setString (t_atom *a, int *i, const char *s)
{
    int n = *i;
    
    PD_ASSERT (s);
    
    while (*s) {
        unsigned char byte = (unsigned char)(*s);
        OSC_SETCHAR (a + n, byte);
        n++;
        s++;
    }
    
    OSC_SETCHAR (a + n, 0);
    n++;
    
    while (n & 3) {
        OSC_SETCHAR (a + n, 0);
        n++;
    }
    
    *i = n;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static int oscformat_proceedGetArgumentsSize (t_oscformat *x, int argc, t_atom *argv, int *numberOfTypeTags)
{
    int i = 0; 
    int k = 0;
    int size = 0;
    
    const char *t = symbol_getName (x->x_format);
    
    while (i < argc) {
    //
    t_atom *a = argv + i;
    char type = 'f';
    
    if (*t) {
        type = *t; t++;
    } else if (IS_SYMBOL (a)) {
        type = oscformat_getTypeFromSymbol (GET_SYMBOL (a));
    }
    
    if (type == 's') {
        t_symbol *s = IS_SYMBOL (a) ? GET_SYMBOL (a) : sym___question__;
        size += OSC_4ROUND ((int)strlen (symbol_getName (s)) + 1);
        i++;
        
    } else if (type == 'b') {
        int blobSize = (IS_FLOAT (a) && (GET_FLOAT (a) >= 0)) ? (int)GET_FLOAT (a) : (int)PD_INT_MAX;
        blobSize = PD_MIN (argc - i - 1, blobSize);
        size += 4 + OSC_4ROUND (blobSize);
        i++; i += blobSize;
    
    } else if (type == 't') {
        size += 8;
        i += STAMP_SIZE;
        
    } else if (type == 'd') {
        size += 8;
        i++;
    
    } else if (type == 'T' || type == 'F' || type == 'N' || type == 'I') {
    
        size += 0;      /* No bytes in arguments. */
        i++;
        
    } else {            /* Type is 'i' of 'f'. */
    
        size += 4;
        i++;
    }
    
    k++;
    //
    }
    
    *numberOfTypeTags = k;
    
    return size;
}

static int oscformat_proceedFillFloat (t_oscformat *x, int argc, t_atom *argv, int j, int *m, t_atom *a)
{
    int n = *m;
    
    t_pun32 z;
    pun32_setFloat (&z, (float)atom_getFloat (argv + j));
    OSC_4WRITE (a + n, pun32_getInteger (&z));
    n += 4;
    
    *m = n; j++; return j;
}

static int oscformat_proceedFillInteger (t_oscformat *x, int argc, t_atom *argv, int j, int *m, t_atom *a)
{
    int n = *m;

    int v = (int)atom_getFloat (argv + j);
    OSC_4WRITE (a + n, v);
    n += 4;
    
    *m = n; j++; return j;
}

static int oscformat_proceedFillString (t_oscformat *x, int argc, t_atom *argv, int j, int *m, t_atom *a)
{
    t_symbol *s = IS_SYMBOL (argv + j) ? GET_SYMBOL (argv + j) : sym___question__;
    
    oscformat_setString (a, m, symbol_getName (s));
    
    j++; return j;
}

static int oscformat_proceedFillBlob (t_oscformat *x, int argc, t_atom *argv, int j, int *m, t_atom *a)
{
    int n = *m;

    t_atom *start = argv + j;
    int i, blobSize = (IS_FLOAT (start) && GET_FLOAT (start) >= 0) ? GET_FLOAT (start) : PD_INT_MAX;
    
    blobSize = PD_MIN (argc - j - 1, blobSize);
    OSC_4WRITE (a + n, blobSize);
    n += 4;
    
    for (i = 0; i < blobSize; i++) {
    //
    unsigned char byte = 0;
    
    if (IS_FLOAT (start + 1 + i)) { byte = (unsigned char)GET_FLOAT (start + 1 + i); }
    else if (IS_SYMBOL (start + 1 + i)) {
        t_symbol *s = GET_SYMBOL (start + 1 + i); byte = (unsigned char)(*(symbol_getName (s)));
    }
    
    OSC_SETCHAR (a + n + i, byte);
    //
    }
    
    while (i & 3) { OSC_SETCHAR (a + n + i, 0); i++; }
    
    n += i;
    j += blobSize;
    
    *m = n; j++; return j;
}

static int oscformat_proceedFillStamp (t_oscformat *x, int argc, t_atom *argv, int j, int *m, t_atom *a)
{
    int n = *m;
    
    t_stamp stamp; stamp_set (&stamp);
    t_atom *start = argv + j;
    int available = argc - j;
    t_error err = stamp_deserialize (available, start, &stamp);
    
    if (err) { error_invalid (cast_object (x), sym_oscformat, sym_stamp); }
    
    OSC_8WRITE (a + n, stamp);
    
    n += 8;
    j += STAMP_SIZE;
    
    *m = n; return j;
}

static int oscformat_proceedFillDouble (t_oscformat *x, int argc, t_atom *argv, int j, int *m, t_atom *a)
{
    int n = *m;
    
    t_pun64 z;
    pun64_setDouble (&z, (double)atom_getFloat (argv + j));
    OSC_4WRITE (a + n, pun64_getMostSignificantBytes (&z));
    n += 4;
    OSC_4WRITE (a + n, pun64_getLeastSignificantBytes (&z));
    n += 4;
    
    *m = n; j++; return j;
}

static int oscformat_proceedFillDummy (t_oscformat *x, int argc, t_atom *argv, int j, int *m, t_atom *a)
{
    j++; return j;
}

static t_error oscformat_proceedFill (t_oscformat *x,
    int argc,
    t_atom *argv,
    int argumentsStart,
    t_atom *a,
    int size)
{
    int i = 0;
    int j = 0;
    int n = argumentsStart;

    const char *t = symbol_getName (x->x_format);
    
    oscformat_setString (a, &i, x->x_path);
    
    OSC_SETCHAR (a + i, ',');
    i++;
    
    while (j < argc) {

        char type = 'f';
        
        if (*t) {
            type = *t; t++;
        } else if (IS_SYMBOL (argv + j)) {
            type = oscformat_getTypeFromSymbol (GET_SYMBOL (argv + j));
        }
        
        OSC_SETCHAR (a + i, type);
        i++;
        
        switch (type) {
            case 'f': j = oscformat_proceedFillFloat (x, argc, argv, j, &n, a);   break;
            case 'i': j = oscformat_proceedFillInteger (x, argc, argv, j, &n, a); break;
            case 's': j = oscformat_proceedFillString (x, argc, argv, j, &n, a);  break;
            case 'b': j = oscformat_proceedFillBlob (x, argc, argv, j, &n, a);    break;
            case 't': j = oscformat_proceedFillStamp (x, argc, argv, j, &n, a);   break;
            case 'd': j = oscformat_proceedFillDouble (x, argc, argv, j, &n, a);  break;
            case 'T': /* Falls through. */
            case 'F': /* Falls through. */
            case 'N': /* Falls through. */
            case 'I': j = oscformat_proceedFillDummy (x, argc, argv, j, &n, a);   break;
            default : return PD_ERROR;
        }
    }
    
    OSC_SETCHAR (a + i, 0);
    i++;
    
    while (i & 3) { OSC_SETCHAR (a + i, 0); i++; }
    
    PD_ASSERT (i == argumentsStart);
    PD_ASSERT (n == size);
    
    return PD_ERROR_NONE;
}

static t_error oscformat_proceed (t_oscformat *x, int argc, t_atom *argv)
{
    t_error err = PD_ERROR_NONE;
    
    int numberOfTypeTags = 0;
    int argumentsSize    = oscformat_proceedGetArgumentsSize (x, argc, argv, &numberOfTypeTags);
    int argumentsStart   = OSC_4ROUND ((int)strlen (x->x_path) + 1) + OSC_4ROUND (numberOfTypeTags + 2);
    int size             = argumentsStart + argumentsSize;
    
    t_atom *a = NULL;
    
    PD_ATOMS_ALLOCA (a, size);

    err = oscformat_proceedFill (x, argc, argv, argumentsStart, a, size);
    
    if (!err) { outlet_list (x->x_outlet, size, a); }
    
    PD_ATOMS_FREEA (a, size);
    
    return err;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void oscformat_list (t_oscformat *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc) {
    //
    if ((*x->x_path) == 0) { error_unspecified (cast_object (x), sym_oscformat, sym_path); }
    else {
        t_error err = oscformat_proceed (x, argc, argv);
        
        if (err) { 
            error_failed (cast_object (x), sym_oscformat);
        }
    }
    //
    }
}

static void oscformat_anything (t_oscformat *x, t_symbol *s, int argc, t_atom *argv)
{
    utils_anythingToList (cast_pd (x), (t_listmethod)oscformat_list, s, argc, argv);
}

static void oscformat_bang (t_oscformat *x)
{
    utils_anythingToList (cast_pd (x), (t_listmethod)oscformat_list, &s_bang, 0, NULL);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void oscformat_set (t_oscformat *x, t_symbol *s, int argc, t_atom *argv)
{
    t_error err = PD_ERROR_NONE;
    char t[PD_STRING] = { 0 };
    int i;

    for (i = 0; i < argc; i++) {
        err |= string_add (t, PD_STRING, "/");
        if ((i == argc - 1) || atom_getSymbol (argv + i) != sym___slash__) {
            err |= string_addAtom (t, PD_STRING, argv + i);
        }
    }
    
    if (err) { error_invalid (cast_object (x), sym_oscformat, sym_path); }
    else {
    //
    err = string_copy (x->x_path, PD_STRING, t);
        
    PD_UNUSED (err); PD_ASSERT (!err);
        
    buffer_clear (x->x_pathBuffer); buffer_append (x->x_pathBuffer, argc, argv);
    //
    }
}

static void oscformat_format (t_oscformat *x, t_symbol *s)
{
    const char *t = NULL;
    
    for (t = symbol_getName (s); *t; t++) {
        if (!osc_isValidTypetag (*t)) { error_invalid (cast_object (x), sym_oscformat, sym_format); return; }
    }
    
    x->x_format = s;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *oscformat_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_oscformat *x = (t_oscformat *)z;
    t_buffer *b = buffer_new();
    int n = buffer_getSize (x->x_pathBuffer);
    
    if (n) {
        buffer_appendSymbol (b, sym_set);
        buffer_append (b, n, buffer_getAtoms (x->x_pathBuffer));
    }
    
    if (n && x->x_format) { buffer_appendComma (b); }
    
    if (x->x_format) {
        buffer_appendSymbol (b, sym_format);
        buffer_appendSymbol (b, x->x_format);
    }
    
    return b;
    //
    }
    
    return NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *oscformat_new (t_symbol *s, int argc, t_atom *argv)
{
    t_oscformat *x = (t_oscformat *)pd_new (oscformat_class);
    
    x->x_path       = (char *)PD_MEMORY_GET (PD_STRING);
    x->x_pathBuffer = buffer_new();
    x->x_format     = &s_;
    x->x_outlet     = outlet_newList (cast_object (x));
    
    while (argc > 0) {
    //
    t_symbol *t = atom_getSymbolAtIndex (0, argc, argv);

    if (argc > 1 && IS_SYMBOL (argv + 1) && (t == sym___dash__format)) {
        oscformat_format (x, GET_SYMBOL (argv + 1));
        argc -= 2;
        argv += 2;
        
    } else {
        break;
    }
    //
    }

    error__options (cast_object (x), s, argc, argv);
    
    oscformat_set (x, NULL, argc, argv);
    
    return x;
}

static void oscformat_free (t_oscformat *x)
{
    buffer_free (x->x_pathBuffer); PD_MEMORY_FREE (x->x_path);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void oscformat_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_oscformat,
            (t_newmethod)oscformat_new,
            (t_method)oscformat_free,
            sizeof (t_oscformat),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
    
    class_addBang (c, (t_method)oscformat_bang);
    class_addList (c, (t_method)oscformat_list);
    class_addAnything (c, (t_method)oscformat_anything);
    
    class_addMethod (c, (t_method)oscformat_set,    sym_set,    A_GIMME, A_NULL);
    class_addMethod (c, (t_method)oscformat_format, sym_format, A_DEFSYMBOL, A_NULL);

    class_setDataFunction (c, oscformat_functionData);

    oscformat_class = c;
}

void oscformat_destroy (void)
{
    class_free (oscformat_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
