
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

class ConsoleWindow;
class DevicesWindow;
class PreferencesWindow;
class SearchPathsWindow;
class PatchesHolder;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

enum class QuitStatus { quit, wait, cancel };
enum class ExitStatus { work, wait, cancel };

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class SpaghettisInstance {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit SpaghettisInstance() :
        lookAndFeel_ (std::make_unique<LNF>()),
        commandManager_ (std::make_unique<juce::ApplicationCommandManager>()),
        menu_ (std::make_unique<MenuModel> (commandManager_.get())),
        core_ (std::make_unique<Wrapper>()),
        audioDevices_ (std::make_unique<AudioDevices>()),
        midiDevices_ (std::make_unique<MidiDevices>()),
        patches_ (std::make_unique<PatchesHolder>()),
        autocomplete_ (std::make_unique<Autocomplete>()),
        currentOpenDirectory_ (juce::File::getSpecialLocation (juce::File::userHomeDirectory)),
        dspIsRunning_ (false),
        quit_ (QuitStatus::quit)
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
        jassert (devicesWindow_      == nullptr);
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

public:
    bool requestToQuit();
    void closeAllPatches();

private:
    void showExitWindow();
    
public:
    ExitStatus getExitStatus() const;
    
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
    void openDevicesWindow();
    void closeDevicesWindow();
    
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
    void updateMenuBar()
    {
        getCommandManager().commandStatusChanged();
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void setDspState (bool isRunning)
    {
        dspIsRunning_ = isRunning; updateMenuBar();
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
    void resetConsole();
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void setCurrentOpenDirectory (const juce::File& file)
    {
        if (file.isDirectory()) { currentOpenDirectory_ = file; }
    }
    
    juce::File getCurrentOpenDirectory() const
    {
        return currentOpenDirectory_;
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    LNF& getLookAndFeel()
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
    
    AudioDevices& getAudioDevices()
    {
        return *audioDevices_;
    }
    
    MidiDevices& getMidiDevices()
    {
        return *midiDevices_;
    }
    
    PatchesHolder& getPatches()
    {
        return *patches_;
    }
    
    Autocomplete& getAutocomplete()
    {
        return *autocomplete_;
    }
    
    Snapshots& getSnapshots()
    {
        return core_->getSnapshots();
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    core::Cached<juce::Colour> getCachedColour (const juce::String& key) const
    {
        return preferences_->getCached<juce::Colour> (Tag::Colors, key);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    const std::unique_ptr<LNF> lookAndFeel_;
    const std::unique_ptr<juce::ApplicationCommandManager> commandManager_;
    const std::unique_ptr<MenuModel> menu_;
    const std::unique_ptr<Wrapper> core_;
    const std::unique_ptr<AudioDevices> audioDevices_;
    const std::unique_ptr<MidiDevices> midiDevices_;
    const std::unique_ptr<PatchesHolder> patches_;
    const std::unique_ptr<Autocomplete> autocomplete_;

private:
    juce::File currentOpenDirectory_;
    bool dspIsRunning_;
    QuitStatus quit_;
    juce::StringArray recentFiles_;
    
private:
    std::unique_ptr<ConsoleWindow> consoleWindow_;
    std::unique_ptr<DevicesWindow> devicesWindow_;
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

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
