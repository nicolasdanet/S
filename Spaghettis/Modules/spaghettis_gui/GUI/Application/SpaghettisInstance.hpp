
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

class ConsoleWindow;
class PreferencesWindow;
class SearchPathsWindow;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class SpaghettisInstance {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit SpaghettisInstance() :
        lookAndFeel_ (std::make_unique<LookAndFeel>()),
        commandManager_ (std::make_unique<juce::ApplicationCommandManager>()),
        menu_ (std::make_unique<MenuModel> (commandManager_.get())),
        core_ (std::make_unique<Wrapper>()),
        boxRegister_ (std::make_unique<CallOutBoxRegister>()),
        currentOpenDirectory_ (juce::File::getSpecialLocation (juce::File::userHomeDirectory)),
        dspIsRunning_ (false)
    {
        const juce::File home = juce::File::getSpecialLocation (juce::File::userHomeDirectory);

        #if JUCE_MAC
        const juce::File file = home.getChildFile ("Library/Application Support/Spaghettis");
        #endif
        
        #if JUCE_LINUX
        const juce::File file = home.getChildFile (".config/spaghettis");
        #endif
        
        const juce::File properties  = file.getChildFile ("properties.xml");
        const juce::File preferences = file.getChildFile ("preferences.xml");
        
        properties_  = std::make_unique<juce::PropertiesFile> (properties, juce::PropertiesFile::Options());
        preferences_ = std::make_unique<Preferences> (preferences);
        
        loadRecentFiles();  /* MUST be at end. */
    }
    
    ~SpaghettisInstance()
    {
        #if ! ( SPAGHETTIS_MENUBAR )
        
        jassert (juce::MenuBarModel::getMacMainMenu() == nullptr);
        
        #endif
        
        jassert (consoleWindow_      == nullptr);
        jassert (preferencesWindow_  == nullptr);
        jassert (searchPathsWindow_  == nullptr);
    
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

public:
    void handle (const Perform& f)
    {
        core_->addInput (f);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void newPatch()
    {
        const juce::String name (core::getUntitled());
        const juce::String suffix (core::getPatchExtension());
        const juce::File file (getCurrentOpenDirectory().getNonexistentChildFile (name, suffix, false));
        
        handle (Inputs::newPatch (file));
    }
    
    void openPatch (const juce::File& file)
    {
        JUCE_ASSERT_MESSAGE_THREAD
                
        setCurrentOpenDirectory (file.getParentDirectory());
        
        handle (Inputs::openPatch (file));
    }
    
    void openPatch();

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

public:
    void appendRecentFile (const juce::File&);
    int  getNumberOfRecentFiles() const;
    void clearRecentFiles();
    void setRecentFilesMenu (juce::PopupMenu&);
    void openRecentFile (int n);

private:
    void loadRecentFiles();
    void saveRecentFiles();

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void openPreferencesWindow();
    void closePreferencesWindow();

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void openSearchPathsWindow();
    void closeSearchPathsWindow();
    juce::StringArray getSearchPaths();
    void setSearchPaths (const juce::StringArray&);

private:
    void updateSearchPaths (const juce::StringArray&, Inputs::Logged type);
    
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

    void clearConsole();
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void setCurrentOpenDirectory (const juce::File& file)
    {
        if (file.isDirectory()) { currentOpenDirectory_ = file; }
        else {
            jassertfalse;
        }
    }
    
    juce::File getCurrentOpenDirectory() const
    {
        return currentOpenDirectory_;
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    LookAndFeel& getLookAndFeel()
    {
        return *lookAndFeel_;
    }

    juce::MenuBarModel& getMenuBarModel()
    {
        return *menu_;
    }

    juce::ApplicationCommandManager& getCommandManager()
    {
        return *commandManager_;
    }

    juce::PropertiesFile& getProperties()
    {
        return *properties_;
    }
    
    Preferences& getPreferences()
    {
        return *preferences_;
    }
    
    CallOutBoxRegister& getBoxRegister()
    {
        return *boxRegister_;
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    juce::Colour getColour (int colourId) const
    {
        return lookAndFeel_->findColour (colourId);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    const std::unique_ptr<LookAndFeel> lookAndFeel_;
    const std::unique_ptr<juce::ApplicationCommandManager> commandManager_;
    const std::unique_ptr<MenuModel> menu_;
    const std::unique_ptr<Wrapper> core_;
    const std::unique_ptr<CallOutBoxRegister> boxRegister_;

private:
    juce::File currentOpenDirectory_;
    bool dspIsRunning_;
    juce::StringArray recentFiles_;
    
private:
    std::unique_ptr<ConsoleWindow> consoleWindow_;
    std::unique_ptr<PreferencesWindow> preferencesWindow_;
    std::unique_ptr<SearchPathsWindow> searchPathsWindow_;
    std::unique_ptr<juce::PropertiesFile> properties_;
    std::unique_ptr<Preferences> preferences_;
    std::unique_ptr<juce::FileChooser> fileChooser_;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpaghettisInstance)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
