
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ColourSpace::mouseDrag (const juce::MouseEvent& e)
{
    const float x = static_cast<float> (e.x - edge_);
    const float y = static_cast<float> (e.y - edge_);
    const float w = static_cast<float> (getWidth() - (edge_ * 2));
    const float h = static_cast<float> (getHeight() - (edge_ * 2));
    
    const float s = x / w;
    const float v = 1.0f - (y / h);

    owner_.setSV (s, v);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ColourSelector::paint (juce::Graphics& g)
{
    g.fillAll (Spaghettis()->getColour (Colours::callOutBoxBackground));
}
    
void ColourSelector::resized()
{
    colourSpace_->setBounds (getLocalBounds());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ColourSelector::updateColour()
{
    const juce::Colour c (LookAndFeel::getColourFromValue (value_));
    
    c.getHSB (h_, s_, v_); a_ = c.getFloatAlpha();
}

void ColourSelector::updateComponents()
{
    colourSpace_->update();
}

void ColourSelector::update()
{
    updateColour(); updateComponents();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ColourSelector::setColour (const juce::Colour& c)
{
    LookAndFeel::setValueWithColour (value_, c); update();
}

void ColourSelector::setHue (float h)
{
    h = juce::jlimit (0.0f, 1.0f, h);

    if (h != h_) { setColour (juce::Colour (h, s_, v_, a_)); }
}

void ColourSelector::setSV (float s, float v)
{
    s = juce::jlimit (0.0f, 1.0f, s);
    v = juce::jlimit (0.0f, 1.0f, v);

    if (s != s_ || v != v_) { setColour (juce::Colour (h_, s, v, a_)); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
