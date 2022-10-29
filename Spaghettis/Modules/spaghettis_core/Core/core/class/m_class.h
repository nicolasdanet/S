
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef M_CLASS_H_
#define M_CLASS_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef void (*t_freemethod)        (t_pd *x);
typedef void (*t_bangmethod)        (t_pd *x);
typedef void (*t_floatmethod)       (t_pd *x, t_float f);
typedef void (*t_symbolmethod)      (t_pd *x, t_symbol *s);
typedef void (*t_listmethod)        (t_pd *x, t_symbol *s, int argc, t_atom *argv);
typedef void (*t_anythingmethod)    (t_pd *x, t_symbol *s, int argc, t_atom *argv);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

enum {
    SAVE_DEFAULT        = 0,
    SAVE_COPY           = 1,
    SAVE_UNDO           = 2,
    SAVE_ENCAPSULATE    = 4,
    SAVE_UPDATE         = 8
    };

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define SAVED_DEEP(flags)   (flags & SAVE_UNDO || flags & SAVE_ENCAPSULATE)

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef void (*t_savefn)    (t_object *x, t_buffer *b, int flags);

#if defined ( PD_BUILDING_APPLICATION )

typedef void (*t_parametersgetfn)  (t_object *x, core::Group& t);
typedef void (*t_parameterssetfn)  (t_object *x, const core::Group& t);

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _entry {
    t_symbol                *me_name;
    t_method                me_method;
    t_atomtype              me_arguments[PD_ARGUMENTS + 1];
    } t_entry;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

struct _class {
    t_symbol                *c_name;
    t_symbol                *c_helpName;
    t_symbol                *c_helpDirectory;
    t_entry                 *c_methods;
    int                     c_methodsSize;
    t_freemethod            c_methodFree;
    t_bangmethod            c_methodBang;
    t_floatmethod           c_methodFloat;
    t_symbolmethod          c_methodSymbol;
    t_listmethod            c_methodList;
    t_anythingmethod        c_methodAnything;
    t_savefn                c_fnSave;
    t_datafn                c_fnData;
    t_dismissfn             c_fnDismiss;
    #if defined ( PD_BUILDING_APPLICATION )
    t_parametersgetfn       c_fnGetParameters;
    t_parameterssetfn       c_fnSetParameters;
    #endif
    int                     c_requirePending;
    int                     c_hasSignal;
    int                     c_hasFirstInlet;
    int                     c_hasDsp;
    int                     c_type;
    size_t                  c_size;
    };

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_method   class_getMethod                     (t_class *c, t_symbol *s);
PD_LOCAL int        class_hasMethod                     (t_class *c, t_symbol *s);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL int        class_hasOverrideBangMethod         (t_class *c);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static inline int class_isBox (t_class *c)
{
    return (c->c_type == CLASS_BOX);
}

static inline int class_isAbstract (t_class *c)
{
    return (c->c_type == CLASS_ABSTRACT);
}

static inline int class_isInvisible (t_class *c)
{
    return (c->c_type == CLASS_INVISIBLE);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static inline size_t class_getInstanceSize (t_class *c)
{
    return c->c_size;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static inline int class_hasDsp (t_class *c)
{
    return (c->c_hasDsp != 0);
}

static inline int class_hasFirstInlet (t_class *c)
{
    return (c->c_hasFirstInlet != 0);
}

static inline int class_hasFirstInletAsSignal (t_class *c)
{
    return (class_hasFirstInlet (c) && c->c_hasSignal);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static inline int class_hasPendingRequired (t_class *c)
{
    return (c->c_requirePending != 0);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static inline int class_hasFreeMethod (t_class *c)
{
    return (c->c_methodFree != NULL);
}

static inline t_freemethod class_getFreeMethod (t_class *c)
{
    return c->c_methodFree;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static inline t_bangmethod class_getBangMethod (t_class *c)
{
    return c->c_methodBang;
}

static inline t_floatmethod class_getFloatMethod (t_class *c)
{
    return c->c_methodFloat;
}

static inline t_symbolmethod class_getSymbolMethod (t_class *c)
{
    return c->c_methodSymbol;
}

static inline t_listmethod class_getListMethod (t_class *c)
{
    return c->c_methodList;
}

static inline t_anythingmethod class_getAnythingMethod (t_class *c)
{
    return c->c_methodAnything;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static inline t_symbol *class_getName (t_class *c)
{
    return c->c_name;
}

static inline const char *class_getNameAsString (t_class *c)
{
    return c->c_name->s_name;
}

static inline const char *class_getHelpNameAsString (t_class *c)
{
    return c->c_helpName->s_name;
}

static inline const char *class_getHelpDirectoryAsString (t_class *c)
{
    return c->c_helpDirectory->s_name;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static inline int class_hasSaveFunction (t_class *c)
{
    return (c->c_fnSave != NULL);
}

static inline int class_hasDataFunction (t_class *c)
{
    return (c->c_fnData != NULL);
}

static inline int class_hasDismissFunction (t_class *c)
{
    return (c->c_fnDismiss != NULL);
}

#if defined ( PD_BUILDING_APPLICATION )

static inline int class_hasParametersFunction (t_class *c)
{
    return (c->c_fnGetParameters != NULL && c->c_fnSetParameters != NULL);
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static inline t_savefn class_getSaveFunction (t_class *c)
{
    return c->c_fnSave;
}

static inline t_datafn class_getDataFunction (t_class *c)
{
    return c->c_fnData;
}

static inline t_dismissfn class_getDismissFunction (t_class *c)
{
    return c->c_fnDismiss;
}

#if defined ( PD_BUILDING_APPLICATION )

static inline t_parametersgetfn class_getParametersGetter (t_class *c)
{
    return c->c_fnGetParameters;
}

static inline t_parameterssetfn class_getParametersSetter (t_class *c)
{
    return c->c_fnSetParameters;
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static inline void class_setSaveFunction (t_class *c, t_savefn f)
{
    c->c_fnSave = f;
}

#if defined ( PD_BUILDING_APPLICATION )

static inline void class_setParametersFunctions (t_class *c, t_parametersgetfn f, t_parameterssetfn g)
{
    c->c_fnGetParameters = f;
    c->c_fnSetParameters = g;
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // M_CLASS_H_
