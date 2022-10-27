
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef S_SCHEDULER_H_
#define S_SCHEDULER_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_error    scheduler_main                      (void);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL double     scheduler_getRealTimeInSeconds      (void);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_systime  scheduler_getLogicalTime            (void);
PD_LOCAL t_systime  scheduler_getLogicalTimeAfter       (double ms);
PD_LOCAL double     scheduler_getMillisecondsSince      (t_systime systime);
PD_LOCAL double     scheduler_getUnitsSince             (t_systime systime, double unit, int isSamples);
PD_LOCAL void       scheduler_needToExit                (void);
PD_GUI   void       scheduler_needToExitWithError       (void);
PD_LOCAL int        scheduler_isExiting                 (void);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // S_SCHEDULER_H_
