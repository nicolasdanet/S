
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

EditView::EditView (PatchRoot& patch, const juce::ValueTree& tree) :
    View (patch, tree),
    maker_ (this),
    isAbstraction_ (core::Patch (viewTree_).isAbstraction()),
    isLocked_ (core::Patch (viewTree_).isLocked()),
    undo_ (core::Patch (viewTree_).getCached<juce::String> (Tag::Attributes, Tag::Undo)),
    redo_ (core::Patch (viewTree_).getCached<juce::String> (Tag::Attributes, Tag::Redo)),
    patchBackgroundColour_ (Spaghettis()->getCachedColour (Tag::PatchBackground)),
    scale_ (1.0f)
{
    viewTree_.addListener (this);
    undo_.attach ([]() { Spaghettis()->updateMenuBar(); });
    redo_.attach ([]() { Spaghettis()->updateMenuBar(); });
    patchBackgroundColour_.attach (PainterPolicy::repaint (this));
    setOpaque (true);
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

void EditView::mouseDragProceed (const juce::MouseEvent& e, bool isChild, DragFlag flag)
{
    if (!drag_) {
        if (Mouse::isAltClick (e))          { drag_ = std::make_unique<ActionHand> (this);    }
        else if (Mouse::hasModifier (e))    { }
        else if (flag == DragFlag::Resize)  { drag_ = std::make_unique<ActionResize> (this);  }
        else if (flag == DragFlag::Move)    { drag_ = std::make_unique<ActionMove> (this);    }
        else if (flag == DragFlag::Pin)     { drag_ = std::make_unique<ActionConnect> (this); }
        else if (!isChild)                  { drag_ = std::make_unique<ActionLasso> (this);   }
    }

    if (drag_) { drag_->mouseDrag (e); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditView::handleMouseDown (const juce::MouseEvent& e)
{
    mouseDown (e.getEventRelativeTo (this));
}

void EditView::handleMouseUp (const juce::MouseEvent& e)
{
    mouseUp (e.getEventRelativeTo (this));
}

void EditView::handleMouseDrag (const juce::MouseEvent& e, DragFlag flag)
{
    mouseDragProceed (e.getEventRelativeTo (this), true, flag);
}

void EditView::handleMouseDragAbort()
{
    DBG ("ABORTED"); drag_ = nullptr;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* First let create the object when dismissing the Maker's CallOut Box clicking into the view. */

void EditView::mouseDown (const juce::MouseEvent& e)
{
    if (Mouse::isSimpleClick (e)) {
    //
    if (maker_.isActive() == false) { deselectAll(); }
    else {
        const auto [pt, s] = maker_.getContent(); createObject (pt, s);
    }
    //
    }
}

void EditView::mouseDrag (const juce::MouseEvent& e)
{
    mouseDragProceed (e.getEventRelativeTo (this), false, DragFlag::None);
}

void EditView::mouseUp (const juce::MouseEvent& e)
{
    if (drag_) { drag_->mouseUp (e); }
    
    drag_ = nullptr;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditView::setMouseCursorRecursive (const juce::MouseCursor& m)
{
    auto f = [m](const auto& p) { p->setMouseCursor (m); };
    
    objects_.forEach (f); lines_.forEach (f);
    
    setMouseCursor (m);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

juce::Point<int> getMinimum (std::optional<juce::Point<int>> pt, juce::Point<int> b, juce::Point<int> offset)
{
    const juce::Point<int> a = pt.value_or (b);
    
    const int x1 = a.getX();
    const int x2 = b.getX();
    const int y1 = a.getY();
    const int y2 = b.getY();
    
    return juce::Point<int> (juce::jmin (x1, x2), juce::jmin (y1, y2)) + offset;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

std::optional<juce::Point<int>> EditView::getGlobalMousePosition() const
{
    if (isMouseOver (true)) {
    //
    return localPointToGlobal (getMouseXYRelative());
    //
    }

    return {};
}

std::optional<juce::Point<int>> EditView::getRealMousePosition() const
{
    if (isMouseOver (true)) {
    //
    return Coordinates::localToReal (Coordinates::unscaled (getMouseXYRelative(), getScale()));
    //
    }

    return {};
}

std::optional<juce::Point<int>> EditView::getRealPositionOfSelectedObjects (juce::Point<int> offset) const
{
    std::optional<juce::Point<int>> pt;
    
    auto f = [&pt, offset](const auto& p)
    {
        if (p->isSelected()) {
            pt = getMinimum (pt, Coordinates::localToReal (p->getLocalPosition()), offset);
        }
    };

    objects_.forEach (f);
    
    return pt;
}

juce::Rectangle<int> EditView::getGlobalVisibleArea() const
{
    return getPort()->getGlobalVisibleArea();
}

juce::Rectangle<int> EditView::getRealVisibleArea() const
{
    return getPort()->getRealVisibleArea();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool EditView::isAbstractionOrInside() const
{
    return isAbstraction_ || isLocked_;
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

bool EditView::hasSelected() const
{
    return (getNumberOfSelectedObjects() > 0) || (getNumberOfSelectedLines() > 0);
}

bool EditView::hasSelectedObject() const
{
    return (getNumberOfSelectedObjects() > 0);
}

int EditView::getNumberOfSelectedObjects() const
{
    return getNumberOfSelected (objects_);
}

int EditView::getNumberOfSelectedLines() const
{
    return getNumberOfSelected (lines_);
}

ObjectComponent* EditView::getSelectedObject() const
{
    return getObjectComponent (getSelected (objects_));
}

LineComponent* EditView::getSelectedLine() const
{
    return getLineComponent (getSelected (lines_));
}

bool EditView::selectObjects (const juce::Rectangle<int>& r)
{
    bool done = false;
    
    auto f = [&](const auto& p)
    {
        if (r.intersects (p->getBounds())) { p->setSelected (true); done = true; }
    };

    objects_.forEach (f);

    return done;
}

bool EditView::selectLines (const juce::Rectangle<int>& r)
{
    bool done = false;
    
    auto f = [&done, t = r.toFloat()](const auto& p)
    {
        if (p->intersects (t)) { p->setSelected (true); done = true; }
    };

    lines_.forEach (f);

    return done;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Sync EditView::getSynchronized() const
{
    if (getNumberOfSelectedLines() == 1)        { return Sync (getSelectedLine()->getLine());     }
    else if (getNumberOfSelectedObjects() == 1) { return Sync (getSelectedObject()->getObject()); }
    else {
        return Sync (core::Patch (viewTree_));
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool EditView::hasOnlyOnePatchSelected() const
{
    if (getNumberOfSelectedObjects() == 1) {
        if (getSelectedObject()->getObject().isSubpatch()) {
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

bool EditView::hasPaste() const
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
    if (selectObjects (r) == false) { selectLines (r); }
}

void EditView::dragObjectsStart()
{
    if (!isAbstractionOrInside()) {
    //
    objects_.forEach ([](const auto& p) { p->dragStart(); });
    //
    }
}

void EditView::dragObjects (juce::Point<int> offset)
{
    if (!isAbstractionOrInside()) {
    //
    objects_.forEach ([offset](const auto& p) { if (p->isSelected()) { p->drag (offset); } });
    //
    }
}

void EditView::resizeObjects (juce::Point<int> offset)
{
    if (!isAbstractionOrInside()) {
    //
    objects_.forEach ([offset](const auto& p) { if (p->isSelected()) { p->resize (offset); } });
    //
    }
}

void EditView::moveBack()
{
    if (!isAbstractionOrInside()) {
    //
    objects_.forEach ([](const auto& p) { if (p->isSelected()) { p->moveBack(); } });
    //
    }
}

void EditView::moveFront()
{
    if (!isAbstractionOrInside()) {
    //
    objects_.forEach ([](const auto& p) { if (p->isSelected()) { p->moveFront(); } });
    //
    }
}

void EditView::snapToGrid()
{
    if (!isAbstractionOrInside()) {
    //
    objects_.forEach ([](const auto& p) { if (p->isSelected()) { p->snap(); } });
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditView::undo()
{
    if (!isAbstractionOrInside()) { EditCommands::undo (getIdentifierOfView()); }
}

void EditView::redo()
{
    if (!isAbstractionOrInside()) { EditCommands::redo (getIdentifierOfView()); }
}

bool EditView::hasUndo() const
{
    return undo_.get().isNotEmpty();
}

bool EditView::hasRedo() const
{
    return redo_.get().isNotEmpty();
}

juce::String EditView::getUndoAction() const
{
    if (hasUndo()) {
    //
    return juce::String (NEEDS_TRANS ("Undo")) + " " + Helpers::firstLetterCapitalized (undo_.get());
    //
    }
    
    return juce::String();
}

juce::String EditView::getRedoAction() const
{
    if (hasRedo()) {
    //
    return juce::String (NEEDS_TRANS ("Redo")) + " " + Helpers::firstLetterCapitalized (redo_.get());
    //
    }
    
    return juce::String();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditView::cut()
{
    if (!isAbstractionOrInside()) {
    //
    setPaste();
    
    EditCommands::cut (getIdentifierOfView());
    //
    }
}

void EditView::copy()
{
    if (!isAbstractionOrInside()) {
    //
    setPaste();
    
    EditCommands::copy (getIdentifierOfView());
    //
    }
}

void EditView::paste()
{
    if (!isAbstractionOrInside()) {
    //
    const int n = Spaghettis()->getPreferences().getCached<int> (Tag::Editing, Tag::GridSize);
    
    const juce::Rectangle<int> area  = getRealVisibleArea();
    const juce::Point<int> centre    = area.getCentre();
    const juce::Point<int> mouse     = getRealMousePosition().value_or (centre);
    const juce::Point<int> offset    = juce::Point<int> (n * 2, n * 2);
    const juce::Point<int> selection = getRealPositionOfSelectedObjects (offset).value_or (mouse);
    const juce::Point<int> pt        = area.contains (selection) ? selection : centre;
    
    EditCommands::paste (getIdentifierOfView(), pt);
    //
    }
}

void EditView::duplicate()
{
    if (!isAbstractionOrInside()) {
    //
    EditCommands::duplicate (getIdentifierOfView());
    //
    }
}

void EditView::remove()
{
    if (!isAbstractionOrInside()) {
    //
    EditCommands::remove (getIdentifierOfView());   /* Remove all selected objects. */
    
    deconnectSelectedLines (lines_);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditView::createObject (juce::Point<int> pt, const juce::String& s)
{
    if (s.isNotEmpty()) { Spaghettis()->handle (Inputs::createObject (getIdentifierOfView(), pt, s)); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditView::openMaker (bool isFromMenu)
{
    if (!isAbstractionOrInside()) {
    //
    std::optional<juce::Point<int>> a (getGlobalMousePosition());
    std::optional<juce::Point<int>> b (getRealMousePosition());
    
    bool useCentre = isFromMenu;
    
    #if JUCE_LINUX
    useCentre |= !a.has_value();
    #endif
    
    if (useCentre) {
        a = getGlobalVisibleArea().getCentre();
        b = getRealVisibleArea().getCentre();
    }
    
    maker_.showEditor (a, b);
    //
    }
}

void EditView::handleMaker (juce::Point<int> pt, const juce::String& s)
{
    createObject (pt, s);
    
    dismissMaker();
}

void EditView::dismissMaker()
{
    maker_.hideEditor();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditView::encapsulate()
{
    EditCommands::encapsulate (getIdentifierOfView());
}

void EditView::deencapsulate()
{
    if (!isAbstractionOrInside()) {
    //
    const ObjectComponent* o = getSelectedObject();
    
    jassert (o);
    jassert (o->getObject().isPatch());
    
    EditCommands::deencapsulate (o->getIdentifier());
    //
    }
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

ObjectComponent* EditView::getObjectComponent (core::UniqueId u) const
{
    return objects_.get (u);
}

LineComponent* EditView::getLineComponent (core::UniqueId u) const
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
    juce::ValueTree i (Tree::getParentIfChangedPropertyEquals (t, Tag::Selected));
    
    if (i.isValid() && isChildOf (viewTree_, i) && inspector_) { inspector_->update(); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
