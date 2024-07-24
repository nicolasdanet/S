
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

int glist_graphicsGetSize (t_glist *g)
{
    return buffer_getSize (g->gl_graphics);
}

t_object *glist_graphicsGetObjectAt (t_glist *g, int i)
{
    PD_ASSERT (i >= 0);
    
    if (i < buffer_getSize (g->gl_graphics)) { return buffer_getObjectAt (g->gl_graphics, i); }
    else {
        return NULL;
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int glist_graphicsGetIndexOf (t_glist *g, t_object *y)
{
    int i, n = buffer_getSize (g->gl_graphics);
    
    for (i = 0; i < n; i++) {
        if (buffer_getObjectAt (g->gl_graphics, i) == y) { return i; }
    }

    return -1;
}

int glist_graphicsGetIndexAmongSelected (t_glist *g, t_object *y)
{
    int k = 0;
        
    int i, n = buffer_getSize (g->gl_graphics);
    
    for (i = 0; i < n; i++) {
        t_object *t = buffer_getObjectAt (g->gl_graphics, i);
        if (glist_objectIsSelected (g, t)) {
            if (t == y) { return k; }
            else {
                k++;
            }
        }
    }
    
    return -1;
}

int glist_graphicsGetCountByClass (t_glist *g, t_class *c)
{
    int k = 0;
    
    int i, n = buffer_getSize (g->gl_graphics);
    
    for (i = 0; i < n; i++) {
        if (pd_class (buffer_getObjectAt (g->gl_graphics, i)) == c) { k++; }
    }
    
    return k;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void glist_graphicsInsert (t_glist *g, t_object *y, int i)
{
    buffer_insertAsObjectAtIndex (g->gl_graphics, i, y);
}

void glist_graphicsAppend (t_glist *g, t_object *y)
{
    buffer_appendAsObject (g->gl_graphics, y);
}

static void glist_graphicsPrepend (t_glist *g, t_object *y)
{
    buffer_prependAsObject (g->gl_graphics, y);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

// TODO: Consider a better approach for multiple objects removed at once?

void glist_graphicsRemove (t_glist *g, t_object *y)
{
    int i = glist_graphicsGetIndexOf (g, y);
    
    PD_ASSERT (i >= 0);
    
    t_error err = buffer_removeAtIndex (g->gl_graphics, i);
    
    PD_ASSERT (!err); PD_UNUSED (err);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_object *glist_graphicsGetFirst (t_glist *g)
{
    return glist_graphicsGetObjectAt (g, 0);
}

t_object *glist_graphicsGetLast (t_glist *g)
{
    return glist_graphicsGetObjectAt (g, glist_graphicsGetSize (g) - 1);
}

t_object *glist_graphicsGetNext (t_glist *g, t_object *y)
{
    int i = glist_graphicsGetIndexOf (g, y);
    
    if (i >= 0) { return glist_graphicsGetObjectAt (g, i + 1); }
    else {
        return NULL;
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void glist_graphicsMoveAtFirst (t_glist *g, t_object *y)
{
    glist_graphicsRemove (g, y);
    glist_graphicsPrepend (g, y);
}

void glist_graphicsMoveAtLast (t_glist *g, t_object *y)
{
    glist_graphicsRemove (g, y);
    glist_graphicsAppend (g, y);
}

void glist_graphicsMoveAt (t_glist *g, t_object *y, int n)
{
    glist_graphicsRemove (g, y);
    glist_graphicsInsert (g, y, n);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
