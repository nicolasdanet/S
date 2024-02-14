
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

RunView::RunView (const PatchBase& base) :
    PatchView (base),
    patchBackgroundColour_ (Painted (Spaghettis()->getCachedColour (Tag::PatchBackground), this))
{
    getViewTree().addListener (this);
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

float RunView::getScale() const
{
    return 1.0f;
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

void RunView::handleAsyncUpdate()
{
    repaint(); layout_.sort(); layout_.arrange (getLocalBounds().reduced (10));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void addComponent (RunView* view, Table<ObjectComponent>& objects, const juce::ValueTree& child)
{
    if (Tree::isObject (child)) {
    //
    const core::Object object (child);
    
    if (object.isGraphic()) { objects.add (view, object); }
    //
    }
}

void removeComponent (RunView* view, Table<ObjectComponent>& objects, const juce::ValueTree& child)
{
    if (Tree::isObject (child)) {
    //
    const core::Object object (child);
    
    if (object.isGraphic()) { objects.remove (object.getIdentifier()); }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void RunView::valueTreeChildAdded (juce::ValueTree& t, juce::ValueTree& child)
{
    addComponent (this, objects_, child);
}

void RunView::valueTreeChildRemoved (juce::ValueTree& t, juce::ValueTree& child, int)
{
    removeComponent (this, objects_, child);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void RunView::initialize (const juce::ValueTree& tree)
{
    for (const auto& child : tree) { addComponent (this, objects_, child); initialize (child); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
