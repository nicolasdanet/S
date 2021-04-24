
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
    SpaghettisInstance() :
        lookAndFeel_ (std::make_unique<LookAndFeel>()),
        commandManager_ (std::make_unique<juce::ApplicationCommandManager>()),
        menu_ (std::make_unique<MenuModel>(commandManager_.get())),
        core_ (std::make_unique<Wrapper>()),
        currentOpenDirectory_ (juce::File::getSpecialLocation (juce::File::userHomeDirectory))
    {
        juce::File home = juce::File::getSpecialLocation (juce::File::userHomeDirectory);

        #if ( JUCE_MAC )
        juce::File file = home.getChildFile ("Library/Application Support/Spaghettis/spaghettis.settings");
        #endif
        
        #if ( JUCE_LINUX )
        juce::File file = home.getChildFile (".config/spaghettis/spaghettis.settings");
        #endif
        
        preferences_.reset (new juce::PropertiesFile (file, juce::PropertiesFile::Options()));
        
        #if ! ( SPAGHETTIS_MENUBAR )
        
        juce::MenuBarModel::setMacMainMenu (menu_.get());
        
        #endif
    }
    
    ~SpaghettisInstance()
    {
        #if ! ( SPAGHETTIS_MENUBAR )
        
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

private:
    void handle (const std::function<void()>& f)
    {
        core_->handle (f);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void switchDsp()
    {
        handle (Inputs::switchDsp());
    }
    
    void openPatch (const juce::File& file)
    {
        JUCE_ASSERT_MESSAGE_THREAD
        
        juce::File parent = file.getParentDirectory();
        
        if (parent.isDirectory()) { currentOpenDirectory_ = std::move (parent); }
        else {
            jassertfalse;
        }
        
        handle (Inputs::openFile (file));
    }
    
    void openPatch();
    
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

    juce::MenuBarModel* getMenuBarModel() const
    {
        return menu_.get();
    }
    
    juce::PropertiesFile* getPreferences() const
    {
        return preferences_.get();
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Handy for less typing. */

public:
    juce::Colour getColour (int colourId) const
    {
        return lookAndFeel_->findColour (colourId);
    }
    
    const juce::Font& getConsoleFont() const
    {
        return lookAndFeel_->getConsoleFont();
    }
    
    int getDefaultMenuBarHeight() const
    {
        return lookAndFeel_->getDefaultMenuBarHeight();
    }

private:
    std::unique_ptr<LookAndFeel> lookAndFeel_;
    std::unique_ptr<juce::ApplicationCommandManager> commandManager_;
    std::unique_ptr<MenuModel> menu_;
    std::unique_ptr<Wrapper> core_;
    juce::File currentOpenDirectory_;

private:
    std::unique_ptr<juce::PropertiesFile> preferences_;
    std::unique_ptr<juce::FileChooser> fileChooser_;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpaghettisInstance)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
