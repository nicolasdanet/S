
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

typedef struct _textinsert {
    t_textclient    x_textclient;               /* Must be the first. */
    t_float         x_line;
    } t_textinsert;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_class *textinsert_class;               /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static void textinsert_list (t_textinsert *x, t_symbol *s, int argc, t_atom *argv)
{
    t_buffer *b = textclient_fetchBuffer (&x->x_textclient);
    
    if (b) {
    //
    int line = PD_MAX (0, x->x_line);
    int start, end, count = argc;
    int i;
    
    t_atom a; SET_SEMICOLON (&a);
    
    if (buffer_messagesGetAt (b, line, &start, &end) == PD_ERROR_NONE) {
    
        buffer_extend (b, start, start, count + 1);
        buffer_setAtIndex (b, start + count, &a);
        
    } else {
    
        int oldSize = buffer_getSize (b);
        int addSemi = !buffer_messagesIsLastProperlyEnded (b);
        int newSize = oldSize + addSemi + count + 1;
        
        buffer_resize (b, newSize);
        buffer_setAtIndex (b, newSize - 1, &a);
            
        start = oldSize;
            
        if (addSemi) { buffer_setAtIndex (b, oldSize, &a); start++; }
    }
    
    for (i = 0; i < count; i++) { buffer_setAtIndex (b, start + i, argv + i); }
    
    textclient_update (&x->x_textclient);
    //
    } else { error_undefined (cast_object (x), sym_text__space__insert, sym_text); }
}

static void textinsert_anything (t_textinsert *x, t_symbol *s, int argc, t_atom *argv)
{
    utils_anythingToList (cast_pd (x), (t_listmethod)textinsert_list, s, argc, argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *textinsert_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_textinsert *x = (t_textinsert *)z;
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, sym__restore);
    buffer_appendFloat (b, x->x_line);
    buffer_appendSymbol (b, textclient_getName (&x->x_textclient));

    return b;
    //
    }
    
    return NULL;
}

static void textinsert_restore (t_textinsert *x, t_symbol *s, int argc, t_atom *argv)
{
    t_textinsert *old = (t_textinsert *)instance_pendingFetch (cast_object (x));
    
    t_float line   = old ? old->x_line : atom_getFloatAtIndex (0, argc, argv);
    t_symbol *name = old ? textclient_getName (&old->x_textclient) : atom_getSymbolAtIndex (1, argc, argv);
    
    x->x_line = line;
    
    textclient_setName (&x->x_textclient, name);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void *textinsert_new (t_symbol *s, int argc, t_atom *argv)
{
    t_textinsert *x = (t_textinsert *)pd_new (textinsert_class);
    
    t_error err = textclient_init (&x->x_textclient, &argc, &argv);
    
    if (!err) {
    
        x->x_line = 0;
        
        inlet_newFloat (cast_object (x), &x->x_line);
        
        if (argc && IS_FLOAT (argv)) { x->x_line = GET_FLOAT (argv); argc--; argv++; }
        
        if (argc) { warning_unusedArguments (cast_object (x), sym_text__space__insert, argc, argv); }
        
        inlet_newSymbol (cast_object (x), TEXTCLIENT_NAME (&x->x_textclient));
    
    } else {
        
        error_invalidArguments (cast_object (x), sym_text__space__insert, argc, argv);
        
        pd_free (cast_pd (x)); x = NULL;
    }
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void textinsert_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_text__space__insert,
            (t_newmethod)textinsert_new,
            NULL,
            sizeof (t_textinsert),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
            
    class_addList (c, (t_method)textinsert_list);
    class_addAnything (c, (t_method)textinsert_anything);
    
    class_addMethod (c, (t_method)textinsert_restore, sym__restore, A_GIMME, A_NULL);
    
    class_setDataFunction (c, textinsert_functionData);
    class_requirePending (c);

    class_setHelpName (c, sym_text);
    
    textinsert_class = c;
}

void textinsert_destroy (void)
{
    class_free (textinsert_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
