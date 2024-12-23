
/* Copyright (c) 2021 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

class ConsoleWindow;
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
        patches_ (std::make_unique<PatchesHolder>()),
        autocomplete_ (std::make_unique<Autocomplete>()),
        devices_ (std::make_unique<AvailableDevices>()),
        documentation_ (std::make_unique<Documentation>()),
        dspIsRunning_ (false),
        quit_ (QuitStatus::quit),
        recentFiles_()
    {
        const juce::File home = juce::File::getSpecialLocation (juce::File::userHomeDirectory);

        Directories::getInstance()->setDefaultOpen (home);
        
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
        preferences_ = std::make_unique<Preferences> (preferences, getAvailableDevices());
        
        loadRecentFiles();  /* MUST be at end. */
    }
    
    ~SpaghettisInstance()
    {
        #if ! ( SPAGHETTIS_MENUBAR )
        
        jassert (juce::MenuBarModel::getMacMainMenu() == nullptr);
        
        #endif
        
        jassert (consoleWindow_      == nullptr);
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
        const juce::File open (Directories::getInstance()->getDefaultOpen());
        const juce::File file (open.getNonexistentChildFile (name, suffix, false));
        
        handle (Inputs::patchNew (file));
    }
    
    void openPatch (const juce::File& file)
    {
        JUCE_ASSERT_MESSAGE_THREAD
                
        Directories::getInstance()->setDefaultOpen (file.getParentDirectory());
        
        handle (Inputs::patchOpen (file));
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
    void updateSearchPaths  (Inputs::Logged type = Inputs::Logged::base);
    void updateAudioDevices ();
    void updateMidiDevices  ();

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void rescanSearchPaths  (Inputs::Logged type = Inputs::Logged::base);
    void rescanMidiDevices  (Inputs::Logged type = Inputs::Logged::base);
    void rescanAudioDevices (Inputs::Logged type = Inputs::Logged::base);
    
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
    
    SnapshotsManager& getSnapshots()
    {
        return core_->getSnapshots();
    }
    
    PatchesHolder& getPatches()
    {
        return *patches_;
    }
    
    Autocomplete& getAutocomplete()
    {
        return *autocomplete_;
    }
    
    AvailableDevices& getAvailableDevices()
    {
        return *devices_;
    }
    
    Documentation& getDocumentation()
    {
        return *documentation_;
    }
    
    Preferences& getPreferences()
    {
        jassert (preferences_.get()); return *preferences_;
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
    const std::unique_ptr<PatchesHolder> patches_;
    const std::unique_ptr<Autocomplete> autocomplete_;
    const std::unique_ptr<AvailableDevices> devices_;
    const std::unique_ptr<Documentation> documentation_;

private:
    bool dspIsRunning_;
    QuitStatus quit_;
    juce::StringArray recentFiles_;
    
private:
    std::unique_ptr<Preferences> preferences_;
    std::unique_ptr<juce::PropertiesFile> properties_;
    std::unique_ptr<ConsoleWindow> consoleWindow_;
    std::unique_ptr<PreferencesWindow> preferencesWindow_;
    std::unique_ptr<juce::FileChooser> fileChooser_;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpaghettisInstance)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
