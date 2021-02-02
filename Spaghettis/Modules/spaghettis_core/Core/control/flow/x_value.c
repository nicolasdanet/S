
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../list/x_list.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *valuecommon_class;          /* Shared. */
static t_class *value_class;                /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _valuecommon {
    t_pd            x_pd;                   /* Must be the first. */
    int             x_referenceCount;
    t_listinlet     x_list;
    } t_valuecommon;

typedef struct _value {
    t_object        x_obj;                  /* Must be the first. */
    t_symbol        *x_name;
    t_listinlet     *x_raw;
    t_outlet        *x_outlet;
    } t_value;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_symbol *valuecommon_name (t_symbol *s)
{
    return symbol_addPrefix (s, sym__VALUE_);
}

static t_listinlet *valuecommon_fetch (t_symbol *s)
{
    t_symbol *name   = valuecommon_name (s);
    t_valuecommon *x = (t_valuecommon *)symbol_getThingByClass (name, valuecommon_class);
    
    if (!x) {
        x = (t_valuecommon *)pd_new (valuecommon_class);
        x->x_referenceCount = 0;
        listinlet_init (&x->x_list);
            t_atom a; SET_FLOAT (&a, 0.0); listinlet_listSet (&x->x_list, 1, &a);
        pd_bind (cast_pd (x), name);
    }
    
    x->x_referenceCount++;
    
    return (&x->x_list);
}

static void valuecommon_release (t_symbol *s)
{
    t_symbol *name   = valuecommon_name (s);
    t_valuecommon *x = (t_valuecommon *)symbol_getThingByClass (name, valuecommon_class);
    
    PD_ASSERT (x != NULL);
    
    x->x_referenceCount--;
    
    if (!x->x_referenceCount) {
    //
    listinlet_free (&x->x_list); pd_unbind (cast_pd (x), name); pd_free (cast_pd (x));
    //
    }
}

static int valuecommon_count (t_symbol *s)
{
    t_symbol *name   = valuecommon_name (s);
    t_valuecommon *x = (t_valuecommon *)symbol_getThingByClass (name, valuecommon_class);
    
    if (x) { return x->x_referenceCount; }
    
    return -1;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void value_set (t_value *x, t_symbol *s)
{
    if (x->x_name != &s_) { valuecommon_release (x->x_name); }
    
    x->x_raw  = NULL;
    x->x_name = s;
    
    if (x->x_name != &s_) { x->x_raw = valuecommon_fetch (x->x_name); }
}

static void value_output (t_value *x)
{
    t_atom *t = NULL;
    int count = listinlet_listSize (x->x_raw);
    
    if (!count) { outlet_list (x->x_outlet, 0, NULL); }
    else {
    //
    PD_ATOMS_ALLOCA (t, count);

    listinlet_copyAtomsUnchecked (x->x_raw, t);
    outlet_list (x->x_outlet, count, t);

    PD_ATOMS_FREEA (t, count);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void value_bang (t_value *x)
{
    if (x->x_raw) { value_output (x); }
}

static void value_list (t_value *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->x_raw) { listinlet_listSet (x->x_raw, argc, argv); }
}

static void value_anything (t_value *x, t_symbol *s, int argc, t_atom *argv)
{
    utils_anythingToList (cast_pd (x), (t_listmethod)value_list, s, argc, argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *value_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_value *x  = (t_value *)z;
    
    if (x->x_raw) {
    //
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, sym__restore);
    buffer_appendSymbol (b, x->x_name);
    listinlet_listGet (x->x_raw, b);
    
    return b;
    //
    }
    //
    }
    
    return NULL;
}

static void value_restore (t_value *x, t_symbol *s, int argc, t_atom *argv)
{
    t_value *old = (t_value *)instance_pendingFetch (cast_object (x));

    value_set (x, old ? old->x_name : atom_getSymbolAtIndex (0, argc, argv));
    
    if (!old) {     /* Restore the value if it is the only client. */
    //
    if (argc > 1) {
        if (x->x_raw && valuecommon_count (x->x_name) == 1) {
            listinlet_listSet (x->x_raw, argc - 1, argv + 1);
        }
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *value_new (t_symbol *s)
{
    t_value *x = (t_value *)pd_new (value_class);
    
    x->x_name   = &s_;
    x->x_outlet = outlet_newList (cast_object (x));
    
    value_set (x, s);
    
    return x;
}

static void value_free (t_value *x)
{
    value_set (x, &s_);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void value_setup (void)
{
    valuecommon_class = class_new (sym_value,
                                NULL,
                                NULL,
                                sizeof (t_valuecommon),
                                CLASS_INVISIBLE,
                                A_NULL);
    
    value_class = class_new (sym_value,
                                (t_newmethod)value_new,
                                (t_method)value_free,
                                sizeof (t_value),
                                CLASS_DEFAULT,
                                A_DEFSYMBOL,
                                A_NULL);
                                
    class_addCreator ((t_newmethod)value_new, sym_v, A_DEFSYMBOL, A_NULL);
    
    class_addBang (value_class, (t_method)value_bang);
    class_addList (value_class, (t_method)value_list);
    class_addAnything (value_class, (t_method)value_anything);
    
    class_addMethod (value_class, (t_method)value_set,      sym_set,        A_DEFSYMBOL, A_NULL);
    class_addMethod (value_class, (t_method)value_restore,  sym__restore,   A_GIMME, A_NULL);

    class_setDataFunction (value_class, value_functionData);
    class_requirePending (value_class);
}

PD_LOCAL void value_destroy (void)
{
    class_free (valuecommon_class);
    class_free (value_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
