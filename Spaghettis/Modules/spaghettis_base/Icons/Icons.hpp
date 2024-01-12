
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

class Icons : private juce::DeletedAtShutdown {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    enum IconsIds : int {
        add             = 1,
        autoscroll,
        clear,
        edit,
        error,
        find,
        inspector,
        message,
        presets,
        restore,
        run,
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
        addIconAction ("Add",           0, "icon_add_svg");
        addIconToggle ("Autoscroll",    0, "icon_system_update_alt_svg");
        addIconAction ("Clear",         0, "icon_delete_forever_svg");
        addIconAction ("Edit",          0, "icon_construction_svg");
        addIconToggle ("Error",         4, "icon_error_outline_svg");
        addIconAction ("Find",          0, "icon_place_svg");
        addIconToggle ("Inspector",     1, "icon_info_svg", false);
        addIconToggle ("Message",       2, "icon_mail_svg");
        addIconToggle ("Presets",       0, "icon_tune_svg", false);
        addIconAction ("Restore",       0, "icon_restore_svg");
        addIconAction ("Run",           0, "icon_visibility_svg");
        addIconAction ("Synchronize",   0, "icon_sync_svg");
        addIconAction ("SortUp",        0, "icon_text_rotate_up_svg");
        addIconAction ("SortDown",      0, "icon_text_rotation_down_svg");
    }

    ~Icons()
    {
        clearSingletonInstance();
    }

    JUCE_DECLARE_SINGLETON_SINGLETHREADED (Icons, true);
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    static juce::Image imagefromSVG (const char*, juce::Rectangle<int>, juce::Colour);
    static juce::Image imagefromSVG (const char*);
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    juce::String getName (int itemId) const
    {
        return drawables_[getIconIndex (itemId)].getName();
    }
    
    int getExtraSpace (int itemId) const
    {
        return drawables_[getIconIndex (itemId)].getExtraSpace();
    }
    
    std::unique_ptr<juce::Drawable> getIconOff (int itemId) const
    {
        return drawables_[getIconIndex (itemId)].getIconOff();
    }

    std::unique_ptr<juce::Drawable> getIconOn (int itemId) const
    {
        return drawables_[getIconIndex (itemId)].getIconOn();
    }
    
    bool isToggle (int itemId) const
    {
        return drawables_[getIconIndex (itemId)].isToggle();
    }
    
    bool getDefaultState (int itemId) const
    {
        return drawables_[getIconIndex (itemId)].getDefaultState();
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    int getItemId (const juce::String& name) const
    {
        int size = static_cast<int> (drawables_.size());
        
        for (int i = 0; i < size; ++i) { if (drawables_[i].getName() == name) { return i + 1; } }
        
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
        jassert (static_cast<std::vector<IconsElement>::size_type> (i) < drawables_.size());
        
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
    std::vector<IconsElement> drawables_;
};
 
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

