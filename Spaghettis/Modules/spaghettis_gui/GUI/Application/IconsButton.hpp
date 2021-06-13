
/* Copyright (c) 2021 Nicolas Danet. */

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
    IconsButton (int item) :    juce::ToolbarButton (item,
                                    "",
                                    Icons::getInstance().getIconOff (item),
                                    Icons::getInstance().getIconOn (item)),
                                itemId_ (item),
                                extra_ (Icons::getInstance().getExtra (item)),
                                isToggle_ (Icons::getInstance().isToggle (item))
                                
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

bool getToolbarItemSizes (int thickness, bool isVertical, int& size, int& min, int& max) override
{
    if (!isVertical) {
    //
    min  = thickness;
    size = thickness + extra_;
    max  = thickness + extra_ + extra_;
    //
    } else { return juce::ToolbarButton::getToolbarItemSizes (thickness, isVertical, size, min, max); }
    
    return true;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    int getItemId() const
    {
        return itemId_;
    }
    
    bool isToggle() const
    {
        return isToggle_;
    }
    
private:
    int  itemId_;
    int  extra_;
    bool isToggle_;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IconsButton)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

