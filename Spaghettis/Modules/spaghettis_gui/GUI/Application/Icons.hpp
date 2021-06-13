
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

using DrawablePointer   = std::unique_ptr<juce::Drawable>;
using DrawableTuple     = std::tuple<DrawablePointer, DrawablePointer, bool, int>;
using DrawableContainer = std::vector<DrawableTuple>;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    enum IconsIds : int {
        add             = 1,
        autoscroll,
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
        addIconToggle ("system_update_alt_svg");
        addIconAction ("delete_forever_svg");
        addIconToggle ("error_outline_svg", 4);
        addIconToggle ("mail_svg", 2);
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
    
    int getExtra (int itemId) const
    {
        return std::get<3> (drawable_[getIconIndex (itemId)]);
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
    void addIconAction (const char* image, int extra = 0)
    {
        addIconProceed (image, image, false, extra);
    }
    
    void addIconToggle (const char* image, int extra = 0)
    {
        addIconProceed (image, image, true,  extra);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

private:
    void addIconProceed (const char*, const char*, bool, int);
    
private:
    static std::unique_ptr<juce::Drawable> getDrawable (const char*, juce::Colour);
        
private:
    DrawableContainer drawable_;
};
 
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

