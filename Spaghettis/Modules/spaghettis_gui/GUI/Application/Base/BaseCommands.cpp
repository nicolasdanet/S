
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void BaseCommands::set (juce::CommandID command, std::function<void()> f, std::function<bool()> g)
{
    jassert (!get (command)); enabled_.emplace_back (command, f, g);
}

bool BaseCommands::get (juce::CommandID command, bool invoke)
{
    for (const auto& [c, f, g] : enabled_) {
        if (c == command) {
            if (g()) {
                if (invoke) { f(); } return true;
            } else {
                return false;
            }
        }
    }
    
    return false;
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::String BaseCommands::getCommandName (juce::CommandID command)
{
    switch (command) {
    //
    case Commands::preferences          : return NEEDS_TRANS ("Preferences...");
    case Commands::newPatch             : return NEEDS_TRANS ("New Patch");
    case Commands::openPatch            : return NEEDS_TRANS ("Open...");
    case Commands::clearRecentFiles     : return NEEDS_TRANS ("Clear Menu");
    case Commands::save                 : return NEEDS_TRANS ("Save");
    case Commands::closeWindow          : return NEEDS_TRANS ("Close Window");
    case Commands::paths                : return NEEDS_TRANS ("Paths...");
    case Commands::rescan               : return NEEDS_TRANS ("Rescan");
    case Commands::rescanLogged         : return NEEDS_TRANS ("Rescan Logged");
    case Commands::selectAll            : return NEEDS_TRANS ("Select All");
    case Commands::undo                 : return NEEDS_TRANS ("Undo");
    case Commands::redo                 : return NEEDS_TRANS ("Redo");
    case Commands::moveBack             : return NEEDS_TRANS ("Move Back");
    case Commands::moveFront            : return NEEDS_TRANS ("Move Front");
    case Commands::snap                 : return NEEDS_TRANS ("Snap");
    case Commands::zoomIn               : return NEEDS_TRANS ("Zoom In");
    case Commands::zoomOut              : return NEEDS_TRANS ("Zoom Out");
    case Commands::zoomReset            : return NEEDS_TRANS ("Actual size");
    case Commands::clearConsole         : return NEEDS_TRANS ("Clear Console");
    case Commands::devices              : return NEEDS_TRANS ("Devices...");
    case Commands::dspSwitch            : return NEEDS_TRANS ("Run DSP");
    default : break;
    //
    }
    
    return "";
}

void BaseCommands::getCommandInfo (juce::CommandID command, juce::ApplicationCommandInfo& r)
{
    const juce::String general = NEEDS_TRANS ("General");
    const juce::String file    = NEEDS_TRANS ("File");
    const juce::String edit    = NEEDS_TRANS ("Edit");
    const juce::String view    = NEEDS_TRANS ("View");
    const juce::String media   = NEEDS_TRANS ("Media");
    
    const juce::String name    = getCommandName (command);
    
    switch (command) {
    //
    case Commands::preferences :
        r.setInfo (name,  NEEDS_TRANS ("Set preferences"),            general, 0);
        r.addDefaultKeypress (',', juce::ModifierKeys::commandModifier);
        break;
    case Commands::newPatch :
        r.setInfo (name,       NEEDS_TRANS ("Create a new patch"),         file, 0);
        r.addDefaultKeypress ('n', juce::ModifierKeys::commandModifier);
        break;
    case Commands::openPatch :
        r.setInfo (name,         NEEDS_TRANS ("Open a patch"),               file, 0);
        r.addDefaultKeypress ('o', juce::ModifierKeys::commandModifier);
        break;
    case Commands::clearRecentFiles :
        r.setInfo (name,      NEEDS_TRANS ("Clears all recent files"),    file, 0);
        r.setActive (Spaghettis()->getNumberOfRecentFiles() > 0);
        break;
    case Commands::save :
        r.setInfo (name,            NEEDS_TRANS ("Save file"),                  file, 0);
        r.addDefaultKeypress ('s', juce::ModifierKeys::commandModifier);
        r.setActive (get (command));
        break;
    case Commands::closeWindow :
        r.setInfo (name,    NEEDS_TRANS ("Close active window"),        file, 0);
        r.addDefaultKeypress ('w', juce::ModifierKeys::commandModifier);
        r.setActive (get (command));
        break;
    case Commands::paths :
        r.setInfo (name,        NEEDS_TRANS ("Set search paths"),           file, 0);
        break;
    case Commands::rescan :
        r.setInfo (name,          NEEDS_TRANS ("Rescan search paths"),        file, 0);
        break;
    case Commands::rescanLogged :
        r.setInfo (name,   NEEDS_TRANS ("Rescan search paths"),        file, 0);
        break;
    case Commands::selectAll :
        r.setInfo (name,      NEEDS_TRANS ("Select all"),                 edit, 0);
        r.addDefaultKeypress ('a', juce::ModifierKeys::commandModifier);
        r.setActive (get (command));
        break;
    case Commands::undo :
        r.setInfo (name,            NEEDS_TRANS ("Undo action"),                edit, 0);
        r.addDefaultKeypress ('z', juce::ModifierKeys::commandModifier);
        r.setActive (get (command));
        break;
    case Commands::redo :
        r.setInfo (name,            NEEDS_TRANS ("Redo action"),                edit, 0);
        r.addDefaultKeypress ('z', juce::ModifierKeys::shiftModifier | juce::ModifierKeys::commandModifier);
        r.setActive (get (command));
        break;
    case Commands::moveBack :
        r.setInfo (name,       NEEDS_TRANS ("Move backward"),              edit, 0);
        r.setActive (get (command));
        break;
    case Commands::moveFront :
        r.setInfo (name,      NEEDS_TRANS ("Move frontward"),             edit, 0);
        r.setActive (get (command));
        break;
    case Commands::snap :
        r.setInfo (name,            NEEDS_TRANS ("Snap objects to grid"),       edit, 0);
        r.setActive (get (command));
        break;
    case Commands::zoomIn :
        r.setInfo (name,         NEEDS_TRANS ("Increase magnification"),     view, 0);
        r.addDefaultKeypress ('+', juce::ModifierKeys::commandModifier);
        r.setActive (get (command));
        break;
    case Commands::zoomOut :
        r.setInfo (name,        NEEDS_TRANS ("Decrease magnification"),     view, 0);
        r.addDefaultKeypress ('-', juce::ModifierKeys::commandModifier);
        r.setActive (get (command));
        break;
    case Commands::zoomReset :
        r.setInfo (name,     NEEDS_TRANS ("Reset to original size"),     view, 0);
        r.setActive (get (command));
        break;
    case Commands::clearConsole :
        r.setInfo (name,   NEEDS_TRANS ("Clear the console"),          view, 0);
        r.addDefaultKeypress ('l', juce::ModifierKeys::commandModifier);
        break;
    case Commands::devices :
        r.setInfo (name,      NEEDS_TRANS ("Audio/MIDI devices"),         media, 0);
        break;
    case Commands::dspSwitch :
        r.setInfo (name,         NEEDS_TRANS ("DSP On/Off"),                 media, 0);
        r.addDefaultKeypress ('r', juce::ModifierKeys::shiftModifier | juce::ModifierKeys::commandModifier);
        r.setTicked (Spaghettis()->isDspRunning());
        break;
    default : break;
    //
    }
}

void BaseCommands::getAllCommands (juce::Array<juce::CommandID>& c)
{
    juce::Array<juce::CommandID> commands
        {
            Commands::preferences,
            Commands::newPatch,
            Commands::openPatch,
            Commands::clearRecentFiles,
            Commands::save,
            Commands::closeWindow,
            Commands::paths,
            Commands::rescan,
            Commands::rescanLogged,
            Commands::selectAll,
            Commands::undo,
            Commands::redo,
            Commands::moveBack,
            Commands::moveFront,
            Commands::snap,
            Commands::zoomIn,
            Commands::zoomOut,
            Commands::zoomReset,
            Commands::clearConsole,
            Commands::devices,
            Commands::dspSwitch
        };
        
    c.addArray (commands);
}

void BaseCommands::getApplicationCommands (juce::Array<juce::CommandID>& c)
{
    c.add (Commands::preferences);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

bool performDefaultCommand (const juce::ApplicationCommandTarget::InvocationInfo& info)
{
    switch (info.commandID) {
    //
    case Commands::preferences      : Spaghettis()->openPreferencesWindow();
                                      return true;
    case Commands::newPatch         : Spaghettis()->newPatch();
                                      return true;
    case Commands::openPatch        : Spaghettis()->openPatch();
                                      return true;
    case Commands::clearRecentFiles : Spaghettis()->clearRecentFiles();
                                      return true;
    case Commands::paths            : Spaghettis()->openSearchPathsWindow();
                                      return true;
    case Commands::rescan           : Spaghettis()->handle (Inputs::rescanSearchPaths (Inputs::Logged::base));
                                      return true;
    case Commands::rescanLogged     : Spaghettis()->handle (Inputs::rescanSearchPaths (Inputs::Logged::full));
                                      return true;
    case Commands::clearConsole     : Spaghettis()->clearConsole();
                                      return true;
    case Commands::devices          : Spaghettis()->openDevicesWindow();
                                      return true;
    case Commands::dspSwitch        : Spaghettis()->handle (Inputs::switchDsp());
                                      return true;
    default : break;
    //
    }

    return false;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool BaseCommands::perform (const juce::ApplicationCommandTarget::InvocationInfo& info)
{
    if (get (info.commandID, true) == false) { return performDefaultCommand (info); }
    
    return true;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
