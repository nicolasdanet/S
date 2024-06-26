
/* Copyright (c) 2022 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

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
    updateComponent (juce::Rectangle<int> (e.getMouseDownPosition(), e.getPosition()));
}

void ActionLasso::mouseUp (const juce::MouseEvent&)
{
    if (lassoComponent_) {
        view_->removeChildComponent (lassoComponent_.get());
        view_->select (lassoComponent_->getBounds());
        lassoComponent_ = nullptr;
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
