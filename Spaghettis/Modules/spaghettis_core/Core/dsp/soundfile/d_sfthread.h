
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef D_SFTHREAD_H_
#define D_SFTHREAD_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

enum {
    SFTHREAD_READER     = 0,
    SFTHREAD_WRITER     = 1
    };

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Takes file ownership. */

PD_LOCAL t_sfthread     *sfthread_new           (t_object *x,
                                                    int type,
                                                    int bufferSize,
                                                    int fd,
                                                    t_audioproperties *p);
                                                    
PD_LOCAL t_ringbuffer   *sfthread_getBuffer     (t_sfthread *x);

PD_LOCAL void   sfthread_release                (t_sfthread *x);

PD_LOCAL int    sfthread_getNumberOfChannels    (t_sfthread *x);
PD_LOCAL int    sfthread_getBytesPerSample      (t_sfthread *x);
PD_LOCAL int    sfthread_isBigEndian            (t_sfthread *x);
PD_LOCAL int    sfthread_isEnd                  (t_sfthread *x);
PD_LOCAL void   sfthread_setCorrupted           (t_sfthread *x);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // D_SFTHREAD_H_
