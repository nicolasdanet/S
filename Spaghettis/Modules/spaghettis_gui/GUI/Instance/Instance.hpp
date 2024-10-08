
/* Copyright (c) 2021 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

class ConsoleWindow;
class DevicesWindow;
class PreferencesWindow;
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
        commandManager_ (std::make_unique<juce::ApplicationCommandManager>()),
        menu_ (std::make_unique<MenuModel> (commandManager_.get())),
        core_ (std::make_unique<Wrapper>()),
        audioDevices_ (std::make_unique<AudioDevices>()),
        midiDevices_ (std::make_unique<MidiDevices>()),
        patches_ (std::make_unique<PatchesHolder>()),
        autocomplete_ (std::make_unique<Autocomplete>()),
        dspIsRunning_ (false),
        quit_ (QuitStatus::quit)
    {
        const juce::File home = juce::File::getSpecialLocation (juce::File::userHomeDirectory);

        Folders::getInstance()->setDefaultOpen (home);
        
        #if JUCE_MAC
        const juce::File file = home.getChildFile ("Library/Application Support/Spaghettis");
        #endif
        
        #if JUCE_LINUX
        const juce::File file = home.getChildFile (".config/spaghettis");
        #endif
        
        const juce::PropertiesFile::Options options;
        
        const juce::File properties  = file.getChildFile ("Configuration.settings");
        const juce::File preferences = file.getChildFile ("Preferences.settings");
        
        properties_  = std::make_unique<juce::PropertiesFile> (properties, options);
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
        const juce::File open (Folders::getInstance()->getDefaultOpen());
        const juce::File file (open.getNonexistentChildFile (name, suffix, false));
        
        handle (Inputs::newPatch (file));
    }
    
    void openPatch (const juce::File& file)
    {
        JUCE_ASSERT_MESSAGE_THREAD
                
        Folders::getInstance()->setDefaultOpen (file.getParentDirectory());
        
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
    void updateSearchPaths (Inputs::Logged type = Inputs::Logged::base);

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
    juce::ApplicationCommandManager& getCommandManager()
    {
        return *commandManager_;
    }

    juce::MenuBarModel& getMenu()
    {
        return *menu_;
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
    
    SnapshotsManager& getSnapshots()
    {
        return core_->getSnapshots();
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    data::Cached<juce::Colour> getCachedColour (const juce::String& key) const
    {
        return preferences_->getCached<juce::Colour> (Tag::Colors, key);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    const std::unique_ptr<juce::ApplicationCommandManager> commandManager_;
    const std::unique_ptr<MenuModel> menu_;
    const std::unique_ptr<Wrapper> core_;
    const std::unique_ptr<AudioDevices> audioDevices_;
    const std::unique_ptr<MidiDevices> midiDevices_;
    const std::unique_ptr<PatchesHolder> patches_;
    const std::unique_ptr<Autocomplete> autocomplete_;

private:
    bool dspIsRunning_;
    QuitStatus quit_;
    juce::StringArray recentFiles_;
    
private:
    std::unique_ptr<ConsoleWindow> consoleWindow_;
    std::unique_ptr<DevicesWindow> devicesWindow_;
    std::unique_ptr<PreferencesWindow> preferencesWindow_;
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
