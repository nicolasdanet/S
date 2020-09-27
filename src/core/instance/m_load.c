
/* Copyright (c) 1997-2020 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* A patch is bind to a context (the symbol #X). */
/* Most of the lines of a file can be considered as messages to the current context. */
/* Nested patch are handled with a stack mechanism. */
/* Contexts are pushed and popped to go down and up in the tree. */
/* Note that abstractions cannot be recursively instantiated. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void eval_buffer       (t_buffer *, t_pd *, int, t_atom *);
PD_LOCAL void eval_fileByBuffer (t_symbol *, t_symbol *, t_buffer *);
PD_LOCAL void eval_file         (t_symbol *, t_symbol *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void instance_contextStore (void)
{
    instance_get()->pd_stack.s_contextCached = instance_contextGetCurrent();
}

static void instance_contextRestore (void)
{
    instance_contextSetCurrent (instance_get()->pd_stack.s_contextCached);
    
    instance_get()->pd_stack.s_contextCached = NULL;
}

static t_glist *instance_contextGetStored (void)
{
    return instance_get()->pd_stack.s_contextCached;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static int instance_loadAbstractionIsValid (t_symbol *filename)
{
    int i;
    
    for (i = 0; i < instance_get()->pd_stack.s_stackIndex; i++) {
    //
    t_stackelement *e = instance_get()->pd_stack.s_stack + i;
    if (e->s_loadedAbstraction == filename) { return 0; }
    //
    }
    
    instance_get()->pd_loadingAbstraction = filename;
    
    return 1;
}

static void instance_loadAbstractionProceed (t_symbol *filename,
    t_symbol *directory,
    int argc,
    t_atom *argv,
    t_buffer *b)
{
    if (instance_loadAbstractionIsValid (filename)) {
    //
    instance_environmentSetArguments (argc, argv);
    
    if (b) { eval_fileByBuffer (filename, directory, b); } else { eval_file (filename, directory); }
    
    if (instance_contextGetCurrent() != instance_contextGetStored()) {
    
        instance_objectSetNewest (cast_pd (instance_contextGetCurrent()));
        instance_stackPopPatch (instance_contextGetCurrent());
    }
    
    instance_environmentResetArguments();
    //
    } else {
        error_recursiveInstantiation (filename);
    }
}

static void instance_loadAbstractionFile (t_symbol *name, int argc, t_atom *argv)
{
    t_fileproperties p; fileproperties_initAbstraction (&p, name);
    
    if (glist_fileExist (instance_contextGetCurrent(), name->s_name, PD_PATCH, &p)) {
    //
    t_symbol *filename  = gensym (fileproperties_getName (&p));
    t_symbol *directory = gensym (fileproperties_getDirectory (&p));
    
    instance_loadAbstractionProceed (filename, directory, argc, argv, NULL);
    //
    }
}

static t_error instance_loadAbstractionSnippet (t_symbol *key, int argc, t_atom *argv)
{
    t_abstractions *t   = glist_getAbstractions (instance_contextGetCurrent());
    t_symbol *filename  = NULL;
    t_symbol *directory = NULL;
    t_buffer *snippet   = abstractions_getSnippet (t, key, &filename, &directory);
    
    if (snippet) {
    //
    instance_loadAbstractionProceed (filename, directory, argc, argv, snippet);
    
    return PD_ERROR_NONE;
    //
    }
    
    return PD_ERROR;
}

PD_LOCAL void instance_loadAbstraction (t_symbol *name, int argc, t_atom *argv)
{
    if (pool_check (name)) {    /* Abstraction cached (e.g. encapsulation)? */
    //
    if (instance_loadAbstractionSnippet (name, argc, argv) == PD_ERROR_NONE) { return; }
    //
    }
    
    instance_loadAbstractionFile (name, argc, argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void instance_loadPatchLoadbang (void)
{
    if (instance_get()->pd_stack.s_contextPopped) {
        glist_loadbang (instance_get()->pd_stack.s_contextPopped);
        instance_get()->pd_stack.s_contextPopped = NULL;
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static int instance_loadPatch (t_symbol *name, t_symbol *directory)
{
    int done = 0;
    
    instance_contextStore();
    instance_contextSetCurrent (NULL);          /* The root patch do NOT have parent. */
        
    eval_file (name, directory);
    
    if (instance_contextGetCurrent() != NULL) {
    //
    t_glist *glist = instance_contextGetCurrent();
    
    instance_stackPopPatch (glist);
    
    done = 1;
    //
    }
    
    PD_ASSERT (instance_contextGetCurrent() == NULL);
    
    instance_loadPatchLoadbang();
    instance_contextRestore();
    
    return done;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Context of the stack temporary bypassed to eval the buffer. */

PD_LOCAL void instance_loadSnippet (t_glist *glist, t_buffer *b)
{
    instance_contextStore();
    instance_contextSetCurrent (glist);
    eval_buffer (b, NULL, 0, NULL);
    instance_contextRestore();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_FORCE void instance_patchNew (t_symbol *name, t_symbol *directory)
{
    instance_environmentSetFile (name, directory);
    
    glist_newPatchPop (&s_, NULL, 1, 0);
    
    instance_environmentResetFile();
}

PD_LOCAL t_error instance_patchOpen (t_symbol *name, t_symbol *directory)
{
    int state = dsp_suspend();
    int done  = instance_loadPatch (name, directory);
    
    dsp_resume (state);
    
    return (done != 1);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
