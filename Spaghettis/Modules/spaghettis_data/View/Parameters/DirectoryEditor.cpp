
/* Copyright (c) 2024 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

juce::Colour getColourFromFileStatus (const Folder& f)
{
    if (f.isValid()) { return Colours::fetchColour (Colours::parametersFolderText); }

    return Colours::fetchColour (Colours::parametersFolderTextWrong);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void DirectoryEditor::paint (juce::Graphics& g)
{
    const Folder f (value_.toString());
    const juce::Rectangle<int> r (getLocalBounds().reduced (4, 2));
    const juce::String text (f.toString());
    
    g.fillAll (Colours::fetchColour (Colours::parametersColourBackground));
    g.setFont (font_);
    g.setColour (getColourFromFileStatus (f));
    g.drawText (text, r, juce::Justification::centredLeft, true);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void DirectoryEditor::setDirectory (const juce::File& file)
{
    if (file.isDirectory()) {
    //
    // file.getFullPathName();
    
    Directories::getInstance()->setDefaultOpen (file.getParentDirectory());
    //
    }
}

void DirectoryEditor::chooseDirectory()
{
    /*
    fileChooser_ = std::make_unique<juce::FileChooser> (NEEDS_TRANS ("Choose a folder..."),
                        Directories::getInstance()->getDefaultOpen());

    const int t = juce::FileBrowserComponent::canSelectMultipleItems
                        | juce::FileBrowserComponent::openMode
                        | juce::FileBrowserComponent::canSelectDirectories;
    
    auto callback = [this] (const juce::FileChooser& fileChooser)
    {
        auto files = fileChooser.getResults(); for (const auto& f : files) { appendFile (f); }
    };
                    
    fileChooser_->launchAsync (t, callback);
    */
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void DirectoryEditor::mouseDown (const juce::MouseEvent&)
{
    if (isEnabled()) {
    //
    DBG ("!!!");
    //
    }
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
