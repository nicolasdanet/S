
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

RunView::RunView (PatchRoot& patch, const juce::ValueTree& tree) :
    PatchView (patch, tree),
    patchBackgroundColour_ (Spaghettis()->getCachedColour (Tag::PatchBackground))
{
    getViewTree().addListener (this);
    patchBackgroundColour_.attach (PainterStrategy::repaint (this));
    setOpaque (true);
    initialize (getViewTree());
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
    const juce::String text (NEEDS_TRANS ("Widgets"));
    
    g.setColour (Colours::fetchColour (Colours::windowsText));
    g.setFont (Fonts::getFontRescaled (1.5f));
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

ObjectComponent* RunView::getObjectComponent (core::UniqueId u) const
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
    repaint(); layout_.sort(); layout_.arrange (getLocalBounds().reduced (10));
}

void RunView::handleAsyncUpdate()
{
    update();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void RunView::addComponent (const juce::ValueTree& child)
{
    if (Tree::isObject (child)) {
    //
    const core::Object object (child);
    
    if (object.isGraphic()) { objects_.add (this, object); }
    //
    }
}

void RunView::removeComponent (const juce::ValueTree& child)
{
    if (Tree::isObject (child)) {
    //
    const core::Object object (child);
    
    if (object.isGraphic()) { objects_.remove (object); }
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
