
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class IconsFactory : public juce::ToolbarItemFactory {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    IconsFactory() = default;
    ~IconsFactory() = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

public:
    juce::ToolbarItemComponent* createItem (int itemId) override
    {
        return createButton (itemId, "");
    }
    
    virtual void setToolbarButton (int itemId, bool isToogle, juce::ToolbarButton* button) = 0;
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    juce::ToolbarButton* createButton (int itemId, const juce::String& text)
    {
        auto t = std::make_unique<juce::ToolbarButton> (itemId,
                    text,
                    icons_->getIconOff (itemId),
                    icons_->getIconOn (itemId));
        
        if (t) {
        //
        bool isToggle = icons_->isToggle (itemId);
        
        if (isToggle) { t->setClickingTogglesState (true); }
        else {
            t->setToggleState (true, juce::dontSendNotification);
        }
        
        setToolbarButton (itemId, isToggle, t.get());
        //
        } else { jassertfalse; }
        
        return t.release();
    }

private:
    juce::SharedResourcePointer<IconsShared> icons_;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IconsFactory)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

