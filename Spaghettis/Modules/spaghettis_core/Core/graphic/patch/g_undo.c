
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PD_LOCAL int undomanager_undoNeedToTriggerParent (t_undomanager *, t_items *, t_items *);
PD_LOCAL int undomanager_redoNeedToTriggerParent (t_undomanager *, t_items *, t_items *);
PD_LOCAL int undomanager_triggerParentIsPossible (t_glist *, t_items *, t_items *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL int glist_undoIsOk (t_glist *glist)
{
    if (glist_isAbstractionOrInside (glist)) { return 0; }
    
    return (glist->gl_undoEnabled && !instance_undoIsRecursive());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL int glist_undoHasSeparatorAtLast (t_glist *glist)
{
    return (undomanager_hasSeparatorAtLast (glist_getUndoManager (glist)));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void glist_undoAppendSeparator (t_glist *glist)
{
    undomanager_appendSeparator (glist_getUndoManager (glist));
}

PD_LOCAL void glist_undoAppend (t_glist *glist, t_undoaction *a)
{
    undomanager_append (glist_getUndoManager (glist), a);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Raw function to use with care. */

PD_LOCAL t_undomanager *glist_undoReplaceManager (t_glist *glist, t_undomanager *undo)
{
    t_undomanager *t = glist->gl_undomanager; glist->gl_undomanager = undo; return t;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_APPLICATION )

PD_LOCAL void glist_undo (t_glist *glist)
{
    PD_ASSERT (glist);
    
    if (glist_undoIsOk (glist)) {
    //
    t_undomanager *m = glist_getUndoManager (glist);
    t_items i; items_init (&i);
    t_items o; items_init (&o);
    int t = undomanager_undoNeedToTriggerParent (m, &i, &o);
    undomanager_undo (m);
    if (t && undomanager_triggerParentIsPossible (glist, &i, &o)) { glist_undo (glist_getParent (glist)); }
    //
    }
}

PD_LOCAL void glist_redo (t_glist *glist)
{
    PD_ASSERT (glist);
    
    if (glist_undoIsOk (glist)) {
    //
    t_undomanager *m = glist_getUndoManager (glist);
    t_items i; items_init (&i);
    t_items o; items_init (&o);
    int t = undomanager_redoNeedToTriggerParent (m, &i, &o);
    undomanager_redo (m);
    if (t && undomanager_triggerParentIsPossible (glist, &i, &o)) { glist_redo (glist_getParent (glist)); }
    //
    }
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
