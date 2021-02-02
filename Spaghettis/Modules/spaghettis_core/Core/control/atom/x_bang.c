
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *bang_class;         /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _bang {
    t_object    x_obj;
    t_outlet    *x_outlet;
    } t_bang;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void bang_bang (t_bang *x)
{
    outlet_bang (x->x_outlet);
}

static void bang_float (t_bang *x, t_float f)
{
    bang_bang (x);
}

static void bang_symbol (t_bang *x, t_symbol *s)
{
    bang_bang (x);
}

static void bang_list (t_bang *x, t_symbol *s, int argc, t_atom *argv)
{
    bang_bang (x);
}

static void bang_anything (t_bang *x, t_symbol *s, int argc, t_atom *argv)
{
    bang_bang (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Called by the t_bangmethod of the object maker class. */

static void *bang_newByTyped (t_pd *dummy)
{
    t_bang *x = (t_bang *)pd_new (bang_class);
    
    x->x_outlet = outlet_newBang (cast_object (x));
    
    instance_objectSetNewest (cast_pd (x));
    
    return x;
}

static void *bang_newByRegular (void)
{
    return bang_newByTyped (NULL);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void bang_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (&s_bang,
            (t_newmethod)bang_newByTyped,
            NULL,
            sizeof (t_bang),
            CLASS_DEFAULT,
            A_NULL);
            
    class_addCreator ((t_newmethod)bang_newByRegular, sym_b, A_NULL);
    
    class_addBang (c, (t_method)bang_bang);
    class_addFloat (c, (t_method)bang_float);
    class_addSymbol (c, (t_method)bang_symbol);
    class_addList (c, (t_method)bang_list);
    class_addAnything (c, (t_method)bang_anything);
    
    bang_class = c;
}

PD_LOCAL void bang_destroy (void)
{
    class_free (bang_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
