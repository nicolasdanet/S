
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
    explicit ConsoleWindow (juce::ApplicationCommandManager& command, juce::PropertiesFile* propertiesFile) :
        BaseWindow (command,
            propertiesFile,
            core::getApplicationName(),
            "ConsoleWindow")
    {
        content_ = std::make_unique<ConsoleComponent> (command, propertiesFile, getKeyName());
        
        setContentNonOwned (content_.get(), true);

        tooltip_.setMillisecondsBeforeTipAppears (1000);
        
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
