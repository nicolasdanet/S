
/* Copyright (c) 2021 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Perform Inputs::newPatch (juce::File file)
{
    return [f = std::move (file)]() { core::inputs_patchNew (f); };
}

Perform Inputs::openPatch (juce::File file)
{
    return [f = std::move (file)]() { core::inputs_patchOpen (f); };
}

Perform Inputs::rescanSearchPaths (Logged type)
{
    return [logged = loggedAsInteger (type)]() { core::inputs_searchPathsRescan (logged); };
}

Perform Inputs::switchDsp()
{
    return []() { core::inputs_dspSwitch(); };
}

Perform Inputs::setSearchPaths (juce::StringArray paths)
{
    return [p = std::move (paths)]() { core::inputs_searchPathsSet (p); };
}

Perform Inputs::setSnapToGrid (bool isSet)
{
    return [isSet]() { core::inputs_snapSet (isSet); };
}

Perform Inputs::setSnapToGridSize (int size)
{
    return [size]() { core::inputs_snapSetGrid (size); };
}

Perform Inputs::rescanDevices (Logged type)
{
    return [logged = loggedAsInteger (type)]() { core::inputs_devicesRescan (logged); };
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Perform Inputs::closePatch (core::UniqueId u)
{
    return [u]() { core::inputs_patchClose (u); };
}

Perform Inputs::savePatch (core::UniqueId u)
{
    return [u]() { core::inputs_patchSave (u); };
}

Perform Inputs::setDirty (core::UniqueId u)
{
    return [u]() { core::inputs_patchSetDirty (u); };
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Perform Inputs::setEditView (core::UniqueId u, juce::Rectangle<int> bounds)
{
    return [u, bounds]() { core::inputs_patchSetEditView (u, bounds); };
}

Perform Inputs::setEditViewProperties (core::UniqueId u, core::Point::Real pt, int z, bool inspector, int w)
{
    return [u, pt, z, inspector, w]() { core::inputs_patchSetEditViewProperties (u, pt, z, inspector, w); };
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Perform Inputs::undo (core::UniqueId u)
{
    return [u]() { core::inputs_patchUndo (u); };
}

Perform Inputs::redo (core::UniqueId u)
{
    return [u]() { core::inputs_patchRedo (u); };
}

Perform Inputs::cut (core::UniqueId u)
{
    return [u]() { core::inputs_patchCut (u); };
}

Perform Inputs::copy (core::UniqueId u)
{
    return [u]() { core::inputs_patchCopy (u); };
}

Perform Inputs::paste (core::UniqueId u, core::Point::Real pt)
{
    return [u, pt]() { core::inputs_patchPaste (u, pt); };
}

Perform Inputs::duplicate (core::UniqueId u)
{
    return [u]() { core::inputs_patchDuplicate (u); };
}

Perform Inputs::remove (core::UniqueId u)
{
    return [u]() { core::inputs_patchRemove (u); };
}

Perform Inputs::encapsulate (core::UniqueId u)
{
    return [u]() { core::inputs_patchEncapsulate (u); };
}

Perform Inputs::deencapsulate (core::UniqueId u)
{
    return [u]() { core::inputs_patchDeencapsulate (u); };
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Perform Inputs::selectObject (core::UniqueId u)
{
    return [u]() { core::inputs_objectSelect (u); };
}

Perform Inputs::deselectObject (core::UniqueId u)
{
    return [u]() { core::inputs_objectDeselect (u); };
}

Perform Inputs::moveBackObject (core::UniqueId u)
{
    return [u]() { core::inputs_objectMoveBack (u); };
}

Perform Inputs::moveFrontObject (core::UniqueId u)
{
    return [u]() { core::inputs_objectMoveFront (u); };
}

Perform Inputs::snapObject (core::UniqueId u)
{
    return [u]() { core::inputs_objectSnap (u); };
}

Perform Inputs::positionObject (core::UniqueId u, core::Point::Real pt)
{
    return [u, pt]() { core::inputs_objectPosition (u, pt); };
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Perform Inputs::sendObjectBang (core::UniqueId u)
{
    return [u]() { core::inputs_objectSendBang (u); };
}

Perform Inputs::sendObjectFloat (core::UniqueId u, double f)
{
    return [u, f]() { core::inputs_objectSendFloat (u, f); };
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Perform Inputs::openHelp (core::UniqueId u)
{
    return [u]() { core::inputs_objectOpenHelp (u); };
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Perform Inputs::createObject (core::UniqueId u, core::Point::Real pt, juce::String s)
{
    return [u, pt, s]() { core::inputs_patchCreateObject (u, pt, s); };
}

Perform Inputs::parametersObject (core::UniqueId u, data::Group group)
{
    return [u, g = std::move (group)]() { core::inputs_objectSetParameters (u, g); };
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Perform Inputs::disconnectLine (core::UniqueId u, int m, core::UniqueId v, int n)
{
    return [u, m, v, n]() { core::inputs_lineDisconnect (u, m, v, n); };
}

Perform Inputs::connectLine (core::UniqueId u, int m, core::UniqueId v, int n)
{
    return [u, m, v, n]() { core::inputs_lineConnect (u, m, v, n); };
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
