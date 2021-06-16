
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
using DrawableTuple     = std::tuple<juce::String, DrawablePointer, DrawablePointer, bool, int>;
using DrawableContainer = std::vector<DrawableTuple>;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    enum IconsIds : int {
        add             = 1,
        autoscroll,
        clear,
        error,
        find,
        message,
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
        addIconAction ("add",           "add_svg");
        addIconToggle ("autoscroll",    "system_update_alt_svg");
        addIconAction ("clear",         "delete_forever_svg");
        addIconToggle ("error",         "error_outline_svg",        4);
        addIconAction ("find",          "place_svg");
        addIconToggle ("message",       "mail_svg",                 2);
        addIconAction ("remove",        "remove_svg");
        addIconAction ("synchronize",   "sync_svg");
        addIconAction ("sortUp",        "text_rotate_up_svg");
        addIconAction ("sortDown",      "text_rotation_down_svg");
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
    juce::String getName (int itemId) const
    {
        return std::get<0> (drawables_[getIconIndex (itemId)]);
    }
    
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
        return std::get<3> (drawables_[getIconIndex (itemId)]);
    }
    
    int getExtra (int itemId) const
    {
        return std::get<4> (drawables_[getIconIndex (itemId)]);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    int getItemId (const juce::String& name) const
    {
        int size = static_cast<int> (drawables_.size());
        
        for (int i = 0; i < size; ++i) { if (std::get<0> (drawables_[i]) == name) { return i + 1; } }
        
        return 0;
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    int getIconIndex (int itemId) const
    {
        int i = itemId - 1;
        
        jassert (i >= 0);
        jassert (static_cast<DrawableContainer::size_type> (i) < drawables_.size());
        jassert (std::get<1> (drawables_[i]) != nullptr);
        jassert (std::get<2> (drawables_[i]) != nullptr);
        
        return i;
    }
    
    std::unique_ptr<juce::Drawable> getIconProceed (int itemId, bool isOn) const
    {
        int i = getIconIndex (itemId);
        
        return isOn ? std::get<2> (drawables_[i])->createCopy() : std::get<1> (drawables_[i])->createCopy();
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

private:
    void addIconAction (const juce::String& name, const char* image, int extra = 0)
    {
        addIconProceed (name, image, image, false, extra);
    }
    
    void addIconToggle (const juce::String& name, const char* image, int extra = 0)
    {
        addIconProceed (name, image, image, true,  extra);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

private:
    void addIconProceed (const juce::String&, const char*, const char*, bool, int);
    
private:
    static std::unique_ptr<juce::Drawable> getDrawable (const char*, juce::Colour);
        
private:
    DrawableContainer drawables_;
};
 
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

