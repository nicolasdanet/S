
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < http://opensoundcontrol.org/introduction-osc > */ 

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Serialize and deserialize OSC (or whatever) with the SLIP protocol. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < https://en.wikipedia.org/wiki/Serial_Line_Internet_Protocol > */
/* < http://cnmat.berkeley.edu/content/osc-over-usb-serial-transport > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Note that for now that object is reset with encapsulation. */

// TODO: Fetch states with pending?

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_class *oscstream_class;        /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _oscstream {
    t_object    x_obj;                  /* Must be the first. */
    int         x_escaped;
    t_buffer    *x_bufferEncode;
    t_buffer    *x_bufferDecode;
    t_outlet    *x_outlet;
    } t_oscstream;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define OSCSTREAM_END       0xc0        // 192
#define OSCSTREAM_ESC       0xdb        // 219
#define OSCSTREAM_ESC_END   0xdc        // 220
#define OSCSTREAM_ESC_ESC   0xdd        // 221

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Empty list are discarded. */

static int oscstream_encode (t_oscstream *x, int argc, t_atom *argv)
{
    if (argc) {
    //
    int i;
    
    buffer_appendFloat (x->x_bufferEncode, (t_float)OSCSTREAM_END);

    for (i = 0; i < argc; i++) {
    //
    unsigned char byte = (unsigned char)atom_getFloat (argv + i);
    
    t_error err = !(IS_FLOAT (argv + i));
    
    if (!err) {
        err |= (GET_FLOAT (argv + i) < (t_float)0x00);
        err |= (GET_FLOAT (argv + i) > (t_float)0xff);
    }
    
    if (err) {
        error_invalid (cast_object (x), sym_oscstream, sym_value);
    }
    
    switch (byte) {
        case OSCSTREAM_END  : buffer_appendFloat (x->x_bufferEncode, (t_float)OSCSTREAM_ESC);
                              buffer_appendFloat (x->x_bufferEncode, (t_float)OSCSTREAM_ESC_END);
                              break;
        case OSCSTREAM_ESC  : buffer_appendFloat (x->x_bufferEncode, (t_float)OSCSTREAM_ESC);
                              buffer_appendFloat (x->x_bufferEncode, (t_float)OSCSTREAM_ESC_ESC);
                              break;
        default             : buffer_appendFloat (x->x_bufferEncode, (t_float)byte);
    }
    //
    }

    buffer_appendFloat (x->x_bufferEncode, (t_float)OSCSTREAM_END);
    
    return 1;
    //
    }
    
    return 0;
}

static int oscstream_decode (t_oscstream *x, t_float f)
{
    int done = 0;
    
    unsigned char byte = f;

    if (x->x_escaped) {
        switch (byte) {
            case OSCSTREAM_ESC_END  : buffer_appendFloat (x->x_bufferDecode, (t_float)OSCSTREAM_END); break;
            case OSCSTREAM_ESC_ESC  : buffer_appendFloat (x->x_bufferDecode, (t_float)OSCSTREAM_ESC); break;
            default                 : error_invalid (cast_object (x), sym_oscstream, sym_value);
        }
        
        x->x_escaped = 0;
        
    } else {
        switch (byte) {
            case OSCSTREAM_END      : done = 1;         break;
            case OSCSTREAM_ESC      : x->x_escaped = 1; break;
            default                 : buffer_appendFloat (x->x_bufferDecode, (t_float)byte);
        }
    }
    
    return done;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void oscstream_float (t_oscstream *x, t_float f)
{
    if (oscstream_decode (x, f)) {
    //
    if (buffer_getSize (x->x_bufferDecode)) {
    //
    outlet_list (x->x_outlet, buffer_getSize (x->x_bufferDecode), buffer_getAtoms (x->x_bufferDecode));
    
    buffer_clear (x->x_bufferDecode);
    //
    }
    //
    }
}

static void oscstream_list (t_oscstream *x, t_symbol *s, int argc, t_atom *argv)
{
    if (oscstream_encode (x, argc, argv)) {
    //
    if (buffer_getSize (x->x_bufferEncode)) {
    //
    outlet_list (x->x_outlet, buffer_getSize (x->x_bufferEncode), buffer_getAtoms (x->x_bufferEncode));
    
    buffer_clear (x->x_bufferEncode);
    //
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *oscstream_new (void)
{
    t_oscstream *x = (t_oscstream *)pd_new (oscstream_class);
    
    x->x_bufferEncode = buffer_new();
    x->x_bufferDecode = buffer_new();
    x->x_outlet = outlet_newList (cast_object (x));
    
    return x;
}

static void oscstream_free (t_oscstream *x)
{
    buffer_free (x->x_bufferEncode);
    buffer_free (x->x_bufferDecode);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void oscstream_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_oscstream,
            (t_newmethod)oscstream_new,
            (t_method)oscstream_free,
            sizeof (t_oscstream),
            CLASS_DEFAULT,
            A_NULL);
    
    class_addFloat (c, (t_method)oscstream_float);
    class_addList (c, (t_method)oscstream_list);
    
    oscstream_class = c;
}

void oscstream_destroy (void)
{
    class_free (oscstream_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
