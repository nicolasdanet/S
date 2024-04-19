
/* Copyright (c) 2024 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

ToolbarComponent::ToolbarComponent (int item) : ToolbarItemComponent (item, "", true),
    iconOff_ (Icons::getInstance()->getIconOff (item)),
    iconOn_ (Icons::getInstance()->getIconOn (item)),
    current_ (nullptr)
{
    
}

ToolbarComponent::~ToolbarComponent()
{
    if (current_) { removeChildComponent (current_); current_ = nullptr; }
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
    updateIcon();
}

void ToolbarComponent::buttonStateChanged()
{
    updateIcon();
}

void ToolbarComponent::resized()
{
    ToolbarItemComponent::resized();
    
    updateIcon();
}

void ToolbarComponent::enablementChanged()
{
    ToolbarItemComponent::enablementChanged();
    
    updateIcon();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ToolbarComponent::updateIcon()
{
    juce::Drawable* icon = getToggleState() ? iconOn_.get() : iconOff_.get();
    
    jassert (icon != nullptr);
    
    if (icon != current_) {
        if (current_) { removeChildComponent (current_); }
        current_ = icon;
        addAndMakeVisible (current_);
    }
    
    current_->setInterceptsMouseClicks (false, false);
    current_->setTransformToFit (getContentArea().reduced (2).toFloat(), juce::RectanglePlacement::centred);
    current_->setAlpha (isEnabled() ? 1.0f : 0.5f);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
