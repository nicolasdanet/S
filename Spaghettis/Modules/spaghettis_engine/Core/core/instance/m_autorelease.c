
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define AUTORELEASE_PERIOD  PD_SECONDS_TO_MILLISECONDS (3.0)

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void pd_unbindQuiet (t_pd *, t_symbol *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void instance_autoreleaseDrainProceed (t_symbol *s)
{
    if (symbol_hasThingQuiet (s)) { pd_message (symbol_getThing (s), sym__autorelease, 0, NULL); }
}

static void instance_autoreleaseDrain (void)
{
    instance_autoreleaseDrainProceed (sym__autoreleaseA);
    instance_autoreleaseDrainProceed (sym__autoreleaseB);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void instance_autoreleaseTask (void *dummy)
{
    instance_get()->pd_autoreleaseCount++;
    
    {
        int n = (instance_get()->pd_autoreleaseCount & 1ULL);
    
        instance_autoreleaseDrainProceed (n ? sym__autoreleaseB : sym__autoreleaseA);
    }
    
    clock_delay (instance_get()->pd_autorelease, AUTORELEASE_PERIOD);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void instance_autoreleaseRun (void)
{
    instance_get()->pd_autorelease = clock_new ((void *)NULL, (t_method)instance_autoreleaseTask);
    clock_delay (instance_get()->pd_autorelease, AUTORELEASE_PERIOD);
}

void instance_autoreleaseStop (void)
{
    instance_autoreleaseDrain(); clock_free (instance_get()->pd_autorelease);
    instance_get()->pd_autorelease = NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void instance_autoreleaseRegister (t_pd *x)
{
    int n = (instance_get()->pd_autoreleaseCount & 1ULL);
    
    pd_bind (x, n ? sym__autoreleaseB : sym__autoreleaseA);

    /* While quitting the application. */
    
    if (!instance_get()->pd_autorelease) { instance_autoreleaseDrain(); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Function that might be called at last by registered object while autoreleased. */

void instance_autoreleaseProceed (t_pd *x)
{
    pd_unbindQuiet (x, sym__autoreleaseA);
    pd_unbindQuiet (x, sym__autoreleaseB);
    pd_free (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
