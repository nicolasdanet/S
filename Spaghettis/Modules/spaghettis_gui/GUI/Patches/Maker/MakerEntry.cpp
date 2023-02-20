
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

MakerEntry::MakerEntry (juce::Value& v, EditView* view, const juce::Point<int>& pt) :
    v_ (v),
    view_ (view),
    pt_ (pt)
{
    v_.addListener (this);
    editor_.addListener (this);
    
    editor_.setFont (Spaghettis()->getLookAndFeel().getMakerEntryFont());
    editor_.setPopupMenuEnabled (false);
    
    addAndMakeVisible (editor_);
    
    setOpaque (true);
}

MakerEntry::~MakerEntry()
{
    editor_.removeListener (this);
    v_.removeListener (this);
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

void MakerEntry::enter()
{
    if (view_.getComponent()) { view_->handleNewObject (pt_, editor_.getText()); }
}

void MakerEntry::dismiss()
{
    if (view_.getComponent()) { view_->dismissNewObject(); }
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void MakerEntry::textEditorTextChanged (juce::TextEditor& editor)
{
    v_.setValue (editor_.getText());
}

void MakerEntry::textEditorReturnKeyPressed (juce::TextEditor&)
{
    enter();
}

void MakerEntry::textEditorEscapeKeyPressed (juce::TextEditor&)
{
    dismiss();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void MakerEntry::valueChanged (juce::Value& v)
{
    editor_.setText (v.toString(), false);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
