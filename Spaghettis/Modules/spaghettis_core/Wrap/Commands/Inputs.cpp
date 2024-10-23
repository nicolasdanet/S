
/* Copyright (c) 2021 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Perform Inputs::dspSwitch()
{
    return []() { core::inputs_dspSwitch(); };
}

Perform Inputs::searchPathsRescan (Logged type)
{
    return [logged = loggedAsInteger (type)]() { core::inputs_searchPathsRescan (logged); };
}

Perform Inputs::searchPathsSet (juce::StringArray paths)
{
    return [p = std::move (paths)]() { core::inputs_searchPathsSet (p); };
}

Perform Inputs::snapToGridSet (bool isSet)
{
    return [isSet]() { core::inputs_snapSet (isSet); };
}

Perform Inputs::snapToGridSetSize (int size)
{
    return [size]() { core::inputs_snapSetGrid (size); };
}

Perform Inputs::devicesRescan (Logged type)
{
    return [logged = loggedAsInteger (type)]() { core::inputs_devicesRescan (logged); };
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Perform Inputs::patchNew (juce::File file)
{
    return [f = std::move (file)]() { core::inputs_patchNew (f); };
}

Perform Inputs::patchOpen (juce::File file)
{
    return [f = std::move (file)]() { core::inputs_patchOpen (f); };
}

Perform Inputs::patchClose (core::UniqueId u)
{
    return [u]() { core::inputs_patchClose (u); };
}

Perform Inputs::patchSave (core::UniqueId u)
{
    return [u]() { core::inputs_patchSave (u); };
}

Perform Inputs::patchSetDirty (core::UniqueId u)
{
    return [u]() { core::inputs_patchSetDirty (u); };
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Perform Inputs::patchSetEditView (core::UniqueId u, juce::Rectangle<int> bounds)
{
    return [u, bounds]() { core::inputs_patchSetEditView (u, bounds); };
}

Perform Inputs::patchSetEditViewProperties (core::UniqueId u,
    core::Point::Real pt,
    int z,
    bool inspector,
    int w)
{
    return [u, pt, z, inspector, w]() { core::inputs_patchSetEditViewProperties (u, pt, z, inspector, w); };
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Perform Inputs::patchUndo (core::UniqueId u)
{
    return [u]() { core::inputs_patchUndo (u); };
}

Perform Inputs::patchRedo (core::UniqueId u)
{
    return [u]() { core::inputs_patchRedo (u); };
}

Perform Inputs::patchCut (core::UniqueId u)
{
    return [u]() { core::inputs_patchCut (u); };
}

Perform Inputs::patchCopy (core::UniqueId u)
{
    return [u]() { core::inputs_patchCopy (u); };
}

Perform Inputs::patchPaste (core::UniqueId u, core::Point::Real pt)
{
    return [u, pt]() { core::inputs_patchPaste (u, pt); };
}

Perform Inputs::patchDuplicate (core::UniqueId u)
{
    return [u]() { core::inputs_patchDuplicate (u); };
}

Perform Inputs::patchRemove (core::UniqueId u)
{
    return [u]() { core::inputs_patchRemove (u); };
}

Perform Inputs::patchEncapsulate (core::UniqueId u)
{
    return [u]() { core::inputs_patchEncapsulate (u); };
}

Perform Inputs::patchDeencapsulate (core::UniqueId u)
{
    return [u]() { core::inputs_patchDeencapsulate (u); };
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Perform Inputs::patchCreateObject (core::UniqueId u, core::Point::Real pt, juce::String s)
{
    return [u, pt, s]() { core::inputs_patchCreateObject (u, pt, s); };
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Perform Inputs::objectSelect (core::UniqueId u)
{
    return [u]() { core::inputs_objectSelect (u); };
}

Perform Inputs::objectDeselect (core::UniqueId u)
{
    return [u]() { core::inputs_objectDeselect (u); };
}

Perform Inputs::objectMoveBack (core::UniqueId u)
{
    return [u]() { core::inputs_objectMoveBack (u); };
}

Perform Inputs::objectMoveFront (core::UniqueId u)
{
    return [u]() { core::inputs_objectMoveFront (u); };
}

Perform Inputs::objectSnap (core::UniqueId u)
{
    return [u]() { core::inputs_objectSnap (u); };
}

Perform Inputs::objectPosition (core::UniqueId u, core::Point::Real pt)
{
    return [u, pt]() { core::inputs_objectPosition (u, pt); };
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Perform Inputs::objectSendBang (core::UniqueId u)
{
    return [u]() { core::inputs_objectSendBang (u); };
}

Perform Inputs::objectSendFloat (core::UniqueId u, double f)
{
    return [u, f]() { core::inputs_objectSendFloat (u, f); };
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Perform Inputs::objectOpenHelp (core::UniqueId u)
{
    return [u]() { core::inputs_objectOpenHelp (u); };
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Perform Inputs::objectSetParameters (core::UniqueId u, data::Group group)
{
    return [u, g = std::move (group)]() { core::inputs_objectSetParameters (u, g); };
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Perform Inputs::lineConnect (core::UniqueId u, int m, core::UniqueId v, int n)
{
    return [u, m, v, n]() { core::inputs_lineConnect (u, m, v, n); };
}

Perform Inputs::lineDisconnect (core::UniqueId u, int m, core::UniqueId v, int n)
{
    return [u, m, v, n]() { core::inputs_lineDisconnect (u, m, v, n); };
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
