
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if defined ( PD_BUILDING_APPLICATION )

PD_FORCE void glist_copy (t_glist *glist)
{
    clipboard_copy (glist);
}

PD_FORCE void glist_cut (t_glist *glist)
{
    glist_copy (glist);
    
    if (glist_undoIsOk (glist)) { glist_undoAppend (glist, undocut_new()); }
    
    glist_objectRemoveSelected (glist);
}

PD_FORCE void glist_paste (t_glist *glist)
{
    clipboard_paste (glist);
}

PD_FORCE void glist_duplicate (t_glist *glist)
{
    clipboard_copyDuplicate (glist);
    clipboard_pasteDuplicate (glist);
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
