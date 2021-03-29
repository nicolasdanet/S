
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if defined ( PD_BUILDING_APPLICATION )

#define POST_NORMAL     Logger::Type::normal
#define POST_WARNING    Logger::Type::warning
#define POST_ERROR      Logger::Type::error

#else

#define POST_NORMAL     0
#define POST_WARNING    1
#define POST_ERROR      2

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if PD_WITH_DEBUG
    
PD_FORCE void post_syslog (const char *s)
{
    #if PD_APPLE
        openlog (PD_NAME, LOG_CONS | LOG_PID, LOG_USER);
    #else
        openlog (PD_NAME, LOG_CONS | LOG_PID | LOG_PERROR, LOG_USER);
    #endif
    
    syslog (LOG_ERR, "%s", s);
    
    closelog();
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_APPLICATION )

Wrapper *main_wrapper;

static void post_console (int k, const char *s, Logger::Type type)
{
    jassert (main_wrapper != nullptr);
    
    if (k < 0 || k >= PD_STRING) { warning_tooManyCharacters (sym_console); }
    else {
        main_wrapper->post (juce::String (s), type);
    }
}

#else

static void post_console (int k, const char *s, int type)
{
    #if PD_WITH_DEBUG
    
    if (k < 0 || k >= PD_STRING) { warning_tooManyCharacters (sym_console); }
    else {
        post_syslog (s);
    }
    
    #endif
}

#endif // PD_BUILDING_APPLICATION

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Note that it is NOT thread-safe. */

PD_LOCAL void post (const char *fmt, ...)
{
    int k;
    char t[PD_STRING] = { 0 };
    va_list ap;
    
    PD_ASSERT (sys_isControlThread());
    
    va_start (ap, fmt);
    k = vsnprintf (t, PD_STRING, fmt, ap);
    va_end (ap);
    
    post_console (k, t, POST_NORMAL);
}

PD_LOCAL void post_warning (const char *fmt, ...)
{
    int k;
    char t[PD_STRING] = { 0 };
    va_list ap;
    
    PD_ASSERT (sys_isControlThread());
    
    va_start (ap, fmt);
    k = vsnprintf (t, PD_STRING, fmt, ap);
    va_end (ap);
    
    post_console (k, t, POST_WARNING);
}

PD_LOCAL void post_error (const char *fmt, ...)
{
    int k;
    char t[PD_STRING] = { 0 };
    va_list ap;
    
    PD_ASSERT (sys_isControlThread());
    
    va_start (ap, fmt);
    k = vsnprintf (t, PD_STRING, fmt, ap);
    va_end (ap);
    
    post_console (k, t, POST_ERROR);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Note that it is NOT thread-safe. */

#if PD_WITH_DEBUG

PD_LOCAL void post_log (const char *fmt, ...)
{
    int k;
    char t[PD_STRING] = { 0 };
    va_list ap;
    
    va_start (ap, fmt);
    k = vsnprintf (t, PD_STRING, fmt, ap);
    va_end (ap);
    
    if (k >= 0 && k < PD_STRING) {
        post_syslog (t);
    } else {
        post_syslog ("...");
    }
}

#else

PD_LOCAL void post_log (const char *fmt, ...)
{
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
