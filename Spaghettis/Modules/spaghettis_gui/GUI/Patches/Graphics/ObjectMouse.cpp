
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void openSubPatch (View* v, const core::Object& o)
{
    if (o.isPatch()) { v->getPatch().openSubPatchWindow (o); }
}

void deselectAll (View* v)
{

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ObjectComponent::mouseMove (const juce::MouseEvent&)
{
    if (!isInsideRunView()) {
    //
    //
    }
}

void ObjectComponent::mouseEnter (const juce::MouseEvent&)
{
    if (!isInsideRunView()) {
    //
    //
    }
}

void ObjectComponent::mouseExit (const juce::MouseEvent&)
{
    if (!isInsideRunView()) {
    //
    //
    }
}

void ObjectComponent::mouseDown (const juce::MouseEvent& e)
{
    if (!isInsideRunView()) {
    //
    if (e.mods.isLeftButtonDown()) {
        if (e.getNumberOfClicks() == 1) {
            if (e.mods.isShiftDown() == false) { deselectAll (view_); }
            if (e.mods.isShiftDown() && selected_.get()) { EditCommands::deselect (object_); }
            else {
                EditCommands::select (object_);
            }
        } else if (e.getNumberOfClicks() == 2) {
            openSubPatch (view_, object_);
        }
    }
    //
    }
}

void ObjectComponent::mouseDrag (const juce::MouseEvent&)
{
    if (!isInsideRunView()) {
    //
    //
    }
}

void ObjectComponent::mouseUp (const juce::MouseEvent&)
{
    if (!isInsideRunView()) {
    //
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
