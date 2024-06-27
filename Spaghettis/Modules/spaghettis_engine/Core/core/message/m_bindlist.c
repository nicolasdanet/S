
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

t_class *bindlist_class;        /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void bindlist_create (t_symbol *s, t_pd *x)
{
    t_bindlist *b    = (t_bindlist *)pd_new (bindlist_class);
    t_bindelement *e = (t_bindelement *)PD_MEMORY_GET (sizeof (t_bindelement));
    
    e->e_what  = x;
    e->e_next  = NULL;
    b->b_list  = e;
    
    s->s_thing = &b->b_pd;
}

static void bindlist_prepend (t_symbol *s, t_pd *x)
{
    PD_ASSERT (pd_class (s->s_thing) == bindlist_class);
    
    t_bindlist *b    = (t_bindlist *)s->s_thing;
    t_bindelement *e = (t_bindelement *)PD_MEMORY_GET (sizeof (t_bindelement));
    
    e->e_what = x;
    e->e_next = b->b_list;
    b->b_list = e;
}

void bindlist_add (t_symbol *s, t_pd *x)
{
    if (s->s_thing) { bindlist_prepend (s, x); }
    else {
        bindlist_create (s, x);
    }
}

t_error bindlist_remove (t_symbol *s, t_pd *x)
{
    if (s->s_thing) {
    //
    PD_ASSERT (pd_class (s->s_thing) == bindlist_class);
        
    t_bindlist *b     = (t_bindlist *)s->s_thing;
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
    //
    }
    
    return PD_ERROR;
}

int bindlist_hasThing (t_symbol *s)
{
    PD_ASSERT (pd_class (s->s_thing) == bindlist_class);
    
    t_bindlist *b = (t_bindlist *)s->s_thing;
    
    return (b->b_list != NULL);
}

t_pd *bindlist_getThingByClass (t_symbol *s, t_class *c)
{
    PD_ASSERT (pd_class (s->s_thing) == bindlist_class);
    
    t_pd *x = NULL;
    
    t_bindlist *b    = (t_bindlist *)s->s_thing;
    t_bindelement *e = NULL;
        
    for (e = b->b_list; e; e = e->e_next) {
        if (*e->e_what == c) {
            if (x != NULL) { warning_multipleBinding (NULL, s); }
            x = e->e_what;
        }
    }
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Note that binding or/and unbinding might be possible while broadcasting. */
/* It means therefore that a receiver can unbound itself also during the sender call. */
/* A bindlist is created to manage multiple binds. */
/* That bindlist remains until the end. */
/* Note that traversal is NOT reentrant. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static int bindlist_traverseCheck (t_bindlist *x)
{
    return (x->b_used == 0);
}

static t_bindelement *bindlist_traverseStart (t_bindlist *x)
{
    x->b_used = 1; x->b_cached = x->b_list ? x->b_list->e_next : NULL;
    
    return x->b_list;
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

static void bindlist_bang (t_bindlist *x)
{
    if (bindlist_traverseCheck (x)) {
    //
    t_bindelement *e = bindlist_traverseStart (x);
    
    while (e) { pd_bang (e->e_what); e = bindlist_traverseNext (x); }
    
    bindlist_traverseEnd (x);
    //
    }
}

static void bindlist_float (t_bindlist *x, t_float f)
{
    if (bindlist_traverseCheck (x)) {
    //
    t_bindelement *e = bindlist_traverseStart (x);
    
    while (e) { pd_float (e->e_what, f); e = bindlist_traverseNext (x); }
    
    bindlist_traverseEnd (x);
    //
    }
}

static void bindlist_symbol (t_bindlist *x, t_symbol *s)
{
    if (bindlist_traverseCheck (x)) {
    //
    t_bindelement *e = bindlist_traverseStart (x);
    
    while (e) { pd_symbol (e->e_what, s); e = bindlist_traverseNext (x); }
    
    bindlist_traverseEnd (x);
    //
    }
}

static void bindlist_list (t_bindlist *x, t_symbol *s, int argc, t_atom *argv)
{
    if (bindlist_traverseCheck (x)) {
    //
    t_bindelement *e = bindlist_traverseStart (x);
    
    while (e) { pd_list (e->e_what, argc, argv); e = bindlist_traverseNext (x); }
    
    bindlist_traverseEnd (x);
    //
    }
}

static void bindlist_anything (t_bindlist *x, t_symbol *s, int argc, t_atom *argv)
{
    if (bindlist_traverseCheck (x)) {
    //
    t_bindelement *e = bindlist_traverseStart (x);
    
    while (e) { pd_message (e->e_what, s, argc, argv); e = bindlist_traverseNext (x); }
    
    bindlist_traverseEnd (x);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void bindlist_free (t_bindlist *x)
{
    PD_ASSERT (x->b_list == NULL);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void bindlist_setup (void)
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

void bindlist_destroy (void)
{
    class_free (bindlist_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
