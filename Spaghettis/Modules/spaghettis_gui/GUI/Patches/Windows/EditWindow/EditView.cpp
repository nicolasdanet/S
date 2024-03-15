
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

bool EditView::isDragging() const
{
    return drag_ != nullptr;
}

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

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool EditView::hasUndo() const
{
    return undo_.get().isNotEmpty();
}

bool EditView::hasRedo() const
{
    return redo_.get().isNotEmpty();
}

bool EditView::hasPaste() const
{
    return hasPaste_;
}

bool EditView::hasSelected() const
{
    return (getNumberOfSelectedObjects() > 0) || (getNumberOfSelectedLines() > 0);
}

bool EditView::hasSelectedObject() const
{
    return (getNumberOfSelectedObjects() > 0);
}

bool EditView::hasOnlyOnePatchSelected() const
{
    if (getNumberOfSelectedObjects() == 1) {
        if (auto o = getSelectedObject(); o->isPatch() && !o->isAbstraction()) {
            return true;
        }
    }
        
    return false;
}

bool EditView::hasInspector() const
{
    jassert (inspector_); return inspector_->isActive();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditView::dragAbort()
{
    drag_ = nullptr;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

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

ObjectComponent* EditView::getObjectComponent (core::UniqueId u) const
{
    return objects_.get (u);
}

LineComponent* EditView::getLineComponent (core::UniqueId u) const
{
    return lines_.get (u);
}

int EditView::getInspectorWidth() const
{
    jassert (inspector_); return inspector_->getWidth();
}

float EditView::getScale() const
{
    return scale_;
}

std::optional<core::Point::Real> EditView::getMousePosition() const
{
    if (isMouseOver (true)) {
        return core::Point::Real (core::Point::Scaled (getMouseXYRelative(), getScale()));
    } else {
        return std::nullopt;
    }
}

Synchronizer EditView::getSynchronized() const
{
    if (getNumberOfSelectedLines() == 1)        { return Synchronizer (getSelectedLine()->getLine());     }
    else if (getNumberOfSelectedObjects() == 1) { return Synchronizer (getSelectedObject()->getObject()); }
    else {
        return Synchronizer (getPatch());
    }
}

EditPort* EditView::getPort() const
{
    jassert (port_ != nullptr); return port_;
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

void EditView::setPort (EditPort* owner)
{
    port_ = owner;
}

void EditView::setMouseCursorRecursive (const juce::MouseCursor& m)
{
    auto f = [m](const auto& p) { p->setMouseCursor (m); };
    
    objects_.doForEach (f); lines_.doForEach (f);
    
    setMouseCursor (m);
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
    if (!Mouse::hasAltKey (e)) { getPort()->hideLocator(); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void mouseDragProceed (EditView* view,
        std::unique_ptr<DragStrategy>& drag,
        const juce::MouseEvent& e,
        bool isChild,
        DragFlag flag)
{
    if (!drag) {
        if (Mouse::hasCommandKey (e))       { }
        else if (Mouse::isAltClick (e))     { drag = std::make_unique<ActionHand> (view);    }
        else if (flag == DragFlag::Resize)  { drag = std::make_unique<ActionResize> (view);  }
        else if (flag == DragFlag::Move)    { drag = std::make_unique<ActionMove> (view);    }
        else if (flag == DragFlag::Pin)     { drag = std::make_unique<ActionConnect> (view); }
        else if (!isChild)                  { drag = std::make_unique<ActionLasso> (view);   }
    }

    if (drag) { drag->mouseDrag (e); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditView::handleMouseDown (const juce::MouseEvent& e)
{
    mouseDown (e.getEventRelativeTo (this));
}

void EditView::handleMouseDrag (const juce::MouseEvent& e, DragFlag flag)
{
    mouseDragProceed (this, drag_, e.getEventRelativeTo (this), true, flag);
}

void EditView::handleMouseUp (const juce::MouseEvent& e)
{
    mouseUp (e.getEventRelativeTo (this));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditView::mouseDown (const juce::MouseEvent& e)
{
    hideLocator (e);
    
    if (Mouse::isSimpleClick (e)) {
        if (maker_.isActive()) {
            const auto [pt, s] = maker_.getContent(); Broadcast::create (getIdentifier(), pt, s);
        } else {
            deselectAll();
        }
    }
    
    if (Mouse::isRightClick (e)) { ContextMenu (this).open (e); }
}

void EditView::mouseDrag (const juce::MouseEvent& e)
{
    mouseDragProceed (this, drag_, e.getEventRelativeTo (this), false, DragFlag::None);
}

void EditView::mouseUp (const juce::MouseEvent& e)
{
    if (drag_) { drag_->mouseUp (e); }
    
    dragAbort();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

bool selectObjects (Table<ObjectComponent>& objects, const juce::Rectangle<int>& r)
{
    bool done = false;
    
    auto f = [&done, t = r](const auto& p)
    {
        if (t.intersects (p->getBounds())) { p->setSelected (true); done = true; }
    };

    objects.doForEach (f);

    return done;
}

bool selectLines (Table<LineComponent>& lines, const juce::Rectangle<int>& r)
{
    bool done = false;
    
    auto f = [&done, t = r.toFloat()](const auto& p)
    {
        if (p->intersects (t)) { p->setSelected (true); done = true; }
    };

    lines.doForEach (f);

    return done;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

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
    if (selectObjects (objects_, r) == false) { selectLines (lines_, r); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditView::dragStart()
{
    if (!isAbstractionOrInside()) {
    //
    objects_.doForEach ([](const auto& p) { p->dragStart(); });
    //
    }
}

void EditView::drag (core::Vector::Real offset)
{
    if (!isAbstractionOrInside()) {
    //
    objects_.doForEachSelected ([offset](const auto& p) { p->drag (offset); });
    //
    }
}

void EditView::resize (core::Vector::Real offset)
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

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

template <class F> void forObjectOrAllSelected (ObjectComponent* c, Table<ObjectComponent>& objects, F f)
{
    if (c && !c->isSelected()) { objects.doForUnique (f, c->getIdentifier()); }
    else {
        objects.doForEachSelected (f);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditView::include (ObjectComponent* c)
{
    if (!isAbstractionOrInside()) {
    //
    forObjectOrAllSelected (c, objects_, [](const auto& p) { p->include(); });
    //
    }
}

void EditView::exclude (ObjectComponent* c)
{
    if (!isAbstractionOrInside()) {
    //
    forObjectOrAllSelected (c, objects_, [](const auto& p) { p->exclude(); });
    //
    }
}

void EditView::moveBack (ObjectComponent* c)
{
    if (!isAbstractionOrInside()) {
    //
    forObjectOrAllSelected (c, objects_, [](const auto& p) { p->moveBack(); });
    //
    }
}

void EditView::moveFront (ObjectComponent* c)
{
    if (!isAbstractionOrInside()) {
    //
    forObjectOrAllSelected (c, objects_, [](const auto& p) { p->moveFront(); });
    //
    }
}

void EditView::snapToGrid (ObjectComponent* c)
{
    if (!isAbstractionOrInside()) {
    //
    forObjectOrAllSelected (c, objects_, [](const auto& p) { p->snap(); });
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
    
    const int n = Spaghettis()->getPreferences().get<int> (Tag::Editing, Tag::GridSize);
    
    auto f = [&pt, n](const auto& p)
    {
        pt = getMinimum (pt, p->getPosition()) + core::Vector::Real (n * 2, n * 2);
    };

    objects.doForEachSelected (f);
    
    return pt;
}

void deconnectSelectedLines (Table<LineComponent>& t)
{
    t.doForEachSelected ([](const auto& p) { p->disconnect(); });
}

void setPaste (bool& b)
{
    if (b == false) { b = true; Spaghettis()->updateMenuBar(); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditView::cut()
{
    if (!isAbstractionOrInside()) {
    //
    setPaste (hasPaste_);
    
    Broadcast::cut (getIdentifier());
    //
    }
}

void EditView::copy()
{
    if (!isAbstractionOrInside()) {
    //
    setPaste (hasPaste_);
    
    Broadcast::copy (getIdentifier());
    //
    }
}

void EditView::paste()
{
    if (!isAbstractionOrInside()) {
    //
    const core::Area::Real area       = getPort()->getVisibleArea();
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

void EditView::undo()
{
    if (!isAbstractionOrInside()) { Broadcast::undo (getIdentifier()); }
}

void EditView::redo()
{
    if (!isAbstractionOrInside()) { Broadcast::redo (getIdentifier()); }
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

void EditView::makerRequire (bool isFromMenu)
{
    if (!isAbstractionOrInside()) {
    //
    std::optional<core::Point::Real> pt (getMousePosition());
    
    bool useCentre = isFromMenu;
    
    #if JUCE_LINUX
    useCentre |= !pt.has_value();
    #endif
    
    if (useCentre) {
        pt = core::Point::Real (getPort()->getVisibleArea().getCentre());
    }
    
    if (pt.has_value()) { maker_.showEditor (core::Point::Scaled (pt.value(), getScale())); }
    //
    }
}

void EditView::makerOpen (core::Point::Scaled pt)
{
    maker_.showEditor (pt);
}

void EditView::makerHandle (core::Point::Real pt, const juce::String& s)
{
    Broadcast::create (getIdentifier(), pt, s);
    
    makerDismiss();
}

void EditView::makerDismiss()
{
    maker_.hideEditor();
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

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void updateOrder (EditView* view, Table<ObjectComponent>& objects, Table<LineComponent>& lines)
{
    objects.sort (view->getPatch().getObjects());
    
    auto f = [c = static_cast<juce::Component*> (nullptr)](const auto& p) mutable
    {
        p->moveBehind (c); c = p.get();
    };
    
    objects.doForEachReversed (f);
    
    lines.doForEachReversed ([](const auto& p) { p->updateOrder(); });
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void addComponent (EditView* view,
    Table<ObjectComponent>& objects,
    Table<LineComponent>& lines,
    const juce::ValueTree& child)
{
    if (Tree::isObject (child))    { objects.add (view, core::Object (child)); }
    else if (Tree::isLine (child)) { lines.add (view, core::Line (child));     }
}

void removeComponent (EditView* view,
    Table<ObjectComponent>& objects,
    Table<LineComponent>& lines,
    const juce::ValueTree& child)
{
    if (Tree::isObject (child))    { objects.remove (core::Object (child).getIdentifier()); }
    else if (Tree::isLine (child)) { lines.remove (core::Line (child).getIdentifier());     }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditView::handleAsyncUpdate()
{
    updateOrder (this, objects_, lines_);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditView::valueTreeChildAdded (juce::ValueTree& t, juce::ValueTree& child)
{
    if (isSameAsPatch (t)) { addComponent (this, objects_, lines_, child); }
}

void EditView::valueTreeChildRemoved (juce::ValueTree& t, juce::ValueTree& child, int)
{
    if (isSameAsPatch (t)) { removeComponent (this, objects_, lines_, child); }
}

void EditView::valueTreeChildOrderChanged (juce::ValueTree& t, int oldIndex, int newIndex)
{
    if (isSameAsPatch (t)) { triggerAsyncUpdate(); }
}

void EditView::valueTreePropertyChanged (juce::ValueTree& t, const juce::Identifier&)
{
    juce::ValueTree i (Tree::getParentIfChangedPropertyEquals (t, Tag::Selected));
    
    jassert (inspector_);
    
    if (i.isValid() && isSameAsPatch (i.getParent())) { inspector_->update(); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditView::initialize (const juce::ValueTree& tree)
{
    for (const auto& child : tree) { addComponent (this, objects_, lines_, child); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
