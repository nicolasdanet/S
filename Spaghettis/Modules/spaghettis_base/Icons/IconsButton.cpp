
/* Copyright (c) 2024 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

IconsButton::IconsButton (int item) :
    ToolbarComponent (item),
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
