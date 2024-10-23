
/* Copyright (c) 2022 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

struct Broadcast {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void undo (core::UniqueId i)
{
    Spaghettis()->handle (Inputs::patchUndo (i));
}

static void redo (core::UniqueId i)
{
    Spaghettis()->handle (Inputs::patchRedo (i));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void select (core::UniqueId i)
{
    Spaghettis()->handle (Inputs::selectObject (i));
}

static void deselect (core::UniqueId i)
{
    Spaghettis()->handle (Inputs::deselectObject (i));
}

static void cut (core::UniqueId i)
{
    Spaghettis()->handle (Inputs::patchCut (i));
}

static void copy (core::UniqueId i)
{
    Spaghettis()->handle (Inputs::patchCopy (i));
}

static void paste (core::UniqueId i, core::Point::Real pt)
{
    Spaghettis()->handle (Inputs::patchPaste (i, pt));
}

static void duplicate (core::UniqueId i)
{
    Spaghettis()->handle (Inputs::patchDuplicate (i));
}

static void remove (core::UniqueId i)
{
    Spaghettis()->handle (Inputs::patchRemove (i));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void disconnect (core::UniqueId u, int m, core::UniqueId v, int n)
{
    Spaghettis()->handle (Inputs::disconnectLine (u, m, v, n));
}

static void connect (core::UniqueId u, int m, core::UniqueId v, int n)
{
    Spaghettis()->handle (Inputs::connectLine (u, m, v, n));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void encapsulate (core::UniqueId i)
{
    Spaghettis()->handle (Inputs::patchEncapsulate (i));
}

static void deencapsulate (core::UniqueId i)
{
    Spaghettis()->handle (Inputs::patchDeencapsulate (i));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void moveBack (core::UniqueId i)
{
    Spaghettis()->handle (Inputs::moveBackObject (i));
}

static void moveFront (core::UniqueId i)
{
    Spaghettis()->handle (Inputs::moveFrontObject (i));
}

static void snap (core::UniqueId i)
{
    Spaghettis()->handle (Inputs::snapObject (i));
}

static void position (core::UniqueId i, core::Point::Real pt)
{
    Spaghettis()->handle (Inputs::positionObject (i, pt));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void parameters (core::UniqueId i, const data::Group& copy)
{
    jassert (copy.hasUniqueOwnership());    /* Parameters MUST be a deep copy. */
    
    Spaghettis()->handle (Inputs::parametersObject (i, copy));
}

static void create (core::UniqueId u, core::Point::Real pt, const juce::String& s)
{
    if (s.isNotEmpty()) { Spaghettis()->handle (Inputs::createObject (u, pt, s)); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void dirty (core::UniqueId u)
{
    Spaghettis()->handle (Inputs::patchSetDirty (u));
}

static void editView (core::UniqueId u, const juce::Rectangle<int>& bounds)
{
    Spaghettis()->handle (Inputs::patchSetEditView (u, bounds));
}

static void editViewProperties (core::UniqueId u, core::Point::Real pt, int zoom, bool inspector, int width)
{
    Spaghettis()->handle (Inputs::patchSetEditViewProperties (u, pt, zoom, inspector, width));
}

static void savePatch (core::UniqueId u)
{
    Spaghettis()->handle (Inputs::patchSave (u));
}

static void closePatch (core::UniqueId u)
{
    Spaghettis()->handle (Inputs::patchClose (u));
}

static void openHelp (core::UniqueId u)
{
    Spaghettis()->handle (Inputs::openHelp (u));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void sendBang (core::UniqueId u)
{
    Spaghettis()->handle (Inputs::sendObjectBang (u));
}

static void sendFloat (core::UniqueId u, double f)
{
    Spaghettis()->handle (Inputs::sendObjectFloat (u, f));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

