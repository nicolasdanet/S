
/* Copyright (c) 1997-2020 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PD_FORCE void glist_objectSelect (t_glist *glist, t_object *y)
{
    object_setSelected (y, 1);
}

PD_FORCE void glist_objectSelectAll (t_glist *glist)
{
    t_object *y = NULL; for (y = glist->gl_graphics; y; y = y->g_next) { glist_objectSelect (glist, y); }
}

PD_FORCE void glist_objectDeselect (t_glist *glist, t_object *y)
{
    object_setSelected (y, 0);
}

PD_FORCE void glist_objectDeselectAll (t_glist *glist)
{
    t_object *y = NULL; for (y = glist->gl_graphics; y; y = y->g_next) { glist_objectDeselect (glist, y); }
}

PD_LOCAL int glist_objectIsSelected (t_glist *glist, t_object *y)
{
    return (object_getSelected (y) != 0);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
