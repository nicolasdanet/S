
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

ActionLasso::ActionLasso (EditView* view) : view_ (view)
{
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ActionLasso::createComponent (const juce::Rectangle<int>& r)
{
    const int area = r.getWidth() * r.getHeight();
    const int k = 4;
    
    if (area > k) {
        lassoComponent_ = std::make_unique<ActionLassoComponent> (r);
        view_->addAndMakeVisible (lassoComponent_.get());
    }
}

void ActionLasso::updateComponent (const juce::Rectangle<int>& r)
{
    if (lassoComponent_) { lassoComponent_->setBounds (r); }
    else {
        createComponent (r);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ActionLasso::mouseDrag (const juce::MouseEvent& e)
{
    const juce::Point<int> a (e.getMouseDownPosition());
    const juce::Point<int> b (a + e.getOffsetFromDragStart());
    const juce::Rectangle<int> r (a, b);
    
    updateComponent (r);
}

void ActionLasso::mouseUp (const juce::MouseEvent&)
{
    if (lassoComponent_) {
    //
    view_->removeChildComponent (lassoComponent_.get());
    view_->select (lassoComponent_->getBounds());
    lassoComponent_ = nullptr;
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
