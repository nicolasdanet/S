
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

RunView::RunView (Patch& patch, const juce::ValueTree& tree) :
    View (patch),
    tree_ (tree),
    patchBackgroundColour_ (Spaghettis()->getCachedColour (Tags::PatchBackground)),
    patchTextColour_ (Spaghettis()->getCachedColour (Tags::PatchText))
{
    tree_.addListener (this);
    patchBackgroundColour_.attach (PainterPolicy::repaint (this));
    patchTextColour_.attach (PainterPolicy::repaint (this));
    setOpaque (true);
    initialize (tree_);
}

RunView::~RunView()
{
    objects_.clear();
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void RunView::paint (juce::Graphics& g)
{
    g.fillAll (patchBackgroundColour_.get());
    
    if (layout_.isEmpty()) {
    //
    juce::String text (NEEDS_TRANS ("No widget to show!"));
    
    g.setColour (patchTextColour_.get());
    g.setFont (Spaghettis()->getLookAndFeel().getObjectsFont (1.5f));
    g.drawText (text, getLocalBounds(), juce::Justification::centred, true);
    //
    }
}

void RunView::resized()
{
    triggerAsyncUpdate();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

ObjectComponent* RunView::getObject (core::UniqueId u)
{
    return objects_.get (u);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void RunView::show (ObjectComponent* o, const juce::Rectangle<int>& bounds)
{
    layout_.add (o, bounds); triggerAsyncUpdate();
}

void RunView::hide (ObjectComponent* o)
{
    layout_.remove (o); triggerAsyncUpdate();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void RunView::update()
{
    repaint(); layout_.arrange (getLocalBounds().reduced (10));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void RunView::addComponent (const juce::ValueTree& child)
{
    if (child.getType() == Ids::OBJECT) {
    //
    const core::Object object (child);
    
    if (object.isGUI()) { objects_.add (this, object); }
    //
    }
}

void RunView::removeComponent (const juce::ValueTree& child)
{
    if (child.getType() == Ids::OBJECT) {
    //
    const core::Object object (child);
    
    if (object.isGUI()) { objects_.remove (object); }
    //
    }
}

void RunView::initialize (const juce::ValueTree& tree)
{
    for (const auto& child : tree) { addComponent (child); initialize (child); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void RunView::valueTreeChildAdded (juce::ValueTree& t, juce::ValueTree& child)
{
    addComponent (child);
}

void RunView::valueTreeChildRemoved (juce::ValueTree& t, juce::ValueTree& child, int)
{
    removeComponent (child);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
