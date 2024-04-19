
/* Copyright (c) 2021 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

LNF::LNF()
{
    Colours::initialize (this);
    
    setDefaultSansSerifTypefaceName (Fonts::getFont().getTypefaceName());
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void LNF::drawArrowDown (juce::Graphics& g, const juce::Rectangle<int>& r)
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

void LNF::drawArrowUp (juce::Graphics& g, const juce::Rectangle<int>& r)
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

void LNF::drawArrowRight (juce::Graphics& g, const juce::Rectangle<int>& r)
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

void LNF::drawCross (juce::Graphics& g, const juce::Rectangle<int>& r)
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

void LNF::drawTick (juce::Graphics& g, const juce::Rectangle<int>& r)
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
