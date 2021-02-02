
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

// -- TODO: Add other directories that can be useful?

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_class *directories_class;          /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

extern t_symbol *main_directorySupport;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _directories {
    t_object    x_obj;                      /* Must be the first. */
    t_glist     *x_owner;
    t_outlet    *x_outlet;
    } t_directories;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void directories_bang (t_directories *x)
{
    t_environment *e = glist_getEnvironment (x->x_owner);
    
    PD_ASSERT (e);
    
    outlet_symbol (x->x_outlet, environment_getDirectory (e));
}

static void directories_support (t_directories *x)
{
    outlet_symbol (x->x_outlet, main_directorySupport);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *directories_new (void)
{
    t_directories *x = (t_directories *)pd_new (directories_class);
    
    x->x_owner  = instance_contextGetCurrent();
    x->x_outlet = outlet_newSymbol (cast_object (x));
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void directories_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_directories,
            (t_newmethod)directories_new,
            NULL,
            sizeof (t_directories),
            CLASS_DEFAULT,
            A_NULL);
    
    class_addBang (c, (t_method)directories_bang);
    
    class_addMethod (c, (t_method)directories_support, sym_support, A_NULL);
    
    directories_class = c;
}

PD_LOCAL void directories_destroy (void)
{
    class_free (directories_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
