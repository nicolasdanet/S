
/* Copyright (c) 2021 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Perform Outputs::quit()
{
    return []() { juce::JUCEApplication::getInstance()->systemRequestedQuit(); };
}

Perform Outputs::clear()
{
    return []() { Spaghettis()->clearConsole(); };
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Perform Outputs::added (core::UniquePath unique, core::Report view)
{
    return [u = std::move (unique), v = std::move (view)]()
    {
        Spaghettis()->getPatches().add (u, v);
    };
}

Perform Outputs::changed (core::UniquePath unique, core::Report view)
{
    return [u = std::move (unique), v = std::move (view)]()
    {
        Spaghettis()->getPatches().change (u, v);
    };
}

Perform Outputs::removed (core::UniquePath unique)
{
    return [u = std::move (unique)]()
    {
        Spaghettis()->getPatches().remove (u);
    };
}

Perform Outputs::renamed (core::UniquePath unique, core::UniqueId i)
{
    return [i, u = std::move (unique)]()
    {
        Spaghettis()->getPatches().rename (u, i);
    };
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Perform Outputs::patchOpened (juce::File file)
{
    return [f = std::move (file)]() { Spaghettis()->appendRecentFile (f); };
}

Perform Outputs::patchLoadbangBegin (core::UniquePath unique)
{
    return [u = std::move (unique)]()
    {
        Spaghettis()->getPatches().loadbangBegin (u);
    };
}

Perform Outputs::patchLoadbangEnd (core::UniquePath unique)
{
    return [u = std::move (unique)]()
    {
        Spaghettis()->getPatches().loadbangEnd (u);
    };
}


Perform Outputs::patchOrder (core::UniquePath unique, std::vector<core::UniqueId>&& ids)
{
    return [u = std::move (unique), v = std::move (ids)]()
    {
        Spaghettis()->getPatches().setOrder (u, v);
    };
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Perform Outputs::reportDsp (bool state)
{
    return [state]() { Spaghettis()->setDspState (state); };
}

Perform Outputs::reportClassNew (juce::String name)
{
    return [s = std::move (name)]()
    {
        Spaghettis()->getAutocomplete().addContent (s);
    };
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
