
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
    
    updateSearchPaths (getSearchPaths(), Inputs::Logged::none);
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

void SpaghettisInstance::updateSearchPaths (const juce::StringArray& searchpaths, Inputs::Logged type)
{
    handle (Inputs::setSearchPaths (searchpaths));
    handle (Inputs::rescan (type));
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

void SpaghettisInstance::setSearchPaths (const juce::StringArray& searchpaths)
{
    auto root = std::make_unique<juce::XmlElement> ("SEARCHPATHS");
        
    for (const auto& p : searchpaths) {
        juce::XmlElement* e = root->createNewChildElement ("SEARCHPATH");
        e->setAttribute (Ids::path, p);
    }
        
    preferences_->setValue ("SearchPaths", root.get());
    
    updateSearchPaths (searchpaths, Inputs::Logged::base);
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
// MARK: -

void SpaghettisInstance::appendRecentFile (const juce::File& file)
{
    const int maximum = 2;
    
    juce::String s (file.getFullPathName());
    
    recentFiles_.removeString (s);
    recentFiles_.insert (0, s);
    
    while (recentFiles_.size() > maximum) { recentFiles_.remove (recentFiles_.size() - 1); }
    
    menu_->menuItemsChanged(); saveRecentFiles();
}

int SpaghettisInstance::getNumberOfRecentFiles() const
{
    return recentFiles_.size();
}

void SpaghettisInstance::clearRecentFiles()
{
    recentFiles_.clear(); menu_->menuItemsChanged(); saveRecentFiles();
}

void SpaghettisInstance::setRecentFilesMenu (juce::PopupMenu& m)
{
    int i = Commands::recentFiles;
    
    juce::StringArray t (getFilesShortIfPossible (recentFiles_));
    
    for (const auto& path : t) { m.addItem (i++, path); }
    
    if (m.getNumItems() > 0) { m.addSeparator(); }
}

void SpaghettisInstance::openRecentFile (int n)
{
    if (juce::isPositiveAndBelow (n, recentFiles_.size())) { openPatch (juce::File (recentFiles_[n])); }
}

void SpaghettisInstance::loadRecentFiles()
{
    std::unique_ptr<juce::XmlElement> root = preferences_->getXmlValue ("RecentFiles");
        
    if (root && root->hasTagName ("RECENTFILES")) {
    //
    juce::StringArray scoped;
    
    for (auto* e : root->getChildWithTagNameIterator ("RECENTFILE")) {
        if (e->hasAttribute (Ids::path)) {
            scoped.addIfNotAlreadyThere (e->getStringAttribute (Ids::path));
        }
    }
    
    scoped.swapWith (recentFiles_);
    //
    }
}

void SpaghettisInstance::saveRecentFiles()
{
    auto root = std::make_unique<juce::XmlElement> ("RECENTFILES");
        
    for (const auto& f : recentFiles_) {
        juce::XmlElement* e = root->createNewChildElement ("RECENTFILE");
        e->setAttribute (Ids::path, f);
    }
        
    preferences_->setValue ("RecentFiles", root.get());
}

juce::StringArray SpaghettisInstance::getFilesShortIfPossible (const juce::StringArray& a)
{
    juce::StringArray t; t.ensureStorageAllocated (a.size());
    
    for (const auto& s : a) { if (!t.addIfNotAlreadyThere (juce::File (s).getFileName())) { return a; } }
    
    return t;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void SpaghettisInstance::clearConsole()
{
    if (console_) { console_->clear(); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

JUCE_IMPLEMENT_SINGLETON (SpaghettisInstance)

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
