
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

ActionConnect::ActionConnect (EditView* view) : view_ (view)
{
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ActionConnect::createComponent (const juce::Rectangle<int>& r)
{
    const int area = r.getWidth() * r.getHeight();
    const int k = 4;
    
    if (area > k) {
        connectComponent_ = std::make_unique<ActionConnectComponent> (r);
        view_->addAndMakeVisible (connectComponent_.get());
    }
}

void ActionConnect::updateComponent (const juce::Rectangle<int>& r)
{
    if (connectComponent_) { connectComponent_->setBounds (r); }
    else {
        createComponent (r);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ActionConnect::mouseDrag (const juce::MouseEvent& e)
{
    const juce::Point<int> a (e.getMouseDownPosition());
    const juce::Point<int> b (a + e.getOffsetFromDragStart());
    const juce::Rectangle<int> r (a, b);
    
    updateComponent (r);
}

void ActionConnect::mouseUp (const juce::MouseEvent& e)
{
    connectComponent_ = nullptr;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
