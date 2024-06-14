
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

t_class *bindlist_class;                    /* Shared. */

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

static t_bindelement *bindlist_traverseStart (t_bindlist *x)
{
    if (x->b_used) { return NULL; }
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

int bindlist_isEmpty (t_bindlist *x)
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
