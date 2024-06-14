
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void pd_bind (t_pd *x, t_symbol *s)
{
    PD_ASSERT (s != &s__A);
    PD_ASSERT (s != &s__N);
    PD_ASSERT (s != &s__X);
    
    PD_ASSERT (s != &s_);
    
    if (s->s_thing) {
    
        if (pd_class (s->s_thing) == bindlist_class) {
            t_bindlist *b = (t_bindlist *)s->s_thing;
            t_bindelement *e = (t_bindelement *)PD_MEMORY_GET (sizeof (t_bindelement));
            e->e_next = b->b_list;
            e->e_what = x;
            b->b_list = e;
            
        } else {
            t_bindlist *b = (t_bindlist *)pd_new (bindlist_class);
            t_bindelement *e1 = (t_bindelement *)PD_MEMORY_GET (sizeof (t_bindelement));
            t_bindelement *e2 = (t_bindelement *)PD_MEMORY_GET (sizeof (t_bindelement));
            b->b_list  = e1;
            e1->e_what = x;
            e1->e_next = e2;
            e2->e_what = s->s_thing;
            e2->e_next = NULL;
            s->s_thing = &b->b_pd;
        }
        
    } else {
        s->s_thing = x;
    }
}

static t_error pd_unbindProceed (t_pd *x, t_symbol *s)
{
    PD_ASSERT (s != &s_);
    
    if (s->s_thing == x) {
        s->s_thing = NULL; return PD_ERROR_NONE;
        
    } else if (s->s_thing && pd_class (s->s_thing) == bindlist_class) {
        
        t_bindlist *b = (t_bindlist *)s->s_thing;
        t_bindelement *e1 = NULL;
        t_bindelement *e2 = NULL;
        
        if (b->b_list) {
            if ((e1 = b->b_list)->e_what == x) {
                b->b_list = e1->e_next;
                PD_MEMORY_FREE (e1); return PD_ERROR_NONE;
            } else {
                for ((e1 = b->b_list); (e2 = e1->e_next); (e1 = e2)) {
                    if (e2->e_what == x) {
                        e1->e_next = e2->e_next;
                        if (e2 == b->b_cached) { b->b_cached = e2->e_next; }
                        PD_MEMORY_FREE (e2); return PD_ERROR_NONE;
                    }
                }
            }
        }
    }
    
    return PD_ERROR;
}

void pd_unbindQuiet (t_pd *x, t_symbol *s)
{
    pd_unbindProceed (x, s);
}

void pd_unbind (t_pd *x, t_symbol *s)
{
    t_error err = pd_unbindProceed (x, s);
    
    PD_ASSERT (!err); PD_UNUSED (err);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void symbol_hasThingError (t_symbol *s, t_object *o)
{  
    if (s) {
    if (s != sym___hash__A) {
        error_noSuch (o, s, sym_object);
    }
    }
}

static int symbol_hasThingProceed (t_symbol *s, int withError, t_object *o)
{
    int k = 0;
    
    if (s && s->s_thing) {
    //
    if (pd_class (s->s_thing) == bindlist_class) { k = !bindlist_isEmpty ((t_bindlist *)s->s_thing); }
    else {
        k = 1;
    }
    //
    }
    
    if (withError && !k) { symbol_hasThingError (s, o); }
    
    return k;
}

int symbol_hasThing (t_symbol *s, t_object *o)
{
    return symbol_hasThingProceed (s, 1, o);
}

int symbol_hasThingQuiet (t_symbol *s)
{
    return symbol_hasThingProceed (s, 0, NULL);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_pd *symbol_getThingByClass (t_symbol *s, t_class *c)
{
    t_pd *x = NULL;
    
    if (!s->s_thing) { return NULL; }
    if (pd_class (s->s_thing) == c) { return s->s_thing; }
    
    if (pd_class (s->s_thing) == bindlist_class) {
        t_bindlist *b = (t_bindlist *)s->s_thing;
        t_bindelement *e = NULL;
        
        for (e = b->b_list; e; e = e->e_next) {
            if (*e->e_what == c) {
                if (x != NULL) { warning_multipleBinding (NULL, s); }
                x = e->e_what;
            }
        }
    }
    
    return x;
}

t_pd *symbol_getThing (t_symbol *s)
{
    if (symbol_hasThingQuiet (s)) { return s->s_thing; }
    else {
        return NULL;
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

const char *symbol_getName (t_symbol *s)
{
    return s->s_name;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
