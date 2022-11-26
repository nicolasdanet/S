
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
    Spaghettis()->handle (Inputs::undo (i));
}

static void redo (core::UniqueId i)
{
    Spaghettis()->handle (Inputs::redo (i));
}

static void cut (core::UniqueId i)
{
    Spaghettis()->handle (Inputs::cut (i));
}

static void copy (core::UniqueId i)
{
    Spaghettis()->handle (Inputs::copy (i));
}

static void paste (core::UniqueId i, juce::Point<int> pt)
{
    Spaghettis()->handle (Inputs::paste (i, pt));
}

static void duplicate (core::UniqueId i)
{
    Spaghettis()->handle (Inputs::duplicate (i));
}

static void remove (core::UniqueId i)
{
    Spaghettis()->handle (Inputs::remove (i));
}

static void encapsulate (core::UniqueId i)
{
    Spaghettis()->handle (Inputs::encapsulate (i));
}

static void deencapsulate (core::UniqueId i)
{
    Spaghettis()->handle (Inputs::deencapsulate (i));
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
    Spaghettis()->handle (Inputs::positionObject (i, Coordinates::localToReal (pt)));
}

static void parameter (core::UniqueId i, const core::Group& group)
{
    Spaghettis()->handle (Inputs::parameterObject (i, core::Group::createCopy (group)));
}

static void disconnect (core::UniqueId u, int m, core::UniqueId v, int n)
{
    Spaghettis()->handle (Inputs::disconnectLine (u, m, v, n));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

