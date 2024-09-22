
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int instance_pendingRequired (t_object *y)
{
    if (instance_hasPending()) {
    //
    return class_hasPendingRequired (pd_class (y));
    //
    }
    
    return 0;
}

t_object *instance_pendingFetch (t_object *y)
{
    if (instance_hasPending()) {
    //
    t_id u1 = object_getUnique (y);
    t_id s1 = object_getSource (y);
    
    t_buffer *b = instance_get()->pd_pending;
    
    int i, n = buffer_getSize (b);
    
    for (i = 0; i < n; i++) {
    //
    t_object *t = buffer_getObjectAt (b, i);
    
    t_id u2 = object_getUnique (t);
    t_id s2 = object_getSource (t);
        
    if (u1 == u2 || u1 == s2 || s1 == u2 || s1 == s2) { return t; }
    //
    }
    //
    }
    
    return NULL;
}

void instance_pendingAdd (t_object *y)
{
    if (class_hasDismissFunction (pd_class (y))) { (*class_getDismissFunction (pd_class (y))) (y); }

    buffer_appendAsObject (instance_get()->pd_pending, y);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
