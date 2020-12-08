
/* Copyright (c) 1997-2020 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "x_receiver.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Note that for now that object is reset with encapsulation. */

// -- TODO: Fetch states with pending?

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_class *netreceive_class;                  /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _netreceive {
    t_object    nr_obj;                     /* Must be the first. */
    int         nr_fd;
    int         nr_protocol;
    int         nr_isBinary;
    int         nr_numberOfConnections;
    int         nr_size;
    t_receiver  **nr_vector;
    t_outlet    *nr_outletLeft;
    t_outlet    *nr_outletRight;
    } t_netreceive;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define NETRECEIVE_LISTEN_BACKLOG   5

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void netreceive_receiversEnlarge (t_netreceive *x)
{
    size_t oldSize  = sizeof (t_receiver *) * (x->nr_size);
    size_t newSize  = sizeof (t_receiver *) * (x->nr_size * 2);
    x->nr_size      = (x->nr_size * 2);
    x->nr_vector    = (t_receiver **)PD_MEMORY_RESIZE (x->nr_vector, oldSize, newSize);
}

static void netreceive_receiversPurge (t_netreceive *x)
{
    int i, hasEmptySlot = 0;
    
    for (i = 0; i < x->nr_size; i++) {
        if (x->nr_vector[i] != NULL && receiver_isClosed (x->nr_vector[i])) { 
            receiver_free (x->nr_vector[i]); x->nr_vector[i] = NULL; 
        }
    }
    
    for (i = 0; i < x->nr_size; i++) {
        if (x->nr_vector[i] == NULL) { hasEmptySlot = 1; break; } 
    }
    
    if (!hasEmptySlot) { netreceive_receiversEnlarge (x); }
}

static void netreceive_receiversAdd (t_netreceive *x, t_receiver *receiver)
{
    int i, hasFilledEmptySlot = 0;
    
    netreceive_receiversPurge (x);
    
    for (i = 0; i < x->nr_size; i++) {
        if (x->nr_vector[i] == NULL) { 
            x->nr_vector[i] = receiver; hasFilledEmptySlot = 1; break; 
        }
    }
    
    PD_UNUSED (hasFilledEmptySlot); PD_ASSERT (hasFilledEmptySlot);
    
    if (x->nr_protocol == SOCK_STREAM) { outlet_float (x->nr_outletRight, x->nr_numberOfConnections); }
}

static void netreceive_receiversClean (t_netreceive *x)
{
    int i;
    
    for (i = 0; i < x->nr_size; i++) {
    //
    if (x->nr_vector[i] != NULL) {
        receiver_free (x->nr_vector[i]); x->nr_vector[i] = NULL; 
    }
    //
    }
    
    if (x->nr_protocol == SOCK_STREAM) { outlet_float (x->nr_outletRight, x->nr_numberOfConnections); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void netreceive_callbackReceived (void *z, t_buffer *b)
{
    t_netreceive *x = (t_netreceive *)z;
    
    int i, n = buffer_messagesGetNumberOf (b);
    
    for (i = 0; i < n; i++) {
    //
    int start, end;
    
    if (buffer_messagesGetAt (b, i, &start, &end) == PD_ERROR_NONE) {
    //
    int size = end - start;
    
    if (size) {
    //
    /* To ensure compatibility with Pure Data, bytes are sent one by one with TCP mode. */
    
    if (x->nr_isBinary && x->nr_protocol == SOCK_STREAM) { 
        
        int j;
        
        for (j = 0; j < size; j++) { 
            outlet_float (x->nr_outletLeft, atom_getFloat (buffer_getAtomAtIndex (b, start + j)));
        }
        
    } else {
    
        t_atom *t = buffer_getAtomAtIndex (b, start);
        
        if (IS_FLOAT (t)) { 
            if (size == 1) { outlet_float (x->nr_outletLeft, GET_FLOAT (t)); }
            else {
                outlet_list (x->nr_outletLeft, size, t);
            }
        } else if (IS_SYMBOL (t)) {
            if (size == 1) { outlet_symbol (x->nr_outletLeft, GET_SYMBOL (t)); }
            else {
                outlet_anything (x->nr_outletLeft, GET_SYMBOL (t), size - 1, t + 1);
            }
        } else {
            PD_BUG;
        }
    }
    //
    }
    //
    }
    //
    }
}

static void netreceive_callbackClosed (t_netreceive *x, int fd)
{
    if (x->nr_protocol == SOCK_STREAM) {
        x->nr_numberOfConnections--; outlet_float (x->nr_outletRight, x->nr_numberOfConnections);
    }
}

static void netreceive_callbackConnected (t_netreceive *x, int dummy)
{
    int fd = accept (x->nr_fd, 0, 0);
    
    if (fd < 0) { error_failed (sym_netreceive); }
    else {
        t_receiver *t = receiver_new ((void *)x,
                            fd,
                            (t_notifyfn)netreceive_callbackClosed,
                            (t_receivefn)netreceive_callbackReceived,
                            x->nr_isBinary);
        
        x->nr_numberOfConnections++;
        
        netreceive_receiversAdd (x, t);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void netreceive_socketOptions (t_netreceive *x, int fd)
{
    int v = 1;
    int flags = fcntl (fd, F_GETFL, 0);
    
    PD_ASSERT (flags > -1);
    fcntl (fd, F_SETFL, PD_MAX (flags, 0) | O_NONBLOCK);
    
    if (x->nr_protocol == SOCK_STREAM) {
        if (setsockopt (fd, IPPROTO_TCP, TCP_NODELAY, (const void *)&v, sizeof (v)) < 0) { PD_BUG; }
        if (setsockopt (fd, SOL_SOCKET, SO_REUSEADDR, (const void *)&v, sizeof (v)) < 0) { PD_BUG; }
    } else {
        if (setsockopt (fd, SOL_SOCKET, SO_BROADCAST, (const void *)&v, sizeof (v)) < 0) { PD_BUG; }
        if (setsockopt (fd, SOL_SOCKET, SO_REUSEADDR, (const void *)&v, sizeof (v)) < 0) { PD_BUG; }
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void netreceive_close (t_netreceive *x)
{
    int report = (x->nr_fd >= 0);
    
    if (x->nr_protocol == SOCK_STREAM) {
    //
    if (x->nr_fd >= 0) {
        monitor_removePoller (x->nr_fd);
        close (x->nr_fd);
    }
    
    x->nr_numberOfConnections = 0;
    //
    }
    
    netreceive_receiversClean (x);
    
    x->nr_fd = -1;
    
    if (report) { post ("netreceive: closed"); }            // --
}

static void netreceive_listen (t_netreceive *x, t_float f)
{
    int portNumber = f;

    netreceive_close (x);
    
    if (portNumber > 0) {
    //
    int fd = socket (AF_INET, x->nr_protocol, 0);
    
    if (fd < 0) { error_canNotOpen (sym_netreceive); }
    else {
    //
    t_error err = PD_ERROR_NONE;
    
    struct sockaddr_in server;

    post ("netreceive: listening on port %d", portNumber);  // --
    
    netreceive_socketOptions (x, fd);
    
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons ((u_short)portNumber);

    err = (bind (fd, (struct sockaddr *)&server, sizeof (struct sockaddr_in)) < 0);
    
    if (!err) {
    //
    if (x->nr_protocol == SOCK_DGRAM) {
    
        t_receiver *t = receiver_new ((void *)x,
                            fd,
                            (t_notifyfn)netreceive_callbackClosed,
                            (t_receivefn)netreceive_callbackReceived,
                            x->nr_isBinary);
                            
        netreceive_receiversAdd (x, t);
        
    } else {
    
        err = (listen (fd, NETRECEIVE_LISTEN_BACKLOG) < 0);
        
        if (!err) {
            monitor_addPoller (fd, (t_pollfn)netreceive_callbackConnected, (void *)x);
        }
    }
    //
    }
    
    if (err) { close (fd); error_failed (sym_netreceive); }
    else {
        x->nr_fd = fd;
    }
    //
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *netreceive_new (t_symbol *s, int argc, t_atom *argv)
{
    t_netreceive *x = (t_netreceive *)pd_new (netreceive_class);
    
    x->nr_fd          = -1;
    x->nr_protocol    = SOCK_STREAM;
    x->nr_isBinary    = 0;
    x->nr_size        = NETRECEIVE_LISTEN_BACKLOG;
    x->nr_vector      = (t_receiver **)PD_MEMORY_GET (sizeof (t_receiver *) * x->nr_size);
    x->nr_outletLeft  = outlet_newMixed (cast_object (x));
        
    while (argc > 0) {
    //
    t_symbol *t = atom_getSymbolAtIndex (0, argc, argv);

    if (t == sym___dash__binary)   { argc--; argv++; x->nr_isBinary = 1; }
    else if (t == sym___dash__udp) { argc--; argv++; x->nr_protocol = SOCK_DGRAM; }
    else {
        break;
    }
    //
    }

    error__options (s, argc, argv);
    
    if (x->nr_protocol == SOCK_STREAM) { x->nr_outletRight = outlet_newFloat (cast_object (x)); }
    
    {
    //
    t_float port = -1;
    
    if (argc) { if (IS_FLOAT (argv)) { port = GET_FLOAT (argv); } argc--; argv++; }
    if (argc) { warning_unusedArguments (s, argc, argv); }
    
    if (port >= 0) { netreceive_listen (x, port); }
    //
    }
    
    return x;
}

static void netreceive_free (t_netreceive *x)
{
    netreceive_close (x);
    
    PD_MEMORY_FREE (x->nr_vector);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void netreceive_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_netreceive,
            (t_newmethod)netreceive_new,
            (t_method)netreceive_free,
            sizeof (t_netreceive),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
            
    class_addMethod (c, (t_method)netreceive_listen, sym_listen, A_FLOAT, A_NULL);
    class_addMethod (c, (t_method)netreceive_close,  sym_close,  A_NULL);
    
    netreceive_class = c;
}

PD_LOCAL void netreceive_destroy (void)
{
    class_free (netreceive_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
