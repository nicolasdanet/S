
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void class_defaultFloat      (t_pd *, t_float);
static void class_defaultList       (t_pd *, t_symbol *, int, t_atom *);
static void class_defaultAnything   (t_pd *, t_symbol *, int, t_atom *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void class_floatForSignal (t_pd *x, t_float f)
{
    PD_ATOMIC_FLOAT64_WRITE (f, object_getFirstInletSignal (cast_object (x)));
}

static void class_setSignals (t_object *x, t_symbol *s, int argc, t_atom *argv)
{
    object_setSignalValues (x, argc, argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void class_defaultBang (t_pd *x)
{
    t_class *c = pd_class (x);
    
    if (c->c_methodList != class_defaultList) { (*c->c_methodList) (x, &s_bang, 0, NULL); }
    else { 
        (*c->c_methodAnything) (x, &s_bang, 0, NULL);
    }
}

static void class_defaultFloat (t_pd *x, t_float f)
{
    t_class *c = pd_class (x);
    
    t_atom a;
    SET_FLOAT (&a, f);
        
    if (c->c_methodList != class_defaultList) { (*c->c_methodList) (x, &s_float, 1, &a); }
    else {
        (*c->c_methodAnything) (x, &s_float, 1, &a);
    }
}

static void class_defaultSymbol (t_pd *x, t_symbol *s)
{
    t_class *c = pd_class (x);
    
    t_atom a;
    SET_SYMBOL (&a, s);
        
    if (c->c_methodList != class_defaultList) { (*c->c_methodList) (x, &s_symbol, 1, &a); }
    else {
        (*c->c_methodAnything) (x, &s_symbol, 1, &a);
    }
}

static void class_defaultList (t_pd *x, t_symbol *s, int argc, t_atom *argv)
{
    t_class *c = pd_class (x);
    
    if (argc == 0) { if (c->c_methodBang != class_defaultBang) { (*c->c_methodBang) (x); return; } }
    
    if (argc == 1) {
    //
    if (IS_FLOAT (argv) && c->c_methodFloat != class_defaultFloat) {
        (*c->c_methodFloat) (x, GET_FLOAT (argv)); return;
    }
        
    if (IS_SYMBOL (argv) && c->c_methodSymbol != class_defaultSymbol) {
        (*c->c_methodSymbol) (x, GET_SYMBOL (argv)); return;
    }
    //
    }

    if (c->c_methodAnything != class_defaultAnything) { (*c->c_methodAnything) (x, &s_list, argc, argv); }
    else {
    //
    if (class_isBox (c)) { object_distributeAtomsOnInlets (cast_object (x), argc, argv); }
    else {
        class_defaultAnything (x, &s_list, argc, argv);
    }
    //
    }
}

// TODO: Pass context to message error?

static void class_defaultAnything (t_pd *x, t_symbol *s, int argc, t_atom *argv)
{
    error_unknownMethod (NULL, class_getName (pd_class (x)), s);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Default save function for all boxes object. */

static void class_defaultSave (t_object *x, t_buffer *b, int flags)
{
    buffer_appendSymbol (b, sym___hash__X);
    
    if (object_isComment (x))       { buffer_appendSymbol (b, sym_text); }
    else if (object_isObject (x))   { buffer_appendSymbol (b, sym_obj);  }
    else if (object_isMessage (x))  { buffer_appendSymbol (b, sym_msg);  }
    else {
        PD_BUG;
    }
    
    buffer_appendFloat (b, object_getX (x));
    buffer_appendFloat (b, object_getY (x));
    
    buffer_serialize (b, object_getBuffer (x));
    buffer_appendSemicolon (b);
    object_serializeLabel (x, b);
    
    object_saveIdentifiers (x, b, flags);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* For now the class name needs to be unique only if it contains a constructor. */
/* For now it is valid to add inlets with CLASS_NOINLET set at first. */
/* Probably best to make that more restrictive in the future. */

PD_LOCAL t_class *class_new (t_symbol *s,
    t_newmethod newMethod, 
    t_method freeMethod, 
    size_t size, 
    int flags, 
    t_atomtype type1, ...)
{
    va_list ap;
    t_atomtype args[PD_ARGUMENTS + 1] = { A_NULL };
    t_atomtype *vp = args;
    int count = 0;
    t_class *c = NULL;
    int type = flags & CLASS_BOX;
    int hasSignal = (flags & CLASS_SIGNAL);
    
    if (!type) { type = CLASS_BOX; }
    
    *vp = type1;

    va_start (ap, type1);
    
    while (*vp) {
        if (count == PD_ARGUMENTS) { PD_BUG; break; }
        vp++; count++;
        *vp = va_arg (ap, t_atomtype);
    }
    
    va_end (ap);
    
    if (newMethod) {
    //
    class_addMethod (instance_getMakerObjectClass(), (t_method)newMethod, s, args[0], args[1], args[2]);
    //
    }
    
    c = (t_class *)PD_MEMORY_GET (sizeof (t_class));
    c->c_name               = s;
    c->c_helpName           = s;
    c->c_helpDirectory      = &s_;
    c->c_methods            = (t_entry *)PD_MEMORY_GET (0);             /* Allocate 1 byte of memory. */
    c->c_methodsSize        = 0;
    c->c_methodFree         = (t_freemethod)freeMethod;
    c->c_methodBang         = class_defaultBang;
    c->c_methodFloat        = hasSignal ? class_floatForSignal : class_defaultFloat;
    c->c_methodSymbol       = class_defaultSymbol;
    c->c_methodList         = class_defaultList;
    c->c_methodAnything     = class_defaultAnything;
    c->c_fnSave             = (type == CLASS_BOX ? class_defaultSave : NULL);
    c->c_fnData             = NULL;
    c->c_fnDismiss          = NULL;
    #if defined ( PD_BUILDING_APPLICATION )
    c->c_fnParameters       = NULL;
    #endif
    c->c_hasSignal          = hasSignal;
    c->c_hasFirstInlet      = ((flags & CLASS_NOINLET) == 0);
    c->c_type               = type;
    c->c_size               = size;
    
    if (hasSignal) { class_addMethod (c, (t_method)class_setSignals, sym__signals, A_GIMME, A_NULL); }
    
    return c;
}

PD_LOCAL void class_addCreator (t_newmethod newMethod, t_symbol *s, t_atomtype type1, ...)
{
    va_list ap;
    t_atomtype args[PD_ARGUMENTS + 1] = { A_NULL };
    t_atomtype *vp = args;
    int count = 0;
    *vp = type1;

    va_start (ap, type1);
    
    while (*vp) {
        if (count == PD_ARGUMENTS) { PD_BUG; break; }
        vp++; count++;
        *vp = va_arg (ap, t_atomtype);
    } 
    
    va_end (ap);
    
    class_addMethod (instance_getMakerObjectClass(), (t_method)newMethod, s, args[0], args[1], args[2]);
}

PD_LOCAL void class_free (t_class *c)
{
    PD_MEMORY_FREE (c->c_methods);
    PD_MEMORY_FREE (c);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void class_addMethod (t_class *c, t_method fn, t_symbol *s, t_atomtype type1, ...)
{
    va_list ap;
    t_atomtype argtype = type1;
    
    va_start (ap, type1);
    
    if (s == &s_signal) { PD_BUG; return;  }        /* Deprecated. */
    if (s == sym__dsp)  { c->c_hasDsp = 1; }
    
    /* Don't override the anything method of the object factory. */
    
    PD_ASSERT (s != &s_anything || (class_getName (c) != sym_objectmaker));
    
    if (s == &s_bang) {
        if (argtype) { PD_BUG; return; }
        class_addBang (c, fn);
        
    } else if (s == &s_float) {
        if (argtype != A_FLOAT || va_arg (ap, t_atomtype))  { PD_BUG; return; }
        class_addFloat (c, fn);
        
    } else if (s == &s_symbol) {
        if (argtype != A_SYMBOL || va_arg (ap, t_atomtype)) { PD_BUG; return; }
        class_addSymbol (c, fn);
        
    } else if (s == &s_list) {
        if (argtype != A_GIMME) { PD_BUG; return; }
        class_addList (c, fn);
        
    } else if (s == &s_anything) {
        if (argtype != A_GIMME) { PD_BUG; return; }
        class_addAnything (c, fn);
        
    } else {
    //
    int i;
    
    /* Method name must be unique. */
    
    for (i = 0; i < c->c_methodsSize; i++) {
        if (c->c_methods[i].me_name == s) { PD_BUG; return; }
    }
    
    {
        t_entry *m = NULL;
        int n = 0;
                
        size_t oldSize = sizeof (t_entry) * (c->c_methodsSize);
        size_t newSize = sizeof (t_entry) * (c->c_methodsSize + 1);
        
        c->c_methods = (t_entry *)PD_MEMORY_RESIZE (c->c_methods, oldSize, newSize);
        
        m = c->c_methods + c->c_methodsSize;
        m->me_name = s;
        m->me_method = fn;

        c->c_methodsSize++;
        
        while (argtype != A_NULL && n < PD_ARGUMENTS) {
            m->me_arguments[n] = argtype;
            n++;
            argtype = va_arg (ap, t_atomtype);
        }
        
        PD_ASSERT (argtype == A_NULL);

        va_end (ap);
        
        m->me_arguments[n] = A_NULL;
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PD_LOCAL void class_addBang (t_class *c, t_method fn)
{
    PD_ASSERT (c->c_methodBang == class_defaultBang);
    
    c->c_methodBang = (t_bangmethod)fn;
}

PD_LOCAL void class_addFloat (t_class *c, t_method fn)
{
    PD_ASSERT (c->c_methodFloat == class_defaultFloat);
    
    c->c_methodFloat = (t_floatmethod)fn;
}

PD_LOCAL void class_addSymbol (t_class *c, t_method fn)
{
    PD_ASSERT (c->c_methodSymbol == class_defaultSymbol);
    
    c->c_methodSymbol = (t_symbolmethod)fn;
}

PD_LOCAL void class_addList (t_class *c, t_method fn)
{
    PD_ASSERT (c->c_methodList == class_defaultList);
    
    c->c_methodList = (t_listmethod)fn;
}

PD_LOCAL void class_addAnything (t_class *c, t_method fn)
{
    PD_ASSERT (c->c_methodAnything == class_defaultAnything);
    
    c->c_methodAnything = (t_anythingmethod)fn;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_method class_getMethod (t_class *c, t_symbol *s)
{
    t_entry *m = NULL;
    int i;

    for (i = c->c_methodsSize, m = c->c_methods; i--; m++) {
        if (m->me_name == s) { return m->me_method; }
    }
    
    return NULL;
}

PD_LOCAL int class_hasMethod (t_class *c, t_symbol *s)
{
    return (class_getMethod (c, s) != NULL);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL int class_hasOverrideBangMethod (t_class *c)
{
    return (c->c_methodBang != class_defaultBang);
}

PD_FORCE int class_hasOverrideFloatMethod (t_class *c)
{
    return (c->c_methodFloat != class_defaultFloat);
}

PD_FORCE int class_hasOverrideSymbolMethod (t_class *c)
{
    return (c->c_methodSymbol != class_defaultSymbol);
}

PD_FORCE int class_hasOverrideListMethod (t_class *c)
{
    return (c->c_methodList != class_defaultList);
}

PD_FORCE int class_hasOverrideAnythingMethod (t_class *c)
{
    return (c->c_methodAnything != class_defaultAnything);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void class_requirePending (t_class *c)
{
    if (!class_hasDsp (c)) { c->c_requirePending = 1; }
    else {
        PD_BUG;
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void class_setHelpName (t_class *c, t_symbol *s)
{
    c->c_helpName = s;
}

PD_LOCAL void class_setHelpDirectory (t_class *c, t_symbol *s)
{
    c->c_helpDirectory = s;
}

PD_LOCAL void class_setDataFunction (t_class *c, t_datafn f)
{
    c->c_fnData = f;
}

PD_LOCAL void class_setDismissFunction (t_class *c, t_dismissfn f)
{
    c->c_fnDismiss = f;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
