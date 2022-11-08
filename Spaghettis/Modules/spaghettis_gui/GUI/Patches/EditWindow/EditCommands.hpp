
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

struct EditCommands {

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

static void undo (core::UniqueId i)
{
    Spaghettis()->handle (Inputs::undoPatch (i));
}

static void redo (core::UniqueId i)
{
    Spaghettis()->handle (Inputs::redoPatch (i));
}

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

static void position (core::UniqueId i, juce::Point<int> pt)
{
    Spaghettis()->handle (Inputs::positionObject (i, core::Canvas::removeOffset (pt)));
}

static void parameter (core::UniqueId i, const core::Group& group)
{
    Spaghettis()->handle (Inputs::parameterObject (i, core::Group::createCopy (group)));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

