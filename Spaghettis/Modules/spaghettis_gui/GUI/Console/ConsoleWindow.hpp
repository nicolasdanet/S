
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class ConsoleWindow : public BaseWindow {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit ConsoleWindow() :
        BaseWindow (Spaghettis()->getCommandManager(),
            Spaghettis()->getPropertiesFile(),
            core::getApplicationName(),
            "ConsoleWindow")
    {
        content_ = std::make_unique<ConsoleComponent> (getKeyName());
        
        setContentNonOwned (content_.get(), true);

        tooltip_.setMillisecondsBeforeTipAppears (750);
        
        makeVisible();
    }

    ~ConsoleWindow() = default;
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void closeButtonPressed() override
    {
        juce::JUCEApplication::getInstance()->systemRequestedQuit();
    }
    
    void clear()
    {
        content_->clear();
    }
    
    void reset()
    {
        content_->reset();
    }

private:
    juce::TooltipWindow tooltip_;

private:
    std::unique_ptr<ConsoleComponent> content_;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ConsoleWindow)
};
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
