
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

ActionConnectComponent::ActionConnectComponent (core::Point::Scaled pt) :
    start_ (pt),
    end_ (pt),
    connectColour_ (Spaghettis()->getCachedColour (Tag::Lasso))
{
    setOpaque (false);
    setAlwaysOnTop (true);
    setInterceptsMouseClicks (false, true);
    update();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ActionConnectComponent::paint (juce::Graphics& g)
{
    const juce::Point<float> a (getLocalPoint (getParentComponent(), start_.getPoint()).toFloat());
    const juce::Point<float> b (getLocalPoint (getParentComponent(), end_.getPoint()).toFloat());
    
    g.setColour (connectColour_.get());
    
    const float dash[] = { 5.0f, 2.0f };
    
    g.drawDashedLine (juce::Line<float> (a, b), dash, juce::numElementsInArray (dash), 2.0f, 0);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ActionConnectComponent::set (core::Point::Scaled pt)
{
    end_ = pt; update(); repaint();
}

void ActionConnectComponent::update()
{
    setBounds (juce::Rectangle<int> (start_.getPoint(), end_.getPoint()).expanded (2));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
