
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
    int argc = buffer_getSize (instance_get()->pd_view);
    
    if (argc == 5) {
    //
    t_atom *argv = buffer_getAtoms (instance_get()->pd_view);
    
    int x = atom_getFloat (argv + 0);
    int y = atom_getFloat (argv + 1);
    int z = atom_getFloat (argv + 2);
    int i = atom_getFloat (argv + 3);
    int w = atom_getFloat (argv + 4);
    
    glist_setOffset (glist, x, y, 0);
    glist_setZoom (glist, z, 0);
    glist_setInspector (glist, i, w, 0);
    //
    }
    
    buffer_clear (instance_get()->pd_view);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
