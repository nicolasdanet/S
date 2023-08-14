
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

t_class *comment_class;     /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static void comment_anything (t_object *x, t_symbol *s, int argc, t_atom *argv)
{
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if defined ( PD_BUILDING_APPLICATION )

static void comment_set (t_object *o, const juce::String& s)
{
    if (object_setBufferWithString (o, s)) {
    //
    outputs_objectUpdated (o, Tags::attributes (Tag::Content));
    outputs_objectUpdated (o, Tags::parameters (Tag::Text));
    //
    }
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void comment_makeObject (t_glist *glist, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 2) {
    //
    t_object *x = (t_object *)pd_new (comment_class);
    
    int a = atom_getFloat (argv + 0);
    int b = atom_getFloat (argv + 1);
    
    t_buffer *t = buffer_new();

    if (argc > 2) { buffer_deserialize (t, argc - 2, argv + 2); }
    
    object_setBuffer (x, t);
    object_setX (x, a);
    object_setY (x, b);
    object_setType (x, TYPE_COMMENT);
    
    glist_objectAdd (glist, x);
    //
    } else { PD_BUG; }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_APPLICATION )

static void comment_functionGetParameters (t_object *o, core::Group& group, const Tags& t)
{
    static DelegateCache delegate;
    
    if (t.contains (Tag::Text)) {
        group.addParameter (Tag::Text,
            NEEDS_TRANS ("Text"),
            NEEDS_TRANS ("Text of comment"),
            object_getBufferAsString (o),
            delegate);
    }
}

static void comment_functionSetParameters (t_object *o, const core::Group& group)
{
    jassert (group.hasParameter (Tag::Text));
    
    comment_set (o, group.getParameter (Tag::Text).getValueTyped<juce::String>());
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void comment_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_comment,
            NULL,
            NULL, 
            sizeof (t_object),
            CLASS_DEFAULT | CLASS_NOINLET,
            A_NULL);
        
    class_addAnything (c, (t_method)comment_anything);
    
    #if defined ( PD_BUILDING_APPLICATION )
    
    class_setParametersFunctions (c, comment_functionGetParameters, comment_functionSetParameters);
    
    #endif
    
    comment_class = c;
}

PD_LOCAL void comment_destroy (void)
{
    class_free (comment_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
