
/* Copyright (c) 1997-2020 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "x_receiver.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Manage UDP / TCP incoming. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define RECEIVER_BUFFER_SIZE        4096
#define RECEIVER_BUFFER_LIMIT       4096 * 4096

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

struct _receiver {
    void            *r_owner;
    t_buffer        *r_message;
    t_heapstring    *r_heap;
    int             r_fd;
    int             r_isClosed;
    t_notifyfn      r_fnNotify;
    t_receivefn     r_fnReceive;
    };

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL char *heapstring_clearBorrow (t_heapstring *);         /* Caller acquires ownership. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void receiver_closeProceed (t_receiver *x)
{
    if (!x->r_isClosed) {
        close (x->r_fd);
        monitor_removePoller (x->r_fd);
        x->r_isClosed = 1;
    }
}

PD_LOCAL int receiver_isClosed (t_receiver *x)
{
    return x->r_isClosed;
}

static void receiver_close (t_receiver *x, int fd)
{
    (*x->r_fnNotify) (x->r_owner, fd); receiver_closeProceed (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void receiver_readTextReceived (t_receiver *x, int k, char *t)
{
    buffer_clear (x->r_message);
    
    buffer_withStringUnzeroed (x->r_message, t, k);
    
    (*x->r_fnReceive) (x->r_owner, x->r_message);
}

static void receiver_readTextProceed (t_receiver *x, int k, char *t)
{
    PD_ASSERT (t[k] == 0);
    
    string_removeCharacter (t, '\n'); heapstring_add (x->r_heap, t);
    
    int size = heapstring_getSize (x->r_heap);
    
    if (size) {
    //
    if (heapstring_containsCharacterAtEnd (x->r_heap, ';')) {                   /* Messages are completed. */
        receiver_readTextReceived (x, size, heapstring_getRaw (x->r_heap));
        heapstring_clear (x->r_heap);
        
    } else if (string_contains (t, ";")) {                                      /* At least one is. */
        char *borrowed = heapstring_clearBorrow (x->r_heap);
        int n = string_indexOfFirstOccurrenceFromEnd (borrowed, ";");
        PD_ASSERT (n >= 0);
        PD_ASSERT (borrowed[n + 1] != 0);
        heapstring_add (x->r_heap, borrowed + n + 1);
        borrowed[n] = 0;
        receiver_readTextReceived (x, n, borrowed);
        PD_MEMORY_FREE (borrowed);
        
    } else if (size >= RECEIVER_BUFFER_LIMIT) {                                 /* Something went wrong? */
        heapstring_clear (x->r_heap);
        PD_BUG;
    }
    //
    }
}

static void receiver_readText (t_receiver *x, int fd)
{
    char t[RECEIVER_BUFFER_SIZE + 1] = { 0 }; ssize_t k = recv (fd, (void *)t, RECEIVER_BUFFER_SIZE, 0);
    
    if (k <= 0) { receiver_close (x, fd); } else { receiver_readTextProceed (x, k, t); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void receiver_readBinaryReceived (t_receiver *x, int k, char *t)
{
    int i;
    
    buffer_clear (x->r_message);
    
    for (i = 0; i < k; i++) { buffer_appendFloat (x->r_message, (t_float)((unsigned char)(*(t + i)))); }
    
    (*x->r_fnReceive) (x->r_owner, x->r_message);
}

static void receiver_readBinary (t_receiver *x, int fd)
{
    char t[RECEIVER_BUFFER_SIZE + 1] = { 0 }; ssize_t k = recv (fd, (void *)t, RECEIVER_BUFFER_SIZE, 0);
    
    if (k <= 0) { receiver_close (x, fd); } else { receiver_readBinaryReceived (x, k, t); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_receiver *receiver_new (void *owner,
    int fd,
    t_notifyfn notify,
    t_receivefn receive,
    int isBinary)
{
    t_receiver *x = (t_receiver *)PD_MEMORY_GET (sizeof (t_receiver));
    
    x->r_owner     = owner;
    x->r_message   = buffer_new();
    x->r_heap      = heapstring_new (0);
    x->r_fd        = fd;
    x->r_isClosed  = 0;
    x->r_fnNotify  = notify;
    x->r_fnReceive = receive;

    PD_ASSERT (x->r_fnNotify  != NULL);
    PD_ASSERT (x->r_fnReceive != NULL);
    
    monitor_addPoller (x->r_fd, isBinary ? (t_pollfn)receiver_readBinary : (t_pollfn)receiver_readText, x);
    
    return x;
}

PD_LOCAL void receiver_free (t_receiver *x)
{
    receiver_closeProceed (x);
    
    heapstring_free (x->r_heap);
    buffer_free (x->r_message);
    
    PD_MEMORY_FREE (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
