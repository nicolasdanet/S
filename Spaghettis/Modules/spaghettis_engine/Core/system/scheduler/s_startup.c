
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

extern t_symbol *main_directoryExecutable;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_pathlist *pendendfiles_list;       /* Static. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void startup_appendPendedFiles (char *path)
{
    pendendfiles_list = pathlist_newAppend (pendendfiles_list, NULL, path);
}

static void startup_openPendedFilesProceed (const char *path)
{
    char filepath[PD_STRING] = { 0 };
    
    t_error err = path_withDirectoryAndName (filepath,
                        PD_STRING,
                        symbol_getName (main_directoryExecutable),
                        path);
    
    if (!err && path_isFileExistAsRegularFile (filepath)) {
    //
    t_symbol *directory = &s_;
    t_symbol *name      = &s_;

    err = path_toDirectoryAndName (filepath, PD_STRING, &directory, &name);
    
    if (!err) {
        instance_patchOpen (name, directory);
    }
    //
    }
}

void startup_openPendedFiles (void)
{
    t_pathlist *l = pendendfiles_list;

    while (l) {
        const char *path = pathlist_getPath (l);
        l = pathlist_getNext (l);
        startup_openPendedFilesProceed (path);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void startup_release (void)
{
    pathlist_free (pendendfiles_list);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
