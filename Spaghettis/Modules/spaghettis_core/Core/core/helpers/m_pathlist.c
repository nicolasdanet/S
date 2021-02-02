
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define PATHREGISTER_DEFAULT_SIZE   1024

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_pathlist *pathlist_removeAtIndex (t_pathlist *, int);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Collection of hashes to accelerate fetching by testing existence first. */

static t_pathregister *pathregister_new (void)
{
    t_pathregister *x = (t_pathregister *)PD_MEMORY_GET (sizeof (t_pathregister));
    
    x->pr_size      = 0;
    x->pr_allocated = PATHREGISTER_DEFAULT_SIZE;
    x->pr_hashes    = (uint64_t *)PD_MEMORY_GET (sizeof (uint64_t) * x->pr_allocated);
    
    return x;
}

static void pathregister_free (t_pathregister *x)
{
    PD_MEMORY_FREE (x->pr_hashes);
    PD_MEMORY_FREE (x);
}

static int pathregister_size (t_pathregister *x)
{
    return x->pr_size;
}

static void pathregister_add (t_pathregister *x, uint64_t hash)
{
    if (x->pr_size >= x->pr_allocated) {
    //
    size_t oldSize = sizeof (uint64_t) * x->pr_allocated;
    size_t newSize = sizeof (uint64_t) * x->pr_allocated * 2;
    x->pr_hashes = (uint64_t *)PD_MEMORY_RESIZE (x->pr_hashes, oldSize, newSize);
    x->pr_allocated *= 2;
    //
    }
    
    x->pr_hashes[x->pr_size] = hash; x->pr_size++;
}

static void pathregister_remove (t_pathregister *x, uint64_t hash)
{
    if (x->pr_size) {
    //
    int i, j = -1;
    
    for (i = 0; i < x->pr_size; i++) {
        if (x->pr_hashes[i] == hash) {
            j = i; break;
        }
    }
    
    if (j != -1) {
        for (i = j; i < x->pr_size - 1; i++) { x->pr_hashes[i] = x->pr_hashes[i + 1]; }
        x->pr_size--;
    }
    //
    }
}

static int pathregister_contains (t_pathregister *x, uint64_t hash)
{
    int i, k = 0;
    
    for (i = 0; i < x->pr_size; i++) { if (x->pr_hashes[i] == hash) { k = 1; break; } }
    
    return k;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_pathlist *pathlist_newAppend (t_pathlist *x, t_pathlist **duplicates, const char *s)
{
    if (*s) {
    //
    int k = -1;
    uint64_t hash   = string_hash (s);
    t_pathlist *l1  = x;
    t_pathlist *l2  = NULL;
    
    l2 = (t_pathlist *)PD_MEMORY_GET (sizeof (t_pathlist));
    l2->pl_next     = NULL;
    l2->pl_register = l1 ? l1->pl_register : pathregister_new();
    l2->pl_string   = (char *)PD_MEMORY_GET (strlen (s) + 1);
    l2->pl_hash     = hash;
    
    strcpy (l2->pl_string, s);
    
    pathregister_add (l2->pl_register, hash);
    
    if (!l1) { return l2; }
    else {
    //
    int i = 0;
    
    do {
    //
    if (l1->pl_hash == l2->pl_hash) { k = i; } i++;
    //
    } while (l1->pl_next && (l1 = l1->pl_next));
        
    l1->pl_next = l2;
    //
    }
    
    /* Avoid duplicates. */
    
    if (k >= 0) {
        if (duplicates) { *duplicates = pathlist_newAppend (*duplicates, NULL, s); }
        x = pathlist_removeAtIndex (x, k);
    }
    //
    }
    
    return x;
}

PD_LOCAL void pathlist_free (t_pathlist *x)
{
    t_pathlist *l1 = NULL;
    t_pathlist *l2 = NULL;
    
    if (x) { pathregister_free (x->pl_register); }
    
    for (l1 = x; l1; l1 = l2) {
    //
    l2 = l1->pl_next;
    PD_MEMORY_FREE (l1->pl_string);
    PD_MEMORY_FREE (l1);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_pathlist *pathlist_moveFront (t_pathlist *x, int n)
{
    if (n > 0) {
    //
    t_pathlist *l1 = NULL;
    t_pathlist *l2 = NULL;
    
    int i = 0;
    
    for (l1 = x; l1; l1 = l2, i++) {
    
        l2 = l1->pl_next;
        
        if (i == (n - 1)) {
            PD_ASSERT (l1->pl_next);
            l1->pl_next = l2->pl_next;
            l2->pl_next = x;
            return l2;
        }
    }
    //
    }
    
    return x;
}

static t_pathlist *pathlist_removeAtIndex (t_pathlist *x, int n)
{
    PD_ASSERT (n >= 0);
    
    if (n == 0) { return pathlist_removeFirst (x); }
    else {
    //
    t_pathlist *l1 = NULL;
    t_pathlist *l2 = NULL;
    t_pathlist *t  = NULL;
    
    int i = 0;
    
    for (l1 = x; l1; l1 = l2, i++) {
        if (i == (n - 1)) {
            PD_ASSERT (l1->pl_next);
            t = l1->pl_next; l1->pl_next = l1->pl_next->pl_next;
            pathregister_remove (t->pl_register, t->pl_hash);
            PD_MEMORY_FREE (t->pl_string);
            PD_MEMORY_FREE (t);
            break;
        }
    
        l2 = l1->pl_next;
    }
    
    return x;
    //
    }
}

PD_LOCAL t_pathlist *pathlist_removeFirst (t_pathlist *x)
{
    t_pathlist *l = NULL;
    
    if (x) {
        if (!x->pl_next) { pathregister_free (x->pl_register); }
        else {
            pathregister_remove (x->pl_register, x->pl_hash);
        }
    }
    
    if (x) {
        l = x->pl_next;
        PD_MEMORY_FREE (x->pl_string);
        PD_MEMORY_FREE (x);
    }
    
    return l;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL int pathlist_getSize (t_pathlist *x)
{
    return x ? pathregister_size (x->pl_register) : 0;
}

PD_LOCAL int pathlist_contains (t_pathlist *x, const char *s)
{
    return x ? pathregister_contains (x->pl_register, string_hash (s)) : 0;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if PD_WITH_DEBUG

PD_LOCAL t_error pathlist_check (t_pathlist *x)
{
    t_error err = PD_ERROR_NONE;
    int n = 0;
    
    t_pathlist *l1 = NULL;
    t_pathlist *l2 = NULL;
    
    for (l1 = x; l1; l1 = l2) {
    //
    l2 = l1->pl_next;
    PD_ASSERT (l1->pl_register);
    err |= (pathregister_contains (l1->pl_register, l1->pl_hash) != 1);
    n++;
    //
    }
    
    if (n) { err |= (pathregister_size (x->pl_register) != n); }
    
    return err;
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL char *pathlist_getPath (t_pathlist *x)
{
    return (x ? x->pl_string : NULL);
}

PD_LOCAL t_pathlist *pathlist_getNext (t_pathlist *x)
{
    return (x ? x->pl_next : NULL);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
