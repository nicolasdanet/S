
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

EditView::EditView (Patch& patch, const juce::ValueTree& tree) :
    View (patch, tree),
    patchBackgroundColour_ (Spaghettis()->getCachedColour (Tags::PatchBackground)),
    scale_ (1.0f)
{
    viewTree_.addListener (this);
    patchBackgroundColour_.attach (PainterPolicy::repaint (this));
    setOpaque (true);
    setBounds (core::Canvas::getArea (scale_));
    initialize (viewTree_);
}

EditView::~EditView()
{
    lines_.clear(); objects_.clear();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditView::mouseDown (const juce::MouseEvent& e)
{
    if (Mouse::isSimpleClick (e)) { deselectAll(); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditView::deselectAll()
{
    lines_.perform   ([](const auto& p) { p->setSelected (false); });
    objects_.perform ([](const auto& p) { p->setSelected (false); });
}

void EditView::selectAll()
{
    objects_.perform ([](const auto& p) { p->setSelected (true); });
}

void EditView::dragStart()
{
    objects_.perform ([](const auto& p) { p->dragStart(); });
}

void EditView::drag (juce::Point<int> offset)
{
    objects_.perform ([offset](const auto& p) { if (p->isSelected()) { p->drag (offset); } });
}

void EditView::moveBack()
{
    objects_.perform ([](const auto& p) { if (p->isSelected()) { p->moveBack(); } });
}

void EditView::moveFront()
{
    objects_.perform ([](const auto& p) { if (p->isSelected()) { p->moveFront(); } });
}

void EditView::snapToGrid()
{
    objects_.perform ([](const auto& p) { if (p->isSelected()) { p->snap(); } });
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditView::paint (juce::Graphics& g)
{
    g.fillAll (patchBackgroundColour_.get());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

ObjectComponent* EditView::getObjectComponent (core::UniqueId u)
{
    return objects_.get (u);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditView::show (ObjectComponent* o, const juce::Rectangle<int>& r)
{
    o->setBounds (r); o->setVisible (true);
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

juce::Rectangle<int> EditView::getPaintedAreaFromBounds (const juce::Rectangle<int>& bounds)
{
    return (bounds.reduced (0, PainterPolicy::pinHeight (getScale())));
}

juce::Rectangle<int> EditView::getBoundsFromPaintedArea (const juce::Rectangle<int>& painted)
{
    return (painted.expanded (0, PainterPolicy::pinHeight (getScale())));
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
    if (Tree::isObject (child))    { objects_.add (this, core::Object (child)); }
    else if (Tree::isLine (child)) { lines_.add (this, core::Line (child));     }
}

void EditView::removeComponent (const juce::ValueTree& child)
{
    if (Tree::isObject (child))    { objects_.remove (core::Object (child)); }
    else if (Tree::isLine (child)) { lines_.remove (core::Line (child));     }
}

void EditView::initialize (const juce::ValueTree& tree)
{
    for (const auto& child : tree) { addComponent (child); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditView::handleAsyncUpdate()
{
    for (const auto& child : viewTree_) {
        if (Tree::isObject (child)) { objects_.moveAtEnd (core::Object (child).getIdentifier()); }
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditView::valueTreeChildAdded (juce::ValueTree& t, juce::ValueTree& child)
{
    if (isChildOf (viewTree_, child)) { addComponent (child); }
}

void EditView::valueTreeChildRemoved (juce::ValueTree& t, juce::ValueTree& child, int)
{
    if (isChildOf (viewTree_, child)) { removeComponent (child); }
}

void EditView::valueTreeChildOrderChanged (juce::ValueTree& t, int oldIndex, int newIndex)
{
    if (t == viewTree_) { triggerAsyncUpdate(); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
