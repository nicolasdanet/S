
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::PopupMenu MenuModel::getMenuForIndex (int n, const juce::String&)
{
    juce::PopupMenu menu;
    
    if (n == 0) {
    //
    juce::PopupMenu recentFiles; Spaghettis()->setRecentFilesMenu (recentFiles);

    recentFiles.addCommandItem (commandManager_, Commands::clearRecentFiles);
    
    menu.addCommandItem (commandManager_, Commands::newPatch);
    menu.addCommandItem (commandManager_, Commands::openPatch);
    menu.addSubMenu (NEEDS_TRANS ("Open Recent"), recentFiles);
    menu.addSeparator();
    #if SPAGHETTIS_MENUBAR
    menu.addCommandItem (commandManager_, Commands::preferences);
    #endif
    menu.addCommandItem (commandManager_, Commands::paths);
    menu.addSeparator();
    menu.addCommandItem (commandManager_, Commands::rescan);
    menu.addCommandItem (commandManager_, Commands::rescanLogged);
    
    #if SPAGHETTIS_MENUBAR
    menu.addSeparator();
    menu.addCommandItem (commandManager_, juce::StandardApplicationCommandIDs::quit);
    #endif
    //
    } else if (n == 1) {
    //
    menu.addCommandItem (commandManager_, Commands::moveBack);
    menu.addCommandItem (commandManager_, Commands::moveFront);
    menu.addSeparator();
    menu.addCommandItem (commandManager_, Commands::selectAll);
    menu.addSeparator();
    menu.addCommandItem (commandManager_, Commands::snap);
    //
    } else if (n == 2) {
    //
    menu.addCommandItem (commandManager_, Commands::zoomIn);
    menu.addCommandItem (commandManager_, Commands::zoomOut);
    menu.addSeparator();
    menu.addCommandItem (commandManager_, Commands::zoomReset);
    menu.addSeparator();
    menu.addCommandItem (commandManager_, Commands::clearConsole);
    //
    } else if (n == 3) {
    //
    menu.addCommandItem (commandManager_, Commands::devices);
    menu.addSeparator();
    menu.addCommandItem (commandManager_, Commands::dspSwitch);
    //
    }
    
    return menu;
}

void MenuModel::menuItemSelected (int n, int index)
{
    if (n >= Commands::recentFiles) { Spaghettis()->openRecentFile (n - Commands::recentFiles); }
}
 
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
