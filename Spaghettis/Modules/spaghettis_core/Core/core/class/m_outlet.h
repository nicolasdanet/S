
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef M_OUTLET_H_
#define M_OUTLET_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

struct _outconnect {
    struct _outconnect  *oc_next;
    t_pd                *oc_receiver;
    t_id                oc_id;
    };

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static inline t_outconnect *connection_getNext (t_outconnect *x)
{
    return x->oc_next;
}

static inline t_pd *connection_getReceiver (t_outconnect *x)
{
    return x->oc_receiver;
}

static inline t_id connection_getUnique (t_outconnect *x)
{
    return x->oc_id;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

struct _outlet {
    struct _outlet      *o_next;
    t_object            *o_owner;
    t_outconnect        *o_connections;
    t_symbol            *o_type;
    };

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_outconnect   *outlet_addConnection       (t_outlet *x, t_pd *receiver);

PD_LOCAL t_error        outlet_removeConnection     (t_outlet *x, t_pd *receiver, t_id *u);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void   outlet_free                         (t_outlet *x);
PD_LOCAL void   outlet_moveFirst                    (t_outlet *x);
PD_LOCAL int    outlet_isSignal                     (t_outlet *x);
PD_LOCAL int    outlet_getIndex                     (t_outlet *x);
PD_LOCAL int    outlet_getIndexAsSignal             (t_outlet *x);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static inline t_outlet *outlet_getNext (t_outlet *x)
{
    return x->o_next;
}

static inline t_outconnect *outlet_getConnections (t_outlet *x)
{
    return x->o_connections;
}

static inline t_symbol *outlet_getType (t_outlet *x)
{
    return (x->o_type == sym_mixed) ? &s_anything : x->o_type;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // M_OUTLET_H_
