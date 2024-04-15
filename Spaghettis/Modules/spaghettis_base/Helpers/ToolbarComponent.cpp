
/* Copyright (c) 2024 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

ToolbarComponent::ToolbarComponent (int item) : ToolbarItemComponent (item, "", true),
    iconOff_ (Icons::getInstance()->getIconOff (item)),
    iconOn_ (Icons::getInstance()->getIconOn (item))
{
    
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool ToolbarComponent::getToolbarItemSizes (int size, bool, int& preferred, int& min, int& max)
{
    preferred   = size;
    min         = size;
    max         = size;
    
    return true;
}

void ToolbarComponent::paintButtonArea (juce::Graphics&, int, int, bool, bool)
{
}

void ToolbarComponent::contentAreaChanged (const juce::Rectangle<int>&)
{
    buttonStateChanged();
}

void ToolbarComponent::buttonStateChanged()
{
    setCurrentImage (getImageToUse());
}

void ToolbarComponent::resized()
{
    ToolbarItemComponent::resized();
    updateDrawable();
}

void ToolbarComponent::enablementChanged()
{
    ToolbarItemComponent::enablementChanged();
    updateDrawable();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ToolbarComponent::setCurrentImage (juce::Drawable* const newImage)
{
    if (newImage != current_)
    {
        removeChildComponent (current_);
        current_ = newImage;

        if (current_ != nullptr)
        {
            enablementChanged();
            addAndMakeVisible (current_);
            updateDrawable();
        }
    }
}

void ToolbarComponent::updateDrawable()
{
    if (current_ != nullptr)
    {
        current_->setInterceptsMouseClicks (false, false);
        current_->setTransformToFit (getContentArea().toFloat(), juce::RectanglePlacement::centred);
        current_->setAlpha (isEnabled() ? 1.0f : 0.5f);
    }
}

juce::Drawable* ToolbarComponent::getImageToUse() const
{
    if (getStyle() == juce::Toolbar::textOnly)
        return nullptr;

    if (getToggleState() && iconOn_ != nullptr)
        return iconOn_.get();

    return iconOff_.get();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
