
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static int outlet_connectionAlreadyExist (t_outlet *x, t_pd *receiver)
{
    t_outconnect *oc = x->o_connections;

    while (oc) {
    //
    if (oc->oc_receiver == receiver) { return 1; } oc = oc->oc_next;
    //
    }

    return 0;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PD_LOCAL t_outconnect *outlet_addConnection (t_outlet *x, t_pd *receiver)
{
    if (!outlet_connectionAlreadyExist (x, receiver)) {
    //
    t_outconnect *oc1 = NULL;
    t_outconnect *oc2 = NULL;
    
    oc1 = (t_outconnect *)PD_MEMORY_GET (sizeof (t_outconnect));
    oc1->oc_next = NULL;
    oc1->oc_receiver = receiver;
    oc1->oc_id = utils_unique();

    if ((oc2 = x->o_connections)) { while (oc2->oc_next) { oc2 = oc2->oc_next; } oc2->oc_next = oc1; }
    else {
        x->o_connections = oc1;
    }
    
    return oc1;
    //
    }
    
    return NULL;
}

PD_LOCAL t_error outlet_removeConnection (t_outlet *x, t_pd *receiver, t_id *u)
{
    t_outconnect *oc1 = NULL;
    t_outconnect *oc2 = NULL;
    
    if ((oc1 = x->o_connections)) {

        if (oc1->oc_receiver == receiver) {
            x->o_connections = oc1->oc_next;     *u = oc1->oc_id; PD_MEMORY_FREE (oc1); return PD_ERROR_NONE;
            
        } else {
            while ((oc2 = oc1->oc_next)) {
                if (oc2->oc_receiver != receiver) { oc1 = oc2; }
                else {
                    oc1->oc_next = oc2->oc_next; *u = oc2->oc_id; PD_MEMORY_FREE (oc2); return PD_ERROR_NONE;
                }
            }
        }
    }
    
    *u = 0; return PD_ERROR;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static int outlet_isBang (t_outlet *x)
{
    return (x->o_type == &s_bang);
}

static int outlet_isFloat (t_outlet *x)
{
    return (x->o_type == &s_float);
}

static int outlet_isSymbol (t_outlet *x)
{
    return (x->o_type == &s_symbol);
}

static int outlet_isList (t_outlet *x)
{
    return (x->o_type == &s_list);
}

static int outlet_isAnything (t_outlet *x)
{
    return (x->o_type == &s_anything);
}

#if PD_WITH_DEBUG

static int outlet_isMixed (t_outlet *x)
{
    return (x->o_type == sym_mixed);
}

#endif // PD_WITH_DEBUG

PD_LOCAL int outlet_isSignal (t_outlet *x)
{
    return (x->o_type == &s_signal);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void outlet_moveFirst (t_outlet *x)
{
    t_object *owner = x->o_owner;
    
    if (owner->g_outlets != x) {
    //
    t_outlet *o = NULL;
    
    for (o = owner->g_outlets; o; o = o->o_next) {
        if (o->o_next == x) {
            o->o_next = x->o_next;
            x->o_next = owner->g_outlets;
            owner->g_outlets = x;
            return;
        }
    }
    
    PD_BUG;
    //
    }
}

PD_LOCAL int outlet_getIndex (t_outlet *x)
{
    t_outlet *o = NULL;
    int n = 0;
    
    for (o = x->o_owner->g_outlets; o && o != x; o = o->o_next) { n++; }

    return n;
}

PD_LOCAL int outlet_getIndexAsSignal (t_outlet *x)
{
    t_outlet *o = NULL;
    int n = 0;
        
    PD_ASSERT (outlet_isSignal (x));
    
    for (o = x->o_owner->g_outlets; o && o != x; o = o->o_next) {
        if (outlet_isSignal (o)) { n++; }
    }
    
    return n;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Note that it doesn't matter, but it could in the future. */

static void outlet_checkType (t_outlet *x, int match)
{
    #if PD_WITH_DEBUG
    
    if (!match && !outlet_isMixed (x)) {
    //
    PD_BUG; post_log ("*** Type / %s", class_getNameAsString (pd_class (x->o_owner)));
    //
    }
    
    #endif // PD_WITH_DEBUG
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void outlet_bang (t_outlet *x)
{
    outlet_checkType (x, outlet_isBang (x));
    
    if (!instance_overflowPush()) {
        t_outconnect *oc = NULL;
        for (oc = x->o_connections; oc; oc = oc->oc_next) { pd_bang (oc->oc_receiver); }
    }
    
    instance_overflowPop();
}

PD_LOCAL void outlet_float (t_outlet *x, t_float f)
{
    outlet_checkType (x, outlet_isFloat (x));
        
    if (!instance_overflowPush()) {
        t_outconnect *oc = NULL;
        for (oc = x->o_connections; oc; oc = oc->oc_next) { pd_float (oc->oc_receiver, f); }
    }
    
    instance_overflowPop();
}

PD_LOCAL void outlet_symbol (t_outlet *x, t_symbol *s)
{
    outlet_checkType (x, outlet_isSymbol (x));
        
    if (!instance_overflowPush()) {
        t_outconnect *oc = NULL;
        for (oc = x->o_connections; oc; oc = oc->oc_next) { pd_symbol (oc->oc_receiver, s); }
    }
    
    instance_overflowPop();
}

PD_LOCAL void outlet_list (t_outlet *x, int argc, t_atom *argv)
{
    outlet_checkType (x, outlet_isList (x));
        
    if (!instance_overflowPush()) {
        t_outconnect *oc = NULL;
        for (oc = x->o_connections; oc; oc = oc->oc_next) { pd_list (oc->oc_receiver, argc, argv); }
    }
    
    instance_overflowPop();
}

PD_LOCAL void outlet_anything (t_outlet *x, t_symbol *s, int argc, t_atom *argv)
{
    outlet_checkType (x, outlet_isAnything (x));
        
    if (!instance_overflowPush()) {
        t_outconnect *oc = NULL;
        for (oc = x->o_connections; oc; oc = oc->oc_next) { pd_message (oc->oc_receiver, s, argc, argv); }
    }
    
    instance_overflowPop();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_outlet *outlet_new (t_object *owner, t_symbol *s)
{
    t_outlet *x  = (t_outlet *)PD_MEMORY_GET (sizeof (t_outlet));
    t_outlet *o1 = NULL;
    t_outlet *o2 = NULL;
    
    x->o_next        = NULL;
    x->o_owner       = owner;
    x->o_connections = NULL;
    x->o_type        = s;
    
    if ((o1 = owner->g_outlets)) { while ((o2 = o1->o_next)) { o1 = o2; } o1->o_next = x; }
    else {
        owner->g_outlets = x;
    }

    return x;
}

PD_LOCAL void outlet_free (t_outlet *x)
{
    t_object *y = x->o_owner;
    t_outlet *o = NULL;
    
    PD_ASSERT (x->o_connections == NULL);
    
    if (y->g_outlets == x) { y->g_outlets = x->o_next; }
    else {
        for (o = y->g_outlets; o; o = o->o_next) {
            if (o->o_next == x) { o->o_next = x->o_next; break; }
        }
    }
    
    PD_MEMORY_FREE (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_outlet *outlet_newUndefined (t_object *owner)
{
    return outlet_new (owner, NULL);
}

PD_LOCAL t_outlet *outlet_newBang (t_object *owner)
{
    return outlet_new (owner, &s_bang);
}

PD_LOCAL t_outlet *outlet_newFloat (t_object *owner)
{
    return outlet_new (owner, &s_float);
}

PD_LOCAL t_outlet *outlet_newSymbol (t_object *owner)
{
    return outlet_new (owner, &s_symbol);
}

PD_LOCAL t_outlet *outlet_newList (t_object *owner)
{
    return outlet_new (owner, &s_list);
}

PD_LOCAL t_outlet *outlet_newAnything (t_object *owner)
{
    return outlet_new (owner, &s_anything);
}

PD_LOCAL t_outlet *outlet_newMixed (t_object *owner)
{
    return outlet_new (owner, sym_mixed);
}

PD_LOCAL t_outlet *outlet_newSignal (t_object *owner)
{
    return outlet_new (owner, &s_signal);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
