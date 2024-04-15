
/* Copyright (c) 2024 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

IconsButton::IconsButton (int item) : juce::ToolbarButton (item,
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

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void IconsButton::setState (bool shouldBeOn)
{
    setToggleState (shouldBeOn, juce::dontSendNotification);
}

bool IconsButton::getState() const
{
    jassert (isToggle_); return getToggleState();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int IconsButton::getItemId() const
{
    return itemId_;
}

juce::String IconsButton::getName() const
{
    return name_;
}

bool IconsButton::getDefaultState() const
{
    return default_;
}

bool IconsButton::isToggle() const
{
    return isToggle_;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
