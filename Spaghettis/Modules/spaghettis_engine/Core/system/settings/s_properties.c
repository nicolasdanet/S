
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

extern t_symbol *main_filePreferences;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static char *properties_loadBuffer;     /* Static. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static FILE *properties_saveFile;       /* Static. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

t_error properties_loadBegin (void)
{
    t_error err = !path_isFileExist (symbol_getName (main_filePreferences));
    
    if (!err) {
    //
    int f;
    
    err |= ((f = file_openRead (symbol_getName (main_filePreferences))) < 0);
    
    if (!err) {
    //
    off_t length;
    
    err |= ((length = lseek (f, 0, SEEK_END)) < 0);
    err |= (lseek (f, 0, SEEK_SET) < 0); 
    
    if (!err) {
    //
    properties_loadBuffer = (char *)PD_MEMORY_GET ((size_t)length + 2);
    properties_loadBuffer[0] = '\n';
    err |= (read (f, properties_loadBuffer + 1, (size_t)length) < length);
    //
    }
    
    if (err) { properties_loadBuffer[0] = 0; PD_BUG; }
    
    close (f);
    //
    }
    //
    }
    
    return err;
}

void properties_loadClose (void)
{
    if (properties_loadBuffer) {
    //
    PD_MEMORY_FREE (properties_loadBuffer); properties_loadBuffer = NULL; 
    //
    }
}

t_error properties_saveBegin (void)
{
    int f = file_openWrite (symbol_getName (main_filePreferences));
    t_error err = (f < 0);
    if (!err) { err |= ((properties_saveFile = fdopen (f, "w")) == NULL); }
    
    return err;
}

void properties_saveClose (void)
{
    if (properties_saveFile) {
    //
    if (fclose (properties_saveFile) != 0) { PD_BUG; } properties_saveFile = NULL;
    //
    }
}

void properties_setKey (const char *key, const char *value)
{
    if (properties_saveFile) { fprintf (properties_saveFile, "%s: %s\n", key, value); }
}

int properties_getKey (const char *key, char *value, int size)
{
    char t[PD_STRING] = { 0 };
    char *p = NULL;
    char *pEnd = NULL;
    t_error err = string_sprintf (t, PD_STRING, "\n%s:", key);

    PD_ASSERT (properties_loadBuffer != NULL);
    PD_ASSERT (!err);
    PD_UNUSED (err);
    
    p = strstr (properties_loadBuffer, t);
    
    if (p) {
    //
    *value = 0; p += strlen (t);
    
    while (*p == ' ' || *p == '\t') { p++; }
    
    for (pEnd = p; *pEnd && *pEnd != '\n'; pEnd++) { } 
    
    if (*pEnd == '\n') { 
        pEnd--; 
    }
    
    size_t length = pEnd + 1 - p;
    
    if (length > 0) { if (!string_append (value, size, p, (int)length)) { return 1; } }
    //
    }
    
    return 0;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
