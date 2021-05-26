
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
        
    juce::PopupMenu menu (MenuModel::createAppleMenu (commandManager_.get()));
    
    /* Must be done after the console creation to properly set the Apple menu. */
    
    juce::MenuBarModel::setMacMainMenu (menu_.get(), &menu);
        
    #endif
        
    core_->start (commandLine);
}
    
void SpaghettisInstance::shutdown()
{
    core_->shutdown();
    
    closeSearchPathsWindow();
    
    #if ! ( SPAGHETTIS_MENUBAR )
        
    juce::MenuBarModel::setMacMainMenu (nullptr);
        
    #endif
        
    console_ = nullptr;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void SpaghettisInstance::openSearchPathsWindow()
{
    if (searchPaths_ == nullptr) { searchPaths_ = std::make_unique<SearchPaths>(); }
    else {
        searchPaths_.get()->toFront (true);
    }
}

void SpaghettisInstance::closeSearchPathsWindow()
{
    searchPaths_ = nullptr;
}

juce::StringArray SpaghettisInstance::getSearchPaths()
{
    juce::StringArray searchPaths;
    
    std::unique_ptr<juce::XmlElement> root = preferences_->getXmlValue ("SearchPaths");
        
    if (root && root->hasTagName ("SEARCHPATHS")) {
    //
    for (auto* e : root->getChildWithTagNameIterator ("SEARCHPATH")) {
        if (e->hasAttribute (Ids::path)) {
            searchPaths.addIfNotAlreadyThere (e->getStringAttribute (Ids::path));
        }
    }
    //
    }
    
    return searchPaths;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void SpaghettisInstance::openPatch()
{
    fileChooser_ = std::make_unique<juce::FileChooser> (NEEDS_TRANS ("Choose a Patch..."),
                        getCurrentOpenDirectory(),
                        spaghettis::core::getFileExtensions());
    
    const int t = juce::FileBrowserComponent::canSelectMultipleItems
                        | juce::FileBrowserComponent::openMode
                        | juce::FileBrowserComponent::canSelectFiles;
        
    auto callback = [] (const juce::FileChooser& fileChooser)
    {
        auto files = fileChooser.getResults(); for (const auto& f : files) { Spaghettis()->openPatch (f); }
    };
                        
    fileChooser_->launchAsync (t, callback);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

JUCE_IMPLEMENT_SINGLETON (SpaghettisInstance)

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
