
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void glist_lineConnectByIndexNotify (t_object *o)
{
    #if defined ( PD_BUILDING_APPLICATION )
    
    outputs_objectChanged (o, Tags::attributes ( { Tag::Inlets, Tag::Outlets } ));
    
    #endif
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error glist_lineConnectByIndex (t_glist *glist,
    int indexOfObjectOut,
    int indexOfOutlet,
    int indexOfObjectIn,
    int indexOfInlet)
{
    t_object *src  = glist_graphicsGetObjectAt (glist, indexOfObjectOut);
    t_object *dest = glist_graphicsGetObjectAt (glist, indexOfObjectIn);
    
    PD_ASSERT (src);
    PD_ASSERT (dest);
    
    if (src && dest) {
    //
    int m = indexOfOutlet;
    int n = indexOfInlet;
    
    /* Creates dummy outlets and inlets. */
    /* It is required in case of failure at object creation. */
    
    int t1 = 0;
    int t2 = 0;
    
    if (object_isDummy (src)) {
        while (m >= object_getNumberOfOutlets (src)) {
            ++t1; outlet_newMixed (src);
        }
    }
    
    if (object_isDummy (dest)) {
        while (n >= object_getNumberOfInlets (dest)) {
            ++t2; inlet_new (dest, cast_pd (dest), NULL, NULL);
        }
    }

    if (t1 > 0) { glist_lineConnectByIndexNotify (src); }
    if (t2 > 0) { glist_lineConnectByIndexNotify (dest); }
    
    return glist_objectConnect (glist, src, m, dest, n);
    //
    }
    
    return PD_ERROR;
}

t_error glist_lineDisconnectByIndex (t_glist *glist,
    int indexOfObjectOut,
    int indexOfOutlet,
    int indexOfObjectIn,
    int indexOfInlet)
{
    t_traverser t; traverser_start (&t, glist);
    
    while (traverser_next (&t)) {
    //
    if ((traverser_getIndexOfOutlet (&t) == indexOfOutlet)) {
        if ((traverser_getIndexOfInlet (&t) == indexOfInlet)) {

            int m = traverser_getIndexOfSource (&t);
            int n = traverser_getIndexOfDestination (&t);

            if (m == indexOfObjectOut && n == indexOfObjectIn) {
                traverser_disconnect (&t, glist);
                return PD_ERROR_NONE;
            }
        }
    }
    //
    }
    
    return PD_ERROR;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void glist_objectDeleteLinesProceed (t_glist *glist, t_object *o, t_inlet *inlet, t_outlet *outlet)
{
    t_traverser t; traverser_start (&t, glist);
    
    int all = (inlet == NULL && outlet == NULL);
    
    while (traverser_next (&t)) {
    //
    int m = (traverser_getSource (&t) == o      && (all || traverser_getOutlet (&t) == outlet));
    int n = (traverser_getDestination (&t) == o && (all || traverser_getInlet (&t)  == inlet));
    
    if (m || n) {
    //
    traverser_disconnect (&t, glist);
    //
    }
    //
    }
}

void glist_objectDeleteLinesByInlet (t_glist *glist, t_object *o, t_inlet *inlet)
{
    glist_objectDeleteLinesProceed (glist, o, inlet, NULL);
}

void glist_objectDeleteLinesByOutlet (t_glist *glist, t_object *o, t_outlet *outlet)
{
    glist_objectDeleteLinesProceed (glist, o, NULL, outlet);
}

void glist_objectDeleteLines (t_glist *glist, t_object *o)
{
    glist_objectDeleteLinesProceed (glist, o, NULL, NULL);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error glist_objectConnect (t_glist *glist, t_object *src, int m, t_object *dest, int n)
{
    if (object_connect (src, m, dest, n, glist)) {
    //
    if (glist_undoIsOk (glist))   { glist_undoAppend (glist, undoconnect_new (src, m, dest, n)); }
    if (!glist_isLoading (glist)) { glist_setDirty (glist, 1); }
    
    return PD_ERROR_NONE;
    //
    }
    
    return PD_ERROR;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_APPLICATION )

void glist_updateLinesForObject (t_glist *g, t_object *o)
{
    if (g) {
    //
    t_outconnect *c = NULL;
    
    t_traverser t;

    traverser_start (&t, g);
    
    while ((c = traverser_next (&t))) {
    //
    if (traverser_getSource (&t) == o || traverser_getDestination (&t) == o) {
        outputs_lineChanged (connection_getUnique (c),
            traverser_getSource (&t),
            traverser_getIndexOfOutlet (&t),
            traverser_getDestination (&t),
            traverser_getIndexOfInlet (&t),
            g);
    }
    //
    }
    //
    }
}

void glist_updateGraphicInlets (t_glist *g)
{
    int i, n = glist_graphicsGetSize (g);
    
    for (i = 0; i < n; i++) {
    //
    t_object *y = glist_graphicsGetObjectAt (g, i);

    if (pd_class (y) == vinlet_class) { outputs_objectChanged (y, Tags::attributes (Tag::Number)); }
    //
    }
}

void glist_updateGraphicOutlets (t_glist *g)
{
    int i, n = glist_graphicsGetSize (g);
    
    for (i = 0; i < n; i++) {
    //
    t_object *y = glist_graphicsGetObjectAt (g, i);

    if (pd_class (y) == voutlet_class) { outputs_objectChanged (y, Tags::attributes (Tag::Number)); }
    //
    }
}

#else

void glist_updateLinesForObject (t_glist *g, t_object *o)
{

}

void glist_updateGraphicInlets (t_glist *g)
{

}

void glist_updateGraphicOutlets (t_glist *g)
{

}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
