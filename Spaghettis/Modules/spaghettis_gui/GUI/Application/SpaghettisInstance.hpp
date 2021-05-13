
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

class Console;
class SearchPaths;

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
        menu_ (std::make_unique<MenuModel> (commandManager_.get())),
        core_ (std::make_unique<Wrapper>()),
        currentOpenDirectory_ (juce::File::getSpecialLocation (juce::File::userHomeDirectory)),
        dspIsRunning_ (false)
    {
        const juce::File home = juce::File::getSpecialLocation (juce::File::userHomeDirectory);

        #if JUCE_MAC
        juce::File file = home.getChildFile ("Library/Application Support/Spaghettis/spaghettis.settings");
        #endif
        
        #if JUCE_LINUX
        juce::File file = home.getChildFile (".config/spaghettis/spaghettis.settings");
        #endif
        
        preferences_ = std::make_unique<juce::PropertiesFile> (file, juce::PropertiesFile::Options());
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
    void start (const juce::StringArray& commandLine);
    void shutdown();
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void handle (const std::function<void()>& f)
    {
        core_->addInput (f);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void switchDsp()
    {
        handle (Inputs::switchDsp());
    }
    
    void rescan()
    {
        handle (Inputs::rescan());
    }
    
    void openPatch (const juce::File& file)
    {
        JUCE_ASSERT_MESSAGE_THREAD
        
        const juce::File parent = file.getParentDirectory();
        
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
    void searchPathsOpenWindow();
    void searchPathsCloseWindow();
    
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
    void setDspState (bool isRunning)
    {
        dspIsRunning_ = isRunning; getCommandManager().commandStatusChanged();
    }
    
    bool isDspRunning() const
    {
        return dspIsRunning_;
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
    
    juce::MenuBarModel* getMenuBarModel() const
    {
        return menu_.get();
    }

public:
    juce::ApplicationCommandManager& getCommandManager()
    {
        return *commandManager_;
    }

    juce::PropertiesFile& getPreferences()
    {
        return *preferences_;
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

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if ! ( SPAGHETTIS_MENUBAR )

private:
    static juce::PopupMenu createAppleMenu (juce::ApplicationCommandManager *);

#endif

private:
    std::unique_ptr<LookAndFeel> lookAndFeel_;
    std::unique_ptr<juce::ApplicationCommandManager> commandManager_;
    std::unique_ptr<MenuModel> menu_;
    std::unique_ptr<Wrapper> core_;
    juce::File currentOpenDirectory_;
    bool dspIsRunning_;
    
private:
    std::unique_ptr<Console> console_;
    std::unique_ptr<SearchPaths> searchPaths_;
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
