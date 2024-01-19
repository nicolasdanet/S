
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

int undomanager_undoNeedToTriggerParent (t_undomanager *, t_items *, t_items *);
int undomanager_redoNeedToTriggerParent (t_undomanager *, t_items *, t_items *);
int undomanager_triggerParentIsPossible (t_glist *, t_items *, t_items *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int glist_undoIsOk (t_glist *glist)
{
    if (glist_isAbstractionOrInside (glist)) { return 0; }
    
    return (glist->gl_undoEnabled && !instance_undoIsRecursive());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int glist_undoHasSeparatorAtLast (t_glist *glist)
{
    return (undomanager_hasSeparatorAtLast (glist_getUndoManager (glist)));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void glist_undoAppendSeparator (t_glist *glist)
{
    undomanager_appendSeparator (glist_getUndoManager (glist));
}

void glist_undoAppend (t_glist *glist, t_undoaction *a)
{
    undomanager_append (glist_getUndoManager (glist), a);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Raw function to use with extra care. */

t_undomanager *glist_undoReplaceManager (t_glist *glist, t_undomanager *undo)
{
    t_undomanager *t = glist->gl_undomanager; glist->gl_undomanager = undo; return t;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void glist_undo (t_glist *glist)
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

void glist_redo (t_glist *glist)
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

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
