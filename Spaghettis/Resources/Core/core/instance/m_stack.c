
/* Copyright (c) 1997-2020 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void glist_loadEnd     (t_glist *);
PD_LOCAL void glist_undoEnable  (t_glist *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void instance_stackPush (t_glist *x)
{
    t_stackelement *e = instance_get()->pd_stack.s_stack + (instance_get()->pd_stack.s_stackIndex++);
    
    PD_ASSERT (instance_get()->pd_stack.s_stackIndex < INSTANCE_STACK);
    PD_ABORT (instance_get()->pd_stack.s_stackIndex >= INSTANCE_STACK);
    
    e->s_context = instance_contextGetCurrent();
    e->s_loadedAbstraction = instance_get()->pd_loadingAbstraction;
    
    instance_get()->pd_loadingAbstraction = NULL;
    
    instance_contextSetCurrent (x);
}

PD_LOCAL void instance_stackPop (t_glist *x)
{
    t_stackelement *e = instance_get()->pd_stack.s_stack + (--instance_get()->pd_stack.s_stackIndex);
    
    PD_ASSERT (instance_get()->pd_stack.s_stackIndex >= 0);
    PD_ASSERT (instance_contextGetCurrent() == x);
    
    instance_contextSetCurrent (e->s_context);
    
    instance_get()->pd_stack.s_contextPopped = x;
}

PD_LOCAL void instance_stackPopPatch (t_glist *glist)
{
    instance_stackPop (glist);
    glist_inletSort (glist);
    glist_outletSort (glist);
    glist_loadEnd (glist);
    glist_undoEnable (glist);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
