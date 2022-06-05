
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

RunView::RunView (const juce::ValueTree& tree) :
    tree_ (tree),
    backgroundColour_ (Spaghettis()->getCachedColour (Tags::PatchBackground))
{
    tree_.addListener (this);
    backgroundColour_.attach (PainterPolicy::repaint (this));
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
    g.fillAll (backgroundColour_.get());
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
    layout_.arrange (getLocalBounds().reduced (1));
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
