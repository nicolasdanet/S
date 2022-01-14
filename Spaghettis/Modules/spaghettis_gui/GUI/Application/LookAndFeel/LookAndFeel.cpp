
/* Copyright (c) 2021 Jojo and others. */

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

/* Cache the window title height (assume that it doesn't change). */

int LookAndFeel::getWindowTitleHeight (juce::Component *c)
{
    if (!windowTitleHeight_) {
    //
    juce::ComponentPeer *p = c->getPeer();
    
    jassert (p);
    
    windowTitleHeight_ = p->getFrameSize().getTop();
    //
    }
    
    return windowTitleHeight_;
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
