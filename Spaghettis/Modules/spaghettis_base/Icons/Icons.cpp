
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int Icons::getItemId (const juce::String& name) const
{
    int size = static_cast<int> (drawables_.size());
        
    for (int i = 0; i < size; ++i) { if (drawables_[i].getName() == name) { return i + 1; } }
        
    return 0;
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

int getIconIndex (const std::vector<IconsElement>& v, int itemId)
{
    int i = itemId - 1;
        
    jassert (i >= 0);
    jassert (static_cast<std::vector<IconsElement>::size_type> (i) < v.size());
        
    return i;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::String Icons::getName (int itemId) const
{
    return drawables_[getIconIndex (drawables_, itemId)].getName();
}

std::unique_ptr<juce::Drawable> Icons::getIconOff (int itemId) const
{
    return drawables_[getIconIndex (drawables_, itemId)].getIconOff();
}

std::unique_ptr<juce::Drawable> Icons::getIconOn (int itemId) const
{
    return drawables_[getIconIndex (drawables_, itemId)].getIconOn();
}

bool Icons::isToggle (int itemId) const
{
    return drawables_[getIconIndex (drawables_, itemId)].isToggle();
}

bool Icons::getDefaultState (int itemId) const
{
    return drawables_[getIconIndex (drawables_, itemId)].getDefaultState();
}
    
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

void addIconProceed (std::vector<IconsElement>& v,
    const juce::String& name,
    const char* imageOff,
    const char* imageOn,
    bool isToggle,
    bool defaultStateIsOn)
{
    auto t1 (getDrawable (imageOff, Colours::fetchColour (Colours::toolbarIconOff)));
    auto t2 (getDrawable (imageOn,  Colours::fetchColour (Colours::toolbarIconOn)));
    
    v.emplace_back (name, std::move (t1), std::move (t2), isToggle, defaultStateIsOn);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Icons::addIconAction (const juce::String& s, const char* image)
{
    addIconProceed (drawables_, s, image, image, false, true);
}

void Icons::addIconToggleOn (const juce::String& s, const char* image)
{
    addIconProceed (drawables_, s, image, image, true, true);
}

void Icons::addIconToggleOff (const juce::String& s, const char* image)
{
    addIconProceed (drawables_, s, image, image, true, false);
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

JUCE_IMPLEMENT_SINGLETON (Icons)

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
