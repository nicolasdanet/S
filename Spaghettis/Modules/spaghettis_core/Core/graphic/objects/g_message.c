
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *message_class;                  /* Shared. */
static t_class *messageresponder_class;         /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _messageresponder {
    t_pd                mr_pd;                  /* MUST be the first. */
    t_outlet            *mr_outlet;
    } t_messageresponder;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _message {
    t_object            m_obj;                  /* MUST be the first. */
    t_messageresponder  m_responder;
    int                 m_dirty;
    t_buffer            *m_eval;
    } t_message;
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int atom_copyAtomsExpandedWithArguments (t_atom *, int, t_atom *, int, t_glist *, int, t_atom *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void eval_bufferProceed    (int, t_atom *, t_pd *, int, t_atom *);
void eval_buffer           (t_buffer *, t_pd *, int, t_atom *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void messageresponder_bang (t_messageresponder *x)
{
    outlet_bang (x->mr_outlet);
}

static void messageresponder_float (t_messageresponder *x, t_float f)
{
    outlet_float (x->mr_outlet, f);
}

static void messageresponder_symbol (t_messageresponder *x, t_symbol *s)
{
    outlet_symbol (x->mr_outlet, s);
}

static void messageresponder_list (t_messageresponder *x, t_symbol *s, int argc, t_atom *argv)
{
    outlet_list (x->mr_outlet, argc, argv);
}

static void messageresponder_anything (t_messageresponder *x, t_symbol *s, int argc, t_atom *argv)
{
    outlet_anything (x->mr_outlet, s, argc, argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void message_dirty (t_message *x, int notify)
{
    x->m_dirty = 1;
    
    #if defined ( PD_BUILDING_APPLICATION )
    
    /* Don't set dirty flag when modified from patch. */
    
    if (notify) {
        outputs_objectChanged (cast_object (x), Tags::attributes (Tag::Content));
        outputs_objectChanged (cast_object (x), Tags::parameters (Tag::Text));
    }
    
    #endif
}

static t_buffer *message_getBuffer (t_message *x)
{
    if (x->m_dirty) {
    //
    buffer_clear (x->m_eval);
    buffer_appendBuffer (x->m_eval, object_getBuffer (cast_object (x)));
    
    {
        t_atom *a   = buffer_getAtoms (x->m_eval);
        int i, size = buffer_getSize (x->m_eval);
    
        for (i = 0; i < size; i++) {
            t_symbol *s = atom_getSymbolOrDollarSymbol (a + i);
            if (s != &s_ && string_contains (s->s_name, "$$")) {
                SET_SYMBOL (a + i, symbol_replaceDoubleDollar (s));
            }
        }
    }
    
    x->m_dirty = 0;
    //
    }
    
    return x->m_eval;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void message_eval (t_message *x, int argc, t_atom *argv)
{
    t_glist *g  = object_getOwner (cast_object (x));
    t_buffer *b = message_getBuffer (x);
    t_atom *a   = NULL;
    int n       = buffer_getSize (b);
    
    PD_ATOMS_ALLOCA (a, n);
    
    if (atom_copyAtomsExpandedWithArguments (buffer_getAtoms (b), n, a, n, g, argc, argv)) {
        eval_bufferProceed (n, a, cast_pd (&x->m_responder), argc, argv);
    } else {
        eval_buffer (b, cast_pd (&x->m_responder), argc, argv);
    }
    
    PD_ATOMS_FREEA (a, n);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void message_bang (t_message *x)
{
    message_eval (x, 0, NULL);
}

static void message_float (t_message *x, t_float f)
{
    t_atom a; SET_FLOAT (&a, f);
    
    message_eval (x, 1, &a);
}

static void message_symbol (t_message *x, t_symbol *s)
{
    t_atom a; SET_SYMBOL (&a, s);
    
    message_eval (x, 1, &a);
}

static void message_list (t_message *x, t_symbol *s, int argc, t_atom *argv)
{
    message_eval (x, argc, argv);
}

static void message_anything (t_message *x, t_symbol *s, int argc, t_atom *argv)
{
    utils_anythingToList (cast_pd (x), (t_listmethod)message_list, s, argc, argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void message_set (t_message *x, t_symbol *s, int argc, t_atom *argv)
{
    buffer_clear (object_getBuffer (cast_object (x)));
    buffer_append (object_getBuffer (cast_object (x)), argc, argv);
    buffer_reparseIfContainsWhitespace (object_getBuffer (cast_object (x)));        /* Copy and paste. */
    message_dirty (x, 1);
}

static void message_append (t_message *x, t_symbol *s, int argc, t_atom *argv)
{
    buffer_append (object_getBuffer (cast_object (x)), argc, argv);
    buffer_reparseIfContainsWhitespace (object_getBuffer (cast_object (x)));
    message_dirty (x, 1);
}

static void message_add (t_message *x, t_symbol *s, int argc, t_atom *argv)
{
    buffer_append (object_getBuffer (cast_object (x)), argc, argv);
    buffer_appendSemicolon (object_getBuffer (cast_object (x)));
    buffer_reparseIfContainsWhitespace (object_getBuffer (cast_object (x)));
    message_dirty (x, 1);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static void message_addComma (t_message *x)
{
    t_atom a; SET_COMMA (&a);
    
    buffer_appendAtom (object_getBuffer (cast_object (x)), &a);
    message_dirty (x, 1);
}

static void message_addSemicolon (t_message *x)
{
    message_add (x, NULL, 0, NULL);
}

static void message_addDollar (t_message *x, t_float f)
{
    int n = PD_MAX (0, (int)f);
    t_atom a; SET_DOLLAR (&a, n);
    
    buffer_appendAtom (object_getBuffer (cast_object (x)), &a);
    message_dirty (x, 1);
}

static void message_addDollarSymbol (t_message *x, t_symbol *s)
{
    t_atom a;
    
    char t[PD_STRING] = { 0 };
    string_sprintf (t, PD_STRING, "$%s", s->s_name);
    SET_DOLLARSYMBOL (&a, gensym (t));

    buffer_appendAtom (object_getBuffer (cast_object (x)), &a);
    message_dirty (x, 1);
}

static void message_put (t_message *x, t_symbol *s, int argc, t_atom *argv)
{
    t_symbol *t = atom_getSymbolAtIndex (0, argc, argv);
    
    if (t == sym_comma)           { message_addComma (x); }
    else if (t == sym_semicolon)  { message_addSemicolon (x); }
    else if (t == sym_dollar)     {
    //
    if (argc > 1) {
    //
    if (IS_SYMBOL (argv + 1))     { message_addDollarSymbol (x, GET_SYMBOL (argv + 1)); }
    else if (IS_FLOAT (argv + 1)) { message_addDollar (x, GET_FLOAT (argv + 1)); }
    //
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void message_makeObjectProceed (t_glist *glist, t_object *x, int argc, t_atom *argv)
{
    t_buffer *t = buffer_new(); if (argc > 2) { buffer_deserialize (t, argc - 2, argv + 2); }
    
    object_setBuffer (x, t);
    object_setX (x, atom_getFloatAtIndex (0, argc, argv));
    object_setY (x, atom_getFloatAtIndex (1, argc, argv));
    object_setType (x, TYPE_MESSAGE);
    
    glist_objectAdd (glist, x);
}

void message_makeObject (t_glist *glist, t_symbol *s, int argc, t_atom *argv)
{
    t_message *x = (t_message *)pd_new (message_class);
    
    x->m_responder.mr_pd     = messageresponder_class;
    x->m_responder.mr_outlet = outlet_newMixed (cast_object (x));
    x->m_eval                = buffer_new();
    
    message_makeObjectProceed (glist, cast_object (x), argc, argv);
    
    message_dirty (x, 0);
}

static void message_free (t_message *x)
{
    buffer_free (x->m_eval);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_APPLICATION )

static void message_functionGetParameters (t_object *o, core::Group& group, const Tags& t)
{
    static DelegateCache delegate;
    
    if (t.contains (Tag::Text)) {
        group.addParameter (Tag::Text,
            NEEDS_TRANS ("Text"),
            NEEDS_TRANS ("Text of message"),
            object_getBufferAsString (o),
            delegate);
    }
}

static void message_functionSetParameters (t_object *o, const core::Group& group)
{
    t_message *x = (t_message *)o;
    
    jassert (group.hasParameter (Tag::Text));
    
    if (object_setBufferWithString (o, group.getParameter (Tag::Text).getValueTyped<juce::String>())) {
    //
    message_dirty (x, 1);
    
    glist_setDirty (object_getOwner (o), 1);
    //
    }
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void message_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_message,
            NULL,
            (t_method)message_free,
            sizeof (t_message),
            CLASS_DEFAULT,
            A_NULL);
    
    class_addBang (c, (t_method)message_bang);
    class_addFloat (c, (t_method)message_float);
    class_addSymbol (c, (t_method)message_symbol);
    class_addList (c, (t_method)message_list);
    class_addAnything (c, (t_method)message_anything);

    class_addMethod (c, (t_method)message_set,              sym_set,                A_GIMME, A_NULL);
    class_addMethod (c, (t_method)message_append,           sym_append,             A_GIMME, A_NULL);
    class_addMethod (c, (t_method)message_add,              sym_add,                A_GIMME, A_NULL);
    class_addMethod (c, (t_method)message_put,              sym_put,                A_GIMME, A_NULL);
    class_addMethod (c, (t_method)message_addComma,         sym_addcomma,           A_NULL);
    class_addMethod (c, (t_method)message_addSemicolon,     sym_addsemicolon,       A_NULL);
    class_addMethod (c, (t_method)message_addDollar,        sym_adddollar,          A_FLOAT, A_NULL);
    class_addMethod (c, (t_method)message_addDollarSymbol,  sym_adddollarsymbol,    A_SYMBOL, A_NULL);

    #if defined ( PD_BUILDING_APPLICATION )
    
    class_setParametersFunctions (c, message_functionGetParameters, message_functionSetParameters);
    
    #endif
    
    message_class = c;
    
    c = class_new (sym_messageresponder,
            NULL,
            NULL,
            0,
            CLASS_ABSTRACT,
            A_NULL);
            
    class_addBang (c, (t_method)messageresponder_bang);
    class_addFloat (c, (t_method)messageresponder_float);
    class_addSymbol (c, (t_method)messageresponder_symbol);
    class_addList (c, (t_method)messageresponder_list);
    class_addAnything (c, (t_method)messageresponder_anything);
    
    messageresponder_class = c;
}

void message_destroy (void)
{
    class_free (message_class);
    class_free (messageresponder_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
