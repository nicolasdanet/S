
/* Copyright (c) 1997 Miller Puckette and others. */

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

static t_class *textget_class;                  /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _textget {
    t_textclient    x_textclient;               /* Must be the first. */
    t_float         x_fieldStart;
    t_float         x_fieldCount;
    t_outlet        *x_outletLeft;
    t_outlet        *x_outletRight;
    } t_textget;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void textget_float (t_textget *x, t_float f)
{
    t_buffer *b = textclient_fetchBuffer (&x->x_textclient);
        
    if (b) {
    //
    int field = (int)x->x_fieldStart;
    int count = (int)x->x_fieldCount;
    
    int start, end, match = 0;
    t_atomtype type;
    
    if (buffer_messagesGetAtWithTypeOfEnd (b, f, &start, &end, &type) == PD_ERROR_NONE) {
    
        t_atom *t = NULL;
        int size = end - start;
        int i;
                
        if (field < 0) {
        
            outlet_float (x->x_outletRight, (t_float)(type == A_COMMA));
            
            PD_ATOMS_ALLOCA (t, size);
            for (i = 0; i < size; i++) { buffer_getAtIndex (b, start + i, t + i); }
            outlet_list (x->x_outletLeft, size, t); 
            match = 1;
            PD_ATOMS_FREEA (t, size);
            
        } else if (field < size) {
        
            outlet_float (x->x_outletRight, (t_float)(type == A_COMMA));
            
            count = PD_MIN (count, size - field);
            PD_ATOMS_ALLOCA (t, count);
            for (i = 0; i < count; i++) { buffer_getAtIndex (b, start + field + i, t + i); }
            outlet_list (x->x_outletLeft, count, t);
            match = 1;
            PD_ATOMS_FREEA (t, count);
        }
    } 

    if (!match) { outlet_float (x->x_outletRight, 2); outlet_list (x->x_outletLeft, 0, NULL); }
    //
    } else { error_undefined (sym_text__space__get, sym_text); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *textget_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_textget *x = (t_textget *)z;
    t_buffer *b  = buffer_new();
    
    buffer_appendSymbol (b, sym__restore);
    buffer_appendFloat (b, x->x_fieldStart);
    buffer_appendFloat (b, x->x_fieldCount);
    buffer_appendSymbol (b, textclient_getName (&x->x_textclient));
    
    return b;
    //
    }
    
    return NULL;
}

static void textget_restore (t_textget *x, t_symbol *s, int argc, t_atom *argv)
{
    t_textget *old = (t_textget *)instance_pendingFetch (cast_object (x));
    
    t_float start  = old ? old->x_fieldStart : atom_getFloatAtIndex (0, argc, argv);
    t_float count  = old ? old->x_fieldCount : atom_getFloatAtIndex (1, argc, argv);
    t_symbol *name = old ? textclient_getName (&old->x_textclient) : atom_getSymbolAtIndex (2, argc, argv);
    
    x->x_fieldStart = start;
    x->x_fieldCount = count;
    
    textclient_setName (&x->x_textclient, name);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void *textget_new (t_symbol *s, int argc, t_atom *argv)
{
    t_textget *x = (t_textget *)pd_new (textget_class);
    
    t_error err = textclient_init (&x->x_textclient, &argc, &argv);
    
    if (!err) {
    
        x->x_fieldStart  = -1;
        x->x_fieldCount  = 1;
        x->x_outletLeft  = outlet_newList (cast_object (x));
        x->x_outletRight = outlet_newFloat (cast_object (x));
        
        inlet_newFloat (cast_object (x), &x->x_fieldStart);
        inlet_newFloat (cast_object (x), &x->x_fieldCount);
        
        if (argc && IS_FLOAT (argv)) { x->x_fieldStart = GET_FLOAT (argv); argc--; argv++; }
        if (argc && IS_FLOAT (argv)) { x->x_fieldCount = GET_FLOAT (argv); argc--; argv++; }

        if (argc) { warning_unusedArguments (sym_text__space__get, argc, argv); }
        
        inlet_newSymbol (cast_object (x), TEXTCLIENT_NAME (&x->x_textclient));
        
    } else {
    
        error_invalidArguments (sym_text__space__get, argc, argv); pd_free (cast_pd (x)); x = NULL; 
    }
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void textget_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_text__space__get,
            (t_newmethod)textget_new,
            NULL,
            sizeof (t_textget),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
            
    class_addFloat (c, (t_method)textget_float);
    
    class_addMethod (c, (t_method)textget_restore, sym__restore, A_GIMME, A_NULL);
    
    class_setDataFunction (c, textget_functionData);
    class_requirePending (c);
    
    class_setHelpName (c, sym_text);
    
    textget_class = c;
}

PD_LOCAL void textget_destroy (void)
{
    class_free (textget_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
