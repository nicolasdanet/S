
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

static t_class *oscparse_class;         /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _oscpending {
    t_clock             *x_clock;
    t_buffer            *x_buffer;
    struct _oscparse    *x_owner;
    struct _oscpending  *x_next;
    } t_oscpending;

typedef struct _oscparse {
    t_object            x_obj;          /* Must be the first. */
    int                 x_discard;
    t_oscpending        *x_pending;
    t_outlet            *x_outlet;
    } t_oscparse;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_error oscparse_proceedBundle (t_oscparse *, int, t_atom *);
static t_error oscparse_proceed (t_oscparse *, int, t_atom *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void oscpending_free (t_oscpending *x)
{
    buffer_free (x->x_buffer);
    clock_free (x->x_clock);
    
    PD_MEMORY_FREE (x);
}

static void oscpending_task (t_oscpending *x)
{
    t_oscparse *owner = x->x_owner;
    
    if (owner->x_pending == x) { owner->x_pending = x->x_next; }
    else {
        t_oscpending *m = NULL;
        t_oscpending *n = NULL;
        for ((m = owner->x_pending); (n = m->x_next); (m = n)) {
            if (n == x) { m->x_next = n->x_next; break; }
        }
    }
    
    oscparse_proceedBundle (owner, buffer_getSize (x->x_buffer), buffer_getAtoms (x->x_buffer));
    
    oscpending_free (x);
}

static void oscpending_new (t_oscparse *server, t_nano ns, int argc, t_atom *argv)
{
    t_oscpending *x = (t_oscpending *)PD_MEMORY_GET (sizeof (t_oscpending));
    
    x->x_clock  = clock_newSingle ((void *)x, (t_method)oscpending_task);
    x->x_buffer = buffer_new();
    x->x_owner  = server;
    x->x_next   = server->x_pending;
    
    server->x_pending = x;
    
    buffer_append (x->x_buffer, argc, argv);
    
    clock_delay (x->x_clock, PD_NANOSECONDS_TO_MILLISECONDS (ns));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_symbol *oscparse_fetchNextAdressElement (int argc, t_atom *argv, int *i)
{
    int j = *i;
    
    char t[PD_STRING] = { 0 };
    
    /* Multiple-level wildcard matching (i.e. '//'). */
    
    int multiple = (j < argc - 1 && OSC_GETCHAR (argv + j) == '/' && OSC_GETCHAR (argv + j + 1) == '/');
    
    if (multiple) { j += 2; t[0] = '/'; t[1] = 0; }
    else {
    //
    int n = 0;
    
    while (j < argc && OSC_GETCHAR (argv + j) == '/') { j++; }
        
    for (n = 0; n < PD_STRING - 1 && j < argc; n++, j++) {
        char c = OSC_GETCHAR (argv + j);
        if (c == 0 || c == '/') { break; }
        else {
            t[n] = c;
        }
    }
    
    t[n] = 0;
    //
    }
    
    PD_ASSERT (j <= argc);
    
    *i = j;
    
    return gensym (t);
}

static t_symbol *oscparse_fetchString (int argc, t_atom *argv, int *i)
{
    int j = *i;
    
    char t[PD_STRING] = { 0 };
    int n = 0;
    
    for (n = 0; n < PD_STRING - 1 && j < argc; n++, j++) {
        char c = OSC_GETCHAR (argv + j);
        if (c == 0) { break; }
        else {
            t[n] = c;
        }
    }
    
    t[n] = 0;
    
    PD_ASSERT (j <= argc);
    
    *i = OSC_4ROUND (j + 1);
    
    return gensym (t);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_error oscparse_proceedArgumentsFloat (t_oscparse *x,
    int argc,
    t_atom *argv,
    int *dataOffset,
    int *atomOffset,
    t_atom *a,
    int size)
{
    t_error err = PD_ERROR_NONE;
    
    int k = *dataOffset;
    int n = *atomOffset;
    
    if (k > argc - 4) { err = PD_ERROR; }
    else {
        t_pun32 z;
        pun32_setInteger (&z, OSC_4READ (argv + k));
        float f = pun32_getFloat (&z); if (PD_FLOAT32_IS_INVALID_OR_ZERO (f)) { f = 0.0; }
        SET_FLOAT (a + n, (t_float)f);
        n++; k += 4;
    }
    
    *dataOffset = k;
    *atomOffset = n;
    
    return err;
}

static t_error oscparse_proceedArgumentsInteger (t_oscparse *x,
    int argc,
    t_atom *argv,
    int *dataOffset,
    int *atomOffset,
    t_atom *a,
    int size)
{
    t_error err = PD_ERROR_NONE;
    
    int k = *dataOffset;
    int n = *atomOffset;
    
    if (k > argc - 4) { err = PD_ERROR; }
    else {
        SET_FLOAT (a + n, OSC_4READ (argv + k));
        n++; k += 4;
    }
    
    *dataOffset = k;
    *atomOffset = n;
    
    return err;
}

static t_error oscparse_proceedArgumentsString (t_oscparse *x,
    int argc,
    t_atom *argv,
    int *dataOffset,
    int *atomOffset,
    t_atom *a,
    int size)
{
    t_error err = PD_ERROR_NONE;
    
    int k = *dataOffset;
    int n = *atomOffset;
    
    if (k > argc - 1) { err = PD_ERROR; }
    else {
        SET_SYMBOL (a + n, oscparse_fetchString (argc, argv, &k));
        n++;
    }
                
    *dataOffset = k;
    *atomOffset = n;
    
    return err;
}

static t_error oscparse_proceedArgumentsBlob (t_oscparse *x,
    int argc,
    t_atom *argv,
    int *dataOffset,
    int *atomOffset,
    t_atom *a,
    int size)
{
    t_error err = PD_ERROR_NONE;
    
    int k = *dataOffset;
    int n = *atomOffset;
    
    if (k > argc - 4) { err = PD_ERROR; }
    else {
        int blobSize = OSC_4READ (argv + k);
        k += 4;
        err |= (blobSize < 0 || blobSize > argc - k);
        err |= (n + blobSize >= size);
        if (!err) {
            int j;
            SET_FLOAT (a + n, blobSize);
            n++;
            for (j = 0; j < blobSize; j++) {
                SET_FLOAT (a + n, GET_FLOAT (argv + k));
                n++;
                k++;
            }
            k = OSC_4ROUND (k);
        }
    }
                
    *dataOffset = k;
    *atomOffset = n;
    
    return err;
}

static t_error oscparse_proceedArgumentsStamp (t_oscparse *x,
    int argc,
    t_atom *argv,
    int *dataOffset,
    int *atomOffset,
    t_atom *a,
    int size)
{
    t_error err = PD_ERROR_NONE;
    
    int k = *dataOffset;
    int n = *atomOffset;
    
    err |= (k > argc - 8);
    err |= (n + STAMP_SIZE > size);
    
    if (!err) {
        t_stamp t = OSC_8READ (argv + k);
        err = stamp_serialize (STAMP_SIZE, a + n, &t);
        n += STAMP_SIZE; k += 8;
    }
    
    *dataOffset = k;
    *atomOffset = n;
    
    return err;
}

static t_error oscparse_proceedArgumentsDouble (t_oscparse *x,
    int argc,
    t_atom *argv,
    int *dataOffset,
    int *atomOffset,
    t_atom *a,
    int size)
{
    t_error err = PD_ERROR_NONE;
    
    int k = *dataOffset;
    int n = *atomOffset;
    
    if (k > argc - 8) { err = PD_ERROR; }
    else {
        t_pun64 z = pun64_initialize();
        pun64_setMostSignificantBytes (&z, OSC_4READ (argv + k + 0));
        pun64_setLeastSignificantBytes (&z, OSC_4READ (argv + k + 4));
        double f = pun64_getDouble (&z); if (PD_FLOAT64_IS_INVALID_OR_ZERO (f)) { f = 0.0; }
        SET_FLOAT (a + n, (t_float)f);
        n++; k += 8;
    }
    
    *dataOffset = k;
    *atomOffset = n;
    
    return err;
}

static t_error oscparse_proceedArgumentsTrue (t_oscparse *x,
    int argc,
    t_atom *argv,
    int *dataOffset,
    int *atomOffset,
    t_atom *a,
    int size)
{
    int n = *atomOffset;
    
    SET_SYMBOL (a + n, sym_true); n++;
    
    *atomOffset = n;
    
    return PD_ERROR_NONE;
}

static t_error oscparse_proceedArgumentsFalse (t_oscparse *x,
    int argc,
    t_atom *argv,
    int *dataOffset,
    int *atomOffset,
    t_atom *a,
    int size)
{
    int n = *atomOffset;
    
    SET_SYMBOL (a + n, sym_false); n++;
    
    *atomOffset = n;
    
    return PD_ERROR_NONE;
}

static t_error oscparse_proceedArgumentsNull (t_oscparse *x,
    int argc,
    t_atom *argv,
    int *dataOffset,
    int *atomOffset,
    t_atom *a,
    int size)
{
    int n = *atomOffset;
    
    SET_SYMBOL (a + n, sym_null); n++;
    
    *atomOffset = n;
    
    return PD_ERROR_NONE;
}

static t_error oscparse_proceedArgumentsBang (t_oscparse *x,
    int argc,
    t_atom *argv,
    int *dataOffset,
    int *atomOffset,
    t_atom *a,
    int size)
{
    int n = *atomOffset;
    
    SET_SYMBOL (a + n, &s_bang); n++;
    
    *atomOffset = n;
    
    return PD_ERROR_NONE;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_error oscparse_proceedArguments (t_oscparse *x,
    int argc,
    t_atom *argv,
    int i,
    int *k,
    int *n,
    t_atom *a,
    int size)
{
    if (*k <= argc) {
    //
    switch (OSC_GETCHAR (argv + i)) {
        case 'f': return oscparse_proceedArgumentsFloat (x, argc, argv, k, n, a, size);
        case 'i': return oscparse_proceedArgumentsInteger (x, argc, argv, k, n, a, size);
        case 's': return oscparse_proceedArgumentsString (x, argc, argv, k, n, a, size);
        case 'b': return oscparse_proceedArgumentsBlob (x, argc, argv, k, n, a, size);
        case 't': return oscparse_proceedArgumentsStamp (x, argc, argv, k, n, a, size);
        case 'd': return oscparse_proceedArgumentsDouble (x, argc, argv, k, n, a, size);
        case 'T': return oscparse_proceedArgumentsTrue (x, argc, argv, k, n, a, size);
        case 'F': return oscparse_proceedArgumentsFalse (x, argc, argv, k, n, a, size);
        case 'N': return oscparse_proceedArgumentsNull (x, argc, argv, k, n, a, size);
        case 'I': return oscparse_proceedArgumentsBang (x, argc, argv, k, n, a, size);
    }
    //
    }
    
    PD_BUG; return PD_ERROR;
}

static int oscparse_proceedFetch (t_oscparse *x,
    int argc,
    t_atom *argv,
    int typeOnset,
    int numberOfTypeTags,
    t_atom *a,
    int size)
{
    int i, n = 0;

    /* Fill the elements of the adress path. */
    
    for (i = 0; i < typeOnset - 1; n++) {
        if (OSC_GETCHAR (argv + i) == 0) { break; }
        else {
            t_symbol *address = oscparse_fetchNextAdressElement (argc, argv, &i);
            PD_ASSERT (n < size); SET_SYMBOL (a + n, address);
        }
    }
    
    /* Fill the arguments. */
    
    int k = OSC_4ROUND (typeOnset + numberOfTypeTags + 1);
    
    for (i = typeOnset; i < typeOnset + numberOfTypeTags; i++) {
        if (n >= size || oscparse_proceedArguments (x, argc, argv, i, &k, &n, a, size)) { return -1; }
    }
    
    return n;
}

static t_error oscparse_proceedBundle (t_oscparse *x, int argc, t_atom *argv)
{
    t_error err = PD_ERROR_NONE;
    
    /* Notice that timetag is ignored above. */
    
    const int headerBundle = 16;
    const int headerMessage = 4;
    
    if (argc == headerBundle) { warning_empty (cast_object (x), sym_oscparse, sym_bundle); }
    
    if (argc >= headerBundle) {
    //
    int i = headerBundle;
        
    while (!err && (i < argc - headerMessage)) {
    //
    int length = OSC_4READ (argv + i);
        
    err = (length <= 0 || length & 3);  /* Must be a multiple of 4. */
    
    if (!err) { 
        err = oscparse_proceed (x, length, argv + i + headerMessage);
        i += headerMessage + length;
    }
    //
    }
    //
    }
    
    if (err) { error_invalid (cast_object (x), sym_oscparse, sym_bundle); }
    
    return err;
}

static t_error oscparse_proceedQueue (t_oscparse *x, int argc, t_atom *argv)
{
    t_error err = (argc < 16);
    
    if (!err) {
    //
    err |= (OSC_GETCHAR (argv + 1) != 'b');
    err |= (OSC_GETCHAR (argv + 2) != 'u');
    err |= (OSC_GETCHAR (argv + 3) != 'n');
    err |= (OSC_GETCHAR (argv + 4) != 'd');
    err |= (OSC_GETCHAR (argv + 5) != 'l');
    err |= (OSC_GETCHAR (argv + 6) != 'e');
    err |= (OSC_GETCHAR (argv + 7) != 0);
    //
    }
    
    if (!err) {
    //
    t_stamp t = OSC_8READ (argv + 8);
    
    /* Parse immediately OSC bundle if required. */
    /* Do it also for expired timetags (if NOT in discard mode). */
    
    if (stamp_isImmediately (&t)) {
        return oscparse_proceedBundle (x, argc, argv);
        
    } else {
    //
    t_stamp now;
    t_nano ns;
    
    stamp_set (&now);
    
    if (stamp_elapsedNanoseconds (&now, &t, &ns) != PD_ERROR_NONE) {
        if (!x->x_discard) { return oscparse_proceedBundle (x, argc, argv); }
    } else {
        oscpending_new (x, ns, argc, argv);
    }
    //
    }
    //
    }
    
    return err;
}

static t_error oscparse_proceed (t_oscparse *x, int argc, t_atom *argv)
{
    if (OSC_GETCHAR (argv) == '#') { return oscparse_proceedQueue (x, argc, argv); }
    else if (OSC_GETCHAR (argv) == '/') {
    //
    t_error err = PD_ERROR_NONE;
    int i, size = 1;

    /* Get the number of elements in the adress path. */
    
    for (i = 1; i < argc; i++) {
        if (OSC_GETCHAR (argv + i) == 0) { break; }
        else {
            if (OSC_GETCHAR (argv + i) == '/') { 
                size++;
            }
        }
    }
    
    i = OSC_4ROUND (i + 1);
    
    /* Test existence of typetags. */
    
    err = ((OSC_GETCHAR (argv + i) != ',') || (i + 1 >= argc));
    
    i++;
    
    if (!err) {
    //
    int typeOnset = i;              /* Index of first typetag. */
    int numberOfTypeTags = 0;
    int numberOfStamps = 0;
    int hasBlob = 0;
    
    /* Get the number of typetags. */
    
    for (; i < argc; i++) {
        if (OSC_GETCHAR (argv + i) == 0) { break; }
        else {
            if (!osc_isValidTypetag (OSC_GETCHAR (argv + i))) { err = PD_ERROR; }
            else {
                numberOfTypeTags++;
                hasBlob += (OSC_GETCHAR (argv + i) == 'b');
                numberOfStamps += (OSC_GETCHAR (argv + i) == 't');
            }
        }
    }
    
    if (!err) {
    //
    t_atom *a = NULL;
    int n;
    
    /* Extra size required if message contains blobs or timetags. */
    /* Notice that the pessimistic maximum size is allocated. */
    
    size += hasBlob ? argc - typeOnset : numberOfTypeTags;
    size += numberOfStamps * (STAMP_SIZE - 1);
    
    PD_ATOMS_ALLOCA (a, size);
    
    /* Fetch adress and arguments. */
    
    n = oscparse_proceedFetch (x, argc, argv, typeOnset, numberOfTypeTags, a, size);
    
    if (n == -1) { err = PD_ERROR; }
    else {
        outlet_list (x->x_outlet, n, a);
    }
    
    PD_ATOMS_FREEA (a, size);
    //
    }
    //
    }
    
    if (err) { error_invalid (cast_object (x), sym_oscparse, sym_message); }
    
    return err;
    //
    }
    
    return PD_ERROR;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void oscparse_list (t_oscparse *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc) {
    //
    t_error err = PD_ERROR_NONE;
    int i;
    
    for (i = 0; i < argc; i++) {
        err |= (!IS_FLOAT (argv + i) || OSC_GETCHAR (argv + i) < 0 || OSC_GETCHAR (argv + i) > 0xff); 
    }
        
    if (!err) {
        err = oscparse_proceed (x, argc, argv);
    }
    
    if (err) { 
        error_failed (cast_object (x), sym_oscparse);
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void oscparse_discard (t_oscparse *x, t_float f)
{
    x->x_discard = (f != 0.0);
}

static void oscparse_flush (t_oscparse *x)      /* FIFO. */
{
    while (x->x_pending) {
    //
    t_oscpending *m = NULL;
    t_oscpending *n = NULL;
    
    for ((m = x->x_pending); (n = m->x_next); (m = n)) { }
    
    oscpending_task (m);
    //
    }
}

static void oscparse_clear (t_oscparse *x)
{
    t_oscpending *p = NULL; while ((p = x->x_pending)) { x->x_pending = p->x_next; oscpending_free (p); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_oscparse *oscparse_new (t_symbol *s, int argc, t_atom *argv)
{
    t_oscparse *x = (t_oscparse *)pd_new (oscparse_class);
    
    x->x_pending = NULL;
    x->x_outlet  = outlet_newList (cast_object (x));
    
    while (argc > 0) {
    //
    t_symbol *t = atom_getSymbolAtIndex (0, argc, argv);

    if (t == sym___dash__discard) {
        oscparse_discard (x, 1.0);
        argc--;
        argv++;
        
    } else {
        break;
    }
    //
    }

    error__options (cast_object (x), s, argc, argv);
    
    if (argc) { warning_unusedArguments (cast_object (x), s, argc, argv); }
    
    return x;
}

static void oscparse_free (t_oscparse *x)
{
    oscparse_clear (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void oscparse_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_oscparse,
            (t_newmethod)oscparse_new,
            (t_method)oscparse_free,
            sizeof (t_oscparse),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
            
    class_addList (c, (t_method)oscparse_list);
    
    class_addMethod (c, (t_method)oscparse_discard, sym_discard,    A_FLOAT, A_NULL);
    class_addMethod (c, (t_method)oscparse_flush,   sym_flush,      A_NULL);
    class_addMethod (c, (t_method)oscparse_clear,   sym_clear,      A_NULL);
    
    oscparse_class = c;
}

void oscparse_destroy (void)
{
    class_free (oscparse_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
