
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _fdpoll {
    void        *fdp_p;
    int         fdp_fd;
    t_pollfn    fdp_fn;
    } t_fdpoll;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_fdpoll *monitor_pollers;           /* Static. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static int monitor_pollersSize;             /* Static. */
static int monitor_maximumFileDescriptor;   /* Static. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static int monitor_proceed (int microseconds)
{
    t_error err = PD_ERROR_NONE;
    int didSomething = 0;
    struct timeval timeOut;
    t_fdpoll *pollers = NULL;
    int i;
    
    timeOut.tv_sec  = 0;
    timeOut.tv_usec = microseconds;
    
    fd_set rSet;
    fd_set wSet;
    fd_set eSet;
    
    FD_ZERO (&rSet);
    FD_ZERO (&wSet);
    FD_ZERO (&eSet);
    
    for (pollers = monitor_pollers, i = monitor_pollersSize; i--; pollers++) {
        FD_SET (pollers->fdp_fd, &rSet);
    }

    err = (select (monitor_maximumFileDescriptor + 1, &rSet, &wSet, &eSet, &timeOut) < 0);
    
    if (!err) {
    //
    for (i = 0; i < monitor_pollersSize; i++) {
        if (FD_ISSET (monitor_pollers[i].fdp_fd, &rSet)) {
            (*monitor_pollers[i].fdp_fn) (monitor_pollers[i].fdp_p, monitor_pollers[i].fdp_fd);
            didSomething = 1;
        }
    }
    //
    }
    
    return didSomething;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int monitor_blocking (double milliseconds)
{
    return monitor_proceed (PD_MILLISECONDS_TO_MICROSECONDS (milliseconds));
}

int monitor_nonBlocking (void)
{
    return monitor_proceed (0);
}

void monitor_addPoller (int fd, t_pollfn fn, void *ptr)
{
    int n = monitor_pollersSize;
    int oldSize = (int)(n * sizeof (t_fdpoll));
    int newSize = (int)(oldSize + sizeof (t_fdpoll));
    int i;
    t_fdpoll *p = NULL;
    
    for (i = n, p = monitor_pollers; i--; p++) { PD_ASSERT (p->fdp_fd != fd); }
    
    monitor_pollers = (t_fdpoll *)PD_MEMORY_RESIZE (monitor_pollers, oldSize, newSize);
        
    p = monitor_pollers + n;
    p->fdp_p  = ptr;
    p->fdp_fd = fd;
    p->fdp_fn = fn;
        
    monitor_pollersSize = n + 1;
    
    if (fd > monitor_maximumFileDescriptor) { monitor_maximumFileDescriptor = fd; }
}

void monitor_removePoller (int fd)
{
    int n = monitor_pollersSize;
    int oldSize = (int)(n * sizeof (t_fdpoll));
    int newSize = (int)(oldSize - sizeof (t_fdpoll));
    int i;
    t_fdpoll *p;
    
    PD_ASSERT (oldSize != 0);
    
    for (i = n, p = monitor_pollers; i--; p++) {
    //
    if (p->fdp_fd == fd) {
    //
    while (i--) { *p = *(p + 1); p++; }
    monitor_pollers = (t_fdpoll *)PD_MEMORY_RESIZE (monitor_pollers, oldSize, newSize);
    monitor_pollersSize = n - 1;
    return;
    //
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void monitor_initialize (void)
{
    monitor_pollers = (t_fdpoll *)PD_MEMORY_GET (0);
}

void monitor_release (void)
{
    PD_MEMORY_FREE (monitor_pollers);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
