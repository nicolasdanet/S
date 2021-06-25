
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../m_spaghettis.h"
#include "../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_APPLICATION )

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void inputs_ping (void)
{
    post_system (NULL, "?");
}

void inputs_newPatch (const juce::File& f)
{
    if (!f.existsAsFile()) {
        t_symbol *name = gensym (f.getFileName().toRawUTF8());
        t_symbol *directory = gensym (f.getParentDirectory().getFullPathName().toRawUTF8());
        instance_patchNew (name, directory);
    }
}

void inputs_openPatch (const juce::File& f)
{
    if (f.existsAsFile()) {
        t_symbol *name = gensym (f.getFileName().toRawUTF8());
        t_symbol *directory = gensym (f.getParentDirectory().getFullPathName().toRawUTF8());
        instance_patchOpen (name, directory);
    }
}

void inputs_switchDsp (void)
{
    dsp_setState (!dsp_getState());
}

void inputs_rescan (int logged)
{
    searchpath_rescan (logged);
}

void inputs_setSearchPaths (const juce::StringArray& paths)
{
    searchpath_clear(); for (const auto& p : paths) { searchpath_appendRoot (p.toRawUTF8()); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // PD_BUILDING_APPLICATION

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
