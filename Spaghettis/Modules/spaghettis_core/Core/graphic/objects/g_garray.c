
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

t_class *garray_class;                  /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

struct _garray {
    t_object    x_obj;                  /* MUST be the first. */
    int         x_isUsedInDsp;
    int         x_dismissed;
    int         x_embed;
    int         x_size;
    t_word      *x_data;
    t_glist     *x_owner;
    t_symbol    *x_unexpandedName;
    t_symbol    *x_name;
    };

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void garray_dismiss (t_garray *);
static void garray_copy (t_word *, t_word *, int);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define GARRAY_GET(i)       (w_getFloat (x->x_data + i))
#define GARRAY_SET(i, f)    (w_setFloat (x->x_data + i, f))

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void garray_setWithSineWavesProceed (t_garray *x,
    int numberOfPoints,
    int numberOfSineWaves,
    t_atom *amplitudeOfSineWaves,
    int isSine)
{
    double phase, phaseIncrement;
    int i;
    
    numberOfPoints = (numberOfPoints <= 0) ? 512 : numberOfPoints;
    numberOfPoints = PD_MIN (numberOfPoints, 1 << 30);
    
    if (!PD_IS_POWER_2 (numberOfPoints)) { numberOfPoints = (int)PD_NEXT_POWER_2 (numberOfPoints); }
    
    garray_resize (x, (t_float)(numberOfPoints + 3));
    
    phaseIncrement = PD_TWO_PI / numberOfPoints;
    
    for (i = 0, phase = -phaseIncrement; i < x->x_size; i++, phase += phaseIncrement) {
    //
    int j;
    double fj;
    double sum = 0.0;
    
    if (isSine) {
        for (j = 0, fj = phase; j < numberOfSineWaves; j++, fj += phase) { 
            sum += (double)atom_getFloatAtIndex (j, numberOfSineWaves, amplitudeOfSineWaves) * sin (fj);
        }
    } else {
        for (j = 0, fj = 0; j < numberOfSineWaves; j++, fj += phase) {
            sum += (double)atom_getFloatAtIndex (j, numberOfSineWaves, amplitudeOfSineWaves) * cos (fj);
        }
    }
    
    GARRAY_SET (i, (t_float)sum);
    //
    }
    
    garray_redraw (x);
}

static void garray_setWithSineWaves (t_garray *x, t_symbol *s, int argc, t_atom *argv, int isSine)
{    
    if (argc > 1) {
    //
    int n = atom_getFloatAtIndex (0, argc, argv);
    
    argv++;
    argc--;
    
    garray_setWithSineWavesProceed (x, n, argc, argv, isSine);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL int garray_getSize (t_garray *x)
{
    return x->x_size;
}

/* Due to resizing it must NOT be cached unless marked used in DSP. */

PD_LOCAL void garray_getData (t_garray *x, int *size, t_word **w)
{
    *size = x->x_size; *w = x->x_data;
}

PD_LOCAL void garray_setFloatAtIndex (t_garray *x, int i, t_float f)
{
    int size = x->x_size;
    
    if (size) { int n = PD_CLAMP (i, 0, size - 1); GARRAY_SET (n, f); garray_redraw (x); }
}

PD_LOCAL t_float garray_getFloatAtIndex (t_garray *x, int i)
{
    int size = x->x_size;
    
    if (size) { int n = PD_CLAMP (i, 0, size - 1); return GARRAY_GET (n); }
    
    return 0;
}

PD_LOCAL void garray_setFloatFromIndex (t_garray *x, int i, t_float f)
{
    int n, size = x->x_size;
    
    if (size) {
    //
    for (n = PD_CLAMP (i, 0, size - 1); n < size; n++) { GARRAY_SET (n, f); }
    
    garray_redraw (x);
    //
    }
}

PD_LOCAL t_float garray_getAmplitude (t_garray *x)
{
    int i, size = x->x_size;
    
    t_float f = 0.0;
    
    for (i = 0; i < size; i++) { t_float t = GARRAY_GET (i); f = PD_MAX (f, PD_ABS (t)); }

    return f;
}

static t_symbol *garray_getUnexpandedName (t_garray *x)
{
    return x->x_unexpandedName ? x->x_unexpandedName : object_getUnexpandedNameAt (cast_object (x), 1);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void garray_setAsUsedInDsp (t_garray *x, int usedInDsp)
{
    x->x_isUsedInDsp = (usedInDsp != 0);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL int garray_isUsedInDsp (t_garray *x)
{
    return (x->x_isUsedInDsp != 0);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void garray_redraw (t_garray *x)
{

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void garray_list (t_garray *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc > 1) { 
    //
    int i, j = atom_getFloat (argv);
    int size = x->x_size;
        
    argc--;
    argv++;

    if (j < 0) { argc += j; argv -= j; j = 0; }
    
    if (j + argc > size) { argc = size - j; }
    
    if (argc > 0) {
    //
    for (i = 0; i < argc; i++) { t_float f = atom_getFloat (argv + i); GARRAY_SET (i + j, f); }
    
    garray_redraw (x);
    //
    }
    //
    }
}

static void garray_constant (t_garray *x, t_float f)
{
    garray_setFloatFromIndex (x, 0, f);
}

static void garray_normalize (t_garray *x, t_float f)
{
    int i, size = x->x_size;
    
    double maximum = 0.0;

    if (f <= 0.0) { f = (t_float)1.0; }

    for (i = 0; i < size; i++) {
        double t = GARRAY_GET (i); if (PD_ABS (t) > maximum) { maximum = PD_ABS (t); }
    }
    
    if (maximum > 0.0) {
        double k = f / maximum;
        for (i = 0; i < size; i++) {
            t_float t = GARRAY_GET (i) * k; GARRAY_SET (i, t);
        }
    }
    
    garray_redraw (x);
}

static void garray_sinesum (t_garray *x, t_symbol *s, int argc, t_atom *argv)
{    
    garray_setWithSineWaves (x, s, argc, argv, 1);
}

static void garray_cosinesum (t_garray *x, t_symbol *s, int argc, t_atom *argv)
{    
    garray_setWithSineWaves (x, s, argc, argv, 0);
}

static void garray_rename (t_garray *x, t_symbol *s)
{
    if (s != x->x_unexpandedName) {
    //
    t_symbol *expanded = dollar_expandSymbol (s, x->x_owner);
    
    if (garray_fetch (expanded)) { error_alreadyExists (expanded); }
    else {
    //
    pd_unbind (cast_pd (x), x->x_name);
    x->x_unexpandedName = s;
    x->x_name = expanded;
    pd_bind (cast_pd (x), x->x_name);
    dsp_update();
    //
    }
    //
    }
}

static void garray_read (t_garray *x, t_symbol *name)
{
    t_error err = PD_ERROR_NONE;
    
    t_fileproperties p; fileproperties_init (&p);
    
    int f = glist_fileOpen (x->x_owner, name->s_name, "", &p);
    
    if (!(err |= (f < 0))) {
    //
    FILE *file = fdopen (f, "r");
    
    if (!(err |= (file == NULL))) {
        
        int i, size = x->x_size;

        for (i = 0; i < size; i++) {
        //
        double v = 0.0;
        if (!fscanf (file, "%lf", &v)) { break; }
        else {
            GARRAY_SET (i, (t_float)v);
        }
        //
        }
        
        while (i < size) { GARRAY_SET (i, 0.0); i++; }
        
        fclose (file);      /* < http://stackoverflow.com/a/13691168 > */
        
        garray_redraw (x);
    }
    //
    }
    
    if (err) { error_canNotOpen (name); }
}

static void garray_write (t_garray *x, t_symbol *name)
{
    char t[PD_STRING] = { 0 };
    const char *s = environment_getDirectoryAsString (glist_getEnvironment (x->x_owner));
    t_error err = path_withDirectoryAndName (t, PD_STRING, s, name->s_name);

    if (!err) {
    //
    int f = file_openWrite (t);
    
    if (!(err |= (f < 0))) {
    //
    FILE *file = fdopen (f, "w");
    
    if (!(err |= (file == NULL))) {
    
        int i, size = x->x_size;
        
        for (i = 0; i < size; i++) {
            if (fprintf (file, "%.9g\n", GARRAY_GET (i)) < 1) {
                PD_BUG; break;
            }
        }
        
        if (fclose (file) != 0) { PD_BUG; }
    }
    //
    }
    //
    }
    
    if (err) { error_canNotCreate (name); }
}

static void garray_resizeProceed (t_garray *x, int n)
{
    int newSize = PD_MAX (0, n);
    int oldSize = x->x_size;
    
    if (newSize != oldSize) {
    //
    size_t newBytes = newSize * sizeof (t_word);
    size_t oldBytes = oldSize * sizeof (t_word);
    
    t_word *t = x->x_data;
    
    if (garray_isUsedInDsp (x)) {
        x->x_data = (t_word *)PD_MEMORY_GET (newBytes);
        garray_copy (x->x_data, t, PD_MIN (oldSize, newSize));
        garbage_newRaw ((void *)t);
        
    } else {
        x->x_data = (t_word *)PD_MEMORY_RESIZE (t, oldBytes, newBytes);
    }
    
    x->x_size = newSize;
    //
    }
}

PD_LOCAL void garray_resize (t_garray *x, t_float f)
{
    int dspState = 0;
    int dspSuspended = 0;
    
    PD_ASSERT (sys_isControlThread());
    
    if (garray_isUsedInDsp (x)) { dspState = dsp_suspend(); dspSuspended = 1; }
    
    garray_resizeProceed (x, (int)f);
    
    if (dspSuspended) { dsp_resume (dspState); }
    
    garray_redraw (x);
}

static void garray_embed (t_garray *x, t_float f)
{
    x->x_embed = ((int)f != 0);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void garray_functionSave (t_object *z, t_buffer *b, int flags)
{
    t_garray *x = (t_garray *)z;
    
    buffer_appendSymbol (b, sym___hash__X);
    buffer_appendSymbol (b, sym_obj);
    buffer_appendFloat (b,  object_getX (cast_object (x)));
    buffer_appendFloat (b,  object_getY (cast_object (x)));
    buffer_appendSymbol (b, sym_garray);
    buffer_appendSymbol (b, garray_getUnexpandedName (x));
    buffer_appendFloat (b,  x->x_size);
    buffer_appendFloat (b,  x->x_embed);
    buffer_appendSemicolon (b);
    
    object_saveIdentifiers (z, b, flags);
}

static t_buffer *garray_functionData (t_object *z, int flags)
{
    t_garray *x = (t_garray *)z;

    if (SAVED_DEEP (flags) || x->x_embed) {
    //
    t_buffer *b = buffer_new();
    int i, n = x->x_size;
    
    buffer_appendFloat (b, 0); for (i = 0; i < n; i++) { buffer_appendFloat (b, GARRAY_GET (i)); }
    
    if (SAVED_DEEP (flags)) {
        buffer_appendComma (b); buffer_appendSymbol (b, sym__restore);
    }
    
    return b;
    //
    }
    
    return NULL;
}

static void garray_functionDismiss (t_object *z)
{
    garray_dismiss ((t_garray *)z);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void garray_copy (t_word *dest, t_word *src, int n)
{
    int i; for (i = 0; i < n; i++) { w_setFloat (dest + i, w_getFloat (src + i)); }
}

/* For now data are fetch during encapsulation only if properties of array have NOT changed. */

static void garray_restore (t_garray *x)
{
    t_garray *old = (t_garray *)instance_pendingFetch (cast_object (x));
    
    if (!old) {
        old = (t_garray *)garbage_fetch (cast_object (x));
    }
    
    if (old) {
    //
    t_error err = PD_ERROR_NONE;

    err |= (x->x_size  != old->x_size);
    err |= (x->x_embed != old->x_embed);
    err |= (x->x_name  != old->x_name);
    
    if (!err) { garray_copy (x->x_data, old->x_data, x->x_size); }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_garray *garray_fetch (t_symbol *s)
{
    return (t_garray *)symbol_getThingByClass (s, garray_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_symbol *garray_getUnusedBindName (t_symbol *prefix)
{
    int i = 1; char t[PD_STRING] = { 0 };
    
    while (i > 0) {
        t_error err = string_sprintf (t, PD_STRING, "%s-%d", prefix->s_name, i);
        if (err) { break; }
        else {
            t_symbol *name = gensym (t); if (!symbol_getThingByClass (name, garray_class)) { return name; }
            i++;
        }
    }
    
    PD_BUG; return prefix;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *garray_new (t_symbol *s, int argc, t_atom *argv)
{
    t_garray *x = (t_garray *)pd_new (garray_class);
    
    t_symbol *name      = atom_getSymbolAtIndex (0, argc, argv);
    int size            = atom_getFloatAtIndex (1, argc, argv);
    int embed           = atom_getFloatAtIndex (2, argc, argv);
    
    x->x_embed          = (embed != 0);
    x->x_size           = PD_MAX (0, size);
    x->x_data           = (t_word *)PD_MEMORY_GET (x->x_size * sizeof (t_word));
    x->x_owner          = instance_contextGetCurrent();
    x->x_unexpandedName = NULL;
    x->x_name           = symbol_removeCopySuffix (name);
    
    /* Copy-pasted case first. */
    
    if (x->x_name != name) {
        x->x_name = garray_getUnusedBindName (x->x_name);
        x->x_unexpandedName = x->x_name;

    } else if (x->x_name == &s_ || garray_fetch (x->x_name)) {
        if (x->x_name != &s_) { error_alreadyExists (x->x_name); }
        x->x_name = garray_getUnusedBindName (sym_array);
        x->x_unexpandedName = x->x_name;
    }

    pd_bind (cast_pd (x), x->x_name);
    
    garray_redraw (x);
        
    return x;
}

static void garray_dismiss (t_garray *x)
{
    if (!x->x_dismissed) {
    //
    x->x_dismissed = 1; pd_unbind (cast_pd (x), x->x_name);
    //
    }
}

static void garray_free (t_garray *x)
{
    garray_dismiss (x); PD_MEMORY_FREE (x->x_data);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void garray_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_garray,
            (t_newmethod)garray_new,
            (t_method)garray_free,
            sizeof (t_garray),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
    
    class_addList (c, (t_method)garray_list);
    
    class_addMethod (c, (t_method)garray_constant,  sym_constant,   A_DEFFLOAT, A_NULL);
    class_addMethod (c, (t_method)garray_normalize, sym_normalize,  A_DEFFLOAT, A_NULL);
    class_addMethod (c, (t_method)garray_sinesum,   sym_sinesum,    A_GIMME,  A_NULL);
    class_addMethod (c, (t_method)garray_cosinesum, sym_cosinesum,  A_GIMME,  A_NULL);
    class_addMethod (c, (t_method)garray_rename,    sym_rename,     A_SYMBOL, A_NULL);
    class_addMethod (c, (t_method)garray_read,      sym_read,       A_SYMBOL, A_NULL);
    class_addMethod (c, (t_method)garray_write,     sym_write,      A_SYMBOL, A_NULL);
    class_addMethod (c, (t_method)garray_resize,    sym_resize,     A_FLOAT,  A_NULL);
    class_addMethod (c, (t_method)garray_embed,     sym_embed,      A_FLOAT,  A_NULL);
    class_addMethod (c, (t_method)garray_restore,   sym__restore,   A_NULL);
    
    class_setSaveFunction (c, garray_functionSave);
    class_setDataFunction (c, garray_functionData);
    class_setDismissFunction (c, garray_functionDismiss);
    class_requirePending (c);
    
    garray_class = c;
}

PD_LOCAL void garray_destroy (void)
{
    class_free (garray_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
