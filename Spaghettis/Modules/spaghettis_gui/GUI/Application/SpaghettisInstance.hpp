
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class SpaghettisInstance {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    SpaghettisInstance() :  lookAndFeel_ (std::make_unique<LookAndFeel>()),
                            commandManager_ (std::make_unique<juce::ApplicationCommandManager>()),
                            menu_ (std::make_unique<MenuModel>(commandManager_.get())),
                            core_ (std::make_unique<Wrapper>())
    {
        #if defined ( JUCE_MAC )
        
        juce::MenuBarModel::setMacMainMenu (menu_.get());
        
        #endif
    }
    
    ~SpaghettisInstance()
    {
        #if defined ( JUCE_MAC )
        
        jassert (juce::MenuBarModel::getMacMainMenu() == nullptr);
        
        #endif
        
        clearSingletonInstance();
    }

    JUCE_DECLARE_SINGLETON_SINGLETHREADED (SpaghettisInstance, true)

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void start (const juce::StringArray& commandLine)
    {
        core_->start (commandLine);
    }
    
    void shutdown()
    {
        core_->shutdown();
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void handle (const std::function<void()>& f)
    {
        core_->handle (f);
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    bool isRunningFromCommandLine() const
    {
        return (core_->getCommandLine().size() > 1);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void setLogger (Logger* logger)
    {
        core_->setLogger (logger);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    LookAndFeel* getLookAndFeel() const
    {
        return lookAndFeel_.get();
    }
    
    juce::ApplicationCommandManager* getCommandManager() const
    {
        return commandManager_.get();
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    juce::Colour getColour (int colourId) const
    {
        return lookAndFeel_->findColour (colourId);
    }
    
    const juce::Font& getConsoleFont() const
    {
        return lookAndFeel_->getConsoleFont();
    }

private:
    std::unique_ptr<LookAndFeel> lookAndFeel_;
    std::unique_ptr<juce::ApplicationCommandManager> commandManager_;
    std::unique_ptr<MenuModel> menu_;
    std::unique_ptr<Wrapper> core_;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpaghettisInstance)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
