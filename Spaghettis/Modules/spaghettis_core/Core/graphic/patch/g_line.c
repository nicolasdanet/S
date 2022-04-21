
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PD_LOCAL t_outlet *outlet_newUndefined (t_object *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_error glist_lineConnectByIndex (t_glist *glist,
    int indexOfObjectOut,
    int indexOfOutlet,
    int indexOfObjectIn,
    int indexOfInlet)
{
    t_object *src  = glist_objectGetAt (glist, indexOfObjectOut);
    t_object *dest = glist_objectGetAt (glist, indexOfObjectIn);
    
    PD_ASSERT (src);
    PD_ASSERT (dest);
    
    if (src && dest) {
    //
    int m = indexOfOutlet;
    int n = indexOfInlet;
    
    /* Creates dummy outlets and inlets. */
    /* It is required in case of failure at object creation. */
    
    if (object_isDummy (src)) {
        while (m >= object_getNumberOfOutlets (src)) {
            outlet_newUndefined (src);
        }
    }
    
    if (object_isDummy (dest)) {
        while (n >= object_getNumberOfInlets (dest)) {
            inlet_new (dest, cast_pd (dest), NULL, NULL);
        }
    }

    return glist_objectConnect (glist, src, m, dest, n);
    //
    }
    
    return PD_ERROR;
}

PD_LOCAL t_error glist_lineDisconnectByIndex (t_glist *glist,
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

            int m = glist_objectGetIndexOf (glist, traverser_getSource (&t));
            int n = glist_objectGetIndexOf (glist, traverser_getDestination (&t));

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

PD_LOCAL void glist_objectDeleteLinesByInlet (t_glist *glist, t_object *o, t_inlet *inlet)
{
    glist_objectDeleteLinesProceed (glist, o, inlet, NULL);
}

PD_LOCAL void glist_objectDeleteLinesByOutlet (t_glist *glist, t_object *o, t_outlet *outlet)
{
    glist_objectDeleteLinesProceed (glist, o, NULL, outlet);
}

PD_LOCAL void glist_objectDeleteLines (t_glist *glist, t_object *o)
{
    glist_objectDeleteLinesProceed (glist, o, NULL, NULL);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_error glist_objectConnect (t_glist *glist, t_object *src, int m, t_object *dest, int n)
{
    if (object_connect (src, m, dest, n, glist)) {
    //
    if (glist_undoIsOk (glist)) { glist_undoAppend (glist, undoconnect_new (src, m, dest, n)); }

    return PD_ERROR_NONE;
    //
    }
    
    return PD_ERROR;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
