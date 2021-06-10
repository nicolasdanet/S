
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Commands::getAllCommands (juce::Array<juce::CommandID>& c)
{
    juce::Array<juce::CommandID> commands
        {
            Commands::preferences,
            Commands::fileOpen,
            Commands::paths,
            Commands::rescan,
            Commands::rescanLogged,
            Commands::clearConsole,
            Commands::dspSwitch
        };
        
    c.addArray (commands);
}

void Commands::getCommandInfo (const juce::CommandID c, juce::ApplicationCommandInfo& r)
{
    static const char* const general    = NEEDS_TRANS ("General");
    static const char* const file       = NEEDS_TRANS ("File");
    static const char* const edit       = NEEDS_TRANS ("Edit");
    static const char* const media      = NEEDS_TRANS ("Media");

    switch (c) {
    //
    case Commands::preferences :
        r.setInfo (NEEDS_TRANS ("Preferences..."),  NEEDS_TRANS ("Set preferences"),        general, 0);
        r.addDefaultKeypress (',', juce::ModifierKeys::commandModifier);
        break;
    case Commands::fileOpen :
        r.setInfo (NEEDS_TRANS ("Open..."),         NEEDS_TRANS ("Open a patch"),           file, 0);
        r.addDefaultKeypress ('o', juce::ModifierKeys::commandModifier);
        break;
    case Commands::paths :
        r.setInfo (NEEDS_TRANS ("Paths..."),        NEEDS_TRANS ("Set search paths"),       file, 0);
        break;
    case Commands::rescan :
        r.setInfo (NEEDS_TRANS ("Rescan"),          NEEDS_TRANS ("Rescan search paths"),    file, 0);
        break;
    case Commands::rescanLogged :
        r.setInfo (NEEDS_TRANS ("Rescan Logged"),   NEEDS_TRANS ("Rescan search paths"),    file, 0);
        break;
    case Commands::clearConsole :
        r.setInfo (NEEDS_TRANS ("Clear Console"),   NEEDS_TRANS ("Clear the console"),      edit, 0);
        r.addDefaultKeypress ('l', juce::ModifierKeys::commandModifier);
        break;
    case Commands::dspSwitch :
        r.setInfo (NEEDS_TRANS ("Run DSP"),         NEEDS_TRANS ("DSP On/Off"),             media, 0);
        r.addDefaultKeypress ('r', juce::ModifierKeys::shiftModifier | juce::ModifierKeys::commandModifier);
        r.setTicked (Spaghettis()->isDspRunning());
        break;
    default : break;
    //
    }
}

bool Commands::perform (const juce::ApplicationCommandTarget::InvocationInfo& info)
{
    switch (info.commandID) {
    //
    case Commands::preferences  : Spaghettis()->handle (Inputs::ping());                        return true;
    case Commands::fileOpen     : Spaghettis()->openPatch();                                    return true;
    case Commands::paths        : Spaghettis()->openSearchPathsWindow();                        return true;
    case Commands::rescan       : Spaghettis()->handle (Inputs::rescan (Inputs::Logged::base)); return true;
    case Commands::rescanLogged : Spaghettis()->handle (Inputs::rescan (Inputs::Logged::full)); return true;
    case Commands::clearConsole : Spaghettis()->clearConsole();                                 return true;
    case Commands::dspSwitch    : Spaghettis()->handle (Inputs::switchDsp());                   return true;
    default : break;
    //
    }

    return false;
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
