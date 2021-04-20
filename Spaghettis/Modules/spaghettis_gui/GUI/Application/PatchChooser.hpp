
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class PatchChooser {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    PatchChooser() :    openText_ (NEEDS_TRANS ("Choose a Patch to open...")),
                        openExtensions_ (spaghettis::core::getFileExtensions()),
                        openDirectory_ (juce::File::getCurrentWorkingDirectory())
                        
    {
    }
    
    ~PatchChooser() = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void openPatch()
    {
        fileChooser_.reset (new juce::FileChooser (openText_, openDirectory_, openExtensions_));
    
        int flags = juce::FileBrowserComponent::canSelectMultipleItems
                        | juce::FileBrowserComponent::openMode
                        | juce::FileBrowserComponent::canSelectFiles;
        
        auto f = [] (const juce::FileChooser& fc)
            {
                auto files = fc.getResults(); for (auto f : files) { DBG (f.getFullPathName()); }
            };
                        
        fileChooser_->launchAsync (flags, f);
    }
                                 
private:
    std::unique_ptr<juce::FileChooser> fileChooser_;
    juce::String openText_;
    juce::String openExtensions_;
    juce::File openDirectory_;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PatchChooser)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
