
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
    
    if (Mouse::isDoubleClick (e))     { openSubPatch (object_, view_);  }
    else if (Mouse::isShiftClick (e)) { setSelected (!selected_.get()); }
    else if (Mouse::isClick (e))      { setSelected (true); }
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
// MARK: -

void ObjectComponent::setSelected (bool isSelected)
{
    if (isSelected) { EditCommands::select (object_.getIdentifier()); }
    else {
        EditCommands::deselect (object_.getIdentifier());
    }
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
