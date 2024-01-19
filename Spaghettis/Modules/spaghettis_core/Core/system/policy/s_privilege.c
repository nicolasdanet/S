
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static uid_t privilege_euid;     /* Static. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* < https://wiki.sei.cmu.edu/confluence/pages/viewpage.action?pageId=87152405 > */

t_error privilege_start (void)
{
    privilege_euid = geteuid();
    
    PD_ASSERT (privilege_euid != 0); PD_ABORT (privilege_euid == 0);
        
    return (getuid() == 0);
}

t_error privilege_drop (void)
{
    return (seteuid (getuid()) != 0);
}

t_error privilege_restore (void)
{
    t_error err = PD_ERROR_NONE;
    
    if (geteuid() != privilege_euid) { err = (seteuid (privilege_euid) != 0); }
    
    return err;
}

void privilege_check (void)
{
    t_error err = (setuid (0) != -1);
    
    PD_ASSERT (!err); PD_ABORT (err);
}

t_error privilege_relinquish (void)
{
    t_error err = privilege_restore();
    
    if (!err) {
    //
    err = (setuid (getuid()) != 0);
    
    if (!err) { privilege_check(); }
    //
    }
    
    PD_ASSERT (!err);
    
    return err;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
