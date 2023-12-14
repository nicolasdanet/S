
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

ColourSelector::ColourSelector (const juce::Value& v) :
    value_ (v),
    h_ (0.0f),
    s_ (0.0f),
    v_ (0.0f),
    a_ (0.0f),
    colourSpace_ (std::make_unique<ColourSpace> (*this, margin_, h_, s_, v_)),
    hueSelector_ (std::make_unique<HueSelector> (*this, margin_, h_))
{
    getColour(); updateHSV();
    
    std::get<0> (sliders_).reset (new ColourSlider());
    std::get<1> (sliders_).reset (new ColourSlider());
    std::get<2> (sliders_).reset (new ColourSlider());
    std::get<3> (sliders_).reset (new ColourSlider());

    for (auto& slider : sliders_) {
        slider->onValueChange = [this] { setColour(); };
    }
    
    addAndMakeVisible (colourSpace_.get());
    addAndMakeVisible (hueSelector_.get());

    addAndMakeVisible (std::get<0> (sliders_).get());
    addAndMakeVisible (std::get<1> (sliders_).get());
    addAndMakeVisible (std::get<2> (sliders_).get());
    addAndMakeVisible (std::get<3> (sliders_).get());
    
    setOpaque (true); setSize (300, 280);
    
    updateViews();
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ColourSelector::paint (juce::Graphics& g)
{
    g.fillAll (findColour (Colours::callOutBoxBackground));
}
    
void ColourSelector::resized()
{
    juce::Rectangle<int> area (getLocalBounds());
    
    const int h       = area.getHeight();
    const int wHue    = 40;
    const int hSpace  = static_cast<int> (h * 0.75);
    
    hueSelector_->setBounds (area.removeFromRight (wHue));
    colourSpace_->setBounds (area.removeFromTop (hSpace));
    
    area.removeFromBottom (margin_);
    
    const int hSilder = area.getHeight() / sliders_.size();
    
    for (auto& slider : sliders_) {
        slider->setBounds (area.removeFromTop (hSilder));
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ColourSelector::getColour()
{
    colour_ = Colours::getColourFromValue (value_);
}

void ColourSelector::pushColour()
{
    Colours::setValueWithColour (value_, colour_);
}

void ColourSelector::updateHSV()
{
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

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ColourSelector::setHue (float h)
{
    h = juce::jlimit (0.0f, 1.0f, h);

    if (h != h_) {
    //
    h_ = h;
    
    colour_ = juce::Colour (h_, s_, v_, a_); updateViews(); pushColour();
    //
    }
}

void ColourSelector::setSV (float s, float v)
{
    s = juce::jlimit (0.0f, 1.0f, s);
    v = juce::jlimit (0.0f, 1.0f, v);

    if (s != s_ || v != v_) {
    //
    s_ = s; v_ = v;
    
    colour_ = juce::Colour (h_, s_, v_, a_); updateViews(); pushColour();
    //
    }
}

void ColourSelector::setColour()
{
    jassert (std::get<0> (sliders_) != nullptr);
    
    const juce::uint8 a = static_cast<juce::uint8> (std::get<0> (sliders_)->getValue());
    const juce::uint8 r = static_cast<juce::uint8> (std::get<1> (sliders_)->getValue());
    const juce::uint8 g = static_cast<juce::uint8> (std::get<2> (sliders_)->getValue());
    const juce::uint8 b = static_cast<juce::uint8> (std::get<3> (sliders_)->getValue());
    
    colour_ = juce::Colour (r, g, b, a); updateHSV(); updateViews(); pushColour();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
