
/* Copyright (c) 2024 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class IconsElement {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

friend class Icons;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

private:
    explicit IconsElement (const juce::String& name,
            int extra,
            std::unique_ptr<juce::Drawable> off,
            std::unique_ptr<juce::Drawable> on,
            bool isToggle,
            bool isDefaultStateOn) :
        name_ (name),
        extra_ (extra),
        off_ (std::move (off)),
        on_ (std::move (on)),
        isToggle_ (isToggle),
        isDefaultStateOn_ (isDefaultStateOn)
    {
    }

public:
    ~IconsElement() = default;
    
public:
    IconsElement (const IconsElement&) = delete;
    IconsElement& operator = (const IconsElement&) = delete;
    
public:
    IconsElement (IconsElement&&) = default;
    IconsElement& operator = (IconsElement&&) = default;
    
private:
    juce::String name_;
    int extra_;
    std::unique_ptr<juce::Drawable> off_;
    std::unique_ptr<juce::Drawable> on_;
    bool isToggle_;
    bool isDefaultStateOn_;
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

