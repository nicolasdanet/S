
/* Copyright (c) 2022 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Compute the area for the component by removing a placeholder for the label. */

juce::Rectangle<int> LNF::getPropertyComponentContentPosition (juce::PropertyComponent& c)
{
    juce::Rectangle<int> r (c.getWidth(), c.getHeight() - 1);
        
    PropertyLookAndFeel* w = dynamic_cast<PropertyLookAndFeel*> (&c);
    const int requiredWidth = w ? w->getRequiredWidth() : 250;
    const int labelWidth    = juce::jmin (requiredWidth, r.getWidth());
        
    return r.withTrimmedLeft (labelWidth);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Draw the background of the label. */

void LNF::drawPropertyComponentBackground (juce::Graphics& g,
    int w,
    int h,
    juce::PropertyComponent& c)
{
    const juce::Rectangle<int> r (getPropertyComponentContentPosition (c));
    
    const int width = r.getX();     /* Compute its width from the component's offset. */
    
    g.setColour (Colours::fetchColour (Colours::parametersParameterBackground));
    g.fillRect (juce::Rectangle<int> (width, h).withTrimmedBottom (1).withTrimmedRight (2));
}

/* Draw the label. */

void LNF::drawPropertyComponentLabel (juce::Graphics& g,
    int w,
    int h,
    juce::PropertyComponent& c)
{
    const juce::Rectangle<int> r (getPropertyComponentContentPosition (c));
    
    const int width = r.getX();     /* Compute its width from the component's offset. */
    
    const juce::Colour t (Colours::fetchColour (Colours::parametersParameterText));
    
    g.setColour (c.isEnabled() ? t : t.withAlpha (0.25f));
    g.setFont (Fonts::getFont());
    g.drawText (c.getName(),
        juce::Rectangle<int> (width, h).reduced (4, 0).withTrimmedLeft (30),
        juce::Justification::centredLeft,
        true);
}

int LNF::getPropertyPanelSectionHeaderHeight (const juce::String& s)
{
    if (s.isEmpty()) { return 0; }
    else {
        return static_cast<int> (Fonts::getFont().getHeight() * 1.5);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void drawToggleButtonTick (juce::Graphics& g, juce::ToggleButton& b, int w)
{
    const float t = w * 0.6f;
    
    const juce::Rectangle<float> r (juce::Rectangle<float> (w, w).withSizeKeepingCentre (t, t));

    g.setColour (Colours::fetchColour (Colours::parametersBoolean));
    g.drawRoundedRectangle (r, 4.0f, 1.0f);
    
    if (b.getToggleState()) {
        g.setColour (Colours::fetchColour (Colours::parametersBooleanTick));
        LNF::drawTick (g, r.toNearestInt().reduced (4, 5));
    }
}

void drawToggleButtonText (juce::Graphics& g, juce::ToggleButton& b, int w)
{
    const juce::String text (b.getButtonText());
    
    if (text.isNotEmpty()) {
    //
    juce::Rectangle<int> r (b.getLocalBounds().withTrimmedLeft (w + 2).withTrimmedRight (2));
    
    g.setColour (Colours::fetchColour (Colours::parametersBooleanText));
    g.setFont (Fonts::getFont());
    g.drawText (b.getButtonText(), r, juce::Justification::centredLeft, true);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void LNF::drawToggleButton (juce::Graphics& g, juce::ToggleButton& b, bool, bool)
{
    const int w = b.getHeight();

    ChoicesButton* d = dynamic_cast<ChoicesButton*> (&b);
    
    if (d) { DBG ("CHOICES BUTTON"); }
    
    drawToggleButtonTick (g, b, w);
    drawToggleButtonText (g, b, w);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void LNF::drawLabel (juce::Graphics& g, juce::Label& l)
{
    g.fillAll (l.findColour (juce::Label::backgroundColourId));

    if (l.isBeingEdited() == false) {
    //
    const juce::Font font (getLabelFont (l));
    const juce::Rectangle<int> area (getLabelBorderSize (l).subtractedFrom (l.getLocalBounds()));

    g.setColour (l.findColour (juce::Label::textColourId));
    g.setFont (font);
    g.drawText (l.getText(), area, l.getJustificationType(), true);
    //
    }
}

juce::Font LNF::getLabelFont (juce::Label&)
{
    return Fonts::getFont();
}

juce::BorderSize<int> LNF::getLabelBorderSize (juce::Label&)
{
    /* Avoid the text to drift down when editing in preferences. */
    
    return juce::BorderSize<int> { 3, 5, 0, 5 };
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

juce::Path getLineWithStartAndEnd (juce::Point<float> a, juce::Point<float> b)
{
    juce::Path path;
    path.startNewSubPath (a);
    path.lineTo (b);
    return path;
}

void drawLinearSliderHorizontalBar (juce::Graphics& g,
    int x,
    int y,
    int w,
    int h,
    float position,
    float min,
    float max,
    const juce::Slider::SliderStyle style,
    juce::Slider& slider,
    const juce::Colour& background,
    const juce::Colour& track)
{
    const juce::Rectangle<int> r (x, y, w, h);
    
    g.setColour (background);
    g.fillRect (r);
    g.setColour (track);
    g.fillRect (r.reduced (0, 1).withTrimmedRight (static_cast<int> (w - position)));
}

void drawLinearSliderHorizontal (juce::Graphics& g,
    int x,
    int y,
    int w,
    int h,
    float position,
    float min,
    float max,
    const juce::Slider::SliderStyle style,
    juce::Slider& slider)
{
    const float thickness = juce::jmin (6.0f, h * 0.25f);
    const float x1 = static_cast<float> (x);
    const float x2 = static_cast<float> (x + w);
    const float m  = static_cast<float> (y + h * 0.5f);
    
    const juce::Point<float> a (x1, m);
    const juce::Point<float> b (x2, m);
    const juce::Point<float> p (position, m);

    const juce::PathStrokeType type (thickness, juce::PathStrokeType::curved, juce::PathStrokeType::rounded);
    
    g.setColour (slider.findColour (juce::Slider::backgroundColourId));
    g.strokePath (getLineWithStartAndEnd (a, b), type);
    
    g.setColour (slider.findColour (juce::Slider::trackColourId));
    g.strokePath (getLineWithStartAndEnd (a, p), type);
    
    g.setColour (slider.findColour (juce::Slider::thumbColourId));
    g.fillRect (juce::Rectangle<float> (thickness, thickness * 2).withCentre (p));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void LNF::drawLinearSlider (juce::Graphics& g,
    int x,
    int y,
    int w,
    int h,
    float position,
    float min,
    float max,
    const juce::Slider::SliderStyle style,
    juce::Slider& slider)
{
    if (slider.isHorizontal() && !slider.isTwoValue() && !slider.isThreeValue()) {
    //
    if (slider.isBar()) {
        const juce::Colour background = Colours::fetchColour (Colours::parametersSliderBackground);
        const juce::Colour track      = Colours::fetchColour (Colours::parametersSliderTrack);
        drawLinearSliderHorizontalBar (g, x, y, w, h, position, min, max, style, slider, background, track);
    } else {
        drawLinearSliderHorizontal (g, x, y, w, h, position, min, max, style, slider);
    }
    //
    } else { juce::LookAndFeel_V4::drawLinearSlider (g, x, y, w, h, position, min, max, style, slider); }
}
                                
juce::Label* LNF::createSliderTextBox (juce::Slider& slider)
{
    return new SliderLabel();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
