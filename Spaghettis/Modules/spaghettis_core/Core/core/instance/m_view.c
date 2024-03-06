
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void instance_viewSet (int argc, t_atom *argv)
{
    buffer_clear (instance_get()->pd_view); buffer_append (instance_get()->pd_view, argc, argv);
}

void instance_viewConsume (t_glist *glist)
{
    int argc     = buffer_getSize (instance_get()->pd_view);
    t_atom *argv = buffer_getAtoms (instance_get()->pd_view);
    
    DBG (makeString (argc, argv));
    
    buffer_clear (instance_get()->pd_view);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
