
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_pd *pd_new (t_class *c)
{
    t_pd *x;
    
    PD_ASSERT (c != NULL);
    PD_ASSERT (!class_isAbstract (c));
    PD_ASSERT (class_getInstanceSize (c) > 0);

    x = (t_pd *)PD_MEMORY_GET (class_getInstanceSize (c));
    
    *x = c;
    
    if (!class_isInvisible (c)) {
    //
    t_id u = utils_unique();
    
    object_setUnique (cast_object (x), u);
    object_setSource (cast_object (x), u);
    object_setNative (cast_object (x), u);
    
    object_setOwner (cast_object (x), instance_contextGetCurrent());
    //
    }
    
    return x;
}

PD_LOCAL void pd_free (t_pd *x)
{
    t_class *c = pd_class (x);

    PD_ASSERT (c != NULL);
    PD_ASSERT (!class_isAbstract (c));
    PD_ASSERT (class_getInstanceSize (c) > 0);
    
    if (class_hasDataFunction (c)) {
        instance_setBoundA (NULL);      /* In case it is still bind. */
    }
    
    if (class_hasFreeMethod (c)) { (*(class_getFreeMethod (c))) (x); }

    if (class_isBox (c)) {
        while (object_getOutlets (cast_object (x))) { outlet_free (object_getOutlets (cast_object (x))); }
        while (object_getInlets (cast_object (x)))  { inlet_free (object_getInlets (cast_object (x)));   }
        
        if (cast_object (x)->g_buffer) { buffer_free (cast_object (x)->g_buffer); }
    }

    PD_MEMORY_FREE (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
