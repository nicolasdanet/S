
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

t_class *textdefine_class;                              /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _textdefine {
    t_textbuffer    x_textbuffer;                       /* Must be the first. */
    int             x_dismissed;
    int             x_keep;
    t_symbol        *x_name;
    } t_textdefine;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void textdefine_dismiss (t_textdefine *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void textdefine_clear (t_textdefine *x)
{
    buffer_clear (textbuffer_getBuffer (&x->x_textbuffer));
}

static void textdefine_set (t_textdefine *x, t_symbol *s, int argc, t_atom *argv)
{
    buffer_clear (textbuffer_getBuffer (&x->x_textbuffer));
    buffer_deserialize (textbuffer_getBuffer (&x->x_textbuffer), argc, argv);
}

static void textdefine_add (t_textdefine *x, t_symbol *s, int argc, t_atom *argv)
{
    buffer_append (textbuffer_getBuffer (&x->x_textbuffer), argc, argv);
    buffer_appendSemicolon (textbuffer_getBuffer (&x->x_textbuffer));
}

static void textdefine_append (t_textdefine *x, t_symbol *s, int argc, t_atom *argv)
{
    buffer_append (textbuffer_getBuffer (&x->x_textbuffer), argc, argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *textdefine_functionData (t_object *z, int flags)
{
    t_textdefine *x = (t_textdefine *)z;
    
    if (SAVED_DEEP (flags) || x->x_keep) {
    //
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, sym_set);
    buffer_serialize (b, textbuffer_getBuffer (&x->x_textbuffer));
    buffer_appendComma (b);
    buffer_appendSymbol (b, sym__restore);
    
    return b;
    //
    }
    
    return NULL;
}

static void textdefine_functionDismiss (t_object *z)
{
    textdefine_dismiss ((t_textdefine *)z);
}

static void textdefine_restore (t_textdefine *x)
{
    t_textdefine *old = (t_textdefine *)instance_pendingFetch (cast_object (x));

    if (old) {
    //
    t_buffer *b = buffer_new();
    buffer_serialize (b, textbuffer_getBuffer (&old->x_textbuffer));
    textdefine_set (x, NULL, buffer_getSize (b), buffer_getAtoms (b));
    buffer_free (b);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *textdefine_new (t_symbol *s, int argc, t_atom *argv)
{
    t_textdefine *x = (t_textdefine *)pd_new (textdefine_class);

    textbuffer_init (&x->x_textbuffer, object_getOwner (cast_object (x)));
    
    x->x_keep = 0;
    x->x_name = &s_;
    
    while (argc && IS_SYMBOL (argv)) {
    
        t_symbol *t = GET_SYMBOL (argv);
        
        if (t == sym___arrobe__keep) { x->x_keep = 1; argc--; argv++; }
        else {
            break;
        }
    }
    
    if (!error__options (cast_object (x), s, argc, argv)) {
        if (argc && IS_SYMBOL (argv)) {
            pd_bind (cast_pd (x), GET_SYMBOL (argv));
            x->x_name = GET_SYMBOL (argv);
            argc--; argv++;
        }
    }
    
    /* Dollar expansion is zero in abstraction opened as patch. */
    
    if (argc && IS_FLOAT (argv) && (GET_FLOAT (argv) == 0.0)) { argc--; argv++; }
    
    if (argc) { warning_unusedArguments (cast_object (x), s, argc, argv); }
    
    return x;
}

static void *textdefine_makeObject (t_symbol *s, int argc, t_atom *argv)
{
    t_pd *newest = NULL;
    
    instance_objectSetNewest (NULL);
    
    if (!argc || !IS_SYMBOL (argv)) { newest = (t_pd *)textdefine_new (s,   argc, argv); }
    else {
    //
    t_symbol *t = atom_getSymbol (argv);
    
    if (t == sym_define)            { newest = (t_pd *)textdefine_new (s,   argc - 1, argv + 1); }
    else if (t == sym_get)          { newest = (t_pd *)textget_new (s,      argc - 1, argv + 1); }
    else if (t == sym_set)          { newest = (t_pd *)textset_new (s,      argc - 1, argv + 1); }
    else if (t == sym_insert)       { newest = (t_pd *)textinsert_new (s,   argc - 1, argv + 1); }
    else if (t == sym_delete)       { newest = (t_pd *)textdelete_new (s,   argc - 1, argv + 1); }
    else if (t == sym_size)         { newest = (t_pd *)textsize_new (s,     argc - 1, argv + 1); }
    else if (t == sym_search)       { newest = (t_pd *)textsearch_new (s,   argc - 1, argv + 1); }
    else if (t == sym_sequence)     { newest = (t_pd *)textsequence_new (s, argc - 1, argv + 1); }
    else {
        if (t == sym_tolist)        { newest = (t_pd *)texttolist_new (s,   argc - 1, argv + 1); }
        else if (t == sym_fromlist) { newest = (t_pd *)textfromlist_new (s, argc - 1, argv + 1); }
    }
    //
    }
    
    instance_objectSetNewest (newest);
    
    return newest;
}

static void textdefine_dismiss (t_textdefine *x)
{
    if (x->x_name != &s_) { pd_unbind (cast_pd (x), x->x_name); }
    
    x->x_dismissed = 1;
}

static void textdefine_free (t_textdefine *x)
{
    if (!x->x_dismissed) { textdefine_dismiss (x); }
    
    textbuffer_free (&x->x_textbuffer);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void textdefine_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_text__space__define,
            (t_newmethod)textdefine_makeObject,
            (t_method)textdefine_free,
            sizeof (t_textdefine),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
    
    class_addCreator ((t_newmethod)textdefine_makeObject, sym_text, A_GIMME, A_NULL);
            
    class_addMethod (c, (t_method)textbuffer_write,     sym_write,      A_SYMBOL, A_NULL);
    class_addMethod (c, (t_method)textbuffer_read,      sym_read,       A_SYMBOL, A_NULL);
    
    class_addMethod (c, (t_method)textdefine_clear,     sym_clear,      A_NULL);
    class_addMethod (c, (t_method)textdefine_set,       sym_set,        A_GIMME, A_NULL);
    class_addMethod (c, (t_method)textdefine_add,       sym_add,        A_GIMME, A_NULL);
    class_addMethod (c, (t_method)textdefine_append,    sym_append,     A_GIMME, A_NULL);
    class_addMethod (c, (t_method)textdefine_restore,   sym__restore,   A_NULL);

    class_setDataFunction (c, textdefine_functionData);
    class_setDismissFunction (c, textdefine_functionDismiss);
    class_requirePending (c);
    
    class_setHelpName (c, sym_text);

    textdefine_class = c;
}

void textdefine_destroy (void)
{
    class_free (textdefine_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
