
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#define GLIST_X         100
#define GLIST_Y         100
#define GLIST_WIDTH     500
#define GLIST_HEIGHT    350

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void glist_loadBegin   (t_glist *);

void eval_buffer       (t_buffer *, t_pd *, int, t_atom *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Note that an expanded name is expected (with or without the file extension). */
/* At load it could be temporarly set with the unexpanded form. */

static t_symbol *glist_newGetName (t_symbol *name, t_environment *e)
{
    if (name && name != &s_) { return name; }
    else {
        return (e ? environment_getFileName (e) : sym_Untitled);
    }
}

static t_glist *glist_new (t_symbol *name, t_rectangle *window)
{
    t_glist *x = (t_glist *)pd_new (canvas_class);

    x->gl_environment   = instance_environmentFetchIfAny();
    x->gl_abstractions  = NULL;
    x->gl_undomanager   = undomanager_new (x);
    x->gl_name          = glist_newGetName (name, x->gl_environment);
    x->gl_graphics      = buffer_new();
    x->gl_tempObjects   = buffer_new();
    x->gl_tempOutlets   = buffer_new();
    x->gl_tempIndexes   = buffer_new();
    x->gl_zoom          = 100;
    
    if (window) { glist_setEditView (x, window, 0); }
    
    if (glist_isRoot (x)) { x->gl_abstractions = abstractions_new(); }

    return x;
}

void glist_free (t_glist *glist)
{
    PD_ASSERT (!glist_objectGetNumberOf (glist));
    
    if (glist_isRoot (glist)) { instance_rootsRemove (glist); }
    
    buffer_free (glist->gl_tempIndexes);
    buffer_free (glist->gl_tempOutlets);
    buffer_free (glist->gl_tempObjects);
    buffer_free (glist->gl_graphics);
    
    environment_free (glist->gl_environment);
    undomanager_free (glist->gl_undomanager);
    abstractions_free (glist->gl_abstractions);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_glist *glist_newPatchPop (t_symbol *name, t_rectangle *window)
{
    t_glist *x = glist_newPatch (name, window);
        
    PD_ASSERT (instance_contextGetCurrent() == x);
    
    instance_stackPopPatch (x);
    
    return x;
}

t_glist *glist_newPatch (t_symbol *name, t_rectangle *window)
{
    t_rectangle t;
    
    rectangle_set (&t, GLIST_X, GLIST_Y, GLIST_X + GLIST_WIDTH, GLIST_Y + GLIST_HEIGHT);
    
    if (window && !rectangle_isNothing (window)) { rectangle_setCopy (&t, window); }
    
    {
    //
    t_glist *x = glist_new (name, &t);
    
    object_setBuffer (cast_object (x), buffer_new());
    object_setX (cast_object (x), 0);
    object_setY (cast_object (x), 0);
    object_setType (cast_object (x), TYPE_OBJECT);
    
    /* Behaware that below order matters. */
    
    instance_viewConsume (x);
    
    if (glist_isRoot (x)) { instance_rootsAdd (x); }
    else {
        #if defined ( PD_BUILDING_APPLICATION )
        outputs_objectAdded (cast_object (x));
        #endif
    }
    
    glist_loadBegin (x); instance_stackPush (x);
    
    return x;
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void glist_makeObjectProceed (t_glist *glist, int a, int b, t_buffer *t)
{
    instance_objectSetNewest (NULL);
    
    instance_stackPush (glist);
    
    {
    //
    t_environment *e = glist_getEnvironment (instance_contextGetCurrent());
    
    t_object *x = NULL;
    t_error err = PD_ERROR_NONE;
    
    eval_buffer (t,
        instance_getMakerObject(),
        environment_argc (e),
        environment_argv (e));

    if (instance_objectGetNewest()) { x = cast_object (instance_objectGetNewest()); }

    if (!x) {
        x   = (t_object *)pd_new (comment_class);     /* If failed create a dummy box. */
        err = PD_ERROR;
    }

    /* Replace original name of an abstraction created from snippet (e.g. encapsulation). */
    
    if (buffer_getSize (t) && IS_SYMBOL (buffer_getAtomAtIndex (t, 0))) {
    //
    t_symbol *key  = buffer_getSymbolAtIndex (t, 0);
    
    if (pool_check (key)) {
    //
    t_symbol *name = abstractions_getName (glist_getAbstractions (instance_contextGetCurrent()), key);
    
    if (name) { buffer_setSymbolAtIndex (t, 0, name); }
    //
    }
    //
    }
    
    object_setBuffer (x, t);
    object_setX (x, a);
    object_setY (x, b);
    object_setType (x, TYPE_OBJECT);
    
    glist_objectAdd (glist, x);
    
    if (pd_class (x) == vinlet_class)  { glist_inletSort (glist);  }
    if (pd_class (x) == voutlet_class) { glist_outletSort (glist); }
    if (pd_class (x) == block_class)   { dsp_update(); }
    if (pd_class (x) == garray_class)  { dsp_update(); }
    
    if (class_hasDataFunction (pd_class (x))) { instance_setBoundA (cast_pd (x)); }
    
    if (err && buffer_getSize (t)) { error_canNotMake (x, buffer_getSize (t), buffer_getAtoms (t)); }
    //
    }
    
    instance_stackPop (glist);
}

void glist_makeObject (t_glist *glist, int argc, t_atom *argv)
{
    if (argc > 2) {
    //
    int a = atom_getFloat (argv + 0);
    int b = atom_getFloat (argv + 1);
    
    t_buffer *t = buffer_new();
    
    buffer_deserialize (t, argc - 2, argv + 2);
    
    glist_makeObjectProceed (glist, a, b, t);
    //
    } else { PD_BUG; }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void glist_close (t_glist *glist)
{
    if (glist_isRoot (glist)) {
    //
    glist_closebang (glist); pd_free (cast_pd (glist));
    //
    } else { PD_BUG; }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void glist_loadbangAbstractions (t_glist *glist)
{
    int i, n = glist_graphicsGetSize (glist);
    
    for (i = 0; i < n; i++) {
    //
    t_object *y = glist_graphicsGetObjectAt (glist, i);
    
    if (object_isCanvas (y)) {
        if (glist_isAbstraction (cast_glist (y))) { glist_loadbang (cast_glist (y)); }
        else {
            glist_loadbangAbstractions (cast_glist (y));
        }
    }
    //
    }
}

static void glist_loadbangSubpatches (t_glist *glist)
{
    int i, n = glist_graphicsGetSize (glist);
    
    for (i = 0; i < n; i++) {
    //
    t_object *y = glist_graphicsGetObjectAt (glist, i);
    
    if (object_isCanvas (y)) {
        if (!glist_isAbstraction (cast_glist (y))) { glist_loadbangSubpatches (cast_glist (y)); }
    }
    //
    }
    
    for (i = 0; i < n; i++) {
    //
    t_object *y = glist_graphicsGetObjectAt (glist, i);
    
    if (!object_isCanvas (y) && class_hasMethod (pd_class (y), sym__loadbang)) {
        pd_message (cast_pd (y), sym__loadbang, 0, NULL);
    }
    //
    }
}

void glist_loadbang (t_glist *glist)
{
    glist_loadbangAbstractions (glist);
    glist_loadbangSubpatches (glist);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void glist_closebangAbstractions (t_glist *glist)
{
    int i, n = glist_graphicsGetSize (glist);
    
    for (i = 0; i < n; i++) {
    //
    t_object *y = glist_graphicsGetObjectAt (glist, i);
    
    if (object_isCanvas (y)) {
        if (glist_isAbstraction (cast_glist (y))) { glist_closebang (cast_glist (y)); }
        else {
            glist_closebangAbstractions (cast_glist (y));
        }
    }
    //
    }
}

static void glist_closebangSubpatches (t_glist *glist)
{
    int i, n = glist_graphicsGetSize (glist);
    
    for (i = 0; i < n; i++) {
    //
    t_object *y = glist_graphicsGetObjectAt (glist, i);
    
    if (object_isCanvas (y)) {
        if (!glist_isAbstraction (cast_glist (y))) { glist_closebangSubpatches (cast_glist (y)); }
    }
    //
    }
    
    for (i = 0; i < n; i++) {
    //
    t_object *y = glist_graphicsGetObjectAt (glist, i);
    
    if (!object_isCanvas (y) && class_hasMethod (pd_class (y), sym__closebang)) {
        pd_message (cast_pd (y), sym__closebang, 0, NULL);
    }
    //
    }
    
    glist->gl_isCloseBanged = 1;
}

void glist_closebang (t_glist *glist)
{
    if (!glist->gl_isCloseBanged) {
    //
    glist_closebangAbstractions (glist);
    glist_closebangSubpatches (glist);
    
    glist->gl_isCloseBanged = 1;
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
