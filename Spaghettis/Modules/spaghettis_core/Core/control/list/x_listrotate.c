
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "x_list.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *listrotate_class;           /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _listrotate {
    t_object    x_obj;                      /* MUST be the first. */
    t_float     x_rotate;
    t_outlet    *x_outlet;
    } t_listrotate;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void listrotate_list (t_listrotate *x, t_symbol *s, int argc, t_atom *argv)
{
    if (!argc) { outlet_list (x->x_outlet, 0, NULL); }
    else {
    //
    t_atom *t = NULL;
    
    PD_ATOMS_ALLOCA (t, argc);
    
    int i, j = x->x_rotate;
    
    for (i = 0; i < argc; i++) {
    //
    while (j < 0)     { j += argc; }
    while (j >= argc) { j -= argc; }
    
    PD_ASSERT (j >= 0 && j < argc); atom_copyAtom (argv + i, t + j);
    
    j++;
    //
    }
    
    outlet_list (x->x_outlet, argc, t);
    
    PD_ATOMS_FREEA (t, argc);
    //
    }
}

static void listrotate_anything (t_listrotate *x, t_symbol *s, int argc, t_atom *argv)
{
    utils_anythingToList (cast_pd (x), (t_listmethod)listrotate_list, s, argc, argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *listrotate_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_listrotate *x = (t_listrotate *)z;
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, sym__restore);
    buffer_appendFloat (b,  x->x_rotate);

    return b;
    //
    }
    
    return NULL;
}

static void listrotate_restore (t_listrotate *x, t_float f)
{
    t_listrotate *old = (t_listrotate *)instance_pendingFetch (cast_object (x));

    x->x_rotate = old ? old->x_rotate : f;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void *listrotate_new (t_symbol *s, int argc, t_atom *argv)
{
    t_listrotate *x = (t_listrotate *)pd_new (listrotate_class);
    
    if (argc) { x->x_rotate = atom_getFloat (argv); argc--; argv++; }
    
    x->x_outlet = outlet_newList (cast_object (x));
    
    inlet_newFloat (cast_object (x), &x->x_rotate);
    
    if (argc) { warning_unusedArguments (cast_object (x), s, argc, argv); }
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void listrotate_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_list__space__rotate,
            (t_newmethod)listrotate_new,
            NULL,
            sizeof (t_listrotate),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
    
    class_addList (c, (t_method)listrotate_list);
    class_addAnything (c, (t_method)listrotate_anything);
    
    class_addMethod (c, (t_method)listrotate_restore, sym__restore, A_FLOAT, A_NULL);

    class_setDataFunction (c, listrotate_functionData);
    class_requirePending (c);
    
    class_setHelpName (c, &s_list);
    
    listrotate_class = c;
}

void listrotate_destroy (void)
{
    class_free (listrotate_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

