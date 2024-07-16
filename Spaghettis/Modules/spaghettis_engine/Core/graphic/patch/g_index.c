
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if defined ( PD_BUILDING_APPLICATION )

std::vector<UniqueId> glist_objectGetAll (t_glist *);

static void glist_objectMoveNotify (t_glist *g)
{
    outputs_patchOrder (g, glist_objectGetAll (g));
}

#else

static void glist_objectMoveNotify (t_glist *g)
{

}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void glist_objectMoveAtFirst (t_glist *glist, t_object *y)
{
    glist_graphicsRemove (glist, y);
    glist_graphicsPrepend (glist, y);
    glist_objectMoveNotify (glist);
}

void glist_objectMoveAtLast (t_glist *glist, t_object *y)
{
    glist_graphicsRemove (glist, y);
    glist_graphicsAppend (glist, y);
    glist_objectMoveNotify (glist);
}

void glist_objectMoveAt (t_glist *glist, t_object *y, int n)
{
    if (n < 1) { glist_objectMoveAtFirst (glist, y); }
    else {
    //
    glist_graphicsRemove (glist, y);
    glist_graphicsInsert (glist, y, n);
    glist_objectMoveNotify (glist);
    //
    }
}

void glist_objectMoveBack (t_glist *glist, t_object *y)
{
    if (glist_undoIsOk (glist)) {
    //
    glist_undoAppend (glist, undoback_new (y, undosnippet_new (y)));
    //
    }

    glist_objectMoveAtFirst (glist, y);
    glist_setDirty (glist, 1);
}

void glist_objectMoveFront (t_glist *glist, t_object *y)
{
    if (glist_undoIsOk (glist)) {
    //
    glist_undoAppend (glist, undofront_new (y, undosnippet_new (y)));
    //
    }

    glist_objectMoveAtLast (glist, y);
    glist_setDirty (glist, 1);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
