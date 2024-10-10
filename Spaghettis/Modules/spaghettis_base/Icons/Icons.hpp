
/* Copyright (c) 2021 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

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
        autoload        = 1,
        autoscroll,
        clear,
        directory,
        edit,
        error,
        find,
        inspector,
        message,
        presets,
        restore,
        run
    };

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* < https://fonts.google.com/icons > */

public:
    Icons()
    {
        /* Must be in same order as enum above. */
        
        addIconToggleOff    ("Autoload",    "icon_upload_svg");
        addIconToggleOn     ("Autoscroll",  "icon_vertical_align_bottom_svg");
        addIconAction       ("Clear",       "icon_delete_svg");
        addIconAction       ("Directory",   "icon_create_new_folder_svg");
        addIconAction       ("Edit",        "icon_construction_svg");
        addIconToggleOn     ("Error",       "icon_error_svg");
        addIconAction       ("Find",        "icon_location_on_svg");
        addIconToggleOff    ("Inspector",   "icon_info_svg");
        addIconToggleOn     ("Message",     "icon_mail_svg");
        addIconToggleOff    ("Presets",     "icon_tune_svg");
        addIconAction       ("Restore",     "icon_history_svg");
        addIconAction       ("Run",         "icon_visibility_svg");
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
    int getItemId (const juce::String& name) const;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    juce::String getName (int itemId) const;
    std::unique_ptr<juce::Drawable> getIconOff (int itemId) const;
    std::unique_ptr<juce::Drawable> getIconOn (int itemId) const;
    bool isToggle (int itemId) const;
    bool getDefaultState (int itemId) const;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

private:
    void addIconAction (const juce::String&, const char*);
    void addIconToggleOn (const juce::String&, const char*);
    void addIconToggleOff (const juce::String&, const char*);
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    static juce::Image imagefromSVG (const char*);
    
private:
    std::vector<IconsElement> drawables_;
};
 
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

