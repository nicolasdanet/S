
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#define MENUBUTTON_WIDTH_DEFAULT            8
#define MENUBUTTON_WIDTH_MINIMUM            1
#define MENUBUTTON_WIDTH_MAXIMUM            127

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_class *menubutton_class;                  /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

    
typedef struct _menubutton {
    t_object        x_obj;                  /* MUST be the first. */
    int             x_width;
    int             x_embed;
    int             x_index;
    t_heapstring    *x_cachedString;
    t_buffer        *x_cachedBuffer;
    t_buffer        *x_slots;
    t_glist         *x_owner;
    t_outlet        *x_outletLeft;
    t_outlet        *x_outletMiddle;
    t_outlet        *x_outletRight;
    } t_menubutton;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void menubutton_next     (t_menubutton *x);
static void menubutton_previous (t_menubutton *x);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void menubutton_out (t_menubutton *x)
{
    if (!buffer_slotsIsEmpty (x->x_slots)) {
    //
    buffer_clear (x->x_cachedBuffer); buffer_slotsGet (x->x_slots, x->x_index, x->x_cachedBuffer);
    
    outlet_float (x->x_outletRight, x->x_index);
    outlet_list (x->x_outletLeft, buffer_getSize (x->x_cachedBuffer), buffer_getAtoms (x->x_cachedBuffer));
    //
    }
}

static void menubutton_index (t_menubutton *x, int n)
{
    x->x_index = n;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void menubutton_bang (t_menubutton *x)
{
    menubutton_out (x);
}

static void menubutton_float (t_menubutton *x, t_float f)
{
    int n = (int)f;
    int size = buffer_slotsSize (x->x_slots);
    
    if (n >= 0 && n < size) { menubutton_index (x, n); menubutton_out (x); }
    else {
        outlet_bang (x->x_outletMiddle);
    }
}

static void menubutton_list (t_menubutton *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc && IS_FLOAT (argv)) {
    //
    int size = buffer_slotsSize (x->x_slots);
    int n = (int)GET_FLOAT (argv);
    int i = PD_CLAMP (n, 0, size);
   
    buffer_clear (x->x_cachedBuffer); buffer_append (x->x_cachedBuffer, argc - 1, argv + 1);
    
    if (i == size) { buffer_slotsAppend (x->x_slots, x->x_cachedBuffer); }
    else {
        buffer_slotsSet (x->x_slots, i, x->x_cachedBuffer);
    }
    
    if (x->x_embed) { glist_setDirty (x->x_owner, 1); }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void menubutton_set (t_menubutton *x, t_float f)
{
    if (!buffer_slotsIsEmpty (x->x_slots)) {
    //
    int size = buffer_slotsSize (x->x_slots);
    int n = PD_CLAMP ((int)f, 0, size - 1);

    menubutton_index (x, n);
    //
    }
}

static void menubutton_clear (t_menubutton *x)
{
    if (!buffer_slotsIsEmpty (x->x_slots)) {
    //
    buffer_slotsClear (x->x_slots);
    
    x->x_index = 0;
    
    if (x->x_embed) { glist_setDirty (x->x_owner, 1); }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void menubutton_width (t_menubutton *x, t_float f)
{
    x->x_width = PD_CLAMP ((int)f, MENUBUTTON_WIDTH_MINIMUM, MENUBUTTON_WIDTH_MAXIMUM);
}

static void menubutton_next (t_menubutton *x)
{
    menubutton_float (x, x->x_index + 1);
}

static void menubutton_previous (t_menubutton *x)
{
    menubutton_float (x, x->x_index - 1);
}

static void menubutton_menu (t_menubutton *x, t_symbol *s, int argc, t_atom *argv)
{
    int i;
    
    menubutton_clear (x);
    
    for (i = 0; i < argc; i++) {
    //
    buffer_clear (x->x_cachedBuffer); buffer_append (x->x_cachedBuffer, 1, argv + i);
    
    buffer_slotsAppend (x->x_slots, x->x_cachedBuffer);
    //
    }
    
    if (x->x_embed) { glist_setDirty (x->x_owner, 1); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void menubutton_functionSave (t_object *z, t_buffer *b, int flags)
{
    t_menubutton *x = (t_menubutton *)z;

    buffer_appendSymbol (b, sym___hash__X);
    buffer_appendSymbol (b, sym_obj);
    buffer_appendFloat (b,  object_getX (cast_object (x)));
    buffer_appendFloat (b,  object_getY (cast_object (x)));
    buffer_appendSymbol (b, sym_menubutton);
    buffer_appendFloat (b,  x->x_width);
    buffer_appendFloat (b,  x->x_embed);
    if (SAVED_DEEP (flags)) { buffer_appendFloat (b, x->x_index); }
    buffer_appendSemicolon (b);
    
    object_saveIdentifiers (z, b, flags);
}

static t_buffer *menubutton_functionData (t_object *z, int flags)
{
    t_menubutton *x = (t_menubutton *)z;

    if (SAVED_DEEP (flags) || x->x_embed) {
    //
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, sym__restore);
    buffer_serialize (b, x->x_slots);
    
    return b;
    //
    }
    
    return NULL;
}

static void menubutton_restore (t_menubutton *x, t_symbol *s, int argc, t_atom *argv)
{
    t_menubutton *old = (t_menubutton *)instance_pendingFetch (cast_object (x));
    
    menubutton_clear (x); buffer_deserialize (x->x_slots, argc, argv);
    
    if (old) { }    // -- ???
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *menubutton_new (t_symbol *s, int argc, t_atom *argv)
{
    t_menubutton *x = (t_menubutton *)pd_new (menubutton_class);
    
    int width = (argc > 1) ? (int)atom_getFloat (argv + 0) : MENUBUTTON_WIDTH_DEFAULT;
    int embed = (argc > 1) ? (int)atom_getFloat (argv + 1) : 0;
    int index = (argc > 2) ? (int)atom_getFloat (argv + 2) : 0;
    
    x->x_owner          = instance_contextGetCurrent();
    x->x_width          = PD_CLAMP (width, MENUBUTTON_WIDTH_MINIMUM, MENUBUTTON_WIDTH_MAXIMUM);
    x->x_embed          = embed;
    x->x_index          = index;
    x->x_cachedString   = heapstring_new (0);
    x->x_cachedBuffer   = buffer_new();
    x->x_slots          = buffer_new();
    x->x_outletLeft     = outlet_newList (cast_object (x));
    x->x_outletMiddle   = outlet_newBang (cast_object (x));
    x->x_outletRight    = outlet_newFloat (cast_object (x));
    
    return x;
}

static void menubutton_free (t_menubutton *x)
{
    heapstring_free (x->x_cachedString);
    buffer_free (x->x_cachedBuffer);
    buffer_free (x->x_slots);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void menubutton_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_menubutton,
            (t_newmethod)menubutton_new,
            (t_method)menubutton_free,
            sizeof (t_menubutton),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
    
    class_addBang (c, (t_method)menubutton_bang);
    class_addFloat (c, (t_method)menubutton_float);
    class_addList (c, (t_method)menubutton_list);
    
    class_addMethod (c, (t_method)menubutton_width,     sym_width,      A_FLOAT, A_NULL);
    class_addMethod (c, (t_method)menubutton_set,       sym_set,        A_FLOAT, A_NULL);
    class_addMethod (c, (t_method)menubutton_next,      sym_next,       A_NULL);
    class_addMethod (c, (t_method)menubutton_previous,  sym_previous,   A_NULL);
    class_addMethod (c, (t_method)menubutton_clear,     sym_clear,      A_NULL);
    class_addMethod (c, (t_method)menubutton_menu,      sym_menu,       A_GIMME, A_NULL);
    class_addMethod (c, (t_method)menubutton_restore,   sym__restore,   A_GIMME, A_NULL);

    class_setSaveFunction (c, menubutton_functionSave);
    class_setDataFunction (c, menubutton_functionData);
    class_requirePending (c);

    menubutton_class = c;
}

PD_LOCAL void menubutton_destroy (void)
{
    class_free (menubutton_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
