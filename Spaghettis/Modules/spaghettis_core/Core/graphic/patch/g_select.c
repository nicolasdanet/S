
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

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
