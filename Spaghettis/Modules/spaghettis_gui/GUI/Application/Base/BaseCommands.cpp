
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void BaseCommands::set (MenuCommand m)
{
    jassert (!has (m.command_)); enabled_.push_back (m);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool BaseCommands::has (juce::CommandID command)
{
    for (const auto& m : enabled_) {
        if (m.command_ == command) { return m.check_(); }
    }
    
    return false;
}

bool BaseCommands::execute (juce::CommandID command)
{
    for (const auto& m : enabled_) {
        if (m.command_ == command) {
            if (m.check_()) { m.execute_(); return true; }
            else {
                return false;
            }
        }
    }
    
    return false;
}

juce::String BaseCommands::name (juce::CommandID command, juce::String fallback)
{
    for (const auto& m : enabled_) {
        if (m.command_ == command) {
            juce::String s (m.name_()); return (s.isNotEmpty() ? s : fallback);
        }
    }
    
    return fallback;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::String BaseCommands::getCommandText (juce::CommandID command)
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
    case Commands::cut                  : return NEEDS_TRANS ("Cut");
    case Commands::copy                 : return NEEDS_TRANS ("Copy");
    case Commands::paste                : return NEEDS_TRANS ("Paste");
    case Commands::duplicate            : return NEEDS_TRANS ("Duplicate");
    case Commands::remove               : return NEEDS_TRANS ("Delete");
    case Commands::encapsulate          : return NEEDS_TRANS ("Encapsulate");
    case Commands::deencapsulate        : return NEEDS_TRANS ("De-encapsulate");
    case Commands::moveBack             : return NEEDS_TRANS ("Move Back");
    case Commands::moveFront            : return NEEDS_TRANS ("Move Front");
    case Commands::snap                 : return NEEDS_TRANS ("Snap");
    case Commands::zoomIn               : return NEEDS_TRANS ("Zoom In");
    case Commands::zoomOut              : return NEEDS_TRANS ("Zoom Out");
    case Commands::zoomReset            : return NEEDS_TRANS ("Actual size");
    case Commands::inspector            : return NEEDS_TRANS ("Inspector");
    case Commands::clearConsole         : return NEEDS_TRANS ("Clear Console");
    case Commands::devices              : return NEEDS_TRANS ("Devices...");
    case Commands::dspSwitch            : return NEEDS_TRANS ("Run DSP");
    default : break;
    //
    }
    
    return juce::String();
}

juce::String BaseCommands::getCommandDescription (juce::CommandID command)
{
    switch (command) {
    //
    case Commands::preferences          : return NEEDS_TRANS ("Set preferences");
    case Commands::newPatch             : return NEEDS_TRANS ("Create a new patch");
    case Commands::openPatch            : return NEEDS_TRANS ("Open a patch");
    case Commands::clearRecentFiles     : return NEEDS_TRANS ("Clears all recent files");
    case Commands::save                 : return NEEDS_TRANS ("Save file");
    case Commands::closeWindow          : return NEEDS_TRANS ("Close active window");
    case Commands::paths                : return NEEDS_TRANS ("Set search paths");
    case Commands::rescan               : return NEEDS_TRANS ("Rescan search paths");
    case Commands::rescanLogged         : return NEEDS_TRANS ("Rescan search paths");
    case Commands::selectAll            : return NEEDS_TRANS ("Select all");
    case Commands::undo                 : return NEEDS_TRANS ("Undo action");
    case Commands::redo                 : return NEEDS_TRANS ("Redo action");
    case Commands::cut                  : return NEEDS_TRANS ("Cut");
    case Commands::copy                 : return NEEDS_TRANS ("Copy");
    case Commands::paste                : return NEEDS_TRANS ("Paste");
    case Commands::duplicate            : return NEEDS_TRANS ("Duplicate");
    case Commands::remove               : return NEEDS_TRANS ("Delete");
    case Commands::encapsulate          : return NEEDS_TRANS ("Encapsulate");
    case Commands::deencapsulate        : return NEEDS_TRANS ("De-encapsulate");
    case Commands::moveBack             : return NEEDS_TRANS ("Move backward");
    case Commands::moveFront            : return NEEDS_TRANS ("Move frontward");
    case Commands::snap                 : return NEEDS_TRANS ("Snap objects to grid");
    case Commands::zoomIn               : return NEEDS_TRANS ("Increase magnification");
    case Commands::zoomOut              : return NEEDS_TRANS ("Decrease magnification");
    case Commands::zoomReset            : return NEEDS_TRANS ("Reset to original size");
    case Commands::inspector            : return NEEDS_TRANS ("Show/Hide inspector side panel");
    case Commands::clearConsole         : return NEEDS_TRANS ("Clear the console");
    case Commands::devices              : return NEEDS_TRANS ("Audio/MIDI devices");
    case Commands::dspSwitch            : return NEEDS_TRANS ("DSP On/Off");
    default : break;
    //
    }
    
    return juce::String();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void BaseCommands::getCommandInfo (juce::CommandID command, juce::ApplicationCommandInfo& r)
{
    const juce::String general = NEEDS_TRANS ("General");
    const juce::String file    = NEEDS_TRANS ("File");
    const juce::String edit    = NEEDS_TRANS ("Edit");
    const juce::String view    = NEEDS_TRANS ("View");
    const juce::String media   = NEEDS_TRANS ("Media");
    
    const juce::String text         = getCommandText (command);
    const juce::String description  = getCommandDescription (command);
    
    switch (command) {
    //
    case Commands::preferences :
        r.setInfo (text, description, general, 0);
        r.addDefaultKeypress (',', juce::ModifierKeys::commandModifier);
        break;
    case Commands::newPatch :
        r.setInfo (text, description, file, 0);
        r.addDefaultKeypress ('n', juce::ModifierKeys::commandModifier);
        break;
    case Commands::openPatch :
        r.setInfo (text, description, file, 0);
        r.addDefaultKeypress ('o', juce::ModifierKeys::commandModifier);
        break;
    case Commands::clearRecentFiles :
        r.setInfo (text, description, file, 0);
        r.setActive (Spaghettis()->getNumberOfRecentFiles() > 0);
        break;
    case Commands::save :
        r.setInfo (text, description, file, 0);
        r.addDefaultKeypress ('s', juce::ModifierKeys::commandModifier);
        r.setActive (has (command));
        break;
    case Commands::closeWindow :
        r.setInfo (text, description, file, 0);
        r.addDefaultKeypress ('w', juce::ModifierKeys::commandModifier);
        r.setActive (has (command));
        break;
    case Commands::paths :
        r.setInfo (text, description, file, 0);
        break;
    case Commands::rescan :
        r.setInfo (text, description, file, 0);
        break;
    case Commands::rescanLogged :
        r.setInfo (text, description, file, 0);
        break;
    case Commands::selectAll :
        r.setInfo (text, description, edit, 0);
        r.addDefaultKeypress ('a', juce::ModifierKeys::commandModifier);
        r.setActive (has (command));
        break;
    case Commands::undo :
        r.setInfo (name (Commands::undo, text), description, edit, 0);
        r.addDefaultKeypress ('z', juce::ModifierKeys::commandModifier);
        r.setActive (has (command));
        break;
    case Commands::redo :
        r.setInfo (name (Commands::redo, text), description, edit, 0);
        r.addDefaultKeypress ('z', juce::ModifierKeys::shiftModifier | juce::ModifierKeys::commandModifier);
        r.setActive (has (command));
        break;
    case Commands::cut :
        r.setInfo (text, description, edit, 0);
        r.addDefaultKeypress ('x', juce::ModifierKeys::commandModifier);
        r.setActive (has (command));
        break;
    case Commands::copy :
        r.setInfo (text, description, edit, 0);
        r.addDefaultKeypress ('c', juce::ModifierKeys::commandModifier);
        r.setActive (has (command));
        break;
    case Commands::paste :
        r.setInfo (text, description, edit, 0);
        r.addDefaultKeypress ('v', juce::ModifierKeys::commandModifier);
        r.setActive (has (command));
        break;
    case Commands::duplicate :
        r.setInfo (text, description, edit, 0);
        r.addDefaultKeypress ('d', juce::ModifierKeys::commandModifier);
        r.setActive (has (command));
        break;
    case Commands::remove :
        r.setInfo (text, description, edit, 0);
        r.addDefaultKeypress (juce::KeyPress::backspaceKey, juce::ModifierKeys::noModifiers);
        r.setActive (has (command));
        break;
    case Commands::encapsulate :
        r.setInfo (text, description, edit, 0);
        r.setActive (has (command));
        break;
    case Commands::deencapsulate :
        r.setInfo (text, description, edit, 0);
        r.setActive (has (command));
        break;
    case Commands::moveBack :
        r.setInfo (text, description, edit, 0);
        r.setActive (has (command));
        break;
    case Commands::moveFront :
        r.setInfo (text, description, edit, 0);
        r.setActive (has (command));
        break;
    case Commands::snap :
        r.setInfo (text, description, edit, 0);
        r.setActive (has (command));
        break;
    case Commands::zoomIn :
        r.setInfo (text, description, view, 0);
        r.addDefaultKeypress ('+', juce::ModifierKeys::commandModifier);
        r.setActive (has (command));
        break;
    case Commands::zoomOut :
        r.setInfo (text, description, view, 0);
        r.addDefaultKeypress ('-', juce::ModifierKeys::commandModifier);
        r.setActive (has (command));
        break;
    case Commands::zoomReset :
        r.setInfo (text, description, view, 0);
        r.setActive (has (command));
        break;
    case Commands::inspector :
        r.setInfo (text, description, view, 0);
        r.addDefaultKeypress ('i', juce::ModifierKeys::commandModifier);
        r.setActive (has (command));
        break;
    case Commands::clearConsole :
        r.setInfo (text, description, view, 0);
        r.addDefaultKeypress ('l', juce::ModifierKeys::commandModifier);
        break;
    case Commands::devices :
        r.setInfo (text, description, media, 0);
        break;
    case Commands::dspSwitch :
        r.setInfo (text, description, media, 0);
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
            Commands::cut,
            Commands::copy,
            Commands::paste,
            Commands::duplicate,
            Commands::remove,
            Commands::encapsulate,
            Commands::deencapsulate,
            Commands::moveBack,
            Commands::moveFront,
            Commands::snap,
            Commands::zoomIn,
            Commands::zoomOut,
            Commands::zoomReset,
            Commands::inspector,
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
    if (execute (info.commandID) == false) { return performDefaultCommand (info); }
    
    return true;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
