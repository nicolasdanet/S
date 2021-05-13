
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void SpaghettisInstance::start (const juce::StringArray& commandLine)
{
    console_ = std::make_unique<Console>();

    #if ! ( SPAGHETTIS_MENUBAR )
        
    juce::PopupMenu menu (createAppleMenu (commandManager_.get()));
    
    /* Must be done after the console creation to properly set the Apple menu. */
    
    juce::MenuBarModel::setMacMainMenu (menu_.get(), &menu);
        
    #endif
        
    core_->start (commandLine);
}
    
void SpaghettisInstance::shutdown()
{
    core_->shutdown();
    
    searchPathsCloseWindow();
    
    #if ! ( SPAGHETTIS_MENUBAR )
        
    juce::MenuBarModel::setMacMainMenu (nullptr);
        
    #endif
        
    console_ = nullptr;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void SpaghettisInstance::searchPathsOpenWindow()
{
    if (searchPaths_ == nullptr) { searchPaths_ = std::make_unique<SearchPaths>(); }
    else {
        searchPaths_.get()->bringToFront();
    }
}

void SpaghettisInstance::searchPathsCloseWindow()
{
    searchPaths_ = nullptr;
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void SpaghettisInstance::openPatch()
{
    fileChooser_ = std::make_unique<juce::FileChooser> (NEEDS_TRANS ("Choose a Patch to open..."),
                                currentOpenDirectory_,
                                spaghettis::core::getFileExtensions());
    
    int flags = juce::FileBrowserComponent::canSelectMultipleItems
                        | juce::FileBrowserComponent::openMode
                        | juce::FileBrowserComponent::canSelectFiles;
        
    auto callback = [] (const juce::FileChooser& fileChooser)
    {
        auto files = fileChooser.getResults(); for (const auto& f : files) { Spaghettis()->openPatch (f); }
    };
                        
    fileChooser_->launchAsync (flags, callback);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if ! ( SPAGHETTIS_MENUBAR )

juce::PopupMenu SpaghettisInstance::createAppleMenu (juce::ApplicationCommandManager* m)
{
    juce::PopupMenu menu;
    
    menu.addCommandItem (m, Commands::preferences);
    
    return menu;
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

JUCE_IMPLEMENT_SINGLETON (SpaghettisInstance)

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
