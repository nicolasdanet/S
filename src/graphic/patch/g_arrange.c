
/* Copyright (c) 1997-2020 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PD_LOCAL void glist_objectDisplace (t_glist *glist, t_object *object, int deltaX, int deltaY)
{
    object_setSnappedX (object, object_getX (object) + deltaX);
    object_setSnappedY (object, object_getY (object) + deltaY);
    
    if (pd_class (object) == vinlet_class)  { glist_inletSort (glist);   }
    if (pd_class (object) == voutlet_class) { glist_outletSort (glist);  }
    
    glist_setDirty (glist, 1);
    
    if (glist_undoIsOk (glist)) { glist_undoAppend (glist, undomotion_new (object, deltaX, deltaY)); }
}

PD_FORCE void glist_objectSnap (t_glist *glist, t_object *y)
{
    int a = object_getX (y);
    int b = object_getY (y);
    int deltaX = instance_snapGetSnapped (a) - a;
    int deltaY = instance_snapGetSnapped (b) - b;
    
    if (deltaX || deltaY) {
    //
    if (glist_undoIsOk (glist)) { glist_undoAppend (glist, undosnap_new()); }
    
    glist_objectDisplace (glist, y, deltaX, deltaY);
    //
    }
}

PD_LOCAL void glist_objectResizeBox (t_glist *glist, t_object *y, int n)
{
    int m = object_getWidth (y);
    
    object_setWidth (y, n); glist_setDirty (glist, 1);
    
    if (glist_undoIsOk (glist)) { glist_undoAppend (glist, undoresize_new (y, m, n)); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
