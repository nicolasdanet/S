
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

t_undomanager  *glist_undoReplaceManager       (t_glist *, t_undomanager *);

t_rectangle    glist_getBoundingBoxOfAll       (t_glist *);
void           glist_removeInletsAndOutlets    (t_glist *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

t_buffer       *clipboard_copyProceed          (t_glist *, int, int);

int            clipboard_pasteProceed          (t_glist *, t_buffer *, t_point *, int);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _connecthelper {
    t_id                    x_id;
    int                     x_n;
    int                     x_assigned;
    int                     x_index;
    struct _connecthelper   *x_next;
    } t_connecthelper;

typedef struct _throughhelper {
    int                     x_in;
    int                     x_out;
    struct _throughhelper   *x_next;
    } t_throughhelper;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_connecthelper *connecthelper_add (t_connecthelper *h, t_object *o, int n, int assigned)
{
    t_connecthelper *toAdd = (t_connecthelper *)PD_MEMORY_GET (sizeof (t_connecthelper));

    toAdd->x_id       = object_getUnique (o);
    toAdd->x_n        = n;
    toAdd->x_assigned = assigned;
    toAdd->x_index    = -1;

    if (h) { toAdd->x_next = h; }

    return toAdd;
}

static void connecthelper_free (t_connecthelper *h)
{
    t_connecthelper *t = h;
    
    while (t) { t_connecthelper *next = t->x_next; PD_MEMORY_FREE (t); t = next; }
}

/* Note that the identifier of objects are changed by copy/paste operation. */

static void connecthelper_fetchIndexes (t_connecthelper *h)
{
    t_connecthelper *t = h;
    
    while (t) {
    //
    int n = -1; t_error err = unique_objectGetIndexOf (t->x_id, &n);
    
    PD_ASSERT (!err); PD_UNUSED (err);
    
    t->x_index = n;
    
    t = t->x_next;
    //
    }
}

static void connecthelper_fetchUniques (t_connecthelper *h, t_glist *glist, int n)
{
    t_connecthelper *t = h;
    
    while (t) {
    //
    t->x_index += n; t->x_id = object_getUnique (glist_objectGetAt (glist, t->x_index));
    
    t = t->x_next;
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_throughhelper *throughhelper_add (t_throughhelper *h, int m, int n)
{
    t_throughhelper *toAdd = (t_throughhelper *)PD_MEMORY_GET (sizeof (t_throughhelper));

    toAdd->x_in  = m;
    toAdd->x_out = n;

    if (h) { toAdd->x_next = h; }

    return toAdd;
}

static void throughhelper_free (t_throughhelper *h)
{
    t_throughhelper *t = h;
    
    while (t) { t_throughhelper *next = t->x_next; PD_MEMORY_FREE (t); t = next; }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void encapsulate_deencapsulateFetchConnectionsForParent (t_glist *glist,
    t_glist *parent,
    t_connecthelper **inParent,
    t_connecthelper **outParent)
{
    t_connecthelper *inlets  = NULL;
    t_connecthelper *outlets = NULL;
    
    t_traverser t; traverser_start (&t, parent);
    
    while (traverser_next (&t)) {
    //
    t_object *o = traverser_getSource (&t);
    t_object *d = traverser_getDestination (&t);
    int m = traverser_getIndexOfOutlet (&t);
    int n = traverser_getIndexOfInlet (&t);
    
    if (d == cast_object (glist))      { inlets  = connecthelper_add (inlets,  o, m, n); }
    else if (o == cast_object (glist)) { outlets = connecthelper_add (outlets, d, n, m); }
    //
    }
    
    (*inParent)  = inlets;
    (*outParent) = outlets;
}

static void encapsulate_deencapsulateFetchConnectionsForChild (t_glist *glist,
    t_connecthelper **inChild,
    t_connecthelper **outChild,
    t_throughhelper **directChild)
{
    t_connecthelper *inlets  = NULL;
    t_connecthelper *outlets = NULL;
    t_throughhelper *direct  = NULL;
    
    t_traverser t; traverser_start (&t, glist);
    
    while (traverser_next (&t)) {
    //
    t_object *o = traverser_getSource (&t);
    t_object *d = traverser_getDestination (&t);
    int m = traverser_getIndexOfOutlet (&t);
    int n = traverser_getIndexOfInlet (&t);
    
    if (pd_class (d) == voutlet_class) {
        int index = voutlet_getIndex ((t_voutlet *)d);
        if (pd_class (o) != vinlet_class) { outlets = connecthelper_add (outlets, o, m, index); }
        else {
            direct = throughhelper_add (direct, vinlet_getIndex ((t_vinlet *)o), index);
        }
        
    } else if (pd_class (o) == vinlet_class) {
        inlets = connecthelper_add (inlets, d, n, vinlet_getIndex ((t_vinlet *)o));
    }
    //
    }
    
    (*inChild)     = inlets;
    (*outChild)    = outlets;
    (*directChild) = direct;
}

static void encapsulate_deencapsulateRemoveInletsAndOutlets (t_glist *glist, t_glist *parent)
{
    /* Temporary use parent's undomanager. */
    
    t_undomanager *t = glist_undoReplaceManager (glist, glist_getUndoManager (parent));
    
    glist_objectDeleteLines (parent, cast_object (glist));
    glist_removeInletsAndOutlets (glist);
    
    glist_undoReplaceManager (glist, t);
}

static void encapsulate_deencapsulatePaste (t_glist *parent, t_buffer *b, t_rectangle *r, t_point *pt)
{
    int t1    = point_getX (pt);
    int t2    = point_getY (pt) - (rectangle_getHeight (r) / 2.0);
    t_point t = point_make (t1, t2);
    
    clipboard_pasteProceed (parent, b, &t, 0);
}

static void encapsulate_deencapsulateReconnect (t_connecthelper *h1, t_connecthelper *h2)
{
    t_connecthelper *t1 = h1;

    while (t1) {
    //
    t_connecthelper *t2 = h2;
    
    while (t2) {
    //
    if (t1->x_assigned == t2->x_assigned) {
        unique_objectLineConnect (t1->x_id, t1->x_n, t2->x_id, t2->x_n);
    }
    
    t2 = t2->x_next;
    //
    }
    t1 = t1->x_next;
    //
    }
}

static void encapsulate_deencapsulateDirect (t_connecthelper *h1, t_connecthelper *h2, t_throughhelper *d)
{
    t_throughhelper *t0 = d;
    
    while (t0) {
    //
    t_connecthelper *t1 = h1;

    while (t1) {
    //
    if (t1->x_assigned == t0->x_in) {
    
        t_connecthelper *t2 = h2;
    
        while (t2) {
        //
        if (t2->x_assigned == t0->x_out) { unique_objectLineConnect (t1->x_id, t1->x_n, t2->x_id, t2->x_n); }
    
        t2 = t2->x_next;
        //
        }
    }
    
    t1 = t1->x_next;
    //
    }
    
    t0 = t0->x_next;
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void encapsulate_deencapsulate (t_glist *glist)
{
    if (glist_isSubpatch (glist)) {
    //
    t_glist *parent = glist_getParent (glist);
    int undoable    = glist_undoIsOk (parent);
    
    PD_ASSERT (parent != NULL);
        
    if (undoable) { glist_undoAppend (parent, undodeencapsulate_new()); }
    
    instance_pendingBegin();
    
    {
        int n, state = dsp_suspend();
        t_buffer *b  = NULL;
        
        t_rectangle r; t_point p;

        t_connecthelper *inParent  = NULL;
        t_connecthelper *outParent = NULL;
        t_connecthelper *inChild   = NULL;
        t_connecthelper *outChild  = NULL;
        t_throughhelper *direct    = NULL;
    
        encapsulate_deencapsulateFetchConnectionsForParent (glist, parent, &inParent, &outParent);
        encapsulate_deencapsulateFetchConnectionsForChild (glist, &inChild, &outChild, &direct);
        encapsulate_deencapsulateRemoveInletsAndOutlets (glist, parent);
        connecthelper_fetchIndexes (inChild);
        connecthelper_fetchIndexes (outChild);
        r = glist_getBoundingBoxOfAll (glist);
        b = clipboard_copyProceed (glist, 1, 1);
        p = object_getPoint (cast_object (glist));
        glist_objectRemove (parent, cast_object (glist));
        n = glist_objectGetNumberOf (parent);
        encapsulate_deencapsulatePaste (parent, b, &r, &p);
        connecthelper_fetchUniques (inChild,  parent, n);
        connecthelper_fetchUniques (outChild, parent, n);
        encapsulate_deencapsulateReconnect (inParent, inChild);
        encapsulate_deencapsulateReconnect (outChild, outParent);
        encapsulate_deencapsulateDirect (inParent, outParent, direct);
    
        connecthelper_free (inParent);
        connecthelper_free (outParent);
        connecthelper_free (inChild);
        connecthelper_free (outChild);
        throughhelper_free (direct);
    
        dsp_resume (state);
        buffer_free (b);
        glist_setDirty (parent, 1);
    }
    
    instance_pendingEnd();
    
    if (undoable) { glist_undoAppendSeparator (parent); }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
