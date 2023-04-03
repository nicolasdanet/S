
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define REGISTER_DEFAULT_SIZE   1024
#define REGISTER_HOT_SIZE       128

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void register_enlarge (t_register *x)
{
    size_t newSize  = sizeof (t_registerelement) * (x->r_allocated * 2);
    size_t oldSize  = sizeof (t_registerelement) * (x->r_allocated);

    x->r_raw        = (t_registerelement *)PD_MEMORY_RESIZE (x->r_raw, oldSize, newSize);
    x->r_allocated  *= 2;
}

static t_registerelement *register_swapped (t_register *x, size_t i)
{
    int rand = (int)(PD_RAND48_DOUBLE (x->r_state) * REGISTER_HOT_SIZE);
    t_registerelement *m = x->r_raw + i;
    t_registerelement *e = x->r_raw + rand;
    
    {
        t_id id          = e->re_id;
        t_id owner       = e->re_owner;
        t_object *object = e->re_object;
    
        e->re_id         = m->re_id;
        e->re_owner      = m->re_owner;
        e->re_object     = m->re_object;
    
        m->re_id         = id;
        m->re_owner      = owner;
        m->re_object     = object;
    }
    
    return e;
}

/* Fetch the element (raised if too far). */

// TODO: Check to see if it is really an optimization.

static t_registerelement *register_fetch (t_register *x, t_id u)
{
    size_t i; t_registerelement *match = NULL;
    
    for (i = 0; i < x->r_size; i++) {
    //
    t_registerelement *e = x->r_raw + i; if (e->re_id == u) { match = e; break; }
    //
    }
    
    if (match) {
    //
    if (i < REGISTER_HOT_SIZE) { return match; } else { return register_swapped (x, i); }
    //
    }
    
    return match;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Append the element. */

PD_LOCAL void register_add (t_register *x, t_object *o)
{
    if (x->r_size == x->r_allocated) { register_enlarge (x); }
    
    PD_ASSERT (x->r_size < x->r_allocated);
    
    {
        t_glist *owner  = object_getOwner (o);
        
        t_registerelement *next = x->r_raw + x->r_size;
    
        next->re_id     = object_getUnique (o);
        next->re_owner  = owner ? object_getUnique (cast_object (owner)) : 0;
        next->re_object = o;
    
        x->r_size++;
    }
}

/* Fetch, swap with the last, then down size by one. */

PD_LOCAL t_error register_remove (t_register *x, t_id u)
{
    size_t i; t_registerelement *toDelete = NULL;
    
    for (i = 0; i < x->r_size; i++) {
    //
    t_registerelement *e = x->r_raw + i; if (e->re_id == u) { toDelete = e; break; }
    //
    }
    
    if (toDelete != NULL) {
        
        t_registerelement *last = x->r_raw + (x->r_size - 1);
    
        if (toDelete != last) {
            toDelete->re_id     = last->re_id;
            toDelete->re_owner  = last->re_owner;
            toDelete->re_object = last->re_object;
        }
        
        x->r_size--;
        
        return PD_ERROR_NONE;
    }
    
    return PD_ERROR;
}

PD_LOCAL void register_rename (t_register *x, t_id u, t_id v)
{
    size_t i;
    
    for (i = 0; i < x->r_size; i++) {
    //
    t_registerelement *e = x->r_raw + i;
    
    if (e->re_id    == u) { e->re_id    = v; }
    if (e->re_owner == u) { e->re_owner = v; }
    //
    }
}

PD_LOCAL int register_contains (t_register *x, t_id u)
{
    return (register_getObject (x, u) != NULL);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_object *register_getObject (t_register *x, t_id u)
{
    t_registerelement *e = register_fetch (x, u); return (e ? e->re_object : NULL);
}

PD_LOCAL t_glist *register_getOwner (t_register *x, t_id u)
{
    t_registerelement *e = register_fetch (x, u);
    
    if (e && e->re_owner) {
    //
    e = register_fetch (x, e->re_owner);
    
    return (e ? cast_glist (e->re_object) : NULL);
    //
    }
    
    return NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_register *register_new (void)
{
    t_register *x = (t_register *)PD_MEMORY_GET (sizeof (t_register));
 
    x->r_allocated = REGISTER_DEFAULT_SIZE;
    x->r_size      = 0;
    x->r_raw       = (t_registerelement *)PD_MEMORY_GET (x->r_allocated * sizeof (t_registerelement));
    
    PD_RAND48_INIT (x->r_state);
    
    return x;
}

PD_LOCAL void register_free (t_register *x)
{
    PD_MEMORY_FREE (x->r_raw);
    PD_MEMORY_FREE (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
