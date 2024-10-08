
/* Copyright (c) 2021 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void SpaghettisInstance::start (const juce::StringArray& commandLine)
{
    consoleWindow_ = std::make_unique<ConsoleWindow> (*commandManager_, properties_.get());

    #if ! ( SPAGHETTIS_MENUBAR )
        
    juce::PopupMenu menu (MenuModel::createAppleMenu (commandManager_.get()));
    
    /* Must be done after the console creation to properly set the Apple menu. */
    
    juce::MenuBarModel::setMacMainMenu (menu_.get(), &menu);
        
    #endif
    
    core_->start (commandLine);
    
    updateSearchPaths (Inputs::Logged::none);
    
    preferences_->read();
}
    
void SpaghettisInstance::shutdown()
{
    core_->shutdown();
    
    #if ! ( SPAGHETTIS_MENUBAR )
        
    juce::MenuBarModel::setMacMainMenu (nullptr);
        
    #endif
        
    consoleWindow_     = nullptr;
    devicesWindow_     = nullptr;
    preferencesWindow_ = nullptr;
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
    
    if (!AlertWindowRegister::getInstance()->isEmpty()) { return false; }
    else {
    //
    quit_ = QuitStatus::quit;
    
    if (!patches_->isEmpty()) {
        if (preferences_->get<bool> (Tag::General, Tag::AskBeforeQuit) == true) {
            showExitWindow();
        }
    }
    
    return true;
    //
    }
}
    
void SpaghettisInstance::closeAllPatches()
{
    patches_->requestCloseAllPatches();
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

template <class T> void createOrOpenWindow (std::unique_ptr<T>& p,
    juce::ApplicationCommandManager& commandManager,
    juce::PropertiesFile* propertiesFile)
{
    if (p == nullptr) { p = std::make_unique<T> (commandManager, propertiesFile); }
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
    createOrOpenWindow (preferencesWindow_, *commandManager_, properties_.get());
}

void SpaghettisInstance::closePreferencesWindow()
{
    preferencesWindow_ = nullptr;
}

void SpaghettisInstance::openDevicesWindow()
{
    createOrOpenWindow (devicesWindow_, *commandManager_, properties_.get());
}

void SpaghettisInstance::closeDevicesWindow()
{
    devicesWindow_ = nullptr;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void SpaghettisInstance::updateSearchPaths (Inputs::Logged type)
{
    /* ??? */
    
    // handle (Inputs::setSearchPaths (searchpaths));
    // handle (Inputs::rescanSearchPaths (type));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void SpaghettisInstance::openPatch()
{
    fileChooser_ = std::make_unique<juce::FileChooser> (NEEDS_TRANS ("Choose a Patch..."),
                        Folders::getInstance()->getDefaultOpen(),
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
    if (file.existsAsFile() && file.hasFileExtension (core::getPatchExtension())) {
    //
    const int maximum = 16;
    
    juce::String s (file.getFullPathName());
    
    recentFiles_.removeString (s);
    recentFiles_.insert (0, s);
    
    while (recentFiles_.size() > maximum) { recentFiles_.remove (recentFiles_.size() - 1); }
    
    menu_->menuItemsChanged(); saveRecentFiles();
    //
    }
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
        
    if (root && root->hasTagName (Id::RECENTFILES)) {
    //
    juce::StringArray scoped;
    
    for (auto* e : root->getChildWithTagNameIterator (Id::RECENTFILE)) {
        if (e->hasAttribute (Id::path)) {
            scoped.addIfNotAlreadyThere (e->getStringAttribute (Id::path));
        }
    }
    
    scoped.swapWith (recentFiles_);
    //
    }
}

void SpaghettisInstance::saveRecentFiles()
{
    auto root = std::make_unique<juce::XmlElement> (Id::RECENTFILES);
        
    for (const auto& f : recentFiles_) {
        juce::XmlElement* e = root->createNewChildElement (Id::RECENTFILE);
        e->setAttribute (Id::path, f);
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

void SpaghettisInstance::resetConsole()
{
    if (consoleWindow_) { consoleWindow_->reset(); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

JUCE_IMPLEMENT_SINGLETON (SpaghettisInstance)

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
