
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

std::function<void()> Inputs::ping()
{
    return []() { core::inputs_ping(); };
}

std::function<void()> Inputs::newPatch (juce::File file)
{
    return [f = std::move (file)]() { core::inputs_newPatch (f); };
}

std::function<void()> Inputs::openPatch (juce::File file)
{
    return [f = std::move (file)]() { core::inputs_openPatch (f); };
}

std::function<void()> Inputs::rescan (Logged type)
{
    const int logged = (type == Logged::base) ? 0 : ((type == Logged::full) ? 1 : -1);
    
    return [logged]() { core::inputs_rescan (logged); };
}

std::function<void()> Inputs::switchDsp()
{
    return []() { core::inputs_switchDsp(); };
}

std::function<void()> Inputs::setSearchPaths (juce::StringArray paths)
{
    return [p = std::move (paths)]() { core::inputs_setSearchPaths (p); };
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
