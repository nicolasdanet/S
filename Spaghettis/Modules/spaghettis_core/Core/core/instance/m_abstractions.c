
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* For now the memory is released only when the glist is freed. */

// TODO: Purge abstractions with polling mechanism?

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

struct _abstractions {
    t_buffer *x_keys;
    t_buffer *x_names;
    t_buffer *x_filenames;
    t_buffer *x_directories;
    t_buffer *x_snippets;
    };

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static int abstractions_fetch (t_abstractions *x, t_symbol *key)
{
    PD_ASSERT (pool_check (key));
    
    int i;
    
    for (i = 0; i < buffer_getSize (x->x_keys); i++) {
    //
    if (buffer_getSymbolAtIndex (x->x_keys, i) == key) { return i; }
    //
    }
    
    return -1;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_symbol *abstractions_cache (t_abstractions *x, t_glist *glist, t_symbol *name, t_buffer *b)
{
    t_environment *e = glist_getEnvironment (glist);
    
    t_symbol *key = pool_get();
    
    t_atom a; SET_BUFFER (&a, b);
    
    buffer_appendSymbol (x->x_keys,        key);
    buffer_appendSymbol (x->x_names,       name);
    buffer_appendSymbol (x->x_filenames,   environment_getFileName (e));
    buffer_appendSymbol (x->x_directories, environment_getDirectory (e));
    buffer_appendAtom (x->x_snippets,      &a);
    
    return key;
}

t_symbol *abstractions_getName (t_abstractions *x, t_symbol *key)
{
    int i = abstractions_fetch (x, key);
    
    if (i >= 0) { return buffer_getSymbolAtIndex (x->x_names, i); }
    
    return NULL;
}

t_buffer *abstractions_getSnippet (t_abstractions *x,
    t_symbol *key,
    t_symbol **filename,
    t_symbol **directory)
{
    int i = abstractions_fetch (x, key);
    
    if (i >= 0) {
    //
    (*filename)  = buffer_getSymbolAtIndex (x->x_filenames, i);
    (*directory) = buffer_getSymbolAtIndex (x->x_directories, i);
    
    return buffer_getBufferAt (x->x_snippets, i);
    //
    }
    
    (*filename)  = NULL;
    (*directory) = NULL;
    
    return NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void abstractions_clear (t_abstractions *x)
{
    int i;
    
    for (i = 0; i < buffer_getSize (x->x_keys); i++) {
    //
    pool_relinquish (buffer_getSymbolAtIndex (x->x_keys, i));
    //
    }
    
    for (i = 0; i < buffer_getSize (x->x_snippets); i++) {
    //
    buffer_free (buffer_getBufferAt (x->x_snippets, i));
    //
    }
    
    buffer_clear (x->x_snippets);
    buffer_clear (x->x_directories);
    buffer_clear (x->x_filenames);
    buffer_clear (x->x_names);
    buffer_clear (x->x_keys);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_abstractions *abstractions_new (void)
{
    t_abstractions *x = (t_abstractions *)PD_MEMORY_GET (sizeof (t_abstractions));
    
    x->x_keys        = buffer_new();
    x->x_names       = buffer_new();
    x->x_filenames   = buffer_new();
    x->x_directories = buffer_new();
    x->x_snippets    = buffer_new();
    
    return x;
}

void abstractions_free (t_abstractions *x)
{
    if (x) {
    //
    abstractions_clear (x);
    
    buffer_free (x->x_snippets);
    buffer_free (x->x_directories);
    buffer_free (x->x_filenames);
    buffer_free (x->x_names);
    buffer_free (x->x_keys);
    
    PD_MEMORY_FREE (x);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
