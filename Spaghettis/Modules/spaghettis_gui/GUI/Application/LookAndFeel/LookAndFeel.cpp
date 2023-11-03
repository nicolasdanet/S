
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

LookAndFeel::LookAndFeel() : windowTitleHeight_ (0)
{
    Colours::initialize (this);
        
    initializeFonts();
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

juce::String initializeFontsBest (const juce::StringArray& check)
{
    if (check.isEmpty() == false) {
    //
    juce::StringArray fonts (juce::Font::findAllTypefaceNames());
    
    for (const auto& f : check) { if (fonts.contains (f)) { return f; } }
    //
    }
    
    return juce::String();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

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

    /* Raspberry Pi. */
    
    /* < https://forum.juce.com/t/fonts-need-to-be-initialized-on-rpi > */
    
    font18Name_             = font18_.getTypefacePtr()->getName();
    font18MonospacedName_   = font18Monospaced_.getTypefacePtr()->getName();
    font16BoldName_         = font16Bold_.getTypefacePtr()->getName();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Font LookAndFeel::getConsoleFont() const
{
    return font18_;
}

juce::Font LookAndFeel::getMenuFont() const
{
    return font18_;
}

juce::Font LookAndFeel::getColourFont() const
{
    return font18Monospaced_;
}

juce::Font LookAndFeel::getMenuShortcutsFont() const
{
    return font18Monospaced_;
}

juce::Font LookAndFeel::getTooltipsFont() const
{
    return font16Bold_;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Cache the objects fonts used while zooming. */

juce::Font getFontAtHeight (const juce::Font& base, std::vector<std::tuple<int, juce::Font>>& v, int required)
{
    int height = static_cast<int> (base.getHeight());
    
    if (required == height) { return base; }
    else {
    //
    for (const auto& [h, f] : v) { if (h == required) { return f; } }
    
    juce::Font font (base.withHeight (static_cast<float> (required)));
    
    v.emplace_back (required, font);
    
    return font;
    //
    }
}

juce::Font getFontRescaled (const juce::Font& base, std::vector<std::tuple<int, juce::Font>>& v, float scale)
{
    jassert (scale > 0);
    
    int height   = static_cast<int> (base.getHeight());
    int required = static_cast<int> (height * scale);
    
    return getFontAtHeight (base, v, required);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Font LookAndFeel::getListBoxFont() const
{
    return getConsoleFont();
}

juce::Font LookAndFeel::getObjectsFontRescaled (float scale)
{
    return getFontRescaled (font18_, objectsFonts_, scale);
}

juce::Font LookAndFeel::getDialsFontWithHeight (int height)
{
    return getFontAtHeight (font16Bold_, dialsFonts_, height);
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
    
    const juce::ComponentPeer::OptionalBorderSize frameSize = p->getFrameSizeIfPresent();
    
    if (frameSize) { windowTitleHeight_ = frameSize->getTop(); }
    //
    }
    
    return windowTitleHeight_;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void LookAndFeel::drawArrowDown (juce::Graphics& g, const juce::Rectangle<int>& r)
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

void LookAndFeel::drawArrowUp (juce::Graphics& g, const juce::Rectangle<int>& r)
{
    const float side = juce::jmin (r.getWidth(), r.getHeight()) * 0.65f;
    const float x    = static_cast<float> (r.getCentreX());
    const float y    = static_cast<float> (r.getCentreY());
    const float h    = side * 0.25f;
    const float w    = side * 0.5f;
    
    juce::Path path;
    path.startNewSubPath (x - w, y + h);
    path.lineTo (x, y - h);
    path.lineTo (x + w, y + h);

    g.strokePath (path, juce::PathStrokeType (2.0f));
}

void LookAndFeel::drawArrowRight (juce::Graphics& g, const juce::Rectangle<int>& r)
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

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void LookAndFeel::drawCross (juce::Graphics& g, const juce::Rectangle<int>& r)
{
    /* LookAndFeel_V4::getCrossShape */
    
    static const unsigned char pathData[] =
        {
            110, 109,  51,  51, 255,  66,   0,   0,   0,   0,
            108, 205, 204,  13,  67,  51,  51,  99,  65, 108,
              0,   0, 170,  66, 205, 204, 141,  66, 108,  51,
            179,  13,  67,  52,  51, 255,  66, 108,   0,   0,
            255,  66, 205, 204,  13,  67, 108, 205, 204, 141,
             66,   0,   0, 170,  66, 108,  52,  51,  99,  65,
             51, 179,  13,  67, 108,   0,   0,   0,   0,  51,
             51, 255,  66, 108, 205, 204,  98,  66, 204, 204,
            141,  66, 108,   0,   0,   0,   0,  51,  51,  99,
             65, 108,  51,  51,  99,  65,   0,   0,   0,   0,
            108, 205, 204, 141,  66, 205, 204,  98,  66, 108,
             51,  51, 255,  66,   0,   0,   0,   0,  99, 101,
              0,   0
        };

    juce::Path path;
    
    path.loadPathFromData (pathData, sizeof (pathData));

    g.fillPath (path, path.getTransformToScaleToFit (r.toFloat(), false));
}

void LookAndFeel::drawTick (juce::Graphics& g, const juce::Rectangle<int>& r)
{
    /* LookAndFeel_V4::getTickShape */
    
    static const unsigned char pathData[] =
        {
            110, 109,  32, 210, 202,  64, 126, 183, 148,  64,
            108,  39, 244, 247,  64, 245,  76, 124,  64, 108,
            178, 131,  27,  65, 246,  76, 252,  64, 108, 175,
            242,   4,  65, 246,  76, 252,  64, 108, 236,   5,
             68,  65,   0,   0, 160, 180, 108, 240, 150,  90,
             65,  21, 136,  52,  63, 108,  48,  59,  16,  65,
              0,   0,  32,  65, 108,  32, 210, 202,  64, 126,
            183, 148,  64,  99, 101,   0,   0
        };

    juce::Path path;
    
    path.loadPathFromData (pathData, sizeof (pathData));

    g.fillPath (path, path.getTransformToScaleToFit (r.toFloat(), false));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
