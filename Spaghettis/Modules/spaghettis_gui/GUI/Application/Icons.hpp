
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

class Icons {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

using DrawableTuple     = std::tuple<std::unique_ptr<juce::Drawable>, std::unique_ptr<juce::Drawable>, bool>;
using DrawableContainer = std::vector<DrawableTuple>;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    enum IconsIds : int {
        add             = 1,
        deleteForever,
        error,
        message,
        place,
        remove,
        synchronize,
        sortUp,
        sortDown
    };

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Icons()
    {
        addIconAction ("add_svg");
        addIconAction ("delete_forever_svg");
        addIconToggle ("warning_amber_svg");
        addIconToggle ("mail_svg");
        addIconAction ("place_svg");
        addIconAction ("remove_svg");
        addIconAction ("sync_svg");
        addIconAction ("text_rotate_up_svg");
        addIconAction ("text_rotation_down_svg");
    }

public:
    static const Icons& getInstance()
    {
        static Icons s; return s;
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
    
    bool isToggle (int itemId) const
    {
        return std::get<2> (drawable_[getIconIndex (itemId)]);
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    int getIconIndex (int itemId) const
    {
        int i = itemId - 1;
        
        jassert (i >= 0);
        jassert (static_cast<DrawableContainer::size_type> (i) < drawable_.size());
        jassert (std::get<0> (drawable_[i]) != nullptr);
        jassert (std::get<1> (drawable_[i]) != nullptr);
        
        return i;
    }
    
    std::unique_ptr<juce::Drawable> getIconProceed (int itemId, bool isIconOn) const
    {
        int i = getIconIndex (itemId);
        
        return isIconOn ? std::get<1> (drawable_[i])->createCopy() : std::get<0> (drawable_[i])->createCopy();
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

private:
    std::unique_ptr<juce::Drawable> getDrawable (const char* name, juce::Colour colour);
    void addIconProceed (const char* imageOff, const char* imageOn, bool isToggle);

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

