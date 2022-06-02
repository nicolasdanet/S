
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

EditView::EditView (const juce::ValueTree& tree) :
    tree_ (tree),
    backgroundColour_ (Spaghettis()->getCachedColour (Tags::PatchBackground)),
    scale_ (1.0f)
{
    tree_.addListener (this);
    backgroundColour_.attach (PainterPolicy::repaint (this));
    setOpaque (true);
    setBounds (core::Canvas::getArea (scale_));
    initialize (tree_);
}

EditView::~EditView()
{
    lines_.clear(); objects_.clear();
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditView::paint (juce::Graphics& g)
{
    g.fillAll (backgroundColour_.get());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

ObjectComponent* EditView::getObject (core::UniqueId u)
{
    return objects_.get (u);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditView::show (ObjectComponent* o, const juce::Rectangle<int>& r)
{
    o->setBounds (getBoundsFromPaintedArea (r)); o->setVisible (true);
}

void EditView::hide (ObjectComponent* o)
{
    o->setVisible (false);
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditView::setScale (float scale)
{
    jassert (scale > 0.0f);
    
    scale_ = scale;
    
    auto f = [](const auto& p) { p->scaleChanged(); };
    
    objects_.perform (f); lines_.perform (f);
}

float EditView::getScale() const
{
    return scale_;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

bool isChildOf (const juce::ValueTree& t, juce::ValueTree& child)
{
    return (child.getParent() == t);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditView::addComponent (const juce::ValueTree& child)
{
    const juce::Identifier t (child.getType());
    
    if (t == Ids::OBJECT || t == Ids::PATCH) { objects_.add (this, core::Object (child)); }
    else if (t == Ids::LINE) {
        lines_.add (this, core::Line (child));
    }
}

void EditView::removeComponent (const juce::ValueTree& child)
{
    const juce::Identifier t (child.getType());
    
    if (t == Ids::OBJECT || t == Ids::PATCH) { objects_.remove (core::Object (child)); }
    else if (t == Ids::LINE) {
        lines_.remove (core::Line (child));
    }
}

void EditView::initialize (const juce::ValueTree& tree)
{
    for (const auto& child : tree) { addComponent (child); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditView::valueTreeChildAdded (juce::ValueTree& t, juce::ValueTree& child)
{
    if (isChildOf (tree_, child)) { addComponent (child); }
}

void EditView::valueTreeChildRemoved (juce::ValueTree& t, juce::ValueTree& child, int)
{
    if (isChildOf (tree_, child)) { removeComponent (child); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
