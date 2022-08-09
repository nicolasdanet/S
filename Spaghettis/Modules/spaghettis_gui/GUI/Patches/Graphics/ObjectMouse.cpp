
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
    view_->mouseDown (e);
    
    if (Mouse::isClick (e))            { EditCommands::select (object_.getIdentifier()); }
    else if (Mouse::isDoubleClick (e)) { openSubPatch (view_, object_); }
    else if (Mouse::isShiftClick (e))  {
        if (selected_.get()) { EditCommands::deselect (object_.getIdentifier()); }
        else {
            EditCommands::select (object_.getIdentifier());
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
