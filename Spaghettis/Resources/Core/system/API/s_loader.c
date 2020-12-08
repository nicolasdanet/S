
/* Copyright (c) 1997-2020 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < http://duartes.org/gustavo/blog/post/anatomy-of-a-program-in-memory/ > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef void (*t_ctor) (t_symbol *s);
typedef void (*t_dtor) (void);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef void *t_handle;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _loaded {
    struct _loaded  *ll_next;
    t_symbol        *ll_name;
    t_handle        ll_handle;
    } t_loaded;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_loaded *loader_alreadyLoaded;      /* Static. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void loader_externalClose (t_handle handle, t_symbol *name);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static int loader_isAlreadyLoaded (t_symbol *name)
{
    t_loaded *l = NULL;
    
    for (l = loader_alreadyLoaded; l; l = l->ll_next) { if (l->ll_name == name) { return 1; } }
    
    return 0;
}

static void loader_addLoaded (t_symbol *name, t_handle handle)
{
    t_loaded *l = (t_loaded *)PD_MEMORY_GET (sizeof (t_loaded));
    
    l->ll_next   = loader_alreadyLoaded;
    l->ll_name   = name;
    l->ll_handle = handle;
    
    loader_alreadyLoaded = l;
}

static t_error loader_makeStubName (char *dest, size_t size, t_symbol *name, const char *suffix)
{
    t_error err = PD_ERROR_NONE;
    const char *n = name->s_name;

    while (*n && !err) {
    //
    char c = *n;
    if (char_isAlphanumericOrUnderscore (c)) { err |= string_append (dest, size, n, 1);  }
    else if ((c == '~') && (*(n + 1) == 0)) { err |= string_add (dest, size, "_tilde"); }
    else {
        err |= string_add (dest, size, "_");
    }
    n++;
    //
    }
    
    err |= string_add (dest, size, suffix);
    
    return err;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_handle loader_externalOpenNative (char *filepath, char *stub, t_symbol *root)
{
    t_handle handle = dlopen (filepath, RTLD_NOW | RTLD_GLOBAL);
    
    if (!handle) { error__error (filepath, dlerror()); }
    else {
        t_ctor ctor = (t_ctor)dlsym (handle, stub);
        
        if (!ctor) { error_stubNotFound(); }
        else {
            (*ctor) (root); return handle;
        }
    }
    
    if (handle) { loader_externalClose (handle, NULL); }
    
    return NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static void loader_externalCloseNative (t_handle handle, char *stub)
{
    t_dtor dtor = (t_dtor)dlsym (handle, stub);
    
    if (dtor) { (*dtor)(); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static int loader_externalOpen (t_glist *glist, t_symbol *name)
{
    t_handle handle = NULL;
    
    PD_ASSERT (strrchr (name->s_name, '/') == NULL); 
    
    if (loader_isAlreadyLoaded (name)) { return 1; }
    else {
    //
    t_fileproperties p; fileproperties_initExternal (&p, name);

    if (glist_fileExist (glist, name->s_name, PD_PLUGIN, &p)) {
    //
    char filepath[PD_STRING] = { 0 };
    
    char *filename  = fileproperties_getName (&p);
    char *directory = fileproperties_getDirectory (&p);

    if (!path_withDirectoryAndName (filepath, PD_STRING, directory, filename)) {
        char stub[PD_STRING] = { 0 };
        t_error err = loader_makeStubName (stub, PD_STRING, name, "_setup");
        if (!err && (handle = loader_externalOpenNative (filepath, stub, gensym (directory)))) {
            loader_addLoaded (name, handle);
        }
    }
    //
    }
    //
    }
    
    return (handle != NULL);
}

static void loader_externalClose (t_handle handle, t_symbol *name)
{
    if (name) {
    //
    char stub[PD_STRING] = { 0 };
            
    if (!(loader_makeStubName (stub, PD_STRING, name, "_destroy"))) { 
        loader_externalCloseNative (handle, stub); 
    }
    //
    }
    
    dlclose (handle);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* An anything object would by-pass all the load mechanism for abstractions and exterals. */

PD_LOCAL int loader_load (t_glist *glist, t_symbol *name)
{
    if (name == &s_anything) { return 0; } else { return loader_externalOpen (glist, name); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void loader_release (void)
{
    t_loaded *l = loader_alreadyLoaded;

    while (l) {
    //
    t_loaded *next = l->ll_next;
    loader_externalClose (l->ll_handle, l->ll_name);
    PD_MEMORY_FREE (l);
    l = next; 
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
