
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
    const int wHue    = 40;
    const int hSpace  = static_cast<int> (h * 0.75);
    
    hueSelector_->setBounds (area.removeFromRight (wHue));
    colourSpace_->setBounds (area.removeFromTop (hSpace));
    
    area.removeFromBottom (edge_);
    
    const int hSilder = area.getHeight() / sliders_.size();
    
    for (auto& slider : sliders_) {
        slider->setBounds (area.removeFromTop (hSilder));
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ColourSelector::updateColour()
{
    colour_ = LookAndFeel::getColourFromValue (value_);
    
    colour_.getHSB (h_, s_, v_); a_ = colour_.getFloatAlpha();
}

void ColourSelector::updateViews()
{
    colourSpace_->update();
    hueSelector_->update();
    
    std::get<0> (sliders_)->setValue (static_cast<int> (colour_.getAlpha()), juce::dontSendNotification);
    std::get<1> (sliders_)->setValue (static_cast<int> (colour_.getRed()),   juce::dontSendNotification);
    std::get<2> (sliders_)->setValue (static_cast<int> (colour_.getGreen()), juce::dontSendNotification);
    std::get<3> (sliders_)->setValue (static_cast<int> (colour_.getBlue()),  juce::dontSendNotification);
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
    if (c != colour_) { LookAndFeel::setValueWithColour (value_, c); update(); }
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
    jassert (std::get<0> (sliders_) != nullptr);
    
    const juce::uint8 a = static_cast<juce::uint8> (std::get<0> (sliders_)->getValue());
    const juce::uint8 r = static_cast<juce::uint8> (std::get<1> (sliders_)->getValue());
    const juce::uint8 g = static_cast<juce::uint8> (std::get<2> (sliders_)->getValue());
    const juce::uint8 b = static_cast<juce::uint8> (std::get<3> (sliders_)->getValue());
    
    setColour (juce::Colour (r, g, b, a));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
