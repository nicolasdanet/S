
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include <execinfo.h>

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if PD_WITH_DEBUG
    
void post_syslog (const char *s)
{
    #if PD_APPLE
        syslog (LOG_ERR, "%s", s);
    #else
        puts (s);
    #endif
}

void post_syslogFloat (double f)
{
    char t[LOGGER_FLOAT_STRING] = { 0 }; post_syslog (logger_stringWithFloat (t, f));
}

/* https://www.gnu.org/software/libc/manual/html_node/Backtraces.html */

void post_backtrace (void)
{
    void *t[128]  = { 0 };
    int i, size   = backtrace (t, 128);
    char **frames = backtrace_symbols (t, size);
    
    if (frames != NULL) {
        for (i = 0; i < size; i++) { post_syslog (frames[i]); }
    }

    free (frames);
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
