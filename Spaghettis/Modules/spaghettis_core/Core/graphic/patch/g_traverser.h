
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef G_TRAVERSER_H_
#define G_TRAVERSER_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _traverser {
    t_glist             *tr_owner;
    t_outconnect        *tr_connectionCached;
    t_object            *tr_srcObject;
    t_outlet            *tr_srcOutlet;
    t_object            *tr_destObject;
    t_inlet             *tr_destInlet;
    int                 tr_srcIndexOfObject;
    int                 tr_srcIndexOfOutlet;
    int                 tr_srcNumberOfOutlets;
    int                 tr_srcIndexOfNextOutlet;
    int                 tr_destIndexOfInlet;
    int                 tr_destNumberOfInlets;
    } t_traverser;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Notice that objects MUST NOT be deleted during the traversal. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

t_outconnect   *traverser_next          (t_traverser *t);

void           traverser_start          (t_traverser *t, t_glist *glist);
t_error        traverser_disconnect     (t_traverser *t, t_glist *glist);
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int traverser_getIndexOfSource          (t_traverser *t);
int traverser_getIndexOfDestination     (t_traverser *t);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static inline t_object *traverser_getSource (t_traverser *t)
{
    return t->tr_srcObject;
}

static inline t_object *traverser_getDestination (t_traverser *t)
{
    return t->tr_destObject;
}

static inline t_outlet *traverser_getOutlet (t_traverser *t)
{
    return t->tr_srcOutlet;
}

static inline t_inlet *traverser_getInlet (t_traverser *t)
{
    return t->tr_destInlet;                                         /* Can be NULL (first inlet). */
}

static inline int traverser_getIndexOfOutlet (t_traverser *t)
{
    return t->tr_srcIndexOfOutlet;
}

static inline int traverser_getIndexOfInlet (t_traverser *t)
{
    return t->tr_destIndexOfInlet;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // G_TRAVERSER_H_
