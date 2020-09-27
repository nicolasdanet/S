
/* Copyright (c) 1997-2020 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../m_spaghettis.h"
#include "../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_undosnippet *undosnippet_new (t_object *y, t_glist *owner)
{
    t_undosnippet *x = (t_undosnippet *)PD_MEMORY_GET (sizeof (t_undosnippet));
    
    x->us_object = object_getUnique (y);
    x->us_owner  = owner ? object_getUnique (cast_object (owner)) : 0;
    x->us_z      = owner ? glist_objectGetIndexOf (owner, y) : 0;
    x->us_buffer = buffer_new();
    
    object_save (y, x->us_buffer, SAVE_UNDO);
    
    return x;
}

PD_LOCAL void undosnippet_free (t_undosnippet *x)
{
    buffer_free (x->us_buffer);
    
    PD_MEMORY_FREE (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void undosnippet_load (t_undosnippet *x)
{
    if (instance_registerContains (x->us_object) == 0) {
    //
    t_object *t = instance_registerGetObject (x->us_owner);
    
    if (t && object_isCanvas (t)) {
    //
    if (buffer_getSize (x->us_buffer)) { instance_loadSnippet (cast_glist (t), x->us_buffer); }
    //
    }
    //
    }
}

PD_LOCAL void undosnippet_update (t_undosnippet *x)
{
    t_object *t = instance_registerGetObject (x->us_object);
    
    if (t) { buffer_clear (x->us_buffer); object_save (t, x->us_buffer, SAVE_UNDO | SAVE_UPDATE); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void undosnippet_z (t_undosnippet *x)
{
    if (instance_registerContains (x->us_object) == 1) {
    //
    t_object *t = instance_registerGetObject (x->us_owner);
    
    if (t && object_isCanvas (t)) {
    //
    unique_objectMoveAt (x->us_object, x->us_z);
    //
    }
    //
    }
}

PD_LOCAL void undosnippet_front (t_undosnippet *x)
{
    if (instance_registerContains (x->us_object) == 1) {
    //
    t_object *t = instance_registerGetObject (x->us_owner);
    
    if (t && object_isCanvas (t)) {
    //
    unique_objectMoveAtLast (x->us_object);
    //
    }
    //
    }
}

PD_LOCAL void undosnippet_back (t_undosnippet *x)
{
    if (instance_registerContains (x->us_object) == 1) {
    //
    t_object *t = instance_registerGetObject (x->us_owner);
    
    if (t && object_isCanvas (t)) {
    //
    unique_objectMoveAtFirst (x->us_object);
    //
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
