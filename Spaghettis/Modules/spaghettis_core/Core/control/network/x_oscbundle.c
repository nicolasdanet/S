
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < http://opensoundcontrol.org/introduction-osc > */ 

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Note that this object is supposed to be compliant to OSC 1.1 Specification. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "x_osc.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Note that for now that object is reset with encapsulation. */

// TODO: Fetch states with pending?

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_class *oscbundle_class;        /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _oscbundle {
    t_object    x_obj;                  /* Must be the first. */
    t_buffer    *x_buffer;
    t_outlet    *x_outlet;
    } t_oscbundle;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void oscbundle_reserveHeader (t_oscbundle *x)
{
    buffer_clear (x->x_buffer);
    
    {
        t_atom a[16];
    
        OSC_SETCHAR (a + 0,  '#');
        OSC_SETCHAR (a + 1,  'b');
        OSC_SETCHAR (a + 2,  'u');
        OSC_SETCHAR (a + 3,  'n');
        OSC_SETCHAR (a + 4,  'd');
        OSC_SETCHAR (a + 5,  'l');
        OSC_SETCHAR (a + 6,  'e');
        OSC_SETCHAR (a + 7,  0);
    
        OSC_8WRITE (a + 8, 0);
    
        buffer_append (x->x_buffer, 16, a);
    }
}

static void oscbundle_closeBundle (t_oscbundle *x, t_stamp *timetag)
{
    t_atom *a = buffer_getAtoms (x->x_buffer);
    int size  = buffer_getSize (x->x_buffer);
    t_stamp t = (*timetag);
    
    PD_ASSERT (size >= 16);
    
    OSC_8WRITE (a + 8, t);
    
    outlet_list (x->x_outlet, size, a);
    
    oscbundle_reserveHeader (x);
}

static void oscbundle_appendMessage (t_oscbundle *x, t_symbol *s, int argc, t_atom *argv)
{
    t_error err = (argc == 0);
    int i;
    
    for (i = 0; i < argc; i++) {
        if (!IS_FLOAT (argv + i) || OSC_GETCHAR (argv + i) < 0 || OSC_GETCHAR (argv + i) > 0xff) {
            err = PD_ERROR; break;
        }
    }
    
    err |= ((argc & 3) != 0);       /* Must be a multiple of 4. */
    
    if (err) { error_invalid (cast_object (x), sym_oscbundle, sym_message); }
    else {
    //
    t_atom length[4];
    
    OSC_4WRITE (length, argc);
    
    buffer_append (x->x_buffer, 4, length);
    buffer_append (x->x_buffer, argc, argv);
    //
    }
}

static void oscbundle_parseStampThenCloseBundle (t_oscbundle *x, t_symbol *s, int argc, t_atom *argv)
{
    t_error err = PD_ERROR;
    
    PD_ASSERT (stamp_isTagElement (s));
    
    if (argc == STAMP_TAGS_SIZE - 1) {
    //
    t_stamp t; t_atom a[STAMP_TAGS_SIZE];
    
    SET_SYMBOL (a + 0, s); atom_copyAtoms (argv, argc, a + 1, argc);
    
    err = stamp_getWithTags (STAMP_TAGS_SIZE, a, &t);
    
    if (!err) { oscbundle_closeBundle (x, &t); }
    //
    }
    
    if (err) { error_invalid (cast_object (x), sym_oscbundle, sym_stamp); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void oscbundle_bang (t_oscbundle *x)
{
    t_stamp t; stamp_setImmediately (&t); oscbundle_closeBundle (x, &t);
}

static void oscbundle_list (t_oscbundle *x, t_symbol *s, int argc, t_atom *argv)
{
    t_symbol *t = atom_getSymbolAtIndex (0, argc, argv);
    
    if (stamp_isTagElement (t)) { oscbundle_parseStampThenCloseBundle (x, t, argc - 1, argv + 1); }
    else {
        oscbundle_appendMessage (x, s, argc, argv);
    }
}

static void oscbundle_anything (t_oscbundle *x, t_symbol *s, int argc, t_atom *argv)
{
    if (stamp_isTagElement (s)) { oscbundle_parseStampThenCloseBundle (x, s, argc, argv); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *oscbundle_new (void)
{
    t_oscbundle *x = (t_oscbundle *)pd_new (oscbundle_class);
    
    x->x_buffer = buffer_new();
    x->x_outlet = outlet_newList (cast_object (x));
    
    oscbundle_reserveHeader (x);
    
    return x;
}

static void oscbundle_free (t_oscbundle *x)
{
    buffer_free (x->x_buffer);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void oscbundle_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_oscbundle,
            (t_newmethod)oscbundle_new,
            (t_method)oscbundle_free,
            sizeof (t_oscbundle),
            CLASS_DEFAULT,
            A_NULL);
    
    class_addBang (c, (t_method)oscbundle_bang);
    class_addList (c, (t_method)oscbundle_list);
    class_addAnything (c, (t_method)oscbundle_anything);
    
    oscbundle_class = c;
}

void oscbundle_destroy (void)
{
    class_free (oscbundle_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
