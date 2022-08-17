
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PD_LOCAL void glist_objectDisplace (t_glist *glist, t_object *object, int deltaX, int deltaY, int notify)
{
    int movedX = object_setSnappedX (object, object_getX (object) + deltaX);
    int movedY = object_setSnappedY (object, object_getY (object) + deltaY);
    
    if (movedX || movedY) {
    //
    if (notify) { outputs_objectUpdateAttributes (object, glist); }
    
    if (pd_class (object) == vinlet_class)  { glist_inletSort (glist);   }
    if (pd_class (object) == voutlet_class) { glist_outletSort (glist);  }
    
    glist_setDirty (glist, 1);
    
    if (glist_undoIsOk (glist)) { glist_undoAppend (glist, undomotion_new (object, deltaX, deltaY)); }
    //
    }
}

PD_LOCAL void glist_objectPosition (t_glist *glist, t_object *object, int x, int y, int notify)
{
    int deltaX = x - object_getX (object);
    int deltaY = y - object_getY (object);
    
    glist_objectDisplace (glist, object, deltaX, deltaY, notify);
}

PD_LOCAL void glist_objectSnap (t_glist *glist, t_object *y, int notify)
{
    int a = object_getX (y);
    int b = object_getY (y);
    int deltaX = instance_snapGetSnapped (a) - a;
    int deltaY = instance_snapGetSnapped (b) - b;
    
    if (deltaX || deltaY) {
    //
    if (glist_undoIsOk (glist)) { glist_undoAppend (glist, undosnap_new()); }
    
    glist_objectDisplace (glist, y, deltaX, deltaY, notify);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
