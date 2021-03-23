
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

t_class *bindlist_class;                    /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _bindelement {
    t_pd                    *e_what;        /* MUST be the first. */
    struct _bindelement     *e_next;
    } t_bindelement;

typedef struct _bindlist {
    t_pd                    b_pd;           /* MUST be the first. */
    int                     b_used;
    t_bindelement           *b_cached;
    t_bindelement           *b_list;
    } t_bindlist;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Note that binding or/and unbinding might be possible while broadcasting. */
/* It means therefore that a receiver can unbound itself also during the sender call. */
/* A bindlist is created to manage multiple binds. */
/* That bindlist remains until the end. */
/* Note that traversal is NOT reentrant. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_bindelement *bindlist_traverseStart (t_bindlist *x)
{
    if (x->b_used) { error_recursiveCall(); return NULL; }
    else {
    //
    x->b_used = 1; x->b_cached = x->b_list ? x->b_list->e_next : NULL;
    
    return x->b_list;
    //
    }
}

static t_bindelement *bindlist_traverseNext (t_bindlist *x)
{
    t_bindelement *e = x->b_cached;
    
    if (e) { x->b_cached = e->e_next; }
    
    return e;
}

static void bindlist_traverseEnd (t_bindlist *x)
{
    x->b_used = 0;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static int bindlist_isEmpty (t_bindlist *x)
{
    return (x->b_list == NULL);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void bindlist_bang (t_bindlist *x)
{
    t_bindelement *e = bindlist_traverseStart (x);
    
    while (e) { pd_bang (e->e_what); e = bindlist_traverseNext (x); }
    
    bindlist_traverseEnd (x);
}

static void bindlist_float (t_bindlist *x, t_float f)
{
    t_bindelement *e = bindlist_traverseStart (x);
    
    while (e) { pd_float (e->e_what, f); e = bindlist_traverseNext (x); }
    
    bindlist_traverseEnd (x);
}

static void bindlist_symbol (t_bindlist *x, t_symbol *s)
{
    t_bindelement *e = bindlist_traverseStart (x);
    
    while (e) { pd_symbol (e->e_what, s); e = bindlist_traverseNext (x); }
    
    bindlist_traverseEnd (x);
}

static void bindlist_list (t_bindlist *x, t_symbol *s, int argc, t_atom *argv)
{
    t_bindelement *e = bindlist_traverseStart (x);
    
    while (e) { pd_list (e->e_what, argc, argv); e = bindlist_traverseNext (x); }
    
    bindlist_traverseEnd (x);
}

static void bindlist_anything (t_bindlist *x, t_symbol *s, int argc, t_atom *argv)
{
    t_bindelement *e = bindlist_traverseStart (x);
    
    while (e) { pd_message (e->e_what, s, argc, argv); e = bindlist_traverseNext (x); }
    
    bindlist_traverseEnd (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void bindlist_free (t_bindlist *x)
{
    PD_ASSERT (bindlist_isEmpty (x));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void bindlist_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_bindlist,
            NULL,
            (t_method)bindlist_free,
            sizeof (t_bindlist),
            CLASS_INVISIBLE,
            A_NULL);
    
    class_addBang (c, (t_method)bindlist_bang);
    class_addFloat (c, (t_method)bindlist_float);
    class_addSymbol (c, (t_method)bindlist_symbol);
    class_addList (c, (t_method)bindlist_list);
    class_addAnything (c, (t_method)bindlist_anything);
    
    bindlist_class = c;
}

PD_LOCAL void bindlist_destroy (void)
{
    class_free (bindlist_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void pd_bind (t_pd *x, t_symbol *s)
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

PD_LOCAL void pd_unbindQuiet (t_pd *x, t_symbol *s)
{
    pd_unbindProceed (x, s);
}

PD_LOCAL void pd_unbind (t_pd *x, t_symbol *s)
{
    t_error err = pd_unbindProceed (x, s);
    
    PD_ASSERT (!err); PD_UNUSED (err);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void symbol_hasThingError (t_symbol *s)
{  
    if (s) {
    if (s != sym___hash__A) {
        error_noSuch (s, sym_object);
    }
    }
}

static int symbol_hasThingProceed (t_symbol *s, int withError)
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
    
    if (withError && !k) { symbol_hasThingError (s); }
    
    return k;
}

PD_LOCAL int symbol_hasThingQuiet (t_symbol *s)
{
    return symbol_hasThingProceed (s, 0);
}

PD_LOCAL int symbol_hasThing (t_symbol *s)
{
    return symbol_hasThingProceed (s, 1);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_pd *symbol_getThingByClass (t_symbol *s, t_class *c)
{
    t_pd *x = NULL;
    
    if (!s->s_thing) { return NULL; }
    if (pd_class (s->s_thing) == c) { return s->s_thing; }
    
    if (pd_class (s->s_thing) == bindlist_class) {
        t_bindlist *b = (t_bindlist *)s->s_thing;
        t_bindelement *e = NULL;
        
        for (e = b->b_list; e; e = e->e_next) {
            if (*e->e_what == c) {
                if (x != NULL) { warning_multipleBinding (s); }
                x = e->e_what;
            }
        }
    }
    
    return x;
}

PD_LOCAL t_pd *symbol_getThing (t_symbol *s)
{
    if (symbol_hasThingQuiet (s)) { return s->s_thing; }
    else {
        return NULL;
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL const char *symbol_getName (t_symbol *s)
{
    return s->s_name;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
