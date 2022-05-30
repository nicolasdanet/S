
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
    initialize();
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
    g.fillAll (juce::Colours::orange);
    // g.fillAll (backgroundColour_.get());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void RunView::addComponent (const juce::ValueTree& child)
{
    /*
    const juce::Identifier t (child.getType());
    
    if (t == Ids::OBJECT || t == Ids::PATCH) { objects_.add (this, core::Object (child)); }
    else if (t == Ids::LINE) {
        lines_.add (this, core::Line (child));
    }
    */
}

void RunView::removeComponent (const juce::ValueTree& child)
{
    /*
    const juce::Identifier t (child.getType());
    
    if (t == Ids::OBJECT || t == Ids::PATCH) { objects_.remove (core::Object (child)); }
    else if (t == Ids::LINE) {
        lines_.remove (core::Line (child));
    }
    */
}

void RunView::initialize()
{
    // for (const auto& child : tree_) { addComponent (child); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void RunView::valueTreeChildAdded (juce::ValueTree& t, juce::ValueTree& child)
{
    // if (isChildOf (tree_, child)) { addComponent (child); }
}

void RunView::valueTreeChildRemoved (juce::ValueTree& t, juce::ValueTree& child, int)
{
    // if (isChildOf (tree_, child)) { removeComponent (child); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
