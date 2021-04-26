
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Commands::getCommandInfo (const juce::CommandID c, juce::ApplicationCommandInfo& r)
{
    switch (c) {
    //
    case Commands::fileOpen :
        r.setInfo (NEEDS_TRANS ("Open..."), NEEDS_TRANS ("Open a Patch"), NEEDS_TRANS ("File"), 0);
        r.addDefaultKeypress ('o', juce::ModifierKeys::commandModifier);
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

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
