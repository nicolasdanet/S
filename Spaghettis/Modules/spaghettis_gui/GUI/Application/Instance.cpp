
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void SpaghettisInstance::start (const juce::StringArray& commandLine)
{
    consoleWindow_ = std::make_unique<ConsoleWindow>();

    #if ! ( SPAGHETTIS_MENUBAR )
        
    juce::PopupMenu menu (MenuModel::createAppleMenu (commandManager_.get()));
    
    /* Must be done after the console creation to properly set the Apple menu. */
    
    juce::MenuBarModel::setMacMainMenu (menu_.get(), &menu);
        
    #endif
    
    core_->start (commandLine);
    
    updateSearchPaths (getSearchPaths(), Inputs::Logged::none);
    
    preferences_->read();
}
    
void SpaghettisInstance::shutdown()
{
    core_->shutdown();
    
    closeSearchPathsWindow();
    
    #if ! ( SPAGHETTIS_MENUBAR )
        
    juce::MenuBarModel::setMacMainMenu (nullptr);
        
    #endif
        
    consoleWindow_     = nullptr;
    devicesWindow_     = nullptr;
    preferencesWindow_ = nullptr;
    searchPathsWindow_ = nullptr;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void SpaghettisInstance::showExitWindow()
{
    auto f = [this] (int result)
    {
        quit_ = (result == 1) ? QuitStatus::quit : QuitStatus::cancel;
    };
    
    quit_ = QuitStatus::wait;
        
    juce::NativeMessageBox::showYesNoBox (juce::MessageBoxIconType::QuestionIcon,
        core::getApplicationName(),
        NEEDS_TRANS ("Do you really want to quit?"),
        consoleWindow_.get(),
        juce::ModalCallbackFunction::create (f));
}

bool SpaghettisInstance::requestToQuit()
{
    /* On Linux don't let quitting while already opened modal windows remain. */
    
    if (!alertRegister_->isEmpty()) { return false; }
    else {
    //
    quit_ = QuitStatus::quit;
    
    if (!patches_->isEmpty()) {
        if (preferences_->getCached<bool> (Tags::General, Tags::AskBeforeQuit) == true) {
            showExitWindow();
        }
    }
    
    return true;
    //
    }
}
    
void SpaghettisInstance::closeAllPatches()
{
    patches_->closeAllPatches();
}

ExitStatus SpaghettisInstance::getExitStatus() const
{
    if (quit_ == QuitStatus::quit && patches_->isAllRequestsDone()) { return ExitStatus::work; }
    else if (quit_ == QuitStatus::cancel) { return ExitStatus::cancel; }
    else {
        return ExitStatus::wait;
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

template <class T> void createOrOpenWindow (std::unique_ptr<T>& p)
{
    if (p == nullptr) { p = std::make_unique<T>(); }
    else {
        p.get()->toFront (true);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void SpaghettisInstance::openPreferencesWindow()
{
    createOrOpenWindow (preferencesWindow_);
}

void SpaghettisInstance::closePreferencesWindow()
{
    preferencesWindow_ = nullptr;
}

void SpaghettisInstance::openDevicesWindow()
{
    createOrOpenWindow (devicesWindow_);
}

void SpaghettisInstance::closeDevicesWindow()
{
    devicesWindow_ = nullptr;
}

void SpaghettisInstance::openSearchPathsWindow()
{
    createOrOpenWindow (searchPathsWindow_);
}

void SpaghettisInstance::closeSearchPathsWindow()
{
    searchPathsWindow_ = nullptr;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void SpaghettisInstance::updateSearchPaths (const juce::StringArray& searchpaths, Inputs::Logged type)
{
    handle (Inputs::setSearchPaths (searchpaths));
    handle (Inputs::rescanSearchPaths (type));
}

juce::StringArray SpaghettisInstance::getSearchPaths()
{
    juce::StringArray searchPaths;
    
    const std::unique_ptr<juce::XmlElement> root (properties_->getXmlValue ("SearchPaths"));
        
    if (root && root->hasTagName (Ids::SEARCHPATHS)) {
    //
    for (auto* e : root->getChildWithTagNameIterator (Ids::SEARCHPATH)) {
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
    auto root = std::make_unique<juce::XmlElement> (Ids::SEARCHPATHS);
        
    for (const auto& p : searchpaths) {
        juce::XmlElement* e = root->createNewChildElement (Ids::SEARCHPATH);
        e->setAttribute (Ids::path, p);
    }
        
    properties_->setValue ("SearchPaths", root.get());
    
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

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

juce::StringArray getFilesShortIfPossible (const juce::StringArray& a)
{
    juce::StringArray t; t.ensureStorageAllocated (a.size());
    
    for (const auto& s : a) { if (!t.addIfNotAlreadyThere (juce::File (s).getFileName())) { return a; } }
    
    return t;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void SpaghettisInstance::appendRecentFile (const juce::File& file)
{
    const int maximum = 16;
    
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
    const std::unique_ptr<juce::XmlElement> root (properties_->getXmlValue ("RecentFiles"));
        
    if (root && root->hasTagName (Ids::RECENTFILES)) {
    //
    juce::StringArray scoped;
    
    for (auto* e : root->getChildWithTagNameIterator (Ids::RECENTFILE)) {
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
    auto root = std::make_unique<juce::XmlElement> (Ids::RECENTFILES);
        
    for (const auto& f : recentFiles_) {
        juce::XmlElement* e = root->createNewChildElement (Ids::RECENTFILE);
        e->setAttribute (Ids::path, f);
    }
        
    properties_->setValue ("RecentFiles", root.get());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void SpaghettisInstance::clearConsole()
{
    if (consoleWindow_) { consoleWindow_->clear(); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

JUCE_IMPLEMENT_SINGLETON (SpaghettisInstance)

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
