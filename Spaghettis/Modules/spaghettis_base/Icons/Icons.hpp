
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
        /* Must be in same order as enum above. */
        
        addIconAction       ("Add",         "icon_add_svg");
        addIconToggleOn     ("Autoscroll",  "icon_system_update_alt_svg");
        addIconAction       ("Clear",       "icon_delete_svg");
        addIconAction       ("Edit",        "icon_construction_svg");
        addIconToggleOn     ("Error",       "icon_error_svg");
        addIconAction       ("Find",        "icon_location_on_svg");
        addIconToggleOff    ("Inspector",   "icon_info_svg");
        addIconToggleOn     ("Message",     "icon_mail_svg");
        addIconToggleOff    ("Presets",     "icon_tune_svg");
        addIconAction       ("Restore",     "icon_history_svg");
        addIconAction       ("Run",         "icon_visibility_svg");
        addIconAction       ("Synchronize", "icon_sync_svg");
        addIconAction       ("SortUp",      "icon_text_rotate_up_svg");
        addIconAction       ("SortDown",    "icon_text_rotation_down_svg");
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
    void addIconAction (const juce::String& s, const char* image)
    {
        addIconProceed (s, image, image, false, true);
    }
    
    void addIconToggleOn (const juce::String& s, const char* image)
    {
        addIconProceed (s, image, image, true, true);
    }
    
    void addIconToggleOff (const juce::String& s, const char* image)
    {
        addIconProceed (s, image, image, true, false);
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

private:
    void addIconProceed (const juce::String&, const char*, const char*, bool, bool);
    
private:
    std::vector<IconsElement> drawables_;
};
 
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

