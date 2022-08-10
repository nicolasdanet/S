
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

void openSubPatch (const core::Object& o, View* v)
{
    if (o.isPatch()) { v->getPatch().openSubPatchWindow (o); }
}

void toggleSelection (const core::Object& o, bool isSelected)
{
    if (isSelected) { EditCommands::deselect (o.getIdentifier()); }
    else {
        EditCommands::select (o.getIdentifier());
    }
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
    if (Mouse::isCommandClick (e))    { painter_->mouseDown (e); }
    else {
    //
    view_->mouseDown (e);
    
    if (Mouse::isDoubleClick (e))     { openSubPatch (object_, view_);                  }
    else if (Mouse::isShiftClick (e)) { toggleSelection (object_, selected_.get());     }
    else if (Mouse::isClick (e))      { EditCommands::select (object_.getIdentifier()); }
    //
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
