
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void ObjectComponent::mouseMove (const juce::MouseEvent&)
{
    if (!isRunView_) {
    //
    DBG ("MOVE");
    //
    }
}

void ObjectComponent::mouseEnter (const juce::MouseEvent&)
{
    if (!isRunView_) {
    //
    DBG ("ENTER");
    //
    }
}

void ObjectComponent::mouseExit (const juce::MouseEvent&)
{
    if (!isRunView_) {
    //
    DBG ("EXIT");
    //
    }
}

void ObjectComponent::mouseDown (const juce::MouseEvent&)
{
    if (!isRunView_) {
    //
    DBG ("DOWN");
    //
    }
    
    // EditCommands::selectObject (p_->object_);
}

void ObjectComponent::mouseDrag (const juce::MouseEvent&)
{
    if (!isRunView_) {
    //
    DBG ("DRAG");
    //
    }
}

void ObjectComponent::mouseUp (const juce::MouseEvent&)
{
    if (!isRunView_) {
    //
    DBG ("UP");
    //
    }
}
    
void ObjectComponent::mouseDoubleClick (const juce::MouseEvent&)
{
    if (!isRunView_) {
    //
    DBG ("DBL CLICK");
    //
    }
    
    // if (p_->object_.isPatch()) { p_->view_->getPatch().openSubPatchWindow (p_->object_); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
