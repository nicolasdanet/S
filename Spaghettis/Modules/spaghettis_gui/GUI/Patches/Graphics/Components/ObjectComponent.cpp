
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

ObjectComponent::ObjectComponent (PatchView* view, const core::Object& object) :
    MouseHandler (view),
    object_ (object),
    x_ (Resized (object.getCached<int> (Tag::Attributes, Tag::X), this)),
    y_ (Resized (object.getCached<int> (Tag::Attributes, Tag::Y), this)),
    selected_ (Painted (object.getCached<bool> (Tag::Attributes, Tag::Selected), this)),
    visible_ (Resized (object.getCached<bool> (Tag::Attributes, Tag::Visible, true), this)),
    inlets_ (Resized (object.getCached<juce::String> (Tag::Attributes, Tag::Inlets, true), this)),
    outlets_ (Resized (object.getCached<juce::String> (Tag::Attributes, Tag::Outlets, true), this)),
    included_ (Resized (object.getCached<bool> (Tag::Parameters, Tag::Included, true), this)),
    label_ (object.getCached<juce::String> (Tag::Parameters, Tag::Label)),
    painter_ (Painter::createPainter (this, object.get<juce::String> (Tag::Attributes, Tag::Class))),
    origin_ (0, 0),
    hasResize_ (false),
    isLocked_ (object_.isLocked()),
    isInsideRunView_ (getEditView() == nullptr)
{
    jassert (view);
    
    label_.attach ([this]() { updateTooltip(); });
    
    getView()->addChildComponent (this);

    update();
    
    if (isInsideRunView()) { addMouseListener (painter_.get(), true); }
}

ObjectComponent::~ObjectComponent()
{
    if (isInsideRunView()) { removeMouseListener (painter_.get()); }
    
    removeAllChangeListeners();
    removeInletsAndOultets();
    
    getView()->hide (this);
    getView()->removeChildComponent (this);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool ObjectComponent::isInsideRunView() const
{
    return isInsideRunView_;
}

bool ObjectComponent::isLocked() const
{
    return isLocked_;
}

bool ObjectComponent::isPatch() const
{
    return object_.isPatch();
}

bool ObjectComponent::isAbstraction() const
{
    return object_.isAbstraction();
}

bool ObjectComponent::isGraphic() const
{
    return object_.isGraphic();
}

bool ObjectComponent::isSelected() const
{
    return selected_.get();
}

bool ObjectComponent::isIncluded() const
{
    return included_.get();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ObjectComponent::setSelected (bool isSelected)
{
    if (selected_.get() != isSelected) {
        if (isSelected) { Broadcast::select (object_.getIdentifier()); }
        else {
            Broadcast::deselect (object_.getIdentifier());
        }
        
        /* Don't wait core engine feedback. */
        
        selected_.set (isSelected); Spaghettis()->updateMenuBar();
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

core::UniqueId ObjectComponent::getIdentifier() const
{
    return object_.getIdentifier();
}

core::Object ObjectComponent::getObject() const
{
    return object_;
}

core::Point::Real ObjectComponent::getPosition() const
{
    return core::Point::Real (x_.get(), y_.get());
}

int ObjectComponent::getNumberOfInlets() const
{
    return juce::StringArray::fromTokens (inlets_.get(), true).size();
}

int ObjectComponent::getNumberOfOutlets() const
{
    return juce::StringArray::fromTokens (outlets_.get(), true).size();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PinComponent* getPinAt (const std::vector<std::unique_ptr<PinComponent>>& v, int n)
{
    jassert (n >= 0);
    
    if (static_cast<std::vector<std::unique_ptr<PinComponent>>::size_type> (n) < v.size()) {
        return v[n].get();
    } else {
        return nullptr;     /* If not visible or ill-formed (e.g. subpatch two-phase creation). */
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PinComponent* ObjectComponent::getInletAt (int n) const
{
    return getPinAt (iPins_, n);
}

PinComponent* ObjectComponent::getOutletAt (int n) const
{
    return getPinAt (oPins_, n);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void openPatch (const core::Object& o, PatchView* v)
{
    if (o.isPatch()) { v->getPatchRoot().showEditWindow (o.getIdentifier()); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool ObjectComponent::canResize (const juce::MouseEvent& e) const
{
    if (!isLocked() && isGraphic() && painter_->getDimensions().has_value()) {
    //
    /* Bottom-right quarter of object. */
    
    constexpr float f = 0.25f;
    constexpr float g = 1.00f - f;
    
    return getLocalBounds().getProportion (juce::Rectangle<float> (g, g, f, f)).contains (e.getPosition());
    //
    }
    
    return false;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ObjectComponent::mouseMove (const juce::MouseEvent& e)
{
    if (!isInsideRunView()) {
    //
    hasResize_ = (!Mouse::hasModifier (e) && canResize (e));
    
    if (hasResize_) {
        setMouseCursor (juce::MouseCursor::BottomRightCornerResizeCursor);
    } else {
        setMouseCursor (juce::MouseCursor::NormalCursor);
    }
    //
    }
}

void ObjectComponent::mouseEnter (const juce::MouseEvent&)
{
    if (!isInsideRunView()) {
    //
    hasResize_ = false;
    //
    }
}

void ObjectComponent::mouseExit (const juce::MouseEvent&)
{
    if (!isInsideRunView()) {
    //
    hasResize_ = false;

    setMouseCursor (juce::MouseCursor::NormalCursor);
    //
    }
}

void ObjectComponent::mouseDown (const juce::MouseEvent& e)
{
    if (auto view = getEditView()) {
    //
    view->hideLocator (e);
    
    if (forwardMouseDown (e, painter_.get()))   { }
    else if (Mouse::isRightClick (e))           { ContextMenu (view).open (e, this); }
    else if (Mouse::isAltClick (e))             { }
    else if (Mouse::isDoubleClick (e))          { openPatch (object_, view); }
    else if (Mouse::isShiftClick (e))           { setSelected (!isSelected());  }
    else if (Mouse::isSimpleClick (e))          {
        if (!isSelected()) {
            view->deselectAll(); setSelected (true);
        }
    }
    //
    }
}

void ObjectComponent::mouseDrag (const juce::MouseEvent& e)
{
    const DragFlag flag = hasResize_ ? DragFlag::Resize : (isSelected() ? DragFlag::Move : DragFlag::None);
    
    forwardMouseDrag (e, painter_.get(), flag);
}

void ObjectComponent::mouseUp (const juce::MouseEvent& e)
{
    forwardMouseUp (e, painter_.get());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ObjectComponent::dragStart()
{
    origin_     = getPosition();
    dimensions_ = painter_->getDimensions();
}

void ObjectComponent::drag (core::Vector::Real offset)
{
    if (!isLocked()) {
    //
    Broadcast::position (object_.getIdentifier(), origin_ + offset);
    //
    }
}

void ObjectComponent::resize (core::Vector::Real offset)
{
    if (!isLocked() && dimensions_.has_value()) { painter_->setDimensions (offset + dimensions_.value()); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ObjectComponent::moveBehind (juce::Component* c)
{
    if (c) {
        toBehind (c);
        for (const auto& i : iPins_) { i->toBehind (c); }
        for (const auto& o : oPins_) { o->toBehind (c); }
    }
}

void ObjectComponent::moveBack()
{
    if (!isLocked()) { Broadcast::moveBack (object_.getIdentifier()); }
}

void ObjectComponent::moveFront()
{
    if (!isLocked()) { Broadcast::moveFront (object_.getIdentifier()); }
}

void ObjectComponent::snap()
{
    if (!isLocked()) { Broadcast::snap (object_.getIdentifier()); }
}

void ObjectComponent::include()
{
    if (!isLocked() && isGraphic()) { Broadcast::include (object_.getIdentifier()); }
}

void ObjectComponent::exclude()
{
    if (!isLocked() && isGraphic()) { Broadcast::exclude (object_.getIdentifier()); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ObjectComponent::paint (juce::Graphics& g)
{
    painter_->paint (getLocalBounds(), g);
}
    
void ObjectComponent::resized()
{
    update();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ObjectComponent::scaleChanged()
{
    update (false); repaint();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
