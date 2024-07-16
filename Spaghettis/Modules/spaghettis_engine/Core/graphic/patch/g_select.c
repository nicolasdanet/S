
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void glist_objectSelect (t_glist *glist, t_object *y)
{
    object_setSelected (y, 1);
    
    #if defined ( PD_BUILDING_APPLICATION )
    
    outputs_objectChanged (y, Tags::attributes (Tag::Selected));
    
    #endif
}

void glist_objectDeselect (t_glist *glist, t_object *y)
{
    object_setSelected (y, 0);
    
    #if defined ( PD_BUILDING_APPLICATION )
    
    outputs_objectChanged (y, Tags::attributes (Tag::Selected));
    
    #endif
}

int glist_objectIsSelected (t_glist *glist, t_object *y)
{
    return (object_isSelected (y) != 0);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int glist_objectGetNumberOf (t_glist *glist)
{
    return glist_graphicsGetSize (glist);
}

int glist_objectGetNumberOfSelected (t_glist *glist)
{
    int k = 0;
    
    int i, n = glist_graphicsGetSize (glist);
    
    for (i = 0; i < n; i++) {
    //
    t_object *y = glist_graphicsGetObjectAt (glist, i);
    
    k += glist_objectIsSelected (glist, y);
    //
    }
    
    return k;
}

void glist_objectDeselectAll (t_glist *glist)
{
    int i, n = glist_graphicsGetSize (glist);
    
    for (i = 0; i < n; i++) {
    //
    t_object *y = glist_graphicsGetObjectAt (glist, i);
    
    if (glist_objectIsSelected (glist, y)) { glist_objectDeselect (glist, y); }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_APPLICATION )

std::vector<UniqueId> glist_objectGetAll (t_glist *glist)
{
    std::vector<UniqueId> v;
    
    int i, n = glist_graphicsGetSize (glist);
    
    for (i = 0; i < n; i++) { v.push_back (object_getUnique (glist_graphicsGetObjectAt (glist, i))); }
    
    return v;
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
