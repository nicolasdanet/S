
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

t_class *canvas_class;      /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PD_LOCAL void canvas_dsp    (t_glist *, t_signal **);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void       glist_undoDisable                   (t_glist *);
PD_LOCAL void       glist_setUniqueAndSource            (t_glist *, int, t_atom *);
PD_LOCAL void       glist_setUniqueAndSourceOfLast      (t_glist *, int, t_atom *);
PD_LOCAL void       glist_setSourceOfLast               (t_glist *, int, t_atom *);
PD_LOCAL void       glist_setWidthOfLast                (t_glist *, int);
PD_LOCAL t_error    glist_lineConnectByIndex            (t_glist *, int, int, int, int);
PD_LOCAL t_error    glist_lineDisconnectByIndex         (t_glist *, int, int, int, int);

PD_LOCAL void       glist_serialize     (t_glist *g, t_buffer *b, int flags, int isAbstraction);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void canvas_loadbang (t_glist *glist)
{
    glist_loadbang (glist);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void canvas_tagcanvas (t_glist *glist, t_symbol *s, int argc, t_atom *argv)
{
    glist_setUniqueAndSource (glist, argc, argv);
}

static void canvas_tagdollarzero (t_glist *glist, t_symbol *s, int argc, t_atom *argv)
{
    environment_setDollarZero (glist_getEnvironment (glist), (int)atom_getFloatAtIndex (0, argc, argv));
}

static void canvas_tagobject (t_glist *glist, t_symbol *s, int argc, t_atom *argv)
{
    glist_setUniqueAndSourceOfLast (glist, argc, argv);
}

static void canvas_tagobjectsource (t_glist *glist, t_symbol *s, int argc, t_atom *argv)
{
    glist_setSourceOfLast (glist, argc, argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void canvas_restore (t_glist *glist, t_symbol *s, int argc, t_atom *argv)
{
    PD_ASSERT (glist_hasParent (glist));
    PD_ABORT (!glist_hasParent (glist));
    
    if (glist_hasParent (glist)) {
    //
    t_buffer *t = buffer_new(); 
    t_symbol *name = atom_getSymbolOrDollarSymbolAtIndex (3, argc, argv);
        
    if (argc > 2) { buffer_deserialize (t, argc - 2, argv + 2); }
    
    object_setBuffer (cast_object (glist), t);
    object_setX (cast_object (glist), atom_getFloatAtIndex (0, argc, argv));
    object_setY (cast_object (glist), atom_getFloatAtIndex (1, argc, argv));
    object_setWidth (cast_object (glist), 0);
    object_setType (cast_object (glist), TYPE_OBJECT);
    
    glist_setName (glist, dollar_expandSymbol (name, glist));
    glist_objectAdd (glist_getParent (glist), cast_object (glist));
    //
    }
    
    instance_stackPopPatch (glist);
}

static void canvas_width (t_glist *glist, t_symbol *s, int argc, t_atom *argv)
{
    glist_setWidthOfLast (glist, (int)atom_getFloatAtIndex (0, argc, argv));
}

static void canvas_connect (t_glist *glist, t_symbol *s, int argc, t_atom *argv)
{
    if (argc == 4) {
    //
    int m = (int)atom_getFloat (argv + 0);
    int i = (int)atom_getFloat (argv + 1);
    int n = (int)atom_getFloat (argv + 2);
    int j = (int)atom_getFloat (argv + 3);
    
    if (glist_lineConnectByIndex (glist, m, i, n, j) == PD_ERROR_NONE) { return; }
    //
    }
    
    error_failed (cast_object (glist), sym_connect);
}

static void canvas_disconnect (t_glist *glist, t_symbol *s, int argc, t_atom *argv)
{
    if (argc == 4) {
    //
    int m = (int)atom_getFloat (argv + 0);
    int i = (int)atom_getFloat (argv + 1);
    int n = (int)atom_getFloat (argv + 2);
    int j = (int)atom_getFloat (argv + 3);
    
    if (glist_lineDisconnectByIndex (glist, m, i, n, j) == PD_ERROR_NONE) { return; }
    //
    }
    
    error_failed (cast_object (glist), sym_disconnect);
}

static void canvas_obj (t_glist *glist, t_symbol *s, int argc, t_atom *argv)
{
    glist_makeObject (glist, argc, argv);
}

static void canvas_msg (t_glist *glist, t_symbol *s, int argc, t_atom *argv)
{
    message_makeObject (glist, s, argc, argv);
}

static void canvas_floatatom (t_glist *glist, t_symbol *s, int argc, t_atom *argv)
{
    gatom_makeObject (glist, s, argc, argv);
}

static void canvas_comment (t_glist *glist, t_symbol *s, int argc, t_atom *argv)
{
    text_makeObject (glist, s, argc, argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void canvas_functionSaveAbstraction (t_object *x, t_buffer *b, int flags, t_buffer *o)
{
    buffer_appendSymbol (b, sym___hash__X);
    buffer_appendSymbol (b, sym_obj);
    buffer_appendFloat (b,  object_getX (x));
    buffer_appendFloat (b,  object_getY (x));
    buffer_serialize (b,    o);
    buffer_appendSemicolon (b);
    object_serializeWidth (x, b);
    
    object_saveIdentifiers (x, b, flags);
}

static void canvas_functionSaveAbstractionAsSnippet (t_object *x, t_buffer *b, int flags)
{
    t_buffer *t = buffer_newCopy (object_getBuffer (x));
    
    if (buffer_getSize (t) && IS_SYMBOL (buffer_getAtomAtIndex (t, 0))) {
    //
    t_glist *glist    = cast_glist (x);
    t_symbol *name    = buffer_getSymbolAtIndex (t, 0);
    t_buffer *snippet = buffer_new();
    
    glist_serialize (glist, snippet, flags, 1);
    
    buffer_setSymbolAtIndex (t, 0, abstractions_cache (glist_getAbstractions (glist), glist, name, snippet));
    //
    } else { PD_BUG; }
    
    canvas_functionSaveAbstraction (x, b, flags, t);
    
    buffer_free (t);
}

static void canvas_functionSaveAbstractionAsObject (t_object *x, t_buffer *b, int flags)
{
    canvas_functionSaveAbstraction (x, b, flags, object_getBuffer (x));
}

static void canvas_functionSave (t_object *x, t_buffer *b, int flags)
{
    if (!glist_isAbstraction (cast_glist (x))) { glist_serialize (cast_glist (x), b, flags, 0); }
    else {
    //
    if (SAVED_DEEP (flags)) {
        canvas_functionSaveAbstractionAsSnippet (x, b, flags);
    } else {
        canvas_functionSaveAbstractionAsObject (x, b, flags);
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void *canvas_newSubpatch (t_symbol *s)
{
    return glist_newPatchPop (s, NULL, 0);
}

PD_LOCAL void canvas_new (void *dummy, t_symbol *s, int argc, t_atom *argv)
{  
    t_rectangle r;
    
    if (argc && argv) {
    //
    int a = atom_getFloatAtIndex (0, argc, argv);
    int b = atom_getFloatAtIndex (1, argc, argv);
    int w = atom_getFloatAtIndex (2, argc, argv);
    int h = atom_getFloatAtIndex (3, argc, argv);
    
    rectangle_set (&r, a, b, a + PD_ABS (w), b + PD_ABS (h));
    //
    }
    
    glist_newPatch (atom_getSymbolAtIndex (4, argc, argv),
        &r, 
        (int)atom_getFloatAtIndex (5, argc, argv));
}

static void canvas_free (t_glist *glist)
{
    glist_undoDisable (glist);
    glist_objectRemoveAll (glist);
    glist_free (glist);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void canvas_setup (void)
{
    t_class *c = NULL;
        
    c = class_new (sym_canvas,
            NULL, 
            (t_method)canvas_free, 
            sizeof (t_glist), 
            CLASS_DEFAULT | CLASS_NOINLET,
            A_NULL);

    class_addCreator ((t_newmethod)canvas_newSubpatch, sym_pd, A_DEFSYMBOL, A_NULL);
    
    class_addDsp (c, (t_method)canvas_dsp);
    class_addLoadbang (c, (t_method)canvas_loadbang);
    
    /* Used for encapsulation. */
    
    class_addMethod (c, (t_method)canvas_tagcanvas,             sym__tagcanvas,         A_GIMME, A_NULL);
    class_addMethod (c, (t_method)canvas_tagdollarzero,         sym__tagdollarzero,     A_GIMME, A_NULL);
    class_addMethod (c, (t_method)canvas_tagobject,             sym__tagobject,         A_GIMME, A_NULL);
    class_addMethod (c, (t_method)canvas_tagobjectsource,       sym__tagobjectsource,   A_GIMME, A_NULL);
    
    /* Ensure compatibility with Pure Data file format. */
    
    class_addMethod (c, (t_method)canvas_restore,               sym_restore,            A_GIMME, A_NULL);
    class_addMethod (c, (t_method)canvas_width,                 sym_f,                  A_GIMME, A_NULL);
    class_addMethod (c, (t_method)canvas_connect,               sym_connect,            A_GIMME, A_NULL);
    class_addMethod (c, (t_method)canvas_disconnect,            sym_disconnect,         A_GIMME, A_NULL);
    class_addMethod (c, (t_method)canvas_obj,                   sym_obj,                A_GIMME, A_NULL);
    class_addMethod (c, (t_method)canvas_msg,                   sym_msg,                A_GIMME, A_NULL);
    class_addMethod (c, (t_method)canvas_floatatom,             sym_floatatom,          A_GIMME, A_NULL);
    class_addMethod (c, (t_method)canvas_comment,               sym_comment,            A_GIMME, A_NULL);
    class_addMethod (c, (t_method)canvas_comment,               sym_text,               A_GIMME, A_NULL);
    
    class_setSaveFunction (c, canvas_functionSave);
    
    class_setHelpName (c, sym_pd);
    
    canvas_class = c;
}

PD_LOCAL void canvas_destroy (void)
{
    class_free (canvas_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
