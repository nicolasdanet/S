
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

std::unique_ptr<PainterPolicy> createPainter (ObjectComponent* owner, const core::Object& object)
{
    juce::String t (object.get<juce::String> (Tag::Attributes, Tag::Class));
    
    if (t == "bng") { return std::make_unique<BangPainter> (owner, object); }
    else {
        return std::make_unique<BoxPainter> (owner, object);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

ObjectComponent::ObjectComponent (View* view, const core::Object& object) :
    Dragable (view),
    object_ (object),
    x_ (object.getCached<int> (Tag::Attributes, Tag::X)),
    y_ (object.getCached<int> (Tag::Attributes, Tag::Y)),
    selected_ (object.getCached<bool> (Tag::Attributes, Tag::Selected)),
    visible_ (object.getCached<bool> (Tag::Attributes, Tag::Visible, true)),
    inlets_ (object.getCached<juce::String> (Tag::Attributes, Tag::Inlets, true)),
    outlets_ (object.getCached<juce::String> (Tag::Attributes, Tag::Outlets, true)),
    label_ (object.getCached<juce::String> (Tag::Parameters, Tag::Label, true)),
    boxPinBackgroundColour_ (Spaghettis()->getCachedColour (Tag::BoxPinBackground)),
    boxSelectedColour_ (Spaghettis()->getCachedColour (Tag::BoxSelected)),
    painter_ (createPainter (this, object)),
    hasResize_ (false),
    isLocked_ (object_.isLocked()),
    isInsideRunView_ (View::asEditView (view) == nullptr)
{
    jassert (view);
    
    setOpaque (true); setPaintingIsUnclipped (true);
    
    getView()->addChildComponent (this);

    update();
    
    auto f = [this]() { update(); };
    
    visible_.attach (f);
    inlets_.attach (f);
    outlets_.attach (f);
    label_.attach (f);
    x_.attach (f);
    y_.attach (f);
    
    selected_.attach (PainterPolicy::repaint (this));
    boxPinBackgroundColour_.attach (PainterPolicy::repaint (this));
    boxSelectedColour_.attach (PainterPolicy::repaint (this));
    
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

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void openPatch (const core::Object& o, View* v)
{
    if (o.isPatch()) { v->getPatchRoot().openEditWindow (o.getIdentifier()); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool ObjectComponent::canResize (const juce::MouseEvent& e) const
{
    if (!isLocked() && object_.isGraphic()) {
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
    hasResize_ = false;
}

void ObjectComponent::mouseExit (const juce::MouseEvent&)
{
    hasResize_ = false;

    setMouseCursor (juce::MouseCursor::NormalCursor);
}

void ObjectComponent::mouseDown (const juce::MouseEvent& e)
{
    if (auto view = View::asEditView (getView())) {
    //
    if (Mouse::isRightClick (e))        { ContextMenu (view).open (e, this); }
    else if (Mouse::isCommandClick (e)) { painter_->mouseDown (e); }
    else if (Mouse::isAltClick (e))     { }
    else if (Mouse::isDoubleClick (e))  { openPatch (object_, view); }
    else if (Mouse::isShiftClick (e))   { setSelected (!isSelected());  }
    else if (Mouse::isSimpleClick (e))  {
        if (!isSelected()) {
            view->deselectAll(); setSelected (true);
        }
    }
    //
    }
}

void ObjectComponent::mouseDrag (const juce::MouseEvent& e)
{
    if (!isInsideRunView()) {
    //
    const DragFlag flag = hasResize_ ? DragFlag::Resize : (isSelected() ? DragFlag::Move : DragFlag::None);
    
    handleMouseDrag (e, flag);
    //
    }
}

void ObjectComponent::mouseUp (const juce::MouseEvent& e)
{
    if (!isInsideRunView()) {
    //
    handleMouseUp (e);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ObjectComponent::dragStart()
{
    origin_ = getLocalPosition();
    width_  = painter_->getWidth();
}

void ObjectComponent::drag (juce::Point<int> offset)
{
    if (!isLocked()) { EditCommands::position (object_.getIdentifier(), offset + origin_); }
}

void ObjectComponent::resize (juce::Point<int> offset)
{
    if (!isLocked() && width_.has_value()) { painter_->setWidth (offset + width_.value()); }
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

void ObjectComponent::paint (juce::Graphics& g)
{
    const juce::Rectangle<int> bounds (getLocalBounds());
    
    g.setColour (selected_.get() ? boxSelectedColour_.get() : boxPinBackgroundColour_.get());
    g.fillRect (bounds);
    
    painter_->paint (getView()->getPaintedAreaFromBounds (bounds), g);
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

float ObjectComponent::getScale() const
{
    return getView()->getScale();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Point<int> ObjectComponent::getLocalPosition() const
{
    return juce::Point<int> (x_.get(), y_.get());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ObjectComponent::setSelected (bool isSelected)
{
    if (selected_.get() != isSelected) {
        if (isSelected) { EditCommands::select (object_.getIdentifier()); }
        else {
            EditCommands::deselect (object_.getIdentifier());
        }
        
        /* Don't wait core engine feedback. */
        
        selected_.set (isSelected); Spaghettis()->updateMenuBar();
    }
}

bool ObjectComponent::isSelected() const
{
    return selected_.get();
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

void ObjectComponent::moveAllPinsFront()
{
    for (const auto& i : iPins_) { i->toBehind (this); }
    for (const auto& o : oPins_) { o->toBehind (this); }
    
    if (!iPins_.empty())      { toBehind (iPins_.front().get()); }
    else if (!oPins_.empty()) { toBehind (oPins_.front().get()); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void ObjectComponent::moveBack()
{
    if (!isLocked()) { EditCommands::moveBack (object_.getIdentifier()); }
}

void ObjectComponent::moveFront()
{
    if (!isLocked()) { EditCommands::moveFront (object_.getIdentifier()); }
}

void ObjectComponent::snap()
{
    if (!isLocked()) { EditCommands::snap (object_.getIdentifier()); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool ObjectComponent::isInsideRunView() const
{
    return isInsideRunView_;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool ObjectComponent::hasLabel() const
{
    return label_.isValid() && label_.get().isNotEmpty();
}

juce::String ObjectComponent::getLabel() const
{
    return label_.get();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ObjectComponent::update (bool notify)
{
    const bool isRunView = isInsideRunView();
    const bool isVisible = isRunView ? (hasLabel() && visible_.get()) : visible_.get();
    
    removeInletsAndOultets();
    
    if (isVisible) {
        const juce::Rectangle<int> painted (painter_->getRequiredBounds());
        getView()->show (this, getView()->getBoundsFromPaintedArea (painted));
        if (!isRunView) {
            setTooltip (getLabel()); createInletsAndOutlets();
        }
    } else {
        getView()->hide (this);
    }
    
    if (notify) { sendChangeMessage(); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

juce::Rectangle<int> getPinBounds (juce::Rectangle<int> bounds, int index, float f, bool isOutlet)
{
    const int w = PainterPolicy::pinGripX (f) * 2 + PainterPolicy::pinWidth (f);
    const int x = bounds.getX() + (index * w);
    
    bounds.setX (x);
    bounds.setWidth (PainterPolicy::pinWidth (f));
    
    if (isOutlet) {
        bounds = bounds.removeFromBottom (PainterPolicy::pinHeight (f));
    } else {
        bounds = bounds.removeFromTop (PainterPolicy::pinHeight (f));
    }
    
    return bounds.expanded (PainterPolicy::pinGripX (f), PainterPolicy::pinGripY (f));
}

juce::String getPinTooltip (const core::Data& documentation, const juce::String& type, bool isOutlet, int i)
{
    const juce::String t = Helpers::firstLetterCapitalized (type);
    const juce::String k = (isOutlet ? Tag::Outlet : Tag::Inlet) + juce::String (i);
    
    if (documentation.hasParameter (Tag::Documentation, k)) {
        return t + ": " + documentation.getParameter (Tag::Documentation, k).getValueTyped<juce::String>();
    }
    
    return t;
}

std::vector<std::unique_ptr<PinComponent>> createPins (const juce::StringArray& a,
    const juce::Rectangle<int>& bounds,
    const core::Object& object,
    const core::Data& documentation,
    View* view,
    float scale,
    bool isOutlet)
{
    const int n = a.size();
    
    std::vector<std::unique_ptr<PinComponent>> pins; pins.reserve (n);
    
    for (int i = 0; i < n; ++i) {
        const juce::String type (a[i]);
        std::unique_ptr<PinComponent> p = std::make_unique<PinComponent> (view, object, i, type, isOutlet);
        p->setBounds (getPinBounds (bounds, i, scale, isOutlet));
        p->setVisible (true);
        p->setTooltip (getPinTooltip (documentation, type, isOutlet, i));
        pins.push_back (std::move (p));
    }
    
    return pins;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

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

void ObjectComponent::createInletsAndOutlets()
{
    const float scale = getScale();
        
    const juce::StringArray i (juce::StringArray::fromTokens (inlets_.get(), true));
    const juce::StringArray o (juce::StringArray::fromTokens (outlets_.get(), true));
    
    const juce::Rectangle<int> bounds (getBounds());
    
    const core::Data documentation (Documentation::get (object_));
    
    if (!i.isEmpty()) { iPins_ = createPins (i, bounds, object_, documentation, getView(), scale, false); }
    if (!o.isEmpty()) { oPins_ = createPins (o, bounds, object_, documentation, getView(), scale, true);  }
    
    moveAllPinsFront();
}

void ObjectComponent::removeInletsAndOultets()
{
    iPins_.clear();
    oPins_.clear();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
