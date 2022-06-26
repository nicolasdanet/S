
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../m_spaghettis.h"
#include "../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_error unique_objectRemove (t_id u)
{
    t_object *object = instance_registerGetObject (u);
    t_glist *glist   = instance_registerGetOwner (u);

    if (object && glist) {
    //
    glist_objectRemove (glist, object); return PD_ERROR_NONE;
    //
    }

    return PD_ERROR;
}

PD_LOCAL t_error unique_objectDisplace (t_id u, int deltaX, int deltaY)
{
    if (deltaX || deltaY) {
    //
    t_object *object = instance_registerGetObject (u);
    t_glist *glist   = instance_registerGetOwner (u);

    if (object && glist) {
    //
    glist_objectDisplace (glist, object, deltaX, deltaY); return PD_ERROR_NONE;
    //
    }
    //
    }
    
    return PD_ERROR;
}

PD_LOCAL t_error unique_objectMoveAtFirst (t_id u)
{
    t_object *object = instance_registerGetObject (u);
    t_glist *glist   = instance_registerGetOwner (u);

    if (object && glist) {
    //
    glist_objectMoveAtFirst (glist, object); return PD_ERROR_NONE;
    //
    }
    
    return PD_ERROR;
}

PD_LOCAL t_error unique_objectMoveAtLast (t_id u)
{
    t_object *object = instance_registerGetObject (u);
    t_glist *glist   = instance_registerGetOwner (u);

    if (object && glist) {
    //
    glist_objectMoveAtLast (glist, object); return PD_ERROR_NONE;
    //
    }
    
    return PD_ERROR;
}

PD_LOCAL t_error unique_objectMoveAt (t_id u, int n)
{
    t_object *object = instance_registerGetObject (u);
    t_glist *glist   = instance_registerGetOwner (u);

    if (object && glist) {
    //
    glist_objectMoveAt (glist, object, n); return PD_ERROR_NONE;
    //
    }
    
    return PD_ERROR;
}

PD_FORCE t_error unique_objectMessage (t_id u, t_symbol *s, int argc, t_atom *argv)
{
    t_object *object = instance_registerGetObject (u);

    if (object) { pd_message (cast_pd (object), s, argc, argv); return PD_ERROR_NONE; }
    else {
        return PD_ERROR;
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_error unique_objectGetIndexOf (t_id u, int *n)
{
    t_object *object = instance_registerGetObject (u);
    t_glist *glist   = instance_registerGetOwner (u);

    if (object && glist) {
    //
    (*n) = glist_objectGetIndexOf (glist, object); return PD_ERROR_NONE;
    //
    }
    
    return PD_ERROR;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_error unique_objectLineConnect (t_id u, int indexOfOutlet, t_id v, int indexOfInlet)
{
    t_object *src  = instance_registerGetObject (u);
    t_object *dest = instance_registerGetObject (v);

    if (src && dest) {
    //
    t_glist *srcOwner  = instance_registerGetOwner (u);
    t_glist *destOwner = instance_registerGetOwner (v);
    
    if (srcOwner && (srcOwner == destOwner)) {
        return glist_objectConnect (srcOwner, src, indexOfOutlet, dest, indexOfInlet);
    }
    //
    }
    
    return PD_ERROR;
}

PD_LOCAL t_error unique_objectLineDisconnect (t_id u, int indexOfOutlet, t_id v, int indexOfInlet)
{
    t_glist *srcOwner  = instance_registerGetOwner (u);
    t_glist *destOwner = instance_registerGetOwner (v);
    
    if (srcOwner && (srcOwner == destOwner)) {
    //
    t_object *src  = instance_registerGetObject (u);
    t_object *dest = instance_registerGetObject (v);
    
    if (src && dest) {
    //
    t_traverser t; traverser_start (&t, srcOwner);
    
    while (traverser_next (&t)) {
    //
    if (indexOfOutlet == traverser_getIndexOfOutlet (&t)) {
    if (indexOfInlet  == traverser_getIndexOfInlet (&t)) {
    if (src           == traverser_getSource (&t)) {
    if (dest          == traverser_getDestination (&t)) {
    //
    return traverser_disconnect (&t, srcOwner);
    //
    }
    }
    }
    }
    //
    }
    //
    }
    //
    }
    
    return PD_ERROR;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_glist *unique_getPatch (t_id u)
{
    t_object *o = instance_registerGetObject (u);
    
    if (object_isCanvas (o)) { return cast_glist (o); }
    
    return NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_GUARD t_error unique_patchClose (t_id u)
{
    t_glist *g = unique_getPatch (u);
    
    if (g && glist_isRoot (g)) { glist_close (g); return PD_ERROR_NONE; }
    
    return PD_ERROR;
}

PD_GUARD t_error unique_patchSave (t_id u)
{
    t_glist *g = unique_getPatch (u);
    
    if (g && glist_isRoot (g)) { glist_save (g); return PD_ERROR_NONE; }
    
    return PD_ERROR;
}

PD_GUARD t_error unique_patchSetEditView (t_id u, t_rectangle *r)
{
    t_glist *g = unique_getPatch (u);
    
    if (g) { glist_setEditView (g, r, 1); return PD_ERROR_NONE; }
    
    return PD_ERROR;
}

PD_GUARD t_error unique_patchSetRunView (t_id u, t_rectangle *r)
{
    t_glist *g = unique_getPatch (u);
    
    if (g) { glist_setRunView (g, r, 1); return PD_ERROR_NONE; }
    
    return PD_ERROR;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
