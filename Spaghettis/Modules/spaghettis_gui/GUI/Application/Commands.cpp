
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Commands::getCommandInfo (const juce::CommandID c, juce::ApplicationCommandInfo& r)
{
    const juce::String general = NEEDS_TRANS ("General");
    const juce::String file    = NEEDS_TRANS ("File");
    const juce::String edit    = NEEDS_TRANS ("Edit");
    const juce::String view    = NEEDS_TRANS ("View");
    const juce::String media   = NEEDS_TRANS ("Media");

    switch (c) {
    //
    case Commands::preferences :
        r.setInfo (NEEDS_TRANS ("Preferences..."),  NEEDS_TRANS ("Set preferences"),            general, 0);
        r.addDefaultKeypress (',', juce::ModifierKeys::commandModifier);
        break;
    case Commands::newPatch :
        r.setInfo (NEEDS_TRANS ("New Patch"),       NEEDS_TRANS ("Create a new patch"),         file, 0);
        r.addDefaultKeypress ('n', juce::ModifierKeys::commandModifier);
        break;
    case Commands::openPatch :
        r.setInfo (NEEDS_TRANS ("Open..."),         NEEDS_TRANS ("Open a patch"),               file, 0);
        r.addDefaultKeypress ('o', juce::ModifierKeys::commandModifier);
        break;
    case Commands::clearRecentFiles :
        r.setInfo (NEEDS_TRANS ("Clear Menu"),      NEEDS_TRANS ("Clears all recent files"),    file, 0);
        r.setActive (Spaghettis()->getNumberOfRecentFiles() > 0);
        break;
    case Commands::paths :
        r.setInfo (NEEDS_TRANS ("Paths..."),        NEEDS_TRANS ("Set search paths"),           file, 0);
        break;
    case Commands::rescan :
        r.setInfo (NEEDS_TRANS ("Rescan"),          NEEDS_TRANS ("Rescan search paths"),        file, 0);
        break;
    case Commands::rescanLogged :
        r.setInfo (NEEDS_TRANS ("Rescan Logged"),   NEEDS_TRANS ("Rescan search paths"),        file, 0);
        break;
    case Commands::clearConsole :
        r.setInfo (NEEDS_TRANS ("Clear Console"),   NEEDS_TRANS ("Clear the console"),          edit, 0);
        r.addDefaultKeypress ('l', juce::ModifierKeys::commandModifier);
        break;
    case Commands::zoomIn :
        r.setInfo (NEEDS_TRANS ("Zoom In"),         NEEDS_TRANS ("Increase magnification"),     view, 0);
        r.addDefaultKeypress ('+', juce::ModifierKeys::commandModifier);
        break;
    case Commands::zoomOut :
        r.setInfo (NEEDS_TRANS ("Zoom Out"),        NEEDS_TRANS ("Decrease magnification"),     view, 0);
        r.addDefaultKeypress ('-', juce::ModifierKeys::commandModifier);
        break;
    case Commands::zoomReset :
        r.setInfo (NEEDS_TRANS ("Actual size"),     NEEDS_TRANS ("Reset to original size"),     view, 0);
        break;
    case Commands::devices :
        r.setInfo (NEEDS_TRANS ("Devices..."),      NEEDS_TRANS ("Audio/MIDI devices"),         media, 0);
        break;
    case Commands::dspSwitch :
        r.setInfo (NEEDS_TRANS ("Run DSP"),         NEEDS_TRANS ("DSP On/Off"),                 media, 0);
        r.addDefaultKeypress ('r', juce::ModifierKeys::shiftModifier | juce::ModifierKeys::commandModifier);
        r.setTicked (Spaghettis()->isDspRunning());
        break;
    default : break;
    //
    }
}

void Commands::getAllCommands (juce::Array<juce::CommandID>& c)
{
    juce::Array<juce::CommandID> commands
        {
            Commands::preferences,
            Commands::newPatch,
            Commands::openPatch,
            Commands::clearRecentFiles,
            Commands::paths,
            Commands::rescan,
            Commands::rescanLogged,
            Commands::clearConsole,
            Commands::zoomIn,
            Commands::zoomOut,
            Commands::zoomReset,
            Commands::devices,
            Commands::dspSwitch
        };
        
    c.addArray (commands);
}

void Commands::getApplicationCommands (juce::Array<juce::CommandID>& c)
{
    c.add (Commands::preferences);
}

bool Commands::perform (const juce::ApplicationCommandTarget::InvocationInfo& info)
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
    case Commands::zoomIn           : DBG ("ZOOM IN");
                                      return true;
    case Commands::zoomOut          : DBG ("ZOOM OUT");
                                      return true;
    case Commands::zoomReset        : DBG ("ZOOM RESET");
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
