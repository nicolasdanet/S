
/* Copyright (c) 2021 Jojo and others. */

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
using DrawableTuple     = std::tuple<juce::String, int, DrawablePointer, DrawablePointer, bool, bool>;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    enum {
        ICONS_NAME      = 0,
        ICONS_EXTRA     = 1,
        ICONS_OFF,
        ICONS_ON,
        ICONS_TOGGLE,
        ICONS_STATE
    };
    
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
        restore,
        synchronize,
        sortUp,
        sortDown
    };

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit Icons()
    {
        addIconAction ("Add",           0, "add_svg");
        addIconToggle ("Autoscroll",    0, "system_update_alt_svg");
        addIconAction ("Clear",         0, "delete_forever_svg");
        addIconToggle ("Error",         4, "error_outline_svg");
        addIconAction ("Find",          0, "place_svg");
        addIconToggle ("Message",       2, "mail_svg");
        addIconAction ("Restore",       0, "restore_svg");
        addIconAction ("Synchronize",   0, "sync_svg");
        addIconAction ("SortUp",        0, "text_rotate_up_svg");
        addIconAction ("SortDown",      0, "text_rotation_down_svg");
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
        return std::get<ICONS_NAME> (drawables_[getIconIndex (itemId)]);
    }
    
    int getExtraSpace (int itemId) const
    {
        return std::get<ICONS_EXTRA> (drawables_[getIconIndex (itemId)]);
    }
    
    std::unique_ptr<juce::Drawable> getIconOff (int itemId) const
    {
        return std::get<ICONS_OFF> (drawables_[getIconIndex (itemId)])->createCopy();
    }

    std::unique_ptr<juce::Drawable> getIconOn (int itemId) const
    {
        return std::get<ICONS_ON> (drawables_[getIconIndex (itemId)])->createCopy();
    }
    
    bool isToggle (int itemId) const
    {
        return std::get<ICONS_TOGGLE> (drawables_[getIconIndex (itemId)]);
    }
    
    bool getDefaultState (int itemId) const
    {
        return std::get<ICONS_STATE> (drawables_[getIconIndex (itemId)]);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    int getItemId (const juce::String& name) const
    {
        int size = static_cast<int> (drawables_.size());
        
        for (int i = 0; i < size; ++i) { if (std::get<ICONS_NAME> (drawables_[i]) == name) { return i + 1; } }
        
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
        jassert (static_cast<std::vector<DrawableTuple>::size_type> (i) < drawables_.size());
        jassert (std::get<ICONS_OFF> (drawables_[i]) != nullptr);
        jassert (std::get<ICONS_ON> (drawables_[i]) != nullptr);
        
        return i;
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

private:
    void addIconAction (const juce::String& s, int extra, const char* image)
    {
        addIconProceed (s, extra, image, image, false, true);
    }
    
    void addIconToggle (const juce::String& s, int extra, const char* image, bool state = true)
    {
        addIconProceed (s, extra, image, image, true, state);
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

private:
    void addIconProceed (const juce::String&, int, const char*, const char*, bool, bool);
    
private:
    std::vector<DrawableTuple> drawables_;
};
 
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

