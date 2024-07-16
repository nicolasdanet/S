
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
    t_object *y = NULL;
    
    int n = 0;
    
    for (y = g->gl_graphics; y; y = y->g_next) { n++; }
    
    return n;
}

int glist_graphicsGetIndexOf (t_glist *g, t_object *y)
{
    t_object *t = NULL;
    int n = 0;
    
    for (t = g->gl_graphics; t && t != y; t = t->g_next) {
        n++;
    }
    
    return n;
}

int glist_graphicsGetIndexOfSelected (t_glist *g, t_object *y)
{
    t_object *t = NULL;
    
    int n = 0;

    for (t = g->gl_graphics; t && t != y; t = t->g_next) {
        if (glist_objectIsSelected (g, t)) {
            n++;
        }
    }
    
    return n;
}

int glist_graphicsGetCountByClass (t_glist *g, t_class *c)
{
    int i, n = glist_graphicsGetSize (g);
    
    int k = 0;
    
    for (i = 0; i < n; i++) { if (pd_class (glist_graphicsGetObjectAt (g, i)) == c) { k++; } }
    
    return k;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_object *glist_graphicsGetObjectAt (t_glist *g, int i)
{
    t_object *y = NULL;
    
    int n = 0;
    
    for (y = g->gl_graphics; y; y = y->g_next) {
        if (n == i) { return y; }
        else {
            n++;
        }
    }
    
    return NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_object *glist_graphicsGetFirst (t_glist *g)
{
    return g->gl_graphics;
}

t_object *glist_graphicsGetLast (t_glist *g)
{
    if (g->gl_graphics) {
    //
    t_object *t1 = NULL;
    t_object *t2 = NULL;
    
    for ((t1 = g->gl_graphics); (t2 = t1->g_next); (t1 = t2)) { }
    
    return t1;
    //
    }
    
    return NULL;
}

t_object *glist_graphicsGetNext (t_glist *g, t_object *y)
{
    PD_ASSERT (y); return y->g_next;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void glist_graphicsAdd (t_glist *g, t_object *y, t_object *first, int prepend)
{
    y->g_next = NULL;
    
    if (first != NULL) { y->g_next = first->g_next; first->g_next = y; }
    else {
    //
    if (prepend || !g->gl_graphics) {
        y->g_next = g->gl_graphics; g->gl_graphics = y;
    } else {
        t_object *t = NULL; for (t = g->gl_graphics; t->g_next; t = t->g_next) { }
        t->g_next = y;
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void glist_graphicsAppend (t_glist *g, t_object *y)
{
    glist_graphicsAdd (g, y, NULL, 0);
}

void glist_graphicsPrepend (t_glist *g, t_object *y)
{
    glist_graphicsAdd (g, y, NULL, 1);
}

void glist_graphicsInsert (t_glist *g, t_object *y, int i)
{
    glist_graphicsAdd (g, y, glist_graphicsGetObjectAt (g, (i - 1)), 0);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void glist_graphicsRemove (t_glist *g, t_object *y)
{
    if (g->gl_graphics == y) { g->gl_graphics = y->g_next; }
    else {
        t_object *t = NULL;
        for (t = g->gl_graphics; t; t = t->g_next) {
            if (t->g_next == y) { t->g_next = y->g_next; break; }
        }
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
