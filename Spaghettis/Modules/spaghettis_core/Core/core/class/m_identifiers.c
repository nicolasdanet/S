
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Machinery to track objects during encapsulation. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PD_LOCAL t_id object_getUnique (t_object *x)
{
    return x->g_id[0];
}

PD_LOCAL void object_setUnique (t_object *x, t_id u)
{
    x->g_id[0] = u;
}

PD_LOCAL void object_changeUnique (t_object *x, t_id u)
{
    #if defined ( PD_BUILDING_APPLICATION )
    
    instance_registerRename (x, u);
    
    #endif
    
    object_setUnique (x, u);
}

PD_LOCAL void object_serializeUnique (t_object *x, t_symbol *s, t_buffer *b)
{
    buffer_appendSymbol (b, sym___hash__X);
    buffer_appendSymbol (b, s);
    utils_appendUnique (b, object_getUnique (x));
    buffer_appendSemicolon (b);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_id object_getSource (t_object *x)
{
    return x->g_id[1];
}

PD_LOCAL void object_setSource (t_object *x, t_id u)
{
    x->g_id[1] = u;
}

PD_LOCAL void object_changeSource (t_object *x, t_id u)
{
    object_setSource (x, u);
}

PD_LOCAL void object_serializeSource (t_object *x, t_symbol *s, t_buffer *b)
{
    buffer_appendSymbol (b, sym___hash__X);
    buffer_appendSymbol (b, s);
    utils_appendUnique (b, object_getSource (x));
    buffer_appendSemicolon (b);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_id object_getNative (t_object *x)
{
    return x->g_id[2];
}

PD_LOCAL void object_setNative (t_object *x, t_id u)
{
    x->g_id[2] = u;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
