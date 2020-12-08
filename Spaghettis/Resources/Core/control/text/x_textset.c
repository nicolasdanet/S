
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

typedef struct _textset {
    t_textclient    x_textclient;           /* Must be the first. */
    t_float         x_line;
    t_float         x_field;
    } t_textset;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_class *textset_class;              /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static void textset_list (t_textset *x, t_symbol *s, int argc, t_atom *argv)
{
    t_buffer *b = textclient_fetchBuffer (&x->x_textclient);
    
    if (b) {
    //
    int line  = x->x_line;
    int field = x->x_field;
    
    if (line >= 0) {
    //
    int start, end, count = argc; 
    int i;
        
    if (buffer_messagesGetAt (b, line, &start, &end) == PD_ERROR_NONE) {
    
        int size = end - start;
        
        if (field < 0) {
            if (size != count) { buffer_extend (b, start, end, count); }
            
        } else {
            if (field >= size) { return; }
            else { 
                count = PD_MIN (count, size - field); start += field;
            }
        }
        
    } else { 
    
        if (field < 0) {

            int oldSize = buffer_getSize (b);
            int addSemi = !buffer_messagesIsLastProperlyEnded (b);
            int newSize = oldSize + addSemi + count + 1;
            
            t_atom a; SET_SEMICOLON (&a);
            
            buffer_resize (b, newSize);
            buffer_setAtIndex (b, newSize - 1, &a);
            
            start = oldSize;
            
            if (addSemi) { buffer_setAtIndex (b, oldSize, &a); start++; }
            
        } else {
            return;
        }
    }
    
    for (i = 0; i < count; i++) { buffer_setAtIndex (b, start + i, argv + i); }
    
    textclient_update (&x->x_textclient);
    //
    }
    //
    } else { error_undefined (sym_text__space__set, sym_text); }
}

static void textset_anything (t_textset *x, t_symbol *s, int argc, t_atom *argv)
{
    utils_anythingToList (cast_pd (x), (t_listmethod)textset_list, s, argc, argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *textset_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_textset *x = (t_textset *)z;
    t_buffer *b  = buffer_new();
    
    buffer_appendSymbol (b, sym__restore);
    buffer_appendFloat (b, x->x_line);
    buffer_appendFloat (b, x->x_field);
    buffer_appendSymbol (b, textclient_getName (&x->x_textclient));
    
    return b;
    //
    }
    
    return NULL;
}

static void textset_restore (t_textset *x, t_symbol *s, int argc, t_atom *argv)
{
    t_textset *old = (t_textset *)instance_pendingFetch (cast_object (x));
    
    t_float line   = old ? old->x_line : atom_getFloatAtIndex (0, argc, argv);
    t_float field  = old ? old->x_field : atom_getFloatAtIndex (1, argc, argv);
    t_symbol *name = old ? textclient_getName (&old->x_textclient) : atom_getSymbolAtIndex (2, argc, argv);
    
    x->x_line  = line;
    x->x_field = field;
    
    textclient_setName (&x->x_textclient, name);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void *textset_new (t_symbol *s, int argc, t_atom *argv)
{
    t_textset *x = (t_textset *)pd_new (textset_class);
    
    t_error err = textclient_init (&x->x_textclient, &argc, &argv);
    
    if (!err) {
    
        x->x_line  = 0;
        x->x_field = -1;
        
        inlet_newFloat (cast_object (x), &x->x_line);
        inlet_newFloat (cast_object (x), &x->x_field);
        
        if (argc && IS_FLOAT (argv)) { x->x_line  = GET_FLOAT (argv); argc--; argv++; }
        if (argc && IS_FLOAT (argv)) { x->x_field = GET_FLOAT (argv); argc--; argv++; }
        
        if (argc) { warning_unusedArguments (sym_text__space__set, argc, argv); }
        
        inlet_newSymbol (cast_object (x), TEXTCLIENT_NAME (&x->x_textclient));
    
    } else {
        
        error_invalidArguments (sym_text__space__set, argc, argv); pd_free (cast_pd (x)); x = NULL;
    }
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void textset_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_text__space__set,
            (t_newmethod)textset_new,
            NULL,
            sizeof (t_textset),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
            
    class_addList (c, (t_method)textset_list);
    class_addAnything (c, (t_method)textset_anything);
    
    class_addMethod (c, (t_method)textset_restore, sym__restore, A_GIMME, A_NULL);
    
    class_setDataFunction (c, textset_functionData);
    class_requirePending (c);
    
    class_setHelpName (c, sym_text);
    
    textset_class = c;
}

PD_LOCAL void textset_destroy (void)
{
    class_free (textset_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
