
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
        addIcon ("add_svg");
        addIcon ("delete_forever_svg");
        addIcon ("warning_amber_svg");
        addIcon ("notifications_svg");
        addIcon ("place_svg");
        addIcon ("remove_svg");
        addIcon ("sync_svg");
        addIcon ("text_rotate_up_svg");
        addIcon ("text_rotation_down_svg");
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    std::unique_ptr<juce::Drawable> getIconRaw (int itemId, bool isIconOn) const
    {
        int i = itemId - 1;
        
        jassert (i >= 0);
        jassert (static_cast<DrawableContainer::size_type> (i) < drawable_.size());
        jassert (std::get<0> (drawable_[i]) != nullptr);
        jassert (std::get<1> (drawable_[i]) != nullptr);
        
        return isIconOn ? std::get<1> (drawable_[i])->createCopy() : std::get<0> (drawable_[i])->createCopy();
    }
    
public:
    std::unique_ptr<juce::Drawable> getIconOff (int itemId) const
    {
        return getIconRaw (itemId, false);
    }

    std::unique_ptr<juce::Drawable> getIconOn (int itemId) const
    {
        return getIconRaw (itemId, true);
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    std::unique_ptr<juce::Drawable> getDrawable (const char* name, juce::Colour colour)
    {
        int n = 0; const char* data = BinaryData::getNamedResource (name, n);
        jassert (data);
        std::unique_ptr<juce::Drawable> t (juce::Drawable::createFromImageData (data, n));
        t->replaceColour (juce::Colours::black, colour);
        return t;
    }
    
    void addIconRaw (const char* imageOff, const char* imageOn, bool isToggle)
    {
        auto t1 (getDrawable (imageOff, Spaghettis()->getColour (Colours::toolbarIconOff)));
        auto t2 (getDrawable (imageOn,  Spaghettis()->getColour (Colours::toolbarIconOn)));
        
        drawable_.emplace_back (std::move (t1), std::move (t2), isToggle);
    }
    
    void addIcon (const char* image)
    {
        addIconRaw (image, image, false);
    }
    
private:
    DrawableContainer drawable_;
};
 
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

