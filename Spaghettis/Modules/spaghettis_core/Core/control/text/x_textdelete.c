
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "x_text.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _textdelete {
    t_textclient x_textclient;                  /* Must be the first. */
    } t_textdelete;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_class *textdelete_class;               /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static void textdelete_float (t_textdelete *x, t_float f)
{
    t_buffer *b = textclient_fetchBuffer (&x->x_textclient);
    
    if (b) {
    //
    int start, end;
    t_atomtype type;
    
    if (buffer_messagesGetAtWithTypeOfEnd (b, (int)f, &start, &end, &type) == PD_ERROR_NONE) {
        if (type == A_COMMA || type == A_SEMICOLON) { end++; }
        buffer_extend (b, start, end, 0);
    }
        
    textclient_update (&x->x_textclient);
    //
    } else { error_undefined (cast_object (x), sym_text__space__delete, sym_text); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *textdelete_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_textdelete *x = (t_textdelete *)z;
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, sym__restore);
    buffer_appendSymbol (b, textclient_getName (&x->x_textclient));

    return b;
    //
    }
    
    return NULL;
}

static void textdelete_restore (t_textdelete *x, t_symbol *s, int argc, t_atom *argv)
{
    t_textdelete *old = (t_textdelete *)instance_pendingFetch (cast_object (x));
    
    t_symbol *name = old ? textclient_getName (&old->x_textclient) : atom_getSymbolAtIndex (0, argc, argv);
    
    textclient_setName (&x->x_textclient, name);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void *textdelete_new (t_symbol *s, int argc, t_atom *argv)
{
    t_textdelete *x = (t_textdelete *)pd_new (textdelete_class);
    
    t_error err = textclient_init (&x->x_textclient, &argc, &argv);
    
    if (!err) {
    
        if (argc) { warning_unusedArguments (cast_object (x), sym_text__space__delete, argc, argv); }
        
        inlet_newSymbol (cast_object (x), TEXTCLIENT_NAME (&x->x_textclient));
    
    } else {
        
        error_invalidArguments (cast_object (x), sym_text__space__delete, argc, argv);
        
        pd_free (cast_pd (x)); x = NULL;
    }
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void textdelete_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_text__space__delete,
            (t_newmethod)textdelete_new,
            NULL,
            sizeof (t_textdelete),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
            
    class_addFloat (c, (t_method)textdelete_float);
    
    class_addMethod (c, (t_method)textdelete_restore, sym__restore, A_GIMME, A_NULL);
    
    class_setDataFunction (c, textdelete_functionData);
    class_requirePending (c);
    
    class_setHelpName (c, sym_text);
    
    textdelete_class = c;
}

void textdelete_destroy (void)
{
    class_free (textdelete_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
