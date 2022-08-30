
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PD_LOCAL void   glist_objectRemoveRaw   (t_glist *, t_object *);
PD_LOCAL void   glist_objectAddRaw      (t_glist *, t_object *, t_object *, int);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if defined ( PD_BUILDING_APPLICATION )

PD_LOCAL std::vector<UniqueId> glist_objectGetAll (t_glist *);

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void glist_objectMoveNotify (t_glist *g)
{
    #if defined ( PD_BUILDING_APPLICATION )
    
    outputs_patchOrder (g, glist_objectGetAll (g));
    
    #endif
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void glist_objectMoveAtFirst (t_glist *glist, t_object *y)
{
    glist_objectRemoveRaw (glist, y);
    glist_objectAddRaw (glist, y, NULL, 1);
    glist_objectMoveNotify (glist);
}

PD_LOCAL void glist_objectMoveAtLast (t_glist *glist, t_object *y)
{
    glist_objectRemoveRaw (glist, y);
    glist_objectAddRaw (glist, y, NULL, 0);
    glist_objectMoveNotify (glist);
}

PD_LOCAL void glist_objectMoveAt (t_glist *glist, t_object *y, int n)
{
    if (n < 1) { glist_objectMoveAtFirst (glist, y); }
    else {
    //
    glist_objectRemoveRaw (glist, y);
    glist_objectAddRaw (glist, y, glist_objectGetAt (glist, (n - 1)), 0);
    glist_objectMoveNotify (glist);
    //
    }
}

PD_LOCAL void glist_objectMoveBack (t_glist *glist, t_object *y)
{
    if (glist_undoIsOk (glist)) {
    //
    glist_undoAppend (glist, undoback_new  (y, undosnippet_new (y, glist)));
    //
    }

    glist_objectMoveAtFirst (glist, y);
}

PD_LOCAL void glist_objectMoveFront (t_glist *glist, t_object *y)
{
    if (glist_undoIsOk (glist)) {
    //
    glist_undoAppend (glist, undofront_new (y, undosnippet_new (y, glist)));
    //
    }

    glist_objectMoveAtLast (glist, y);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL int glist_objectGetIndexOf (t_glist *glist, t_object *y)
{
    t_object *t = NULL;
    int n = 0;
    
    for (t = glist->gl_graphics; t && t != y; t = t->g_next) {
        n++;
    }
    
    return n;
}

static int glist_objectGetIndexOfAmong (t_glist *glist, t_object *y, int selected)
{
    t_object *t = NULL;
    int n = 0;

    for (t = glist->gl_graphics; t && t != y; t = t->g_next) {
        if (selected == glist_objectIsSelected (glist, t)) {
            n++;
        }
    }
    
    return n;
}

PD_LOCAL int glist_objectGetIndexOfAmongSelected (t_glist *glist, t_object *y)
{
    return glist_objectGetIndexOfAmong (glist, y, 1);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_object *glist_objectGetAt (t_glist *glist, int n)
{
    t_object *t = NULL;
    int i = 0;
    
    for (t = glist->gl_graphics; t; t = t->g_next) {
        if (i == n) { return t; }
        i++;
    }
    
    return NULL;
}

PD_LOCAL t_object *glist_objectGetLast (t_glist *g)
{
    if (g->gl_graphics) {
    //
    t_object *t1 = NULL;
    t_object *t2 = NULL;
    
    for ((t1 = g->gl_graphics); (t2 = t1->g_next); (t1 = t2)) { }
    
    return t1;
    //
    }
    
    return NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
