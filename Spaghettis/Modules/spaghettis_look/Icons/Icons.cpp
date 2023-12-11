
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

std::unique_ptr<juce::Drawable> getDrawable (const char* name, juce::Colour colour)
{
    int n = 0; const char* data = BinaryData::getNamedResource (name, n);
    jassert (data);
    std::unique_ptr<juce::Drawable> t (juce::Drawable::createFromImageData (data, n));
    t->replaceColour (juce::Colours::black, colour);
    return t;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Icons::addIconProceed (const juce::String& name,
    int extra,
    const char* imageOff,
    const char* imageOn,
    bool isToggle,
    bool defaultStateIsOn)
{
    auto t1 (getDrawable (imageOff, Colours::fetchColour<Colours::toolbarIconOff>()));
    auto t2 (getDrawable (imageOn,  Colours::fetchColour<Colours::toolbarIconOn>()));
    
    jassert (extra >= 0);
    
    drawables_.emplace_back (name, extra, std::move (t1), std::move (t2), isToggle, defaultStateIsOn);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Image Icons::imagefromSVG (const char* name, juce::Rectangle<int> r, juce::Colour colour)
{
    juce::Image image (juce::Image::ARGB, r.getWidth(), r.getHeight(), true);
    
        std::unique_ptr<juce::Drawable> drawable (getDrawable (name, colour));
        juce::Graphics g (image);
        drawable->setTransformToFit (r.withZeroOrigin().toFloat(), juce::RectanglePlacement::centred);
        drawable->draw (g, 1.f);

	return image;
}

juce::Image Icons::imagefromSVG (const char* name)
{
    return imagefromSVG (name, juce::Rectangle<int> (24, 24), juce::Colours::grey);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
