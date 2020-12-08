
/* Copyright (c) 1997-2020 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef S_LOGGER_H_
#define S_LOGGER_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Non-blocking logger for low latency debugging. */
/* Handy to post small constant strings while developing DSP code. */
/* Note that only ONE thread might be safely logged at once. */
/* In practice with several it is rarely a problem. */
/* By the way probably not a good idea to use it in the release product. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if PD_WITH_DEBUG 
    #define PD_LOG(s)           logger_appendString (s)
    #define PD_LOG_NUMBER(f)    logger_appendFloat ((double)(f))
#else
    #define PD_LOG(s)
    #define PD_LOG_NUMBER(f)
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_error    logger_initialize                   (void);
PD_LOCAL void       logger_release                      (void);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if PD_WITH_DEBUG

PD_LOCAL void       logger_appendString                 (const char *s);
PD_LOCAL void       logger_appendFloat                  (double f);

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define LOGGER_FLOAT_STRING     32      /* Must be big enough to avoid overflow below. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static inline char *logger_stringWithFloat (char *dest, double f)
{
    char digits[] = "0123456789";
    
    int minus         = (f < 0.0);
    double absolute   = PD_ABS (f);
    double clamped    = PD_MIN (2147483647.0, absolute);
    int    integer    = (int)clamped;
    double fractional = clamped - (double)integer;
       
    char *s = dest + (LOGGER_FLOAT_STRING - 1);
    
    *s-- = 0;
    
    if (fractional > 0) {
        *s-- = digits[(int)(fractional * 1000000.0) % 10];
        *s-- = digits[(int)(fractional * 100000.0)  % 10];
        *s-- = digits[(int)(fractional * 10000.0)   % 10];
        *s-- = digits[(int)(fractional * 1000.0)    % 10];
        *s-- = digits[(int)(fractional * 100.0)     % 10];
        *s-- = digits[(int)(fractional * 10.0)      % 10];    
        *s-- = '.';
    }
    
    do {
    //
    *s-- = digits[integer % 10]; integer /= 10;
    //
    } while (integer != 0);
    
    if (minus) { *s = '-'; }
    else {
        *s = '+';
    }
    
    return s;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // S_LOGGER_H_
