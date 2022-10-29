
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if defined ( PD_BUILDING_APPLICATION )

PD_LOCAL void glist_objectSelect (t_glist *glist, t_object *y)
{
    object_setSelected (y, 1); outputs_objectUpdateAttributes (y, glist);
}

PD_LOCAL void glist_objectDeselect (t_glist *glist, t_object *y)
{
    object_setSelected (y, 0); outputs_objectUpdateAttributes (y, glist);
}

PD_LOCAL int glist_objectIsSelected (t_glist *glist, t_object *y)
{
    return (object_isSelected (y) != 0);
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
