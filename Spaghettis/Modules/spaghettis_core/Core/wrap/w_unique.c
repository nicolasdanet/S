
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../m_spaghettis.h"
#include "../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_FORCE t_error unique_objectSelect (t_id u)
{
    t_object *object = instance_registerGetObject (u);
    t_glist *glist   = instance_registerGetOwner (u);

    if (object && glist) {
    //
    glist_objectSelect (glist, object); return PD_ERROR_NONE;
    //
    }

    return PD_ERROR;
}

PD_FORCE t_error unique_objectDeselect (t_id u)
{
    t_object *object = instance_registerGetObject (u);
    t_glist *glist   = instance_registerGetOwner (u);

    if (object && glist) {
    //
    glist_objectDeselect (glist, object); return PD_ERROR_NONE;
    //
    }

    return PD_ERROR;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_FORCE t_error unique_objectRemove (t_id u)
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

PD_FORCE t_error unique_objectSnap (t_id u)
{
    t_object *object = instance_registerGetObject (u);
    t_glist *glist   = instance_registerGetOwner (u);

    if (object && glist) {
    //
    glist_objectSnap (glist, object, 1); return PD_ERROR_NONE;
    //
    }

    return PD_ERROR;
}

PD_FORCE t_error unique_objectPosition (t_id u, int x, int y)
{
    t_object *object = instance_registerGetObject (u);
    t_glist *glist   = instance_registerGetOwner (u);

    if (object && glist) {
    //
    glist_objectPosition (glist, object, x, y, 1); return PD_ERROR_NONE;
    //
    }
    
    return PD_ERROR;
}

PD_FORCE t_error unique_objectDisplace (t_id u, int deltaX, int deltaY)
{
    if (deltaX || deltaY) {
    //
    t_object *object = instance_registerGetObject (u);
    t_glist *glist   = instance_registerGetOwner (u);

    if (object && glist) {
    //
    glist_objectDisplace (glist, object, deltaX, deltaY, 1); return PD_ERROR_NONE;
    //
    }
    //
    }
    
    return PD_ERROR;
}

PD_FORCE t_error unique_objectMoveAtFirst (t_id u)
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

PD_FORCE t_error unique_objectMoveAtLast (t_id u)
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

PD_FORCE t_error unique_objectMoveAt (t_id u, int n)
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

PD_FORCE t_error unique_objectMoveBack (t_id u)
{
    t_object *object = instance_registerGetObject (u);
    t_glist *glist   = instance_registerGetOwner (u);

    if (object && glist) {
    //
    glist_objectMoveBack (glist, object); return PD_ERROR_NONE;
    //
    }
    
    return PD_ERROR;
}

PD_FORCE t_error unique_objectMoveFront (t_id u)
{
    t_object *object = instance_registerGetObject (u);
    t_glist *glist   = instance_registerGetOwner (u);

    if (object && glist) {
    //
    glist_objectMoveFront (glist, object); return PD_ERROR_NONE;
    //
    }
    
    return PD_ERROR;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_FORCE t_error unique_objectBang (t_id u)
{
    t_object *object = instance_registerGetObject (u);

    if (object) { pd_bang (cast_pd (object)); return PD_ERROR_NONE; }
    else {
        return PD_ERROR;
    }
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

#if defined ( PD_BUILDING_APPLICATION )

PD_FORCE t_error unique_objectParameter (t_id u, const core::Group& group)
{
    t_object *object = instance_registerGetObject (u);
    t_glist *glist   = instance_registerGetOwner (u);

    if (object && glist) {
    //
    t_class *c = pd_class (object);
    
    if (class_hasParametersFunction (c)) {
    //
    t_symbol *s = gensym (group.getParameter (Tag::Label).getValueTyped<juce::String>().toRawUTF8());
    
    if (object_setLabelUpdate (object, s)) { glist_setDirty (glist, 1); }
    
    (*class_getParametersSetter (c)) (object, group);
    
    return PD_ERROR_NONE;
    //
    }
    //
    }
    
    return PD_ERROR;
}

PD_FORCE t_error unique_objectHelp (t_id u)
{
    t_object *object = instance_registerGetObject (u);
    
    if (object) { object_help (object); return PD_ERROR_NONE; }
    
    return PD_ERROR;
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_FORCE t_error unique_objectGetIndexOf (t_id u, int *n)
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

PD_FORCE t_error unique_objectLineConnect (t_id u, int indexOfOutlet, t_id v, int indexOfInlet)
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

PD_FORCE t_error unique_objectLineDisconnect (t_id u, int indexOfOutlet, t_id v, int indexOfInlet)
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

PD_FORCE t_error unique_patchClose (t_id u)
{
    t_glist *g = unique_getPatch (u);
    
    if (g && glist_isRoot (g)) { glist_close (g); return PD_ERROR_NONE; }
    
    return PD_ERROR;
}

PD_FORCE t_error unique_patchSave (t_id u)
{
    t_glist *g = unique_getPatch (u);
    
    if (g && glist_isRoot (g)) { glist_save (g); return PD_ERROR_NONE; }
    
    return PD_ERROR;
}

PD_FORCE t_error unique_patchSetEditView (t_id u, t_rectangle *r)
{
    t_glist *g = unique_getPatch (u);
    
    if (g) { glist_setEditView (g, r, 1); return PD_ERROR_NONE; }
    
    return PD_ERROR;
}

PD_FORCE t_error unique_patchSetRunView (t_id u, t_rectangle *r)
{
    t_glist *g = unique_getPatch (u);
    
    if (g) { glist_setRunView (g, r, 1); return PD_ERROR_NONE; }
    
    return PD_ERROR;
}

PD_FORCE t_error unique_patchUndo (t_id u)
{
    t_glist *g = unique_getPatch (u);
    
    if (g) { glist_undo (g); return PD_ERROR_NONE; }
    
    return PD_ERROR;
}

PD_FORCE t_error unique_patchRedo (t_id u)
{
    t_glist *g = unique_getPatch (u);
    
    if (g) { glist_redo (g); return PD_ERROR_NONE; }
    
    return PD_ERROR;
}

PD_FORCE t_error unique_patchCut (t_id u)
{
    t_glist *g = unique_getPatch (u);
    
    if (g) { glist_cut (g); return PD_ERROR_NONE; }
    
    return PD_ERROR;
}

PD_FORCE t_error unique_patchCopy (t_id u)
{
    t_glist *g = unique_getPatch (u);
    
    if (g) { glist_copy (g); return PD_ERROR_NONE; }
    
    return PD_ERROR;
}

PD_FORCE t_error unique_patchPaste (t_id u, t_point *m)
{
    t_glist *g = unique_getPatch (u);
    
    if (g) { glist_paste (g, m); return PD_ERROR_NONE; }
    
    return PD_ERROR;
}

PD_FORCE t_error unique_patchDuplicate (t_id u)
{
    t_glist *g = unique_getPatch (u);
    
    if (g) { glist_duplicate (g); return PD_ERROR_NONE; }
    
    return PD_ERROR;
}

PD_FORCE t_error unique_patchRemove (t_id u)
{
    t_glist *g = unique_getPatch (u);
    
    if (g) { glist_objectRemoveSelected (g); return PD_ERROR_NONE; }
    
    return PD_ERROR;
}

PD_FORCE t_error unique_patchEncapsulate (t_id u)
{
    t_glist *g = unique_getPatch (u);
    
    if (g) { encapsulate_encapsulate (g); return PD_ERROR_NONE; }
    
    return PD_ERROR;
}

PD_FORCE t_error unique_patchDeencapsulate (t_id u)
{
    t_glist *g = unique_getPatch (u);
    
    if (g) { encapsulate_deencapsulate (g); return PD_ERROR_NONE; }
    
    return PD_ERROR;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_APPLICATION )

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_FORCE t_error unique_patchCreateObject (t_id u, Point::Real pt, const juce::String& s)
{
    t_glist *g = unique_getPatch (u);
    
    if (g) { Maker (pt, s).execute (g); return PD_ERROR_NONE; }
    
    return PD_ERROR;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
