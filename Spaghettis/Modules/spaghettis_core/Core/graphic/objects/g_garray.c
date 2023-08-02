
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
    int         x_width;
    int         x_height;
    int         x_start;
    int         x_end;
    t_float     x_low;
    t_float     x_high;
    int         x_size;
    t_word      *x_data;
    t_symbol    *x_unexpandedName;
    t_symbol    *x_name;
    };

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define GARRAY_WIDTH_MINIMUM    8
#define GARRAY_HEIGHT_MINIMUM   8

#define GARRAY_WIDTH_MAXIMUM    1024
#define GARRAY_HEIGHT_MAXIMUM   1024

#define GARRAY_WIDTH_DEFAULT    200
#define GARRAY_HEIGHT_DEFAULT   140

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
    
    if (size) { int n = PD_CLAMP (i, 0, size - 1); GARRAY_SET (n, f); }
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
}

static void garray_sinesum (t_garray *x, t_symbol *s, int argc, t_atom *argv)
{    
    garray_setWithSineWaves (x, s, argc, argv, 1);
}

static void garray_cosinesum (t_garray *x, t_symbol *s, int argc, t_atom *argv)
{    
    garray_setWithSineWaves (x, s, argc, argv, 0);
}

static void garray_read (t_garray *x, t_symbol *name)
{
    t_error err = PD_ERROR_NONE;
    
    t_fileproperties p; fileproperties_init (&p);
    
    int f = glist_fileOpen (object_getOwner (cast_object (x)), name->s_name, "", &p);
    
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
    }
    //
    }
    
    if (err) { error_canNotOpen (cast_object (x), name); }
}

static void garray_write (t_garray *x, t_symbol *name)
{
    char t[PD_STRING] = { 0 };
    t_glist *owner = object_getOwner (cast_object (x));
    const char *s = environment_getDirectoryAsString (glist_getEnvironment (owner));
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
    
    if (err) { error_canNotCreate (cast_object (x), name); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_APPLICATION )

extern Wrapper *main_wrapper;

static void garray_publish (t_garray *x)
{
    jassert (main_wrapper != nullptr);
    
    main_wrapper->getSnapshots().publish (object_getUnique (cast_object (x)), x->x_data, x->x_size);
}

static void garray_discard (t_garray *x)
{
    jassert (main_wrapper != nullptr);
    
    main_wrapper->getSnapshots().discard (object_getUnique (cast_object (x)));
}

#else

static void garray_publish (t_garray *x)
{
}

static void garray_discard (t_garray *x)
{
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_APPLICATION )

static void garray_objectUpdated (t_garray *x, const Tags& t)
{
    outputs_objectUpdated (cast_object (x), t);
    
    glist_setDirty (object_getOwner (cast_object (x)), 1);
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void garray_rename (t_garray *x, t_symbol *s)
{
    if (s != garray_getUnexpandedName (x)) {
    //
    t_symbol *expanded = dollar_expandSymbol (s, object_getOwner (cast_object (x)));
    
    if (garray_fetch (expanded)) { error_alreadyExists (cast_object (x), expanded); }
    else {
    //
    pd_unbind (cast_pd (x), x->x_name);
    x->x_unexpandedName = s;
    x->x_name = expanded;
    pd_bind (cast_pd (x), x->x_name);
    dsp_update();
    #if defined ( PD_BUILDING_APPLICATION )
    garray_objectUpdated (x, Tags::parameters (Tag::Name));
    #endif
    //
    }
    //
    }
}

static void garray_resizeProceed (t_garray *x, int n)
{
    int newSize = PD_MAX (0, n);
    int oldSize = x->x_size;
    
    if (newSize != oldSize) {
    //
    int dspState = 0;
    int dspSuspended = 0;
    
    size_t newBytes = newSize * sizeof (t_word);
    
    t_word *oldData = x->x_data;
    
    garray_discard (x);
    
    if (garray_isUsedInDsp (x)) { dspState = dsp_suspend(); dspSuspended = 1; }
    
    x->x_data = (t_word *)PD_MEMORY_GET (newBytes);
    x->x_size = newSize;
    
    garray_copy (x->x_data, oldData, PD_MIN (oldSize, newSize));
    
    if (garray_isUsedInDsp (x)) { garbage_newRaw ((void *)oldData); }
    else {
        PD_MEMORY_FREE (oldData);
    }
    
    if (dspSuspended) { dsp_resume (dspState); }
    
    garray_publish (x);
    
    #if defined ( PD_BUILDING_APPLICATION )
    garray_objectUpdated (x, Tags::parameters (Tag::Size));
    #endif
    //
    }
}

PD_LOCAL void garray_resize (t_garray *x, t_float f)
{
    PD_ASSERT (sys_isControlThread()); garray_resizeProceed (x, (int)f);
}

static void garray_embedProceed (t_garray *x, int n)
{
    if (x->x_embed != n) {
        x->x_embed = n;
        #if defined ( PD_BUILDING_APPLICATION )
        garray_objectUpdated (x, Tags::parameters (Tag::Embedded));
        #endif
    }
}

static void garray_embed (t_garray *x, t_float f)
{
    int n = (int)f; garray_embedProceed (x, n != 0);
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
    buffer_appendFloat (b,  x->x_width);
    buffer_appendFloat (b,  x->x_height);
    buffer_appendFloat (b,  x->x_start);
    buffer_appendFloat (b,  x->x_end);
    buffer_appendFloat (b,  x->x_low);
    buffer_appendFloat (b,  x->x_high);
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

static void garray_setWidthAndHeight (t_garray *x, int width, int height, int notify)
{
    #if defined ( PD_BUILDING_APPLICATION )
    
    int w = x->x_width;
    int h = x->x_height;
    
    #endif
    
    width  = width  ? width  : GARRAY_WIDTH_DEFAULT;
    height = height ? height : GARRAY_HEIGHT_DEFAULT;
        
    x->x_width  = PD_CLAMP (width,  GARRAY_WIDTH_MINIMUM, GARRAY_WIDTH_MAXIMUM);
    x->x_height = PD_CLAMP (height, GARRAY_HEIGHT_MINIMUM, GARRAY_HEIGHT_MAXIMUM);
    
    #if defined ( PD_BUILDING_APPLICATION )
    
    if (notify) {
        if (w != x->x_width)  { garray_objectUpdated (x, Tags::parameters (Tag::Width));  }
        if (h != x->x_height) { garray_objectUpdated (x, Tags::parameters (Tag::Height)); }
    }
    
    #endif
}

static void garray_setStartAndEnd (t_garray *x, int start, int end, int notify)
{
    #if defined ( PD_BUILDING_APPLICATION )
    
    int s = x->x_start;
    int e = x->x_end;
    
    #endif
    
    x->x_start = PD_MIN (start, end);
    x->x_end   = PD_MAX (start, end);
    
    #if defined ( PD_BUILDING_APPLICATION )
    
    if (notify) {
        if (s != x->x_start) { garray_objectUpdated (x, Tags::parameters (Tag::Start)); }
        if (e != x->x_end)   { garray_objectUpdated (x, Tags::parameters (Tag::End));   }
    }
    
    #endif
}

static void garray_setLowAndHigh (t_garray *x, t_float low, t_float high, int notify)
{
    #if defined ( PD_BUILDING_APPLICATION )
    
    t_float l = x->x_low;
    t_float h = x->x_high;
    
    #endif
    
    x->x_low  = PD_MIN (low, high);
    x->x_high = PD_MAX (low, high);
    
    #if defined ( PD_BUILDING_APPLICATION )
    
    if (notify) {
        if (l != x->x_low)  { garray_objectUpdated (x, Tags::parameters (Tag::Low));  }
        if (h != x->x_high) { garray_objectUpdated (x, Tags::parameters (Tag::High)); }
    }
    
    #endif
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_APPLICATION )

static void garray_functionGetParameters (t_object *o, core::Group& group, const Tags& t)
{
    t_garray *x = (t_garray *)o;
    
    static DelegateCache delegate;
    
    if (t.contains (Tag::Width)) {
        group.addParameter (Tag::Width,
            NEEDS_TRANS ("Width"),
            NEEDS_TRANS ("Width of the object"),
            x->x_width,
            delegate).setRange (juce::Range<int> (GARRAY_WIDTH_MINIMUM, GARRAY_WIDTH_MAXIMUM));
    }
    
    if (t.contains (Tag::Height)) {
        group.addParameter (Tag::Height,
            NEEDS_TRANS ("Height"),
            NEEDS_TRANS ("Height of the object"),
            x->x_height,
            delegate).setRange (juce::Range<int> (GARRAY_HEIGHT_MINIMUM, GARRAY_HEIGHT_MAXIMUM));
    }
    
    if (t.contains (Tag::Name)) {
        group.addParameter (Tag::Name,
            NEEDS_TRANS ("Name"),
            NEEDS_TRANS ("Unexpanded name of array"),
            makeString (symbol_getName (garray_getUnexpandedName (x))),
            delegate);
    }
    
    if (t.contains (Tag::Size)) {
        group.addParameter (Tag::Size,
            NEEDS_TRANS ("Array Size"),
            NEEDS_TRANS ("Number of values"),
            x->x_size,
            delegate);
    }
    
    if (t.contains (Tag::Embedded)) {
        group.addParameter (Tag::Embedded,
            NEEDS_TRANS ("Embedded"),
            NEEDS_TRANS ("Content saved with patch"),
            static_cast<bool> (x->x_embed),
            delegate);
    }
    
    if (t.contains (Tag::Start)) {
        group.addParameter (Tag::Start,
            NEEDS_TRANS ("Range Start"),
            NEEDS_TRANS ("Begin of the graph"),
            x->x_start,
            delegate);
    }
    
    if (t.contains (Tag::End)) {
        group.addParameter (Tag::End,
            NEEDS_TRANS ("Range End"),
            NEEDS_TRANS ("End of the graph"),
            x->x_end,
            delegate);
    }
    
    if (t.contains (Tag::Low)) {
        group.addParameter (Tag::Low,
            NEEDS_TRANS ("Range Low"),
            NEEDS_TRANS ("Low range for graph"),
            x->x_low,
            delegate);
    }
    
    if (t.contains (Tag::High)) {
        group.addParameter (Tag::High,
            NEEDS_TRANS ("Range High"),
            NEEDS_TRANS ("High range for graph"),
            x->x_high,
            delegate);
    }
}

static void garray_functionSetParameters (t_object *o, const core::Group& group)
{
    t_garray *x = (t_garray *)o;
    
    jassert (group.hasParameter (Tag::Width));
    jassert (group.hasParameter (Tag::Height));
    jassert (group.hasParameter (Tag::Name));
    jassert (group.hasParameter (Tag::Size));
    jassert (group.hasParameter (Tag::Embedded));
    jassert (group.hasParameter (Tag::Start));
    jassert (group.hasParameter (Tag::End));
    jassert (group.hasParameter (Tag::Low));
    jassert (group.hasParameter (Tag::High));
    
    t_symbol *name   = gensym (group.getParameter (Tag::Name).getValueTyped<juce::String>().toRawUTF8());
    const int size   = group.getParameter (Tag::Size).getValueTyped<int>();
    const int embed  = static_cast<int> (group.getParameter (Tag::Embedded).getValueTyped<bool>());
    
    const int width     = group.getParameter (Tag::Width).getValueTyped<int>();
    const int height    = group.getParameter (Tag::Height).getValueTyped<int>();
    const int start     = group.getParameter (Tag::Start).getValueTyped<int>();
    const int end       = group.getParameter (Tag::End).getValueTyped<int>();
    const t_float low   = group.getParameter (Tag::Low).getValueTyped<t_float>();
    const t_float high  = group.getParameter (Tag::High).getValueTyped<t_float>();
    
    garray_rename (x, name);
    garray_resizeProceed (x, size);
    garray_embedProceed (x, embed);
    garray_setWidthAndHeight (x, width, height, 1);
    garray_setStartAndEnd (x, start, end, 1);
    garray_setLowAndHigh (x, low, high, 1);
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void garray_newParameters (t_garray *x, t_symbol *s, int argc, t_atom *argv)
{
    int width       = atom_getFloatAtIndex (0, argc, argv);
    int height      = atom_getFloatAtIndex (1, argc, argv);
    int start       = atom_getFloatAtIndex (2, argc, argv);
    int end         = atom_getFloatAtIndex (3, argc, argv);
    t_float low     = atom_getFloatAtIndex (4, argc, argv);
    t_float high    = atom_getFloatAtIndex (5, argc, argv);
    
    garray_setWidthAndHeight (x, width, height, 0);
    garray_setStartAndEnd (x, start, end, 0);
    garray_setLowAndHigh (x, low, high, 0);
        
    if (argc > 6) { warning_unusedArguments (cast_object (x), s, argc - 6, argv + 6); }
}

static void *garray_new (t_symbol *s, int argc, t_atom *argv)
{
    t_garray *x = (t_garray *)pd_new (garray_class);
    
    t_symbol *name      = atom_getSymbolAtIndex (0, argc, argv);
    int size            = atom_getFloatAtIndex (1, argc, argv);
    int embed           = atom_getFloatAtIndex (2, argc, argv);
    
    x->x_embed          = (embed != 0);
    x->x_size           = PD_MAX (0, size);
    x->x_data           = (t_word *)PD_MEMORY_GET (x->x_size * sizeof (t_word));
    x->x_unexpandedName = NULL;
    x->x_name           = symbol_removeCopySuffix (name);
    
    /* Copy-pasted case first. */
    
    if (x->x_name != name) {
        x->x_name = garray_getUnusedBindName (x->x_name);
        x->x_unexpandedName = x->x_name;

    } else if (x->x_name == &s_ || garray_fetch (x->x_name)) {
        if (x->x_name != &s_) { error_alreadyExists (cast_object (x), x->x_name); }
        x->x_name = garray_getUnusedBindName (sym_array);
        x->x_unexpandedName = x->x_name;
    }

    pd_bind (cast_pd (x), x->x_name);
    
    garray_newParameters (x, s, argc - 3, argv + 3);
    
    garray_publish (x);
    
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
    garray_dismiss (x); garray_discard (x); PD_MEMORY_FREE (x->x_data);
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
    
    #if defined ( PD_BUILDING_APPLICATION )
    
    class_setParametersFunctions (c, garray_functionGetParameters, garray_functionSetParameters);
    
    #endif
    
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
