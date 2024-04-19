
/* Copyright (c) 2024 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

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

public:
    explicit IconsElement (const juce::String& name,
            std::unique_ptr<juce::Drawable> off,
            std::unique_ptr<juce::Drawable> on,
            bool isToggle,
            bool isDefaultStateOn) :
        name_ (name),
        off_ (std::move (off)),
        on_ (std::move (on)),
        isToggle_ (isToggle),
        isDefaultStateOn_ (isDefaultStateOn)
    {
        static_assert (std::is_nothrow_move_constructible_v<IconsElement> == true);
        static_assert (std::is_nothrow_move_assignable_v<IconsElement> == true);
    }

public:
    juce::String getName() const
    {
        return name_;
    }
    
    std::unique_ptr<juce::Drawable> getIconOff() const
    {
        jassert (off_ != nullptr); return off_->createCopy();
    }
    
    std::unique_ptr<juce::Drawable> getIconOn() const
    {
        jassert (on_ != nullptr); return on_->createCopy();
    }
    
    bool isToggle() const
    {
        return isToggle_;
    }
    
    bool getDefaultState() const
    {
        return isDefaultStateOn_;
    }
    
private:
    juce::String name_;
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

