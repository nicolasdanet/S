
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

extern t_symbol *main_directoryHelp;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int object_isDummy (t_object *x)
{
    return (pd_class (x) == comment_class && object_isObject (x));
}

int object_isCanvas (t_object *x)
{
    return (pd_class (x) == canvas_class);
}

int object_isObject (t_object *x)
{
    return (x->g_type == TYPE_OBJECT);
}

int object_isComment (t_object *x)
{
    return (x->g_type == TYPE_COMMENT);
}

int object_isMessage (t_object *x)
{
    return (x->g_type == TYPE_MESSAGE);
}

#if 0

int object_isAtom (t_object *x)
{
    return (x->g_type == TYPE_ATOM);
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static int object_hasDspProceed (t_object *x, int k)
{
    if (pd_class (x) == garray_class)       { return k ? 1 : garray_isUsedInDsp ((t_garray *)x); }
    else if (pd_class (x) == vinlet_class)  { return vinlet_isSignal ((t_vinlet *)x);   }
    else if (pd_class (x) == voutlet_class) { return voutlet_isSignal ((t_voutlet *)x); }
    else {
        return class_hasDsp (pd_class (x));
    }
}

int object_hasDspOrIsGraphicArray (t_object *x)
{
    return object_hasDspProceed (x, 1);
}

int object_hasDsp (t_object *x)
{
    return object_hasDspProceed (x, 0);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_symbol *object_getUnexpandedNameAt (t_object *x, int i)
{
    t_buffer *z = buffer_new();
    buffer_serialize (z, object_getBuffer (x));
    t_symbol *s = atom_getSymbolAtIndex (i, buffer_getSize (z), buffer_getAtoms (z));
    buffer_free (z);
    
    return s;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int object_setSnappedX (t_object *x, int n)
{
    int k = object_getX (x);
    
    if (instance_snapIsSet()) { n = instance_snapGetSnapped (n); }
    
    object_setX (x, n);
    
    return (object_getX (x) - k);
}

int object_setSnappedY (t_object *x, int n)
{
    int k = object_getY (x);

    if (instance_snapIsSet()) { n = instance_snapGetSnapped (n); }
    
    object_setY (x, n);
    
    return (object_getY (x) - k);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void object_setBuffer (t_object *x, t_buffer *b)      /* Acquires ownership. */
{
    if (x->g_buffer) { buffer_free (x->g_buffer); }
    
    PD_ASSERT (b);
    
    x->g_buffer = b;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void object_distributeAtomsOnInlets (t_object *x, int argc, t_atom *argv)
{
    if (argc) { 
        
        int count;
        t_atom *a = NULL;
        t_inlet *i = x->g_inlets;
        
        for (count = argc - 1, a = argv + 1; i && count--; a++, i = inlet_getNext (i)) {
        //
        if (IS_FLOAT (a)) { pd_float (cast_pd (i), GET_FLOAT (a)); }
        else {
            pd_symbol (cast_pd (i), GET_SYMBOL (a));
        }
        //
        }
        
        if (IS_FLOAT (argv)) { pd_float (cast_pd (x), GET_FLOAT (argv)); }
        else {
            pd_symbol (cast_pd (x), GET_SYMBOL (argv));
        }
        
    } else {
    
        if (class_hasOverrideBangMethod (pd_class (x))) {
            (*(class_getBangMethod (pd_class (x)))) (cast_pd (x)); 
        } else {
            (*(class_getAnythingMethod (pd_class (x)))) (cast_pd (x), &s_bang, 0, NULL);
        }
    }
} 

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Note that the linked list of inlets do NOT contains the first inlet. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_outconnect *object_connect (t_object *src, int m, t_object *dest, int n, t_glist *glist)
{
    t_outconnect *oc = NULL;
    t_outlet *o = NULL;
    
    PD_ASSERT (m >= 0);
    PD_ASSERT (n >= 0);
    
    #if defined ( PD_BUILDING_APPLICATION )
        
    int mCached = m;
    int nCached = n;
        
    #endif
    
    for (o = src->g_outlets; o && m; o = outlet_getNext (o), m--) { }
    
    if (o != NULL) { 
    //
    t_pd *receiver = NULL;
    
    if (class_hasFirstInlet (pd_class (dest))) { if (!n) { receiver = cast_pd (dest); } else { n--; } }
    
    if (receiver == NULL) {
        t_inlet *i = NULL; for (i = dest->g_inlets; i && n; i = inlet_getNext (i), n--) { }
        if (i == NULL) { return NULL; }
        else {
            receiver = cast_pd (i);
        }
    }

    oc = outlet_addConnection (o, receiver);            /* Can be NULL if connection already exists. */
    
    #if defined ( PD_BUILDING_APPLICATION )
        
    if (oc) {
        jassert (glist); outputs_lineAdded (connection_getUnique (oc), src, mCached, dest, nCached, glist);
    }
        
    #endif
    
    if (oc && outlet_isSignal (o)) { dsp_update(); }
    //
    }
    
    return oc;
}

t_error object_disconnect (t_object *src, int m, t_object *dest, int n, t_glist *glist)
{
    t_error err = PD_ERROR;
    
    t_outlet *o = NULL;
    
    PD_ASSERT (m >= 0);
    PD_ASSERT (n >= 0);
        
    for (o = src->g_outlets; o && m; o = outlet_getNext (o), m--) { }
    
    if (o != NULL) {
    //
    t_pd *receiver = NULL;
    
    t_id u = 0;
    
    if (class_hasFirstInlet (pd_class (dest))) { if (!n) { receiver = cast_pd (dest); } else { n--; } }
    
    if (receiver == NULL) {
        t_inlet *i = NULL; for (i = dest->g_inlets; i && n; i = inlet_getNext (i), n--) { }
        if (i == NULL) { return PD_ERROR; }
        else {
            receiver = cast_pd (i);
        }
    }

    err = outlet_removeConnection (o, receiver, &u);
    
    #if defined ( PD_BUILDING_APPLICATION )
    
    if (!err) { jassert (glist); outputs_lineRemoved (u, glist); }
    
    #endif
    
    if (!err && outlet_isSignal (o)) { dsp_update(); }
    //
    }
    
    return err;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void object_openHelp (const char *directory, const char *name)
{
    t_fileproperties p; fileproperties_init (&p);
    int f = -1;
    
    if (*directory != 0) { f = file_openReadWithDirectoryAndName (directory, name, PD_HELP, &p); }
    
    if (f < 0 && main_directoryHelp) {
        f = file_openReadConsideringSearchPath (main_directoryHelp->s_name, name, PD_HELP, &p);
    }
    
    if (f < 0) { error_canNotFind (NULL, gensym (name), sym_help); }
    else {
        t_symbol *s1 = gensym (fileproperties_getName (&p));
        t_symbol *s2 = gensym (fileproperties_getDirectory (&p));
        close (f);
        instance_patchOpen (s1, s2);
    }
}

/* First consider the sibling files of an abstraction. */
/* For an external search in its help directory if provided. */
/* Then look for in the application "help" folder. */
/* And last in the user search path. */

void object_help (t_object *y)
{
    const char *directory = NULL;
    char name[PD_STRING] = { 0 };
    t_error err = PD_ERROR_NONE;
    
    if (object_isCanvas (y) && glist_isAbstraction (cast_glist (y))) {
        if (!(err = (buffer_getSize (object_getBuffer (y)) < 1))) {
            atom_toString (buffer_getAtoms (object_getBuffer (y)), name, PD_STRING);
            directory = environment_getDirectoryAsString (glist_getEnvironment (cast_glist (y)));
        }
        
    } else {
        err = string_copy (name, PD_STRING, class_getHelpNameAsString (pd_class (y)));
        directory = class_getHelpDirectoryAsString (pd_class (y));
    }
    
    if (!err) { object_openHelp (directory, name); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int object_getNumberOfInlets (t_object *x)
{
    int n = 0;
    t_inlet *i = NULL;
    if (class_hasFirstInlet (pd_class (x))) { n++; }
    for (i = x->g_inlets; i; i = inlet_getNext (i)) { n++; }
    return n;
}

int object_getNumberOfOutlets (t_object *x)
{
    int n = 0;
    t_outlet *o = NULL;
    for (o = x->g_outlets; o; o = outlet_getNext (o)) { n++; }
    return n;
}

int object_getNumberOfSignalInlets (t_object *x)
{
    int n = 0;
    t_inlet *i = NULL;
    if (class_hasFirstInletAsSignal (pd_class (x)))  { n++; }
    for (i = x->g_inlets; i; i = inlet_getNext (i)) { if (inlet_isSignal (i)) { n++; } }
    return n;
}

int object_getNumberOfSignalOutlets (t_object *x)
{
    int n = 0;
    t_outlet *o = NULL;
    for (o = x->g_outlets; o; o = outlet_getNext (o)) { if (outlet_isSignal (o)) { n++; } }
    return n;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int object_getIndexAsSignalOfInlet (t_object *x, int m)
{
    int n = 0;
    t_inlet *i = NULL;
    
    PD_ASSERT (m >= 0);
        
    if (class_hasFirstInletAsSignal (pd_class (x))) {
        if (!m) { return 0; } else { n++; } 
        m--;
    }
    
    for (i = x->g_inlets; i; i = inlet_getNext (i), m--) {
        if (inlet_isSignal (i)) { 
            if (m == 0) { return n; } 
            else { 
                n++; 
            }
        }
    }
    
    return -1;
}

int object_getIndexAsSignalOfOutlet (t_object *x, int m)
{
    int n = 0;
    t_outlet *o = NULL;
    
    PD_ASSERT (m >= 0);
        
    for (o = x->g_outlets; o; o = outlet_getNext (o), m--) {
        if (outlet_isSignal (o)) {
            if (m == 0) { return n; }
            else {
                n++;
            }
        }
    }
    
    return -1;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if 0

int object_isSignalInlet (t_object *x, int m)
{
    t_inlet *i = NULL;
    
    if (class_hasFirstInlet (pd_class (x))) {
        if (!m) { return (class_hasFirstInletAsSignal (pd_class (x))); }
        else {
            m--;
        }
    }
    
    for (i = x->g_inlets; i && m; i = inlet_getNext (i), m--) { }
    
    return (i && inlet_isSignal (i));
}

#endif

int object_isSignalOutlet (t_object *x, int m)
{
    t_outlet *o = NULL;
    
    for (o = x->g_outlets; o && m--; o = outlet_getNext (o)) { }
    
    return (o && outlet_isSignal (o));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_APPLICATION )

juce::String object_getTypeOfInlets (t_object *x)
{
    juce::StringArray s;
    
    t_inlet *i = NULL;
    
    if (class_hasFirstInlet (pd_class (x))) {
        if (class_hasFirstInletAsSignal (pd_class (x))) { s.add (s_signal.s_name); }
        else {
            s.add (s_anything.s_name);
        }
    }
    
    for (i = x->g_inlets; i; i = inlet_getNext (i)) { s.add (inlet_getType (i)->s_name); }
    
    return s.joinIntoString (" ");
}

juce::String object_getTypeOfOutlets (t_object *x)
{
    juce::StringArray s;
    
    t_outlet *o = NULL;
    
    for (o = x->g_outlets; o; o = outlet_getNext (o)) { s.add (outlet_getType (o)->s_name); }
    
    return s.joinIntoString (" ");
}

juce::String object_getBufferAsString (t_object* x)
{
    juce::String s;
    
    if (x) {
    //
    t_buffer* b = object_getBuffer (x);
    
    if (b && buffer_getSize (b)) {
        s = makeString (buffer_getSize (b), buffer_getAtoms (b));
    }
    //
    }
    
    return s;
}

bool object_setBufferWithString (t_object *x, const juce::String& s)
{
    jassert (x);
    
    if (object_getBufferAsString (x) != s) {
    //
    buffer_withStringUnzeroed (object_getBuffer (x), s.toRawUTF8(), static_cast<int> (s.getNumBytesAsUTF8()));
    
    return true;
    //
    }
    
    return false;
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_buffer *object_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_buffer *b = buffer_new();

    buffer_appendSymbol (b, sym__restore);
    
    return b;
    //
    }
    
    return NULL;
}

void object_save (t_object *x, t_buffer *b, int flags)
{
    if (class_hasSaveFunction (pd_class (x))) { (*(class_getSaveFunction (pd_class (x)))) (x, b, flags); }
    
    if (class_hasDataFunction (pd_class (x))) {
    //
    t_buffer *t = (*(class_getDataFunction (pd_class (x)))) (x, flags);

    if (t) {
        buffer_appendSymbol (b, sym___hash__A);
        buffer_appendBuffer (b, t);
        buffer_appendSemicolon (b);
        buffer_free (t);
    }
    //
    }
}

void object_saveIdentifiers (t_object *x, t_buffer *b, int flags)
{
    if (flags & SAVE_UNDO)        { object_serializeUnique (x, sym__tagobject, b); }
    if (flags & SAVE_ENCAPSULATE) { object_serializeUnique (x, sym__tagobjectsource, b); }
    if (flags & SAVE_UPDATE)      { object_serializeSource (x, sym__tagobjectsource, b); }
}

void object_serializeInclusion (t_object *x, t_buffer *b)
{
    if (object_isIncluded (x)) {
    //
    buffer_appendSymbol (b, sym___hash__X);
    buffer_appendSymbol (b, sym__include);
    buffer_appendSemicolon (b);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_float64Atomic *object_getSignalAtIndex (t_object *x, int m)
{
    t_inlet *i = NULL;
    
    if (class_hasFirstInletAsSignal (pd_class (x))) {
        if (!m) { return object_getFirstInletSignal (x); }
        m--;
    }
    
    for (i = x->g_inlets; i; i = inlet_getNext (i), m--) {
        if (inlet_isSignal (i)) { 
            if (m == 0) {
                return inlet_getSignal (i);
            }
        }
    }
    
    return NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_float object_getSignalValueAtIndex (t_object *x, int m)
{
    t_float64Atomic *t = object_getSignalAtIndex (x, m);
    
    if (t) { return PD_ATOMIC_FLOAT64_READ (t); }
    
    PD_BUG; return 0.0;
}

void object_getSignalValues (t_object *x, t_buffer *b)
{
    int i, n = object_getNumberOfSignalInlets (x);
    
    buffer_appendSymbol (b, sym__signals);
    
    for (i = 0; i < n; i++) { buffer_appendFloat (b, object_getSignalValueAtIndex (x, i)); }
}

static void object_setSignalValueAtIndex (t_object *x, int m, t_float f)
{
    t_float64Atomic *t = object_getSignalAtIndex (x, m);
    
    if (t) { PD_ATOMIC_FLOAT64_WRITE (f, t); } else { PD_BUG; }
}

void object_setSignalValues (t_object *x, int argc, t_atom *argv)
{
    int i;
    
    for (i = 0; i < argc; i++) { object_setSignalValueAtIndex (x, i, atom_getFloatAtIndex (i, argc, argv)); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void object_copySignalValues (t_object *x, t_object *old)
{
    int i, n = object_getNumberOfSignalInlets (x);
    
    for (i = 0; i < n; i++) { object_setSignalValueAtIndex (x, i, object_getSignalValueAtIndex (old, i)); }
}

void object_fetchAndCopySignalValuesIfRequired (t_object *x)
{
    if (object_dspNeedInitializer (x)) {
    //
    t_object *old = garbage_fetch (x);
    
    if (old) {
    //
    object_copySignalValues (x, old);
    //
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int object_setIncludedUpdate (t_object *x, int n)
{
    if (class_hasParametersFunction (pd_class (x))) {
    //
    if (object_isIncluded (x) != n) {
        object_setIncluded (x, n);
        #if defined ( PD_BUILDING_APPLICATION )
        outputs_objectChanged (x, Tags::parameters (Tag::Included));
        #endif
        return 1;
    }
    //
    }
    
    return 0;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
