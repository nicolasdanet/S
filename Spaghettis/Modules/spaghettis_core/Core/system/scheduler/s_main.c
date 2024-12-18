
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_symbol    *main_directoryExecutable;          /* Static. */
t_symbol    *main_directoryHelp;                /* Static. */
t_symbol    *main_directorySupport;             /* Static. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static int  main_version;                       /* Static. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

int  main_argc;                                 /* Static. */

char **main_argv;                               /* Static. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

t_error    audio_initialize            (void);
void       audio_release               (void);
void       message_initialize          (void);
void       message_release             (void);
void       midi_initialize             (void);
void       midi_release                (void);
void       setup_initialize            (void);
void       setup_release               (void);
void       startup_release             (void);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if defined ( PD_BUILDING_TERMINAL )

void       sys_setSignalHandlers       (void);

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void       startup_appendPendedFiles   (char *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* < https://stackoverflow.com/questions/1023306/finding-current-executables-path-without-proc-self-exe > */
/* < https://stackoverflow.com/questions/933850/how-to-find-the-location-of-the-executable-in-c > */

#if PD_APPLE

static t_error main_getExecutablePathNative (char *dest, size_t length)
{
    t_error err = PD_ERROR_NONE;

    char path[PATH_MAX];
    uint32_t size = sizeof (path);

    err = (_NSGetExecutablePath (path, &size) != 0);
    
    if (!err) {
        char *s = NULL;
        if ((s = realpath (path, NULL))) { err |= string_copy (dest, length, s); free (s); }
    }

    return err;
}

#endif // PD_APPLE

#if PD_LINUX

static t_error main_getExecutablePathNative (char *dest, size_t length)
{
    t_error err = PD_ERROR_NONE;
    
    char path[PATH_MAX];
    
    ssize_t t = readlink ("/proc/self/exe", path, PATH_MAX);
    
    if (!(err = (t < 0 || t >= PATH_MAX))) {
        char *s = NULL;
        path[t] = 0;
        if ((s = realpath (path, NULL))) { err |= string_copy (dest, length, s); free (s); }
    }
    
    return err;
}

#endif // PD_LINUX

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_error main_entryVersion (int sendToConsole)
{
    char t[PD_STRING] = { 0 }; t_error err = utils_version (t, PD_STRING);
    
    if (!err) {
        if (sendToConsole) { post_system (NULL, "%s", t); }
        else {
            fprintf (stdout, "%s\n", t);
        }
    }
    
    return err;
}

static t_error main_getParentDirectoryOf (char *t)
{
    t_error err = PD_ERROR_NONE;
    
    char *slash = NULL;

    if (!(err |= !(slash = strrchr (t, '/')))) { *slash = 0; }
    
    return err;
}

/* Name of the executable's parent directory. */

static t_error main_setPathExecutable (void)
{
    t_error err = PD_ERROR_NONE; char t[PD_STRING] = { 0 };
    
    err |= main_getExecutablePathNative (t, PD_STRING);

    if (!err) {
    //
    err |= main_getParentDirectoryOf (t);
    
    if (!err) { main_directoryExecutable = gensym (t); }
    //
    }
    
    return err;
}

static t_error main_setPathSupport (void)
{
    t_error err = PD_ERROR_NONE; char t[PD_STRING] = { 0 };
    
    const char *home = getenv ("HOME");
    
    err |= (home == NULL);
    
    if (!err) {
    //
    #if PD_APPLE
    
    err |= string_sprintf (t, PD_STRING, "%s/Library/Application Support/" PD_NAME, home);
    
    #else
    
    err |= string_sprintf (t, PD_STRING, "%s/.config", home);
    
    if (!err) { err |= path_createDirectoryIfNeeded (t); }
    
    err |= string_sprintf (t, PD_STRING, "%s/.config/" PD_NAME_LOWERCASE, home);
    
    #endif
    
    if (!err) { err |= path_createDirectoryIfNeeded (t); }
    
    if (!err) { main_directorySupport = gensym (t); }
    //
    }
    
    return err;
}

#if defined ( PD_BUILDING_APPLICATION )

static t_error main_setPathHelp()
{
    #if PD_APPLE
        const char *name = "Resources/Help";
    #else
        const char *name = "Help";
    #endif
    
    char t[PD_STRING]        = { 0 };
    char filepath[PD_STRING] = { 0 };
    
    t_error err = string_copy (t, PD_STRING, symbol_getName (main_directoryExecutable));

    if (!err) {
    //
    #if PD_APPLE
        err |= main_getParentDirectoryOf (t);
    #endif
    
    if (!err && !path_withDirectoryAndName (filepath, PD_STRING, t, name)) {
        if (path_isFileExistAsDirectory (filepath)) {
            main_directoryHelp = gensym (filepath);
        }
    }
    //
    }
    
    PD_ASSERT (main_directoryHelp != NULL);
    
    return PD_ERROR_NONE;
}

#else

static t_error main_setPathHelp()
{
    return PD_ERROR_NONE;
}

#endif

static t_error main_parseArguments (int argc, char **argv)
{
    t_error err = PD_ERROR_NONE;
    
    if (argc < 0) { argc = 0; PD_BUG; }
    
    while (!err && (argc > 0) && (**argv == '-')) {
    //
    if (!strcmp (*argv, "--version")) {
        #if defined ( PD_BUILDING_TERMINAL )
            main_version = 1;
        #endif
        argc--; argv++;
    } else {
        err = PD_ERROR;
    }
    //
    }
    
    while (!err && (argc > 0)) {
        if (string_endWith (*argv, PD_PATCH))       { startup_appendPendedFiles (*argv); }
        else if (string_endWith (*argv, PD_LEGACY)) { startup_appendPendedFiles (*argv); }
        argc--; argv++;
    }
    
    if (err) {
    //
    fprintf (stderr, "Usage: " PD_NAME_LOWERCASE " [ --version ] [ settings ] [ files ... ]\n");
    //
    }
    
    return err;
}

/* Create and start the DSP thread. */

static t_error main_create (void)
{
    t_error err = PD_ERROR_NONE;
    
    if (!(err |= privilege_restore())) {
        err |= instance_dspCreate();
        err |= privilege_relinquish();
    }
    
    PD_ASSERT (!err);
    
    return err;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Note that order of calls below may be critical. */

int main_start (void)
{
    t_error err = privilege_start();
    
    if (!err && !(err = privilege_drop())) {
    //
    PD_ASSERT (sys_isControlThread());
    
    denormal_setPolicy();
    
    #if defined ( PD_BUILDING_TERMINAL )
        sys_setSignalHandlers();
    #endif
    
    #if PD_WITH_DEBUG
        leak_initialize();
    #endif
    
    message_initialize();   /* Preallocate symbols and binding mechanism first. */
    
    /* Paths initialization order matters. */
    
    err |= main_setPathExecutable();    PD_ASSERT (main_directoryExecutable != NULL);
    err |= main_setPathSupport();       PD_ASSERT (main_directorySupport    != NULL);
    err |= main_setPathHelp();
        
    err |= main_parseArguments (main_argc - 1, main_argv + 1);

    if (!err) {
    //
    if (main_version) { err |= main_entryVersion (0); }
    else {
    //
    #if defined ( PD_BUILDING_APPLICATION )
        err |= main_entryVersion (1);
    #endif
    
    if (!err) {
    //
    err |= logger_initialize();

    if (!err) {
    //
    err |= audio_initialize();
    
    if (!err) {
    //
    midi_initialize();
    setup_initialize();     /* Instance initialized. */
        
    if (!(err |= main_create())) { err |= scheduler_main(); }
    
    setup_release();        /* Instance released. */
    midi_release();
    audio_release(); 
    //
    }
    
    logger_release();
    //
    }
    //
    }
    //
    }
    //
    }
    
    startup_release();
    message_release();
    
    #if PD_WITH_DEBUG
        leak_release(); post_log ("Shutdown");
    #endif
    //
    }
    
    return err;
}

void main_exit (void)
{
    scheduler_needToExit();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_TESTS )

#if PD_WITH_DEBUG
    #error
#endif

t_error main_begin()
{
    t_error err = privilege_start();
    
    if (!err && !(err = privilege_drop())) {
    //
    denormal_setPolicy();
    message_initialize();
    setup_initialize();
    //
    }
    
    return err;
}

void main_end()
{
    setup_release();
    message_release();
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
