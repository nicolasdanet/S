
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
            Commands::fileOpen,
            Commands::paths,
            Commands::rescan,
            Commands::dspSwitch
        };
        
    c.addArray (commands);
}

void Commands::getCommandInfo (const juce::CommandID c, juce::ApplicationCommandInfo& r)
{
    switch (c) {
    //
    case Commands::fileOpen :
        r.setInfo (NEEDS_TRANS ("Open..."), NEEDS_TRANS ("Open a patch"), NEEDS_TRANS ("File"), 0);
        r.addDefaultKeypress ('o', juce::ModifierKeys::commandModifier);
        break;
    case Commands::paths :
        r.setInfo (NEEDS_TRANS ("Paths..."), NEEDS_TRANS ("Set search paths"), NEEDS_TRANS ("File"), 0);
        break;
    case Commands::rescan :
        r.setInfo (NEEDS_TRANS ("Rescan"), NEEDS_TRANS ("Rescan search paths"), NEEDS_TRANS ("File"), 0);
        break;
    case Commands::dspSwitch :
        r.setInfo (NEEDS_TRANS ("Run DSP"), NEEDS_TRANS ("DSP On/Off"), NEEDS_TRANS ("Media"), 0);
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
    case Commands::fileOpen     : spaghettis::Spaghettis()->openPatch();                return true;
    case Commands::paths        : spaghettis::Spaghettis()->searchPathsOpenWindow();    return true;
    case Commands::rescan       : spaghettis::Spaghettis()->rescan();                   return true;
    case Commands::dspSwitch    : spaghettis::Spaghettis()->switchDsp();                return true;
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
