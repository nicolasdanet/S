
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::String LookAndFeel::initializeFontsBest (const juce::StringArray& check)
{
    if (check.isEmpty() == false) {
    //
    juce::StringArray fonts (juce::Font::findAllTypefaceNames());
    
    for (const auto& f : check) { if (fonts.contains (f)) { return f; } }
    //
    }
    
    return juce::String();
}

void LookAndFeel::initializeFonts()
{
    #if JUCE_LINUX
    
    juce::String fontName (initializeFontsBest (juce::StringArray ("Ubuntu")));
    juce::String fontMonospacedName (initializeFontsBest (juce::StringArray ("Ubuntu Mono")));
    
    #else
    
    juce::String fontName (initializeFontsBest (juce::StringArray()));
    juce::String fontMonospacedName (initializeFontsBest (juce::StringArray()));
    
    #endif
    
    if (fontName.isEmpty())           { fontName           = juce::Font::getDefaultSansSerifFontName();  }
    if (fontMonospacedName.isEmpty()) { fontMonospacedName = juce::Font::getDefaultMonospacedFontName(); }
    
    setDefaultSansSerifTypefaceName (fontName);

    font18_             = juce::Font (fontName, 18.0, juce::Font::plain);
    font18Monospaced_   = juce::Font (fontMonospacedName, 18.0, juce::Font::plain);
    font16Bold_         = juce::Font (fontName, 16.0, juce::Font::bold);

    /* < https://forum.juce.com/t/fonts-need-to-be-initialized-on-rpi > */
    
    font18Name_             = font18_.getTypefacePtr()->getName();
    font18MonospacedName_   = font18Monospaced_.getTypefacePtr()->getName();
    font16BoldName_         = font16Bold_.getTypefacePtr()->getName();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void LookAndFeel::drawMenuBarItem (juce::Graphics& g,
    int width,
    int height,
    int index,
    const juce::String& text,
    bool isMouseOverItem,
    bool isMenuOpen,
    bool,
    juce::MenuBarComponent& m)
{
    const bool highlighted = (isMenuOpen || isMouseOverItem) && m.isEnabled();
        
    if (highlighted) { g.fillAll (findColour (Colours::menubarBackgroundHighlighted)); }

    g.setColour (findColour (Colours::menubarText));
    g.setFont (getMenuBarFont (m, index, text));
    g.drawText (text, 0, 0, width, height, juce::Justification::centred, true);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void LookAndFeel::paintToolbarBackground (juce::Graphics& g, int w, int h, juce::Toolbar& toolbar)
{
    g.fillAll (findColour (Colours::toolbarBackground));
}

void LookAndFeel::paintToolbarButtonBackground (juce::Graphics& g,
    int width,
    int height,
    bool isMouseOver,
    bool isMouseDown,
    juce::ToolbarItemComponent& component)
{
    if (isMouseDown) {
        if (Icons::getInstance().isToggle (component.getItemId()) == false) {
            g.fillAll (findColour (Colours::toolbarBackgroundDown));
        }
        
    } else if (isMouseOver) { g.fillAll (findColour (Colours::toolbarBackgroundOver)); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void LookAndFeel::getIdealPopupMenuItemSize (const juce::String& text,
    const bool isSeparator,
    int,
    int& w,
    int& h)
{
    if (isSeparator) { w = 75; h = 2; }
    else {
        const juce::Font font = getPopupMenuFont();
        const float f = font.getHeight();
        #if JUCE_LINUX
        const float extra = (f * 5.0);      /* Extra space for shortcuts (Ubuntu monospaced is larger). */
        #else
        const float extra = (f * 2.5);
        #endif
        h = static_cast<int> (f * 1.6);
        w = static_cast<int> (font.getStringWidth (text) + extra);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void LookAndFeel::drawPopupMenuItemSelector (juce::Graphics& g, const juce::Rectangle<int>& area)
{
    g.setColour (findColour (Colours::menubarSeparator).withAlpha (0.25f)); g.fillRect (area);
}

void LookAndFeel::drawPopupMenuItemBackground (juce::Graphics& g,
    const juce::Rectangle<int>& area,
    bool  isComboBox)
{
    if (isComboBox) {
        g.setColour (findColour (Colours::devicesPopupBackgroundHighlighted));
    } else {
        g.setColour (findColour (Colours::menubarBackgroundHighlighted));
    }
    
    g.fillRect (area);
}

void LookAndFeel::drawPopupMenuItemTick (juce::Graphics& g, juce::Rectangle<int> t)
{
    const juce::Path path = getTickShape (1.0f);
    g.fillPath (path, path.getTransformToScaleToFit (t.reduced (5).toFloat(), true));
}

void LookAndFeel::drawPopupMenuItemSubMenu (juce::Graphics& g, juce::Rectangle<int>& r)
{
    const int w = static_cast<int> (0.6f * getPopupMenuFont().getAscent());
    
    const juce::Rectangle<int> t (r.removeFromRight (w));
    
    const float x = static_cast<float> (t.getX());
    const float y = static_cast<float> (t.getCentreY());
    const float hArrow = w;
    const float wArrow = w * 0.6f;
    
    juce::Path path;
    path.startNewSubPath (x, y - (hArrow * 0.5f));
    path.lineTo (x + wArrow, y);
    path.lineTo (x, y + (hArrow * 0.5f));

    g.strokePath (path, juce::PathStrokeType (2.0f));
}

void LookAndFeel::drawPopupMenuItemShortcut (juce::Graphics& g,
    const juce::Rectangle<int>& r,
    const juce::String& shortcutText)
{
    g.setFont (getMenuShortcutsFont());
    g.drawText (shortcutText, r, juce::Justification::centredRight, true);
}

void LookAndFeel::drawPopupMenuItemProceed (juce::Graphics& g,
    const juce::Rectangle<int>& area,
    const bool isSeparator,
    const bool isActive,
    const bool isHighlighted,
    const bool isTicked,
    const bool hasSubMenu,
    const bool isComboBox,
    const juce::String& text,
    const juce::String& shortcutText)
{
    if (isSeparator) { drawPopupMenuItemSelector (g, area); }
    else {
    //
    if (isHighlighted && isActive) { drawPopupMenuItemBackground (g, area, isComboBox); }
    
    const int n = isComboBox ? Colours::devicesPopupText : Colours::menubarText;
    const juce::Colour c1 = findColour (n);
    const juce::Colour c2 = c1.withMultipliedAlpha (0.5f);
    
    g.setColour (isActive ? c1 : c2);
    
    const int border = juce::jmin (5, area.getWidth() / 20);
    
    juce::Rectangle<int> r = area.reduced (border, 0);
    juce::Rectangle<int> t = r.removeFromLeft (r.getHeight() * 0.75);
    
    if (isTicked) { drawPopupMenuItemTick (g, std::move (t)); }
    else if (hasSubMenu) {
    //
    drawPopupMenuItemSubMenu (g, r);
    //
    }
    
    r.removeFromRight (3);
    g.setFont (getPopupMenuFont());
    g.drawText (text, r, juce::Justification::centredLeft, true);

    if (shortcutText.isNotEmpty()) { g.setColour (c2); drawPopupMenuItemShortcut (g, r, shortcutText); }
    //
    }
}

void LookAndFeel::drawPopupMenuItemWithOptions (juce::Graphics& g,
    const juce::Rectangle<int>& area,
    bool  isHighlighted,
    const juce::PopupMenu::Item& i,
    const juce::PopupMenu::Options& options)
{
    const bool hasSubMenu = (i.subMenu != nullptr) && (i.itemID == 0 || i.subMenu->getNumItems() > 0);
    const bool isComboBox = (dynamic_cast<juce::ComboBox*> (options.getTargetComponent()) != nullptr);

    drawPopupMenuItemProceed (g,
        area,
        i.isSeparator,
        i.isEnabled,
        isHighlighted,
        i.isTicked,
        hasSubMenu,
        isComboBox,
        i.text,
        i.shortcutKeyDescription);
}
        
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void LookAndFeel::drawComboBox (juce::Graphics& g,
    int width,
    int height,
    bool,
    int,
    int,
    int,
    int,
    juce::ComboBox& box)
{
    juce::Rectangle<int> b (0, 0, width, height);

    g.setColour (findColour (Colours::devicesComboBoxBackground));
    g.fillRect (b);
    g.setColour (findColour (Colours::devicesComboBoxArrow));
    
    b.removeFromRight (b.getCentreY());
    
    LookAndFeel::drawArrowOpened (g, b.removeFromRight (b.getCentreY()));
}

void LookAndFeel::positionComboBoxText (juce::ComboBox& box, juce::Label& label)
{
    const juce::Rectangle<int> b (0, 0, box.getWidth(), box.getHeight());
    
    label.setBounds (b.withTrimmedRight (b.getCentreY() * 2).reduced (1));
    label.setFont (getComboBoxFont());
}

void LookAndFeel::drawComboBoxTextWhenNothingSelected (juce::Graphics&, juce::ComboBox&, juce::Label&)
{

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::TextLayout LookAndFeel::getTooltipLayout (const juce::String& text)
{
    const float maximum = 400;
    
    juce::AttributedString s;
    s.setJustification (juce::Justification::centred);
    s.append (text, getTooltipsFont(), findColour (Colours::tooltipText));

    juce::TextLayout t;
    t.createLayoutWithBalancedLineLengths (s, maximum);
    return t;
}

juce::Rectangle<int> LookAndFeel::getTooltipBounds (const juce::String& text,
    juce::Point<int> pt,
    juce::Rectangle<int> area)
{
    const float extra = getTooltipsFont().getHeight();
    const int offsetX = 12;
    const int offsetY = 6;
    
    const juce::TextLayout t (getTooltipLayout (text));
    
    const int w = static_cast<int> (t.getWidth()  + extra);
    const int h = static_cast<int> (t.getHeight() + extra / 2.0);
    
    const int x = pt.x > area.getCentreX() ? (pt.x - (w + offsetX)) : (pt.x + offsetX);
    const int y = pt.y > area.getCentreY() ? (pt.y - (h + offsetY)) : (pt.y + offsetY);

    return juce::Rectangle<int> (x, y, w, h).constrainedWithin (area);
}

void LookAndFeel::drawTooltip (juce::Graphics& g, const juce::String& text, int width, int height)
{
    g.fillAll (findColour (Colours::tooltipBackground));

    getTooltipLayout (text).draw (g, juce::Rectangle<float> (width, height));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void LookAndFeel::drawPropertyComponentBackground (juce::Graphics& g,
    int w,
    int h,
    juce::PropertyComponent& c)
{
    const juce::Rectangle<int> r (getPropertyComponentContentPosition (c));
    
    g.setColour (findColour (Colours::preferencesParameterBackground));
    g.fillRect (juce::Rectangle<int> (r.getX(), h).withTrimmedBottom (1).withTrimmedRight (2));
}

void LookAndFeel::drawPropertyComponentLabel (juce::Graphics& g,
    int w,
    int h,
    juce::PropertyComponent& c)
{
    const juce::Rectangle<int> r (getPropertyComponentContentPosition (c));
    
    g.setColour (findColour (Colours::preferencesParameterText));
    g.setFont (getConsoleFont());
    g.drawText (c.getName(),
        juce::Rectangle<int> (r.getX(), h).reduced (4, 0).withTrimmedLeft (30),
        juce::Justification::centredLeft,
        true);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void LookAndFeel::drawToggleButton (juce::Graphics& g, juce::ToggleButton& b, bool, bool)
{
    const int   h = b.getHeight();
    const float t = h * 0.6f;
    
    const juce::Rectangle<float> r (juce::Rectangle<float> (h, h).withSizeKeepingCentre (t, t));

    g.setColour (findColour (Colours::preferencesBoolean));
    g.drawRoundedRectangle (r, 4.0f, 1.0f);
    
    if (b.getToggleState()) {
        g.setColour (findColour (Colours::preferencesBooleanTick));
        const juce::Path p = getTickShape (0.75f);
        g.fillPath (p, p.getTransformToScaleToFit (r.reduced (4, 5), false));
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

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

juce::Path getLineWithStartAndEnd (const juce::Point<float>& a, const juce::Point<float>& b)
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
    juce::Slider& slider)
{
    const juce::Rectangle<int> r (x, y, w, h);
    
    g.setColour (Spaghettis()->getColour (Colours::preferencesSliderBackground));
    g.fillRect (r);
    g.setColour (Spaghettis()->getColour (Colours::preferencesSliderTrack));
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
        if (slider.isBar()) {
            drawLinearSliderHorizontalBar (g, x, y, w, h, position, min, max, style, slider);
        } else {
            drawLinearSliderHorizontal (g, x, y, w, h, position, min, max, style, slider);
        }
    } else { juce::LookAndFeel_V4::drawLinearSlider (g, x, y, w, h, position, min, max, style, slider); }
}
                                
juce::Label* LookAndFeel::createSliderTextBox (juce::Slider& slider)
{
    return new SliderLabel();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void LookAndFeel::drawCallOutBoxBackground (juce::CallOutBox&,
    juce::Graphics& g,
    const juce::Path& path,
    juce::Image&)
{
    g.setColour (Spaghettis()->getColour (Colours::callOutBoxBackground));
    g.fillPath (path);

    g.setColour (Spaghettis()->getColour (Colours::callOutBoxOutline));
    g.strokePath (path, juce::PathStrokeType (2.0f));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void LookAndFeel::drawArrowClosed (juce::Graphics& g, const juce::Rectangle<int>& r)
{
    const float side = juce::jmin (r.getWidth(), r.getHeight()) * 0.65f;
    const float x    = static_cast<float> (r.getCentreX());
    const float y    = static_cast<float> (r.getCentreY());
    const float h    = side * 0.5f;
    const float w    = side * 0.25f;
    
    juce::Path path;
    path.startNewSubPath (x - w, y - h);
    path.lineTo (x + w, y);
    path.lineTo (x - w, y + h);

    g.strokePath (path, juce::PathStrokeType (2.0f));
}

void LookAndFeel::drawArrowOpened (juce::Graphics& g, const juce::Rectangle<int>& r)
{
    const float side = juce::jmin (r.getWidth(), r.getHeight()) * 0.65f;
    const float x    = static_cast<float> (r.getCentreX());
    const float y    = static_cast<float> (r.getCentreY());
    const float h    = side * 0.25f;
    const float w    = side * 0.5f;
    
    juce::Path path;
    path.startNewSubPath (x - w, y - h);
    path.lineTo (x, y + h);
    path.lineTo (x + w, y - h);

    g.strokePath (path, juce::PathStrokeType (2.0f));
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
