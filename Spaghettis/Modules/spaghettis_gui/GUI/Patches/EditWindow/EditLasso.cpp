
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

EditLasso::EditLasso (EditView* view) : view_ (view)
{

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditLasso::createComponent (const juce::Rectangle<int>& r)
{
    const int area = r.getWidth() * r.getHeight();
    const int k = 4;
    
    if (area > k) {
        lassoComponent_ = std::make_unique<EditLassoComponent> (r);
        view_->addAndMakeVisible (lassoComponent_.get());
    }
}

void EditLasso::updateComponent (const juce::Rectangle<int>& r)
{
    if (lassoComponent_) { lassoComponent_->setBounds (r); }
    else {
        createComponent (r);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditLasso::mouseDown (const juce::MouseEvent&)
{
    DBG ("DOWN");
}

void EditLasso::mouseDrag (const juce::MouseEvent& e)
{
    const juce::Point<int> a (e.getMouseDownPosition());
    const juce::Point<int> b (a + e.getOffsetFromDragStart());
    const juce::Rectangle<int> r (a, b);
    
    updateComponent (r);
}

void EditLasso::mouseUp (const juce::MouseEvent&)
{
    lassoComponent_ = nullptr;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
