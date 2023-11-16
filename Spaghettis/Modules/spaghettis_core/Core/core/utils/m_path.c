
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

extern t_symbol *main_directorySupport;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int path_isFileExist (const char *filepath)
{
    return (access (filepath, F_OK) == 0);
}

int path_isFileExistAsRegularFile (const char *filepath)
{
    struct stat t; return ((stat (filepath, &t) == 0) && S_ISREG (t.st_mode));
}

int path_isFileExistAsDirectory (const char *filepath)
{
    struct stat t; return ((stat (filepath, &t) == 0) && S_ISDIR (t.st_mode));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static int path_containsHiddenDirectory (const char *filepath)
{
    return string_contains (filepath, "/.");
}

static int path_isInsideSupport (const char *filepath)
{
    return string_startWith (filepath, main_directorySupport->s_name);
}

int path_isValid (const char *filepath)
{
    if (path_containsHiddenDirectory (filepath) && !path_isInsideSupport (filepath)) { return 0; }
    
    return 1;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Note that it fails in case of missing directory in path. */

t_error path_createDirectory (const char *filepath)
{
    return (mkdir (filepath, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) != 0);
}

t_error path_createDirectoryIfNeeded (const char *filepath)
{
    t_error err = PD_ERROR_NONE;
    
    if (!path_isFileExistAsDirectory (filepath)) { err = path_createDirectory (filepath); }
    
    return err;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error path_withDirectoryAndName (char *dest, size_t size, const char *directory, const char *name)
{
    t_error err = PD_ERROR;
    
    PD_ASSERT (directory);
    PD_ASSERT (name);
    
    if (*name) {
    //
    int absolute = (!(*directory) || (name[0] == '/'));
    
    err = PD_ERROR_NONE;
    
    if (!absolute) {
        err |= string_add (dest, size, directory);
        err |= string_add (dest, size, "/");
    }
    
    err |= string_add (dest, size, name);
    //
    }
    
    return err;
}

t_error path_toDirectoryAndName (char *dest, size_t size, t_symbol **directory, t_symbol **name)
{
    char *t = NULL;
    
    if ((t = strrchr (dest, '/'))) {
    //
    *t = 0;
    
    *directory = gensym (dest);
    *name      = gensym (t + 1);
    
    *t = '/';
    
    return PD_ERROR_NONE;
    //
    }

    return PD_ERROR;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
