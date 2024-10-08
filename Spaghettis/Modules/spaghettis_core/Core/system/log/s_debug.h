
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#ifndef S_DEBUG_H_
#define S_DEBUG_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if PD_WITH_DEBUG

#define PD_DBG(s)           post_syslog (s)
#define PD_DBG_NUMBER(f)    post_syslogFloat ((double)(f))
#define PD_DBG_BACKTRACE    post_backtrace()

#else

#define PD_DBG(s)
#define PD_DBG_NUMBER(f)
#define PD_DBG_BACKTRACE

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void post_syslog        (const char *s);
void post_syslogFloat   (double f);
void post_backtrace     (void);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // S_DEBUG_H_
