
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

std::unique_ptr<juce::Drawable> Icons::getDrawable (const char* name, juce::Colour colour)
{
    int n = 0; const char* data = BinaryData::getNamedResource (name, n);
    jassert (data);
    std::unique_ptr<juce::Drawable> t (juce::Drawable::createFromImageData (data, n));
    t->replaceColour (juce::Colours::black, colour);
    return t;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void Icons::addIconProceed (const juce::String& name,
    const char* imageOff,
    const char* imageOn,
    bool isToggle,
    int extra)
{
    auto t1 (getDrawable (imageOff, Spaghettis()->getColour (Colours::toolbarIconOff)));
    auto t2 (getDrawable (imageOn,  Spaghettis()->getColour (Colours::toolbarIconOn)));
    
    jassert (extra >= 0);
    
    drawable_.emplace_back (name, std::move (t1), std::move (t2), isToggle, extra);
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
