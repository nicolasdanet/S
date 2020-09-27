
/* Copyright (c) 1997-2020 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "x_text.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *textsize_class;             /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _textsize {
    t_textclient    x_textclient;           /* Must be the first. */
    t_outlet        *x_outlet;
    } t_textsize;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void textsize_bang (t_textsize *x)
{
    t_buffer *b = textclient_fetchBuffer (&x->x_textclient);
    
    if (b) { outlet_float (x->x_outlet, (t_float)buffer_messagesGetNumberOf (b)); }
    else {
        error_undefined (sym_text__space__size, sym_text);
    }
}

static void textsize_float (t_textsize *x, t_float f)
{
    t_buffer *b = textclient_fetchBuffer (&x->x_textclient);
    
    if (b) {
        int start, end;
        if (buffer_messagesGetAt (b, f, &start, &end) == PD_ERROR_NONE) {
            outlet_float (x->x_outlet, (t_float)(end - start));
        } else {
            outlet_float (x->x_outlet, -1);
        }
        
    } else { error_undefined (sym_text__space__size, sym_text); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *textsize_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_textsize *x = (t_textsize *)z;
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, sym__restore);
    buffer_appendSymbol (b, textclient_getName (&x->x_textclient));

    return b;
    //
    }
    
    return NULL;
}

static void textsize_restore (t_textsize *x, t_symbol *s, int argc, t_atom *argv)
{
    t_textsize *old = (t_textsize *)instance_pendingFetch (cast_object (x));
    
    t_symbol *name = old ? textclient_getName (&old->x_textclient) : atom_getSymbolAtIndex (0, argc, argv);
    
    textclient_setName (&x->x_textclient, name);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void *textsize_new (t_symbol *s, int argc, t_atom *argv)
{
    t_textsize *x = (t_textsize *)pd_new (textsize_class);
    
    t_error err = textclient_init (&x->x_textclient, &argc, &argv);
    
    if (!err) {
    
        x->x_outlet = outlet_newFloat (cast_object (x));
                
        if (argc) { warning_unusedArguments (sym_text__space__size, argc, argv); }
        
        inlet_newSymbol (cast_object (x), TEXTCLIENT_NAME (&x->x_textclient));
        
    } else {
        
        error_invalidArguments (sym_text__space__size, argc, argv); pd_free (cast_pd (x)); x = NULL; 
    }
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void textsize_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_text__space__size,
            (t_newmethod)textsize_new,
            NULL,
            sizeof (t_textsize),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
            
    class_addBang (c, (t_method)textsize_bang);
    class_addFloat (c, (t_method)textsize_float);
    
    class_addMethod (c, (t_method)textsize_restore, sym__restore, A_GIMME, A_NULL);
    
    class_setDataFunction (c, textsize_functionData);
    class_requirePending (c);
    
    class_setHelpName (c, sym_text);
    
    textsize_class = c;
}

PD_LOCAL void textsize_destroy (void)
{
    class_free (textsize_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
