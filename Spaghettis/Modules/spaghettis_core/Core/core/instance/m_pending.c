
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PD_LOCAL int instance_pendingRequired (t_object *y)
{
    if (instance_hasPending()) {
    //
    return class_hasPendingRequired (pd_class (y));
    //
    }
    
    return 0;
}

PD_LOCAL t_object *instance_pendingFetch (t_object *y)
{
    if (instance_hasPending()) {
    //
    t_id u1 = object_getUnique (y);
    t_id s1 = object_getSource (y);
    
    t_object *t = instance_get()->pd_pending;
    
    while (t) {
    //
    t_id u2 = object_getUnique (t);
    t_id s2 = object_getSource (t);
        
    if (u1 == u2 || u1 == s2 || s1 == u2 || s1 == s2) { return t; } else { t = t->g_next; }
    //
    }
    //
    }
    
    return NULL;
}

PD_LOCAL void instance_pendingAdd (t_object *y)
{
    if (class_hasDismissFunction (pd_class (y))) { (*class_getDismissFunction (pd_class (y))) (y); }

    y->g_next = instance_get()->pd_pending; instance_get()->pd_pending = y;
}

PD_LOCAL void instance_pendingRelease (void)
{
    t_object *y = instance_get()->pd_pending; instance_get()->pd_pending = NULL;
    
    while (y) { t_object *t = y; y = y->g_next; pd_free (cast_pd (t)); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
