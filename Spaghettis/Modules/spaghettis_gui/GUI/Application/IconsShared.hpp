
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "BinaryData.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

struct Icons {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

enum IconsIds : int {
    add     = 1,
    clear,
    error,
    notification,
    place,
    remove,
    synchronize,
    sortUp,
    sortDown
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

};
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

struct IconsShared {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

using DrawableTuple     = std::tuple<std::unique_ptr<juce::Drawable>, std::unique_ptr<juce::Drawable>, bool>;
using DrawableContainer = std::vector<DrawableTuple>;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    IconsShared()
    {
        addIconAction ("add_svg");
        addIconAction ("delete_forever_svg");
        addIconAction ("warning_amber_svg");
        addIconAction ("notifications_svg");
        addIconAction ("place_svg");
        addIconAction ("remove_svg");
        addIconAction ("sync_svg");
        addIconAction ("text_rotate_up_svg");
        addIconAction ("text_rotation_down_svg");
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    std::unique_ptr<juce::Drawable> getIconOff (int itemId) const
    {
        return getIconProceed (itemId, false);
    }

    std::unique_ptr<juce::Drawable> getIconOn (int itemId) const
    {
        return getIconProceed (itemId, true);
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    std::unique_ptr<juce::Drawable> getIconProceed (int itemId, bool isIconOn) const
    {
        int i = itemId - 1;
        
        jassert (i >= 0);
        jassert (static_cast<DrawableContainer::size_type> (i) < drawable_.size());
        jassert (std::get<0> (drawable_[i]) != nullptr);
        jassert (std::get<1> (drawable_[i]) != nullptr);
        
        return isIconOn ? std::get<1> (drawable_[i])->createCopy() : std::get<0> (drawable_[i])->createCopy();
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

private:
    std::unique_ptr<juce::Drawable> getDrawable (const char* name, juce::Colour colour)
    {
        int n = 0; const char* data = BinaryData::getNamedResource (name, n);
        jassert (data);
        std::unique_ptr<juce::Drawable> t (juce::Drawable::createFromImageData (data, n));
        t->replaceColour (juce::Colours::black, colour);
        return t;
    }
    
    void addIconProceed (const char* imageOff, const char* imageOn, bool isToggle)
    {
        auto t1 (getDrawable (imageOff, Spaghettis()->getColour (Colours::toolbarIconOff)));
        auto t2 (getDrawable (imageOn,  Spaghettis()->getColour (Colours::toolbarIconOn)));
        
        drawable_.emplace_back (std::move (t1), std::move (t2), isToggle);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

private:
    void addIconAction (const char* image)
    {
        addIconProceed (image, image, false);
    }
    
    void addIconToggle (const char* image)
    {
        addIconProceed (image, image, true);
    }
    
private:
    DrawableContainer drawable_;
};
 
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

