
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PD_LOCAL void glist_copy (t_glist *glist)
{
    clipboard_copy (glist);
}

PD_LOCAL void glist_cut (t_glist *glist)
{
    glist_copy (glist);
    
    if (glist_undoIsOk (glist)) { glist_undoAppend (glist, undocut_new()); }
    
    glist_objectRemoveSelected (glist);
}

PD_LOCAL void glist_paste (t_glist *glist, t_point *m)
{
    clipboard_paste (glist, m);
}

PD_LOCAL void glist_duplicate (t_glist *glist)
{
    clipboard_copyDuplicate (glist);
    clipboard_pasteDuplicate (glist);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
