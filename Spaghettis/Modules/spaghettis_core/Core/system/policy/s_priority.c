
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if PD_APPLE

static t_error priority_setRTNative (pthread_t thread)
{
    t_error err = PD_ERROR_NONE;
    
    struct sched_param param;
    
    memset (&param, 0, sizeof (struct sched_param));
    
    /* On macOS POSIX thread is able to set thread priority only within the lowest priority band (0–63). */
    /* < https://developer.apple.com/library/ios/technotes/tn2169/_index.html > */

    // TODO: Use thread_policy_set instead?
    
    param.sched_priority = 63; 

    err = (pthread_setschedparam (thread, SCHED_RR, &param) != 0);
    
    PD_ASSERT (!err);
    
    return err;
}

#endif // PD_APPLE

#if PD_LINUX

static t_error priority_setRTNative (pthread_t thread)
{
    t_error err = PD_ERROR_NONE;
    
    #if defined ( _POSIX_PRIORITY_SCHEDULING ) && ( _POSIX_PRIORITY_SCHEDULING > 0 )

    struct sched_param param;
    
    memset (&param, 0, sizeof (struct sched_param));
    
    int min = sched_get_priority_min (SCHED_FIFO);
    int max = sched_get_priority_max (SCHED_FIFO);
    
    param.sched_priority = PD_MIN (min + 4, max);       /* Arbitrary. */

    err = (pthread_setschedparam (thread, SCHED_FIFO, &param) != 0);
    
    fprintf (stdout, err ? "RT Disabled\n" : "RT Enabled\n");
    
    #endif
    
    return PD_ERROR_NONE;
}

#endif // PD_LINUX

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_error priority_setPolicy (pthread_t thread)
{
    return priority_setRTNative (thread);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
