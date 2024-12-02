
/* Copyright (c) 2024 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

juce::String getLineTag (const juce::String& s)
{
    juce::String tag (Strings::upToWhitespace (s));
    
    return tag.endsWithChar (':') ? tag.dropLastCharacters (1) : juce::String();
}

juce::String getLineText (const juce::String& s)
{
    return Strings::fromWhitespace (s);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

InfoLine::InfoLine (const juce::String& s, const juce::Font& font) :
    font_ (font),
    tag_ (getLineTag (s)),
    text_ (tag_.isEmpty() ? s : getLineText (s))
{
    
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

int getLineTagWidth (const juce::Font& font, const juce::String& text)
{
    const int w = font.getStringWidth (text);
    const int a = 90;
    const int b = 120;
    
    if (w < a) { return a; } else { return b; }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void paintLineTag (juce::Graphics& g,
    const juce::Rectangle<int>& bounds,
    const juce::Font& font,
    const juce::String& text)
{
    g.setColour (Colours::fetchColour (Colours::parametersInfoTagBackground));
    g.fillRect (bounds.reduced (1));
    g.setColour (Colours::fetchColour (Colours::parametersInfoTagText));
    g.setFont (font);
    g.drawText (text, bounds, juce::Justification::centred, true);
}

void paintLineText (juce::Graphics& g,
    const juce::Rectangle<int>& bounds,
    const juce::Font& font,
    const juce::String& text)
{
    g.setColour (Colours::fetchColour (Colours::parametersInfoText));
    g.setFont (font);
    g.drawText (text, bounds, juce::Justification::centredLeft, true);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void InfoLine::paint (juce::Graphics& g)
{
    juce::Rectangle<int> bounds (getLocalBounds());
    
    g.fillAll (Colours::fetchColour (Colours::parametersInfoBackground));
    
    if (tag_.isNotEmpty()) {
    //
    paintLineTag (g, bounds.removeFromLeft (getLineTagWidth (font_, tag_)), font_, tag_);
    //
    }
    
    paintLineText (g, bounds.withTrimmedLeft (4), font_, text_);
}

void InfoLine::resized()
{

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
