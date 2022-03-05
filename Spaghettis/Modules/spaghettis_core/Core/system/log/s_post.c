
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
#define POST_SYSTEM     Logger::Type::system
#define POST_WARNING    Logger::Type::warning
#define POST_ERROR      Logger::Type::error

#else

#define POST_NORMAL     0
#define POST_SYSTEM     1
#define POST_WARNING    2
#define POST_ERROR      3

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if PD_WITH_DEBUG
    
PD_LOCAL void post_syslog (const char *s)
{
    #if PD_APPLE
        syslog (LOG_ERR, "%s", s);
    #else
        puts (s);
    #endif
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_APPLICATION )

extern Wrapper *main_wrapper;

static void post_toUpperCase (int k, char *s, Logger::Type type)
{
    if (type != Logger::Type::normal) {
    if (k) {
    if (char_isAlphabeticLowercase (*s)) { *s = toupper (*s); }
    }
    }
}

static void post_console (t_object *o, int k, char *s, Logger::Type type)
{
    jassert (main_wrapper != nullptr);
    
    if (k < 0 || k >= PD_STRING) { warning_tooManyCharacters (o, sym_console); }
    else {
    //
    post_toUpperCase (k, s, type);
    
    UniquePath u = o ?
        UniquePath (o, instance_registerGetOwner (object_getUnique (o))) :
        UniquePath::invalid();
    
    main_wrapper->post (juce::String (s), type, u);
    //
    }
}

#else

static void post_console (t_object *dummy, int k, const char *s, int type)
{
    #if PD_WITH_DEBUG
    
    if (k < 0 || k >= PD_STRING) { warning_tooManyCharacters (dummy, sym_console); }
    else {
        post_syslog (s);
    }
    
    #endif
}

#endif // PD_BUILDING_APPLICATION

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void post (t_object *x, const char *fmt, ...)
{
    int k; char t[PD_STRING] = { 0 }; va_list ap;
    
    va_start (ap, fmt);
    k = vsnprintf (t, PD_STRING, fmt, ap);
    va_end (ap);
    
    post_console (x, k, t, POST_NORMAL);
}

PD_LOCAL void post_system (t_object *x, const char *fmt, ...)
{
    int k; char t[PD_STRING] = { 0 }; va_list ap;
    
    va_start (ap, fmt);
    k = vsnprintf (t, PD_STRING, fmt, ap);
    va_end (ap);
    
    post_console (x, k, t, POST_SYSTEM);
}

PD_LOCAL void post_warning (t_object *x, const char *fmt, ...)
{
    int k; char t[PD_STRING] = { 0 }; va_list ap;
    
    va_start (ap, fmt);
    k = vsnprintf (t, PD_STRING, fmt, ap);
    va_end (ap);
    
    post_console (x, k, t, POST_WARNING);
}

PD_LOCAL void post_error (t_object *x, const char *fmt, ...)
{
    int k; char t[PD_STRING] = { 0 }; va_list ap;
    
    va_start (ap, fmt);
    k = vsnprintf (t, PD_STRING, fmt, ap);
    va_end (ap);
    
    post_console (x, k, t, POST_ERROR);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

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
