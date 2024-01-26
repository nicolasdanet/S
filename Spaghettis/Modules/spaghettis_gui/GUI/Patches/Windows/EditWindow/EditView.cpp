
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

EditView::EditView (const PatchBase& base) :
    PatchView (base),
    maker_ (this),
    isAbstraction_ (getPatch().isAbstraction()),
    isLocked_ (getPatch().isLocked()),
    undo_ (getPatch().getCached<juce::String> (Tag::Attributes, Tag::Undo)),
    redo_ (getPatch().getCached<juce::String> (Tag::Attributes, Tag::Redo)),
    patchBackgroundColour_ (Painted (Spaghettis()->getCachedColour (Tag::PatchBackground), this)),
    scale_ (1.0f)
{
    getViewTree().addListener (this);
    undo_.attach ([]() { Spaghettis()->updateMenuBar(); });
    redo_.attach ([]() { Spaghettis()->updateMenuBar(); });
    setOpaque (true);
    initialize (getViewTree());
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

void EditView::hideLocator (const juce::MouseEvent& e)
{
    if (Mouse::hasAltKey (e) == false) { getPort()->hideLocator(); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditView::mouseDragProceed (const juce::MouseEvent& e, bool isChild, DragFlag flag)
{
    if (!drag_) {
        if (Mouse::hasCommandKey (e))       { }
        else if (Mouse::isAltClick (e))     { drag_ = std::make_unique<ActionHand> (this);    }
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
    drag_ = nullptr;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* First let create the object when dismissing the Maker's CallOut Box clicking into the view. */

void EditView::mouseDown (const juce::MouseEvent& e)
{
    hideLocator (e);
    
    if (Mouse::isSimpleClick (e)) {
    //
    if (maker_.isActive() == false) { deselectAll(); }
    else {
        const auto [pt, s] = maker_.getContent(); createObject (pt, s);
    }
    //
    }
    
    if (Mouse::isRightClick (e)) { ContextMenu (this).open (e); }
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
    
    objects_.doForEach (f); lines_.doForEach (f);
    
    setMouseCursor (m);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

std::optional<core::Point::Real> EditView::getMousePosition() const
{
    if (isMouseOver (true)) {
        return core::Point::Real (core::Point::Scaled (getMouseXYRelative(), getScale()));
    }

    return std::nullopt;
}

core::Area::Real EditView::getVisibleArea() const
{
    return getPort()->getVisibleArea();
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
    auto n = t.doCountIf ([](const auto& p) { return p->isSelected(); });

    return static_cast<int> (n);
}

template <class T> core::UniqueId getSelected (T& t)
{
    core::UniqueId u = 0;
    
    auto f = [&u](const auto& p) { u = p->getIdentifier(); };

    t.doForEachSelected (f);
    
    return u;
}

void deconnectSelectedLines (Table<LineComponent>& t)
{
    t.doForEachSelected ([](const auto& p) { p->disconnect(); });
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
    
    auto f = [&r, &done](const auto& p)
    {
        if (r.intersects (p->getBounds())) { p->setSelected (true); done = true; }
    };

    objects_.doForEach (f);

    return done;
}

bool EditView::selectLines (const juce::Rectangle<int>& r)
{
    bool done = false;
    
    auto f = [&done, t = r.toFloat()](const auto& p)
    {
        if (p->intersects (t)) { p->setSelected (true); done = true; }
    };

    lines_.doForEach (f);

    return done;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Sync EditView::getSynchronized()
{
    if (getNumberOfSelectedLines() == 1)        { return Sync (getSelectedLine()->getLine());     }
    else if (getNumberOfSelectedObjects() == 1) { return Sync (getSelectedObject()->getObject()); }
    else {
        return Sync (getPatch());
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool EditView::hasOnlyOnePatchSelected() const
{
    if (getNumberOfSelectedObjects() == 1) {
        if (auto o = getSelectedObject(); o->isPatch() && !o->isAbstraction()) {
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
    lines_.doForEach   ([](const auto& p) { p->setSelected (false); });
    objects_.doForEach ([](const auto& p) { p->setSelected (false); });
}

void EditView::selectAll()
{
    objects_.doForEach ([](const auto& p) { p->setSelected (true); });
}

void EditView::select (const juce::Rectangle<int>& r)
{
    if (selectObjects (r) == false) { selectLines (r); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditView::dragObjectsStart()
{
    if (!isAbstractionOrInside()) {
    //
    objects_.doForEach ([](const auto& p) { p->dragStart(); });
    //
    }
}

void EditView::dragObjects (core::Vector::Real offset)
{
    if (!isAbstractionOrInside()) {
    //
    objects_.doForEachSelected ([offset](const auto& p) { p->drag (offset); });
    //
    }
}

void EditView::resizeObjects (core::Vector::Real offset)
{
    if (!isAbstractionOrInside()) {
    //
    objects_.doForEachSelected ([offset](const auto& p) { p->resize (offset); });
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditView::moveBack (ObjectComponent* c)
{
    if (!isAbstractionOrInside()) {
    //
    forObject (c, [](const auto& p) { p->moveBack(); });
    //
    }
}

void EditView::moveFront (ObjectComponent* c)
{
    if (!isAbstractionOrInside()) {
    //
    forObject (c, [](const auto& p) { p->moveFront(); });
    //
    }
}

void EditView::snapToGrid (ObjectComponent* c)
{
    if (!isAbstractionOrInside()) {
    //
    forObject (c, [](const auto& p) { p->snap(); });
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditView::undo()
{
    if (!isAbstractionOrInside()) { Broadcast::undo (getIdentifier()); }
}

void EditView::redo()
{
    if (!isAbstractionOrInside()) { Broadcast::redo (getIdentifier()); }
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
    return juce::String (NEEDS_TRANS ("Undo")) + " " + Strings::firstLetterCapitalized (undo_.get());
    //
    }
    
    return juce::String();
}

juce::String EditView::getRedoAction() const
{
    if (hasRedo()) {
    //
    return juce::String (NEEDS_TRANS ("Redo")) + " " + Strings::firstLetterCapitalized (redo_.get());
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
    
    Broadcast::cut (getIdentifier());
    //
    }
}

void EditView::copy()
{
    if (!isAbstractionOrInside()) {
    //
    setPaste();
    
    Broadcast::copy (getIdentifier());
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

core::Point::Real getMinimum (std::optional<core::Point::Real> pt, core::Point::Real b)
{
    const core::Point::Real a = pt.value_or (b);
    
    const int x1 = a.getPoint().getX();
    const int x2 = b.getPoint().getX();
    const int y1 = a.getPoint().getY();
    const int y2 = b.getPoint().getY();
    
    return core::Point::Real (juce::jmin (x1, x2), juce::jmin (y1, y2));
}

std::optional<core::Point::Real> getPositionNextSelectedObjects (Table<ObjectComponent>& objects)
{
    std::optional<core::Point::Real> pt;
    
    const int n = Spaghettis()->getPreferences().getCached<int> (Tag::Editing, Tag::GridSize);
    
    auto f = [&pt, n](const auto& p)
    {
        pt = getMinimum (pt, p->getPosition()) + core::Vector::Real (n * 2, n * 2);
    };

    objects.doForEachSelected (f);
    
    return pt;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditView::paste()
{
    if (!isAbstractionOrInside()) {
    //
    const core::Area::Real area       = getVisibleArea();
    const core::Point::Real centre    = area.getCentre();
    const core::Point::Real mouse     = getMousePosition().value_or (centre);
    const core::Point::Real selection = getPositionNextSelectedObjects (objects_).value_or (mouse);
    const core::Point::Real pt        = area.contains (selection) ? selection : centre;
    
    Broadcast::paste (getIdentifier(), pt);
    //
    }
}

void EditView::duplicate()
{
    if (!isAbstractionOrInside()) {
    //
    Broadcast::duplicate (getIdentifier());
    //
    }
}

void EditView::remove()
{
    if (!isAbstractionOrInside()) {
    //
    Broadcast::remove (getIdentifier());     /* Remove all selected objects. */
    
    deconnectSelectedLines (lines_);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditView::createObject (core::Point::Real pt, const juce::String& s)
{
    if (s.isNotEmpty()) { Spaghettis()->handle (Inputs::createObject (getIdentifier(), pt, s)); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditView::requireMaker (bool isFromMenu)
{
    if (!isAbstractionOrInside()) {
    //
    std::optional<core::Point::Real> pt (getMousePosition());
    
    bool useCentre = isFromMenu;
    
    #if JUCE_LINUX
    useCentre |= !pt.has_value();
    #endif
    
    if (useCentre) {
        pt = core::Point::Real (getVisibleArea().getCentre());
    }
    
    if (pt.has_value()) { maker_.showEditor (core::Point::Scaled (pt.value(), getScale())); }
    //
    }
}

void EditView::openMaker (core::Point::Scaled pt)
{
    maker_.showEditor (pt);
}

void EditView::handleMaker (core::Point::Real pt, const juce::String& s)
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
    Broadcast::encapsulate (getIdentifier());
}

void EditView::deencapsulate()
{
    if (!isAbstractionOrInside()) {
    //
    const ObjectComponent* o = getSelectedObject();
    
    jassert (o);
    jassert (o->isPatch());
    
    Broadcast::deencapsulate (o->getIdentifier());
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
    
    objects_.doForEach (f); lines_.doForEach (f);
}

float EditView::getScale() const
{
    return scale_;
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
    if (Tree::isObject (child))    { objects_.remove (core::Object (child).getIdentifier()); }
    else if (Tree::isLine (child)) { lines_.remove (core::Line (child).getIdentifier());     }
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
    objects_.sort (getPatch().getObjects());
    
    auto f = [c = static_cast<juce::Component*> (nullptr)](const auto& p) mutable
    {
        p->moveBehind (c); c = p.get();
    };
    
    objects_.doForEachReversed (f);
    
    lines_.doForEachReversed ([](const auto& p) { p->updateOrder(); });
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
    if (isSameAsPatch (t)) { addComponent (child); }
}

void EditView::valueTreeChildRemoved (juce::ValueTree& t, juce::ValueTree& child, int)
{
    if (isSameAsPatch (t)) { removeComponent (child); }
}

void EditView::valueTreeChildOrderChanged (juce::ValueTree& t, int oldIndex, int newIndex)
{
    if (isSameAsPatch (t)) { triggerAsyncUpdate(); }
}

void EditView::valueTreePropertyChanged (juce::ValueTree& t, const juce::Identifier&)
{
    juce::ValueTree i (Tree::getParentIfChangedPropertyEquals (t, Tag::Selected));
    
    if (i.isValid() && isSameAsPatch (i.getParent()) && inspector_) { inspector_->update(); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
