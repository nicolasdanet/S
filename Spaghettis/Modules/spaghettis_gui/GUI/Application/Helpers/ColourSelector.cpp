
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
    juce::Rectangle<int> area (getLocalBounds());
    
    const int h       = area.getHeight();
    const int hSpace  = static_cast<int> (area.getHeight() * 0.75);
    const int hSilder = static_cast<int> ((h - hSpace) / sliders_.size());
    
    colourSpace_->setBounds (area.removeFromTop (hSpace));
    
    for (auto& slider : sliders_) {
        slider->setBounds (area.removeFromTop (hSilder));
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ColourSelector::updateColour()
{
    const juce::Colour c (LookAndFeel::getColourFromValue (value_));
    
    c.getHSB (h_, s_, v_); a_ = c.getFloatAlpha();
}

void ColourSelector::updateViews()
{
    const juce::Colour c (LookAndFeel::getColourFromValue (value_));
    
    sliders_[0]->setValue (static_cast<int> (c.getRed()),   juce::dontSendNotification);
    sliders_[1]->setValue (static_cast<int> (c.getGreen()), juce::dontSendNotification);
    sliders_[2]->setValue (static_cast<int> (c.getBlue()),  juce::dontSendNotification);
    sliders_[3]->setValue (static_cast<int> (c.getAlpha()), juce::dontSendNotification);
    
    colourSpace_->update();
}

void ColourSelector::update()
{
    updateColour(); updateViews();
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
// MARK: -

void ColourSelector::changeColour()
{
    jassert (sliders_[0] != nullptr);
    
    const juce::uint8 r = static_cast<juce::uint8> (sliders_[0]->getValue());
    const juce::uint8 g = static_cast<juce::uint8> (sliders_[1]->getValue());
    const juce::uint8 b = static_cast<juce::uint8> (sliders_[2]->getValue());
    const juce::uint8 a = static_cast<juce::uint8> (sliders_[3]->getValue());
    
    setColour (juce::Colour (r, g, b, a));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
