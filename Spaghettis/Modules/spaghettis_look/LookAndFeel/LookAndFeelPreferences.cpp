
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Compute the area for the component by removing a placeholder for the label. */

juce::Rectangle<int> LookAndFeel::getPropertyComponentContentPosition (juce::PropertyComponent& c)
{
    juce::Rectangle<int> r (c.getWidth(), c.getHeight() - 1);
        
    ParameterBase* w = dynamic_cast<ParameterBase*> (&c);
    const int requiredWidth = w ? w->getRequiredWidthForLabel() : 250;
    const int labelWidth    = juce::jmin (requiredWidth, r.getWidth());
        
    return r.withTrimmedLeft (labelWidth);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Draw the background of the label. */

void LookAndFeel::drawPropertyComponentBackground (juce::Graphics& g,
    int w,
    int h,
    juce::PropertyComponent& c)
{
    const juce::Rectangle<int> r (getPropertyComponentContentPosition (c));
    
    const int width = r.getX();     /* Compute its width from the component's offset. */
    
    g.setColour (findColour (Colours::parametersParameterBackground));
    g.fillRect (juce::Rectangle<int> (width, h).withTrimmedBottom (1).withTrimmedRight (2));
}

/* Draw the label. */

void LookAndFeel::drawPropertyComponentLabel (juce::Graphics& g,
    int w,
    int h,
    juce::PropertyComponent& c)
{
    const juce::Rectangle<int> r (getPropertyComponentContentPosition (c));
    
    const int width = r.getX();     /* Compute its width from the component's offset. */
    
    const juce::Colour t (findColour (Colours::parametersParameterText));
    
    g.setColour (c.isEnabled() ? t : t.withAlpha (0.25f));
    g.setFont (getConsoleFont());
    g.drawText (c.getName(),
        juce::Rectangle<int> (width, h).reduced (4, 0).withTrimmedLeft (30),
        juce::Justification::centredLeft,
        true);
}

int LookAndFeel::getPropertyPanelSectionHeaderHeight (const juce::String& s)
{
    if (s.isEmpty()) { return 0; }
    else {
        return static_cast<int> (getConsoleFont().getHeight() * 1.5);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void LookAndFeel::drawToggleButton (juce::Graphics& g, juce::ToggleButton& b, bool, bool)
{
    const int   h = b.getHeight();
    const float t = h * 0.6f;
    
    const juce::Rectangle<float> r (juce::Rectangle<float> (h, h).withSizeKeepingCentre (t, t));

    g.setColour (findColour (Colours::parametersBoolean));
    g.drawRoundedRectangle (r, 4.0f, 1.0f);
    
    if (b.getToggleState()) {
        g.setColour (findColour (Colours::parametersBooleanTick));
        LookAndFeel::drawTick (g, r.toNearestInt().reduced (4, 5));
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void LookAndFeel::drawLabel (juce::Graphics& g, juce::Label& l)
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

juce::Font LookAndFeel::getLabelFont (juce::Label&)
{
    return getConsoleFont();
}

juce::BorderSize<int> LookAndFeel::getLabelBorderSize (juce::Label&)
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

void LookAndFeel::drawLinearSlider (juce::Graphics& g,
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
        const juce::Colour background = findColour (Colours::parametersSliderBackground);
        const juce::Colour track      = findColour (Colours::parametersSliderTrack);
        drawLinearSliderHorizontalBar (g, x, y, w, h, position, min, max, style, slider, background, track);
    } else {
        drawLinearSliderHorizontal (g, x, y, w, h, position, min, max, style, slider);
    }
    //
    } else { juce::LookAndFeel_V4::drawLinearSlider (g, x, y, w, h, position, min, max, style, slider); }
}
                                
juce::Label* LookAndFeel::createSliderTextBox (juce::Slider& slider)
{
    return new SliderLabel();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------