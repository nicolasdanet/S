
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define SEARCHPATH_LEVELS   10
#define SEARCHPATH_FDOPEN   15
#define SEARCHPATH_MAXIMUM  256

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_pathlist *searchpath_roots;            /* Static. */
static t_pathlist *searchpath_extended;         /* Static. */
static t_pathlist *searchpath_external;         /* Static. */
static t_pathlist *searchpath_patch;            /* Static. */
static t_pathlist *searchpath_help;             /* Static. */
static t_pathlist *searchpath_duplicates;       /* Static. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static int searchpath_countDirectories;         /* Static. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static int searchpath_scanProceed (const char *path, const struct stat *b, int flag, struct FTW *f)
{
    int abort = 0;
    
    if (flag == FTW_D) {
    //
    if (path_isValid (path)) {
    //
    abort |= (searchpath_countDirectories >= SEARCHPATH_MAXIMUM);
    abort |= (f->level > SEARCHPATH_LEVELS);
    
    if (!abort) {
        searchpath_countDirectories++;
        searchpath_extended = pathlist_newAppend (searchpath_extended, NULL, path);
    }
    //
    }
    //
    }
    
    if (flag == FTW_F) {
    //
    if (path_isValid (path)) {
    //
    const char *filename = path + f->base;
    
    if (string_endWith (path, PD_PLUGIN)) {
        searchpath_external = pathlist_newAppend (searchpath_external, &searchpath_duplicates, filename);
        
    } else if (string_endWith (path, PD_PATCH)) {
        searchpath_patch = pathlist_newAppend (searchpath_patch,       &searchpath_duplicates, filename);
        
    } else if (string_endWith (path, PD_HELP)) {
        searchpath_help = pathlist_newAppend (searchpath_help,         &searchpath_duplicates, filename);
    }
    //
    }
    //
    }
    
    if (abort) { return 1; } else { return scheduler_isExiting(); }
}

PD_LOCAL t_error searchpath_scan (void)
{
    t_error err = PD_ERROR_NONE;
    t_pathlist *l = searchpath_roots;
    
    pathlist_free (searchpath_extended);   searchpath_extended   = NULL;
    pathlist_free (searchpath_external);   searchpath_external   = NULL;
    pathlist_free (searchpath_patch);      searchpath_patch      = NULL;
    pathlist_free (searchpath_help);       searchpath_help       = NULL;
    pathlist_free (searchpath_duplicates); searchpath_duplicates = NULL;
    searchpath_countDirectories = 0;
    
    while (!err && l) {
    //
    char *path = pathlist_getPath (l); l = pathlist_getNext (l);
    
    if (path_isFileExist (path) && path_isValid (path)) {
        err |= (nftw (path, searchpath_scanProceed, SEARCHPATH_FDOPEN, FTW_MOUNT | FTW_PHYS) != 0);
    } else {
        post_warning (NULL, PD_TRANSLATE ("rescan: invalid %s"), path);
    }
    //
    }
    
    PD_ASSERT (!pathlist_check (searchpath_extended));
    PD_ASSERT (!pathlist_check (searchpath_external));
    PD_ASSERT (!pathlist_check (searchpath_patch));
    PD_ASSERT (!pathlist_check (searchpath_help));
    PD_ASSERT (!pathlist_check (searchpath_duplicates));
    
    return err;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if 0

extern t_symbol *main_directorySupport;

static void searchpath_reportWrite (int f, const char *prefix, const char *s)
{
    ssize_t w = 0;      /* Avoid unused return warning. */
    
    w += write (f, prefix, strlen (prefix));
    w += write (f, " ", 1);
    w += write (f, s, strlen (s));
    w += write (f, "\n", 1);

    (void)w;
}

static void searchpath_reportToLogFile (void)
{
    char t[PD_STRING] = { 0 };
    t_error err = string_sprintf (t, PD_STRING, "%s/scan-XXXXXX", main_directorySupport->s_name);
    int f;
    
    if (!err && ((f = mkstemp (t)) != -1)) {
    //
    t_pathlist *l = searchpath_extended;

    post_system (NULL, "rescan: dump %s", t);
    
    while (l) {
        const char *path = pathlist_getPath (l);
        l = pathlist_getNext (l);
        searchpath_reportWrite (f, "Path:", path);
    }
    
    l = searchpath_external;
    
    while (l) {
        const char *path = pathlist_getPath (l);
        l = pathlist_getNext (l);
        searchpath_reportWrite (f, "External:", path);
    }
    
    l = searchpath_patch;
    
    while (l) {
        const char *path = pathlist_getPath (l);
        l = pathlist_getNext (l);
        searchpath_reportWrite (f, "Patch:", path);
    }
    
    l = searchpath_help;
    
    while (l) {
        const char *path = pathlist_getPath (l);
        l = pathlist_getNext (l);
        searchpath_reportWrite (f, "Help:", path);
    }
    
    l = searchpath_duplicates;
    
    while (l) {
        const char *path = pathlist_getPath (l);
        l = pathlist_getNext (l);
        searchpath_reportWrite (f, "Duplicates:", path);
    }
    
    close (f);
    //
    }
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void searchpath_report (void)
{
    t_pathlist *l = searchpath_extended;
    
    while (l) {
        const char *path = pathlist_getPath (l);
        l = pathlist_getNext (l);
        post_system (NULL, PD_TRANSLATE ("rescan: %s"), path);
    }
    
    l = searchpath_external;
    
    while (l) {
        const char *path = pathlist_getPath (l);
        l = pathlist_getNext (l);
        post_system (NULL, PD_TRANSLATE ("rescan: external / %s"), path);
    }
    
    l = searchpath_patch;
    
    while (l) {
        const char *path = pathlist_getPath (l);
        l = pathlist_getNext (l);
        post_system (NULL, PD_TRANSLATE ("rescan: patch / %s"), path);
    }
    
    l = searchpath_help;
    
    while (l) {
        const char *path = pathlist_getPath (l);
        l = pathlist_getNext (l);
        post_system (NULL, PD_TRANSLATE ("rescan: help / %s"), path);
    }
    
    l = searchpath_duplicates;
    
    while (l) {
        const char *path = pathlist_getPath (l);
        l = pathlist_getNext (l);
        post_system (NULL, PD_TRANSLATE ("rescan: duplicates / %s"), path);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL int searchpath_isExternalAvailable (t_symbol *s)
{
    int available = 0; char t[PD_STRING] = { 0 };
    
    if (!string_sprintf (t, PD_STRING, "%s%s", s->s_name, PD_PLUGIN)) {
        available = pathlist_contains (searchpath_external, t);
    }
    
    return available;
}

PD_LOCAL int searchpath_isAbstractionAvailable (t_symbol *s)
{
    int available = 0; char t[PD_STRING] = { 0 };
    
    if (!string_sprintf (t, PD_STRING, "%s%s", s->s_name, PD_PATCH)) {
        available = pathlist_contains (searchpath_patch, t);
    }
    
    return available;
}

PD_LOCAL int searchpath_hasDuplicates (void)
{
    return (pathlist_getSize (searchpath_duplicates) != 0);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void searchpath_extendedMatchedAtIndex (int n)
{
    PD_ASSERT (searchpath_extended);
    
    /* Matching folders are moved to front. */
    
    if (n > 0) { searchpath_extended = pathlist_moveFront (searchpath_extended, n); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_APPLICATION )

PD_LOCAL void searchpath_clear (void)
{
    pathlist_free (searchpath_roots); searchpath_roots = NULL;
}

PD_LOCAL void searchpath_appendRoot (const char *filepath)
{
    searchpath_roots = pathlist_newAppend (searchpath_roots, NULL, filepath);
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PD_LOCAL t_pathlist *searchpath_getExtended (void)
{
    return searchpath_extended;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void searchpath_rescan (int logged)
{
    t_error err = searchpath_scan();
    
    if (logged > 0) { searchpath_report(); }
    if (searchpath_hasDuplicates()) { warning_containsDuplicates (NULL); }
    if (err) { error_searchPathOverflow (NULL); }
    else if (logged >= 0) {
        post_system (NULL, PD_TRANSLATE ("rescan: done"));
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void searchpath_release (void)
{
    PD_ASSERT (!pathlist_check (searchpath_roots));
    PD_ASSERT (!pathlist_check (searchpath_extended));
    PD_ASSERT (!pathlist_check (searchpath_external));
    PD_ASSERT (!pathlist_check (searchpath_patch));
    PD_ASSERT (!pathlist_check (searchpath_help));
    PD_ASSERT (!pathlist_check (searchpath_duplicates));
    
    pathlist_free (searchpath_roots);
    pathlist_free (searchpath_extended);
    pathlist_free (searchpath_external);
    pathlist_free (searchpath_patch);
    pathlist_free (searchpath_help);
    pathlist_free (searchpath_duplicates);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
