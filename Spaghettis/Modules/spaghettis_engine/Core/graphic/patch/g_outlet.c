
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static void glist_updateInlets (t_glist *glist)
{
    #if defined ( PD_BUILDING_APPLICATION )
    
    if (!glist_isLoading (glist)) {
    //
    outputs_objectChanged (cast_object (glist), Tags::attributes (Tag::Inlets));
    //
    }
    
    #endif
}

static void glist_updateOutlets (t_glist *glist)
{
    #if defined ( PD_BUILDING_APPLICATION )
    
    if (!glist_isLoading (glist)) {
    //
    outputs_objectChanged (cast_object (glist), Tags::attributes (Tag::Outlets));
    //
    }
    
    #endif
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_inlet *glist_inletAdd (t_glist *glist, t_pd *receiver, int isSignal)
{
    t_inlet *inlet = inlet_new (cast_object (glist), receiver, (isSignal ? &s_signal : NULL), NULL);
    
    if (!glist_isLoading (glist)) { glist_inletSort (glist); }
    
    glist_updateInlets (glist);
    
    return inlet;
}

void glist_inletRemove (t_glist *glist, t_inlet *inlet)
{
    t_glist *owner = glist_getParent (glist);
    
    if (owner) { glist_objectDeleteLinesByInlet (owner, cast_object (glist), inlet); }
    
    inlet_free (inlet);
    
    glist_updateInlets (glist);
}

void glist_inletSort (t_glist *glist)
{
    int numberOfInlets = glist_graphicsGetCountByClass (glist, vinlet_class);

    if (numberOfInlets > 1) {
    //
    int i, n = glist_graphicsGetSize (glist);
    
    /* Fetch all inlets into a list. */
    
    t_object **inlets  = (t_object **)PD_MEMORY_GET (numberOfInlets * sizeof (t_object *));
    t_point  *boxes    = (t_point *)PD_MEMORY_GET (numberOfInlets * sizeof (t_point));
    t_object **t       = inlets;
    t_point  *b        = boxes;
    
    for (i = 0; i < n; i++) {
    //
    t_object *y = glist_graphicsGetObjectAt (glist, i);
    
    if (pd_class (y) == vinlet_class) { *t = y; *b = object_getPoint (y); t++; b++; }
    //
    }
    
    /* Take the most right inlet and put it first. */
    /* Remove it from the list. */
    /* Do it again. */
    
    for (i = numberOfInlets; i > 0; i--) {
    //
    int j = numberOfInlets;
    int maximumX = -PD_INT_MAX;
    t_object **mostRightInlet = NULL;
    
    for (t = inlets, b = boxes; j--; t++, b++) {
        if (*t && point_getX (b) > maximumX) {
            maximumX = point_getX (b); mostRightInlet = t;
        }
    }
    
    if (mostRightInlet) {
        inlet_moveFirst (vinlet_getInlet (cast_pd (*mostRightInlet))); *mostRightInlet = NULL;
    }
    //
    }
    
    PD_MEMORY_FREE (boxes);
    PD_MEMORY_FREE (inlets);
    
    glist_updateLinesForObject (glist_getParent (glist), cast_object (glist));
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_outlet *glist_outletAddProceed (t_glist *glist, t_outlet *outlet)
{
    if (!glist_isLoading (glist)) { glist_outletSort (glist); }
    
    glist_updateOutlets (glist);
    
    return outlet;
}

t_outlet *glist_outletAddSignal (t_glist *glist)
{
    return glist_outletAddProceed (glist, outlet_newSignal (cast_object (glist)));
}

t_outlet *glist_outletAdd (t_glist *glist)
{
    return glist_outletAddProceed (glist, outlet_newMixed (cast_object (glist)));
}

void glist_outletRemove (t_glist *glist, t_outlet *outlet)
{
    t_glist *owner = glist_getParent (glist);
    
    if (owner) { glist_objectDeleteLinesByOutlet (owner, cast_object (glist), outlet); }

    outlet_free (outlet);
    
    glist_updateOutlets (glist);
}

void glist_outletSort (t_glist *glist)
{
    int numberOfOutlets = glist_graphicsGetCountByClass (glist, voutlet_class);
    
    if (numberOfOutlets > 1) {
    //
    int i, n = glist_graphicsGetSize (glist);
    
    /* Fetch all outlets into a list. */
    
    t_object **outlets = (t_object **)PD_MEMORY_GET (numberOfOutlets * sizeof (t_object *));
    t_point  *boxes    = (t_point *)PD_MEMORY_GET (numberOfOutlets * sizeof (t_point));
    t_object **t       = outlets;
    t_point  *b        = boxes;
        
    for (i = 0; i < n; i++) {
    //
    t_object *y = glist_graphicsGetObjectAt (glist, i);

    if (pd_class (y) == voutlet_class) { *t = y; *b = object_getPoint (y); t++; b++; }
    //
    }
    
    /* Take the most right outlet and put it first. */
    /* Remove it from the list. */
    /* Do it again. */
    
    for (i = numberOfOutlets; i > 0; i--) {
    //
    int j = numberOfOutlets;
    int maximumX = -PD_INT_MAX;
    t_object **mostRightOutlet = NULL;
    
    for (t = outlets, b = boxes; j--; t++, b++) {
        if (*t && point_getX (b) > maximumX) {
            maximumX = point_getX (b); mostRightOutlet = t;
        }
    }
    
    if (mostRightOutlet) {
        outlet_moveFirst (voutlet_getOutlet (cast_pd (*mostRightOutlet))); *mostRightOutlet = NULL;
    }
    //
    }
    
    PD_MEMORY_FREE (boxes);
    PD_MEMORY_FREE (outlets);
    
    glist_updateLinesForObject (glist_getParent (glist), cast_object (glist));
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
