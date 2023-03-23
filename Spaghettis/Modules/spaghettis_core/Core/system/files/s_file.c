
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void searchpath_extendedMatchedAtIndex (int);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static int file_openRawNative (const char *filepath, int oflag)
{
    if (oflag & O_CREAT) { return open (filepath, oflag, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH); }
    else {
        return open (filepath, oflag);
    } 
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL int file_openWrite (const char *filepath)
{
    return file_openRawNative (filepath, O_CREAT | O_TRUNC | O_WRONLY);
}

PD_LOCAL int file_openRead (const char *filepath)
{
    if (!(path_isFileExist (filepath))) { return -1; }
    else if (!(path_isFileExistAsRegularFile (filepath))) { return -1; }
    else {
        return file_openRawNative (filepath, O_RDONLY);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL int file_openReadWithDirectoryAndName (const char *directory,
    const char *name, 
    const char *extension,
    t_fileproperties *p)
{
    int f = -1;
    t_error err = PD_ERROR_NONE;
    char t[PD_STRING] = { 0 };
    
    PD_ASSERT (directory);
    PD_ASSERT (name);
    PD_ASSERT (extension);
    
    fileproperties_clear (p);
    
    err |= path_withDirectoryAndName (t, PD_STRING, directory, name);
    err |= string_add (t, PD_STRING, extension);

    if (!err && (f = file_openRead (t)) >= 0) {
        if (fileproperties_set (p, t) == PD_ERROR_NONE) { return f; }
        else {
            close (f);
        }
    }
    
    return -1;
}

PD_LOCAL int file_openReadConsideringSearchPath (const char *directory, 
    const char *name, 
    const char *extension,
    t_fileproperties *p)
{
    /* Search first (and always) in directory provided (sibling files). */
    
    int f = file_openReadWithDirectoryAndName (directory, name, extension, p);
    int n = 0;
    
    /* For efficiency, test availability before to explore. */
    
    if (f < 0) {
        if (fileproperties_isExternal (p))    {
            if (!searchpath_isExternalAvailable (fileproperties_getKey (p)))    { return -1; }
        }
        if (fileproperties_isAbstraction (p)) {
            if (!searchpath_isAbstractionAvailable (fileproperties_getKey (p))) { return -1; }
        }
    }
    
    /* At last look for in trees. */
    
    if (f < 0) {
        t_pathlist *l = searchpath_getExtended();
        while (l) {
            const char *path = pathlist_getPath (l);
            l = pathlist_getNext (l);
            f = file_openReadWithDirectoryAndName (path, name, extension, p);
            if (f >= 0) { searchpath_extendedMatchedAtIndex (n); break; }
            n++;
        }
        
        if (f < 0) { warning_missingInSearchPath (NULL); }
    }
    
    return f;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if 0

PD_LOCAL t_error file_lock (int fd)
{
    struct flock lock;
    
    lock.l_start    = 0;
    lock.l_len      = 0;
    lock.l_type     = F_WRLCK;
    lock.l_whence   = SEEK_SET;
    
    PD_ASSERT (fd >= 0);
    
    return (fcntl (fd, F_SETLK, &lock) < 0);
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
