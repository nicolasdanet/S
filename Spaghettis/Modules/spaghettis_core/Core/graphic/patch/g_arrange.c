
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PD_LOCAL void glist_objectPosition (t_glist *glist, t_object *object, int x, int y, int notify)
{
    int oldX   = object_getX (object);
    int oldY   = object_getY (object);
    int movedX = object_setSnappedX (object, x);
    int movedY = object_setSnappedY (object, y);
    
    if (movedX || movedY) {
    //
    int newX   = object_getX (object);
    int newY   = object_getY (object);
    
    #if defined ( PD_BUILDING_APPLICATION )
    
    if (notify) { outputs_objectUpdated (object, glist, Tags::attributes ( { Tag::X, Tag::Y } )); }
    
    #endif
    
    if (pd_class (object) == vinlet_class)  { glist_inletSort (glist);   }
    if (pd_class (object) == voutlet_class) { glist_outletSort (glist);  }
    
    glist_setDirty (glist, 1);
    
    if (glist_undoIsOk (glist)) { glist_undoAppend (glist, undomotion_new (object, oldX, oldY, newX, newY)); }
    //
    }
}

PD_LOCAL void glist_objectDisplace (t_glist *glist, t_object *object, int deltaX, int deltaY, int notify)
{
    int x = object_getX (object) + deltaX;
    int y = object_getY (object) + deltaY;
    
    glist_objectPosition (glist, object, x, y, notify);
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
    
    glist_objectPosition (glist, y, a + deltaX, b + deltaY, notify);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
