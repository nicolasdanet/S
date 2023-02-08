
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

MakerEntry::MakerEntry (juce::Value& v, EditView* view) : v_ (v), view_ (view)
{
    editor_.addListener (this);
    
    editor_.setFont (Spaghettis()->getLookAndFeel().getMakerEntryFont());
    editor_.setPopupMenuEnabled (false);
    
    addAndMakeVisible (editor_);
    
    setOpaque (true);
}

MakerEntry::~MakerEntry()
{
    editor_.removeListener (this);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void MakerEntry::paint (juce::Graphics& g)
{
    g.fillAll (Spaghettis()->getColour (Colours::makerBackground));
}
    
void MakerEntry::resized()
{
    editor_.setBounds (getLocalBounds());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void MakerEntry::textEditorTextChanged (juce::TextEditor& editor)
{
    DBG (editor_.getText());
}

void MakerEntry::textEditorReturnKeyPressed (juce::TextEditor&)
{
    DBG ("!");
}

void MakerEntry::textEditorEscapeKeyPressed (juce::TextEditor&)
{
    if (view_.getComponent()) { view_->dismissNewObject(); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
