
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_APPLICATION )

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void outputs_quit (void)
{
    wrapper_send (Outputs::quit());
}

void outputs_clearConsole (void)
{
    wrapper_send (Outputs::clearConsole());
}

void outputs_reportDsp (int n)
{
    wrapper_send (Outputs::reportDsp (n ? true : false));
}

void outputs_patchLoadbangBegin (t_glist *g)
{
    wrapper_send (Outputs::patchLoadbangBegin (UniquePath (cast_object (g))));
}

void outputs_patchLoadbangEnd (t_glist *g)
{
    wrapper_send (Outputs::patchLoadbangEnd (UniquePath (cast_object (g))));
}

void outputs_patchOpened (t_symbol *name, t_symbol *directory)
{
    jassert (name && directory);
    
    juce::File f (makeString (symbol_getName (directory)) + "/" + makeString (symbol_getName (name)));
    
    wrapper_send (Outputs::patchOpened (f));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void outputs_classNew (t_symbol *s)
{
    wrapper_send (Outputs::classNew (makeString (symbol_getName (s))));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void outputs_objectAdded (t_object *x)
{
    const UniquePath p (x); wrapper_send (Outputs::added (p, Report::object (p, x, Tags())));
}

void outputs_objectChanged (t_object *x, const Tags& t)
{
    const UniquePath p (x); wrapper_send (Outputs::changed (p, Report::object (p, x, t)));
}

void outputs_objectUpdated (t_object *x, const Tags& t)
{
    t_glist *g = object_isCanvas (x) ? cast_glist (x) : object_getOwner (x);
    
    outputs_objectChanged (x, t);
    
    jassert (g);
    
    glist_setDirty (g, 1);
}

void outputs_objectRemoved (t_object *x)
{
    wrapper_send (Outputs::removed (UniquePath (x)));
}

void outputs_objectRenamed (t_object *x, t_id t)
{
    wrapper_send (Outputs::renamed (UniquePath (x), t));
}

void outputs_lineAdded (t_id u, t_object *src, int m, t_object *dest, int n, t_glist *owner)
{
    const UniquePath p (u, owner);
    
    wrapper_send (Outputs::added (p, Report::lineAdded (p, src, m, dest, n)));
}

void outputs_lineChanged (t_id u, t_object *src, int m, t_object *dest, int n, t_glist *owner)
{
    const UniquePath p (u, owner);
    
    wrapper_send (Outputs::changed (p, Report::lineChanged (p, src, m, dest, n)));
}

void outputs_lineRemoved (t_id u, t_glist *owner)
{
    wrapper_send (Outputs::removed (UniquePath (u, owner)));
}

void outputs_patchOrder (t_glist *g, std::vector<UniqueId>&& ids)
{
    wrapper_send (Outputs::patchOrder (UniquePath (cast_object (g)), std::move (ids)));
}

void outputs_reportAudioDevices (t_audiodevices *d)
{

}

void outputs_reportMidiDevices (t_mididevices *d)
{

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#else

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void main_exit (void);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void outputs_quit (void)
{
    #if defined ( PD_BUILDING_TERMINAL )
    
    main_exit();
    
    #endif
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
