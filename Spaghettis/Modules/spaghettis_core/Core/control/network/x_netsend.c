
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Note that for now that object is reset with encapsulation. */

// -- TODO: Fetch states with pending?

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_class *netsend_class;              /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _netsend {
    t_object    ns_obj;                     /* Must be the first. */
    int         ns_fd;
    int         ns_protocol;
    int         ns_isBinary;
    int         ns_polling;
    int         ns_pollingDescriptor;
    int         ns_pollingCount;
    t_outlet    *ns_outlet;
    } t_netsend;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define NETSEND_POLL_MAXIMUM    10

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void netsend_socketOptions (t_netsend *x, int fd)
{
    int v = 1;
    int flags = fcntl (fd, F_GETFL, 0);
    
    PD_ASSERT (flags > -1);
    fcntl (fd, F_SETFL, PD_MAX (flags, 0) | O_NONBLOCK);
    
    if (x->ns_protocol == SOCK_STREAM) {
        if (setsockopt (fd, IPPROTO_TCP, TCP_NODELAY, (const void *)&v, sizeof (v)) < 0) { PD_BUG; }
    } else {
        if (setsockopt (fd, SOL_SOCKET, SO_BROADCAST, (const void *)&v, sizeof (v)) < 0) { PD_BUG; }
    }
}

static void netsend_socketClose (t_netsend *x)
{
    if (x->ns_fd >= 0) { close (x->ns_fd); x->ns_fd = -1;
    
    post_system (cast_object (x), "netsend: closed"); }    // --
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_error netsend_sendProceedRaw (t_netsend *x, int fd, char *t, int length)
{
    t_error err = PD_ERROR_NONE;
    
    ssize_t alreadySent = 0;
    char *p = t;

    while (alreadySent < length) {
        ssize_t n = send (fd, p, length - alreadySent, 0);  // -- TODO: Manage EWOULDBLOCK in the future?
        if (n <= 0) {
            error_failsToWrite (cast_object (x), sym_netsend);
            err = PD_ERROR;
            break; 
        } else {
            alreadySent += n; p += n;
        }
    }
    
    return err;
}

// -- TODO: Use memory cached for efficiency?

static t_error netsend_sendProceedText (t_netsend *x, int fd, t_symbol *s, int argc, t_atom *argv)
{   
    t_error err = PD_ERROR_NONE;
    
    t_buffer *b = buffer_new();
    char *t = NULL; int length;
    
        buffer_append (b, argc, argv);
        buffer_appendSemicolon (b);
        buffer_toStringUnzeroed (b, &t, &length);           /* Note that it adds a '\n' at the end. */
    
        err = netsend_sendProceedRaw (x, fd, t, length);

    PD_MEMORY_FREE (t);
    buffer_free (b);

    return err;
}

static t_error netsend_sendProceedBinary (t_netsend *x, int fd, t_symbol *s, int argc, t_atom *argv)
{
    t_error err = PD_ERROR_NONE;
    
    unsigned char *t = (unsigned char *)PD_MEMORY_GET (argc * sizeof (unsigned char));
    
        int i;
        
        for (i = 0; i < argc; i++) {
            int byte = atom_getFloatAtIndex (i, argc, argv);
            byte = PD_CLAMP (byte, 0, 0xff);
            *(t + i) = (unsigned char)byte;
        }
    
        err = netsend_sendProceedRaw (x, fd, (char *)t, argc);

    PD_MEMORY_FREE (t);

    return err;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void netsend_pollingStop (t_netsend *x)
{
    if (x->ns_polling) {
    //
    instance_pollingUnregister (cast_pd (x));
    
    x->ns_polling = 0;
    
    if (x->ns_pollingDescriptor != -1) { close (x->ns_pollingDescriptor); }
    
    x->ns_pollingDescriptor = -1;
    //
    }
}

static void netsend_pollingStart (t_netsend *x, int fd)
{
    netsend_pollingStop (x);
    
    x->ns_pollingCount      = 0;
    x->ns_pollingDescriptor = fd;
    x->ns_polling           = 1;
    
    instance_pollingRegister (cast_pd (x));
}

static void netsend_polling (t_netsend *x)
{
    fd_set rSet, wSet;
    struct timeval timeOut;
    t_error err   = (++x->ns_pollingCount > NETSEND_POLL_MAXIMUM);
    int connected = 0;
    
    int fd = x->ns_pollingDescriptor;
    
    FD_ZERO (&rSet);
    FD_ZERO (&wSet);
    
    FD_SET (fd, &rSet);
    FD_SET (fd, &wSet);
    
    timeOut.tv_sec  = 0;
    timeOut.tv_usec = 0;
    
    post_system (cast_object (x), "netsend: ...");  // --
    
    if (!err) {
    //
    err = (select (fd + 1, &rSet, &wSet, NULL, &timeOut) < 0);

    if (!err) {
    //
    if (FD_ISSET (fd, &rSet) || FD_ISSET (fd, &wSet)) {
        int error;
        socklen_t t = sizeof (error);
        if (getsockopt (fd, SOL_SOCKET, SO_ERROR, &error, &t) < 0) {
            err = PD_ERROR;
        } else {
            if (error) { err = PD_ERROR; }
            else {
                connected = 1;
            }
        }
    }
    //
    }
    //
    }
    
    if (connected) {
    //
    x->ns_fd = fd;
    x->ns_pollingDescriptor = -1;
    netsend_pollingStop (x);
    post_system (cast_object (x), "netsend: connected");    // --
    outlet_float (x->ns_outlet, 1);
    //
    }
    
    if (err) {
    //
    error_failed (cast_object (x), sym_netsend);
    netsend_pollingStop (x);
    outlet_float (x->ns_outlet, 0);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void netsend_connect (t_netsend *x, t_symbol *hostName, t_float f)
{
    netsend_pollingStop (x);
    
    if (x->ns_fd >= 0) { error_unexpected (cast_object (x), sym_netsend, hostName); }
    else {
    //
    int fd = socket (AF_INET, x->ns_protocol, 0);

    if (fd < 0) { error_canNotOpen (cast_object (x), sym_netsend); }
    else {
    //
    int portNumber = (int)f;
    struct hostent *h = gethostbyname (hostName->s_name);
    
    netsend_socketOptions (x, fd);

    post_system (cast_object (x), "netsend: connecting to port %d", portNumber);    // --

    if (h == NULL) { error_invalid (cast_object (x), sym_netsend, hostName); }
    else {
    //
    struct sockaddr_in server;
    
    server.sin_family = AF_INET;
    server.sin_port = htons ((u_short)portNumber);
    memcpy ((char *)&server.sin_addr, (char *)h->h_addr, h->h_length);

    if (connect (fd, (struct sockaddr *)&server, sizeof (server)) < 0) {
        if (errno == EINPROGRESS) { netsend_pollingStart (x, fd); }
        else {
            close (fd);
            error_failed (cast_object (x), sym_netsend);
            PD_ASSERT (x->ns_fd == -1);
            outlet_float (x->ns_outlet, 0);
        }
        
    } else {
        x->ns_fd = fd; outlet_float (x->ns_outlet, 1);
    }
    //
    }
    //
    }
    //
    }
}

static void netsend_disconnect (t_netsend *x)
{
    netsend_pollingStop (x); netsend_socketClose (x); outlet_float (x->ns_outlet, 0);
}

static void netsend_send (t_netsend *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->ns_fd >= 0) {
    //
    t_error err = PD_ERROR_NONE;
    
    if (x->ns_isBinary) { err = netsend_sendProceedBinary (x, x->ns_fd, s, argc, argv); }
    else {
        err = netsend_sendProceedText (x, x->ns_fd, s, argc, argv);
    }
    
    if (err) { netsend_disconnect (x); }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *netsend_new (t_symbol *s, int argc, t_atom *argv)
{
    t_netsend *x = (t_netsend *)pd_new (netsend_class);
    
    x->ns_fd        = -1;
    x->ns_isBinary  = 0;
    x->ns_protocol  = SOCK_STREAM;
    x->ns_outlet    = outlet_newFloat (cast_object (x));
    
    while (argc > 0) {
    //
    t_symbol *t = atom_getSymbolAtIndex (0, argc, argv);

    if (t == sym___dash__binary)   { argc--; argv++; x->ns_isBinary = 1; }
    else if (t == sym___dash__udp) { argc--; argv++; x->ns_protocol = SOCK_DGRAM; }
    else {
        break;
    }
    //
    }
    
    error__options (cast_object (x), s, argc, argv);
    
    {
    //
    t_float port   = -1;
    t_symbol *host = sym_localhost;
    
    if (argc) { if (IS_FLOAT (argv))  { port = GET_FLOAT (argv); } argc--; argv++; }
    if (argc) { if (IS_SYMBOL (argv)) { host = GET_SYMBOL (argv); } argc--; argv++; }
    if (argc) { warning_unusedArguments (cast_object (x), s, argc, argv); }
    
    if (port >= 0) { netsend_connect (x, host, port); }
    //
    }
    
    return x;
}

static void netsend_free (t_netsend *x)
{
    netsend_pollingStop (x);
    netsend_socketClose (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void netsend_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_netsend,
            (t_newmethod)netsend_new,
            (t_method)netsend_free,
            sizeof (t_netsend),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
    
    class_addPolling (c, (t_method)netsend_polling);
    
    class_addMethod (c, (t_method)netsend_connect,      sym_connect,    A_SYMBOL, A_FLOAT, A_NULL);
    class_addMethod (c, (t_method)netsend_disconnect,   sym_disconnect, A_NULL);
    class_addMethod (c, (t_method)netsend_send,         sym_send,       A_GIMME, A_NULL);
        
    netsend_class = c;
}

PD_LOCAL void netsend_destroy (void)
{
    class_free (netsend_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

