
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

MousePolicy::MousePolicy (ObjectComponent* owner) : p_ (owner)
{
    jassert (owner);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void MousePolicy::mouseMove (const juce::MouseEvent&)
{
    // DBG ("Move");
}

void MousePolicy::mouseEnter (const juce::MouseEvent&)
{
    // DBG ("Enter");
}

void MousePolicy::mouseExit (const juce::MouseEvent&)
{
    // DBG ("Exit");
}

void MousePolicy::mouseDown (const juce::MouseEvent&)
{
    // DBG ("Down");
    
    EditCommands::selectObject (p_->object_);
}

void MousePolicy::mouseDrag (const juce::MouseEvent&)
{
    // DBG ("Drag");
}

void MousePolicy::mouseUp (const juce::MouseEvent&)
{
    // DBG ("Up");
}
    
void MousePolicy::mouseDoubleClick (const juce::MouseEvent&)
{
    if (p_->object_.isPatch()) { p_->view_->getPatch().openSubPatchWindow (p_->object_); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
