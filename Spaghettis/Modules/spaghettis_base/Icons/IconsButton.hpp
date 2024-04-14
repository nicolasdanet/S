
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

class IconsButton : public juce::ToolbarButton {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit IconsButton (int item) :   juce::ToolbarButton (item,
                                            "",
                                            Icons::getInstance()->getIconOff (item),
                                            Icons::getInstance()->getIconOn (item)),
        itemId_ (item),
        name_ (Icons::getInstance()->getName (item)),
        isToggle_ (Icons::getInstance()->isToggle (item)),
        default_ (Icons::getInstance()->getDefaultState (item))
    {
        if (isToggle_) { setClickingTogglesState (true); }
        else {
            setToggleState (true, juce::dontSendNotification);
        }
    }
    
    ~IconsButton() = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void setState (bool shouldBeOn)
    {
        setToggleState (shouldBeOn, juce::dontSendNotification);
    }
    
    bool getState() const
    {
        jassert (isToggle_); return getToggleState();
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    int getItemId() const
    {
        return itemId_;
    }
    
    juce::String getName() const
    {
        return name_;
    }
    
    bool isToggle() const
    {
        return isToggle_;
    }
    
    bool getDefaultState() const
    {
        return default_;
    }
    
private:
    int  itemId_;
    juce::String name_;
    bool isToggle_;
    bool default_;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IconsButton)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

