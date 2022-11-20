
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
    undo_ (core::Patch (viewTree_).getCached<juce::String> (Tags::Attributes, Tags::Undo)),
    redo_ (core::Patch (viewTree_).getCached<juce::String> (Tags::Attributes, Tags::Redo)),
    patchBackgroundColour_ (Spaghettis()->getCachedColour (Tags::PatchBackground)),
    scale_ (1.0f),
    lasso_ (this)
{
    viewTree_.addListener (this);
    undo_.attach ([]() { Spaghettis()->updateMenuBar(); });
    redo_.attach ([]() { Spaghettis()->updateMenuBar(); });
    patchBackgroundColour_.attach (PainterPolicy::repaint (this));
    setOpaque (true);
    setBounds (core::Canvas::getAreaScaled (scale_));
    initialize (viewTree_);
}

EditView::~EditView()
{
    lines_.clear(); objects_.clear();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditView::attach (EditInspector* inspector)
{
    inspector_ = inspector;
}

void EditView::detach (EditInspector* inspector)
{
    inspector_ = nullptr;
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditView::mouseDown (const juce::MouseEvent& e)
{
    if (Mouse::isSimpleClick (e)) { deselectAll(); }
}

void EditView::mouseDrag (const juce::MouseEvent& e)
{
    lasso_.mouseDrag (e);
}

void EditView::mouseUp (const juce::MouseEvent& e)
{
    lasso_.mouseUp (e);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

template <class T> int getNumberOfSelected (T& t)
{
    auto n = t.countIf ([](const auto& p) { return p->isSelected(); });

    return static_cast<int> (n);
}

template <class T> core::UniqueId getSelected (T& t)
{
    core::UniqueId u = 0;
    
    auto f = [&u](const auto& p)
    {
        if (p->isSelected()) { u = p->getIdentifier(); }
    };

    t.forEach (f);
    
    return u;
}

void deconnectSelectedLines (Table<LineComponent>& t)
{
    auto f = [](const auto& p)
    {
        if (p->isSelected()) { p->disconnect(); }
    };

    t.forEach (f);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool EditView::hasSelected()
{
    return (getNumberOfSelectedObjects() > 0) || (getNumberOfSelectedLines() > 0);
}

bool EditView::hasSelectedObject()
{
    return (getNumberOfSelectedObjects() > 0);
}

int EditView::getNumberOfSelectedObjects()
{
    return getNumberOfSelected (objects_);
}

int EditView::getNumberOfSelectedLines()
{
    return getNumberOfSelected (lines_);
}

ObjectComponent* EditView::getSelectedObject()
{
    return getObjectComponent (getSelected (objects_));
}

LineComponent* EditView::getSelectedLine()
{
    return getLineComponent (getSelected (lines_));
}

core::Item EditView::getItemForInspector()
{
    if (getNumberOfSelectedObjects() == 1)    { return Documentation::createCopy (getSelectedObject()); }
    else if (getNumberOfSelectedLines() == 1) { return Documentation::createCopy (getSelectedLine());   }
    else {
        return core::Item::createCopy (core::Patch (viewTree_));
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool EditView::hasOnlyOnePatchSelected()
{
    if (getNumberOfSelectedObjects() == 1) {
        if (getSelectedObject()->getObject().isPatch()) {
            return true;
        }
    }
        
    return false;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void EditView::setPaste()
{
    if (hasPaste_ == false) { hasPaste_ = true; Spaghettis()->updateMenuBar(); }
}

bool EditView::hasPaste()
{
    return hasPaste_;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditView::deselectAll()
{
    lines_.forEach   ([](const auto& p) { p->setSelected (false); });
    objects_.forEach ([](const auto& p) { p->setSelected (false); });
}

void EditView::selectAll()
{
    objects_.forEach ([](const auto& p) { p->setSelected (true); });
}

void EditView::select (const juce::Rectangle<int>& r)
{
    auto f = [t = r](const auto& p)
    {
        if (t.intersects (p->getBounds())) { p->setSelected (true); }
    };

    objects_.forEach (f);
}

void EditView::dragStart()
{
    objects_.forEach ([](const auto& p) { p->dragStart(); });
}

void EditView::drag (juce::Point<int> offset)
{
    objects_.forEach ([offset](const auto& p) { if (p->isSelected()) { p->drag (offset); } });
}

void EditView::moveBack()
{
    objects_.forEach ([](const auto& p) { if (p->isSelected()) { p->moveBack(); } });
}

void EditView::moveFront()
{
    objects_.forEach ([](const auto& p) { if (p->isSelected()) { p->moveFront(); } });
}

void EditView::snapToGrid()
{
    objects_.forEach ([](const auto& p) { if (p->isSelected()) { p->snap(); } });
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditView::undo()
{
    EditCommands::undo (core::Patch (viewTree_).getIdentifier());
}

void EditView::redo()
{
    EditCommands::redo (core::Patch (viewTree_).getIdentifier());
}

bool EditView::hasUndo()
{
    return undo_.get().isNotEmpty();
}

bool EditView::hasRedo()
{
    return redo_.get().isNotEmpty();
}

juce::String EditView::getUndoAction()
{
    if (hasUndo()) {
        return juce::String (NEEDS_TRANS ("Undo")) + " " + core::Report::firstLetterCapitalized (undo_.get());
    }
    
    return juce::String();
}

juce::String EditView::getRedoAction()
{
    if (hasRedo()) {
        return juce::String (NEEDS_TRANS ("Redo")) + " " + core::Report::firstLetterCapitalized (redo_.get());
    }
    
    return juce::String();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditView::cut()
{
    setPaste();
    
    EditCommands::cut (core::Patch (viewTree_).getIdentifier());
}

void EditView::copy()
{
    setPaste();
    
    EditCommands::copy (core::Patch (viewTree_).getIdentifier());
}

void EditView::paste()
{
    EditCommands::paste (core::Patch (viewTree_).getIdentifier());
}

void EditView::duplicate()
{
    EditCommands::duplicate (core::Patch (viewTree_).getIdentifier());
}

void EditView::remove()
{
    EditCommands::remove (core::Patch (viewTree_).getIdentifier());     /* Remove all selected objects. */
    
    deconnectSelectedLines (lines_);
}

void EditView::encapsulate()
{
    // EditCommands::encapsulate (core::Patch (viewTree_).getIdentifier());
}

void EditView::deencapsulate()
{
    // EditCommands::deencapsulate ();
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

LineComponent* EditView::getLineComponent (core::UniqueId u)
{
    return lines_.get (u);
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
    
    objects_.forEach (f); lines_.forEach (f);
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

void EditView::updateOrder()
{
    for (const auto& child : viewTree_) {
        if (Tree::isObject (child)) { objects_.moveAtEnd (core::Object (child).getIdentifier()); }
    }
    
    auto f = [c = static_cast<juce::Component*> (nullptr)](const auto& p) mutable
    {
        p->moveBehind (c); c = p.get();
    };
    
    objects_.forEachReversed (f);
    
    lines_.forEachReversed ([](const auto& p) { p->updateOrder(); });
}

void EditView::handleAsyncUpdate()
{
    updateOrder();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditView::valueTreeChildAdded (juce::ValueTree& t, juce::ValueTree& child)
{
    if (t == viewTree_) { addComponent (child); }
}

void EditView::valueTreeChildRemoved (juce::ValueTree& t, juce::ValueTree& child, int)
{
    if (t == viewTree_) { removeComponent (child); }
}

void EditView::valueTreeChildOrderChanged (juce::ValueTree& t, int oldIndex, int newIndex)
{
    if (t == viewTree_) { triggerAsyncUpdate(); }
}

void EditView::valueTreePropertyChanged (juce::ValueTree& t, const juce::Identifier&)
{
    juce::ValueTree i (Tree::getParentIfChangedPropertyEquals (t, Tags::Selected));
    
    if (i.isValid() && isChildOf (viewTree_, i) && inspector_) { inspector_->update(); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
