
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

juce::String initializeBest (const juce::StringArray& check)
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

/* < https://forum.juce.com/t/fonts-need-to-be-initialized-on-rpi > */

FontsOwner::FontsOwner()
{
    #if JUCE_LINUX
    
    juce::String fontName (initializeBest (juce::StringArray ("Ubuntu")));
    juce::String fontMonospacedName (initializeBest (juce::StringArray ("Ubuntu Mono")));
    
    #else
    
    juce::String fontName (initializeBest (juce::StringArray()));
    juce::String fontMonospacedName (initializeBest (juce::StringArray()));
    
    #endif
    
    if (fontName.isEmpty())           { fontName           = juce::Font::getDefaultSansSerifFontName();  }
    if (fontMonospacedName.isEmpty()) { fontMonospacedName = juce::Font::getDefaultMonospacedFontName(); }
    
    font_             = juce::Font (fontName, 18.0, juce::Font::plain);
    fontBold_         = juce::Font (fontName, 18.0, juce::Font::bold);
    fontMonospaced_   = juce::Font (fontMonospacedName, 18.0, juce::Font::plain);
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

juce::Font fontAtHeight (const juce::Font& base, std::vector<FontsElement>& v, int required)
{
    int height = static_cast<int> (base.getHeight());
    
    if (required == height) { return base; }
    else {
    //
    for (const auto& e : v) { if (e.getHeight() == required) { return e.getFont(); } }
    
    juce::Font font (base.withHeight (static_cast<float> (required)));
    
    v.emplace_back (font, required);
    
    return font;
    //
    }
}

juce::Font fontRescaled (const juce::Font& base, std::vector<FontsElement>& v, float scale)
{
    jassert (scale > 0);
    
    int height   = static_cast<int> (base.getHeight());
    int required = static_cast<int> (height * scale);
    
    return fontAtHeight (base, v, required);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Font FontsOwner::getFont() const
{
    return font_;
}

juce::Font FontsOwner::getFontRescaled (float scale)
{
    return fontRescaled (font_, fonts_, scale);
}

juce::Font FontsOwner::getFontWithHeight (int height)
{
    return fontAtHeight (font_, fonts_, height);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Font FontsOwner::getMonospacedFont() const
{
    return fontMonospaced_;
}

juce::Font FontsOwner::getMonospacedFontRescaled (float scale)
{
    return fontRescaled (fontMonospaced_, fontsMonospaced_, scale);
}

juce::Font FontsOwner::getMonospacedFontWithHeight (int height)
{
    return fontAtHeight (fontMonospaced_, fontsMonospaced_, height);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Font FontsOwner::getBoldFont() const
{
    return fontBold_;
}

juce::Font FontsOwner::getBoldFontRescaled (float scale)
{
    return fontRescaled (fontBold_, fontsBold_, scale);
}

juce::Font FontsOwner::getBoldFontWithHeight (int height)
{
    return fontAtHeight (fontBold_, fontsBold_, height);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

JUCE_IMPLEMENT_SINGLETON (FontsOwner)

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
