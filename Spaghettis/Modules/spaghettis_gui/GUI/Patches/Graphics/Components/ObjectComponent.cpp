
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

std::unique_ptr<PainterStrategy> createPainter (ObjectComponent* owner, const juce::String& type)
{
    if (type == "bng")          { return std::make_unique<BangPainter> (owner);     }
    else if (type == "comment") { return std::make_unique<CommentPainter> (owner);  }
    else if (type == "gatom")   { return std::make_unique<AtomPainter> (owner);     }
    else if (type == "garray")  { return std::make_unique<ArrayPainter> (owner);    }
    else if (type == "inlet")   { return std::make_unique<InletPainter> (owner);    }
    else if (type == "message") { return std::make_unique<MessagePainter> (owner);  }
    else if (type == "outlet")  { return std::make_unique<InletPainter> (owner);    }
    else if (type == "tgl")     { return std::make_unique<TogglePainter> (owner);   }
    else if (type == "hradio")  { return std::make_unique<RadioPainter> (owner);    }
    else if (type == "hslider") { return std::make_unique<SliderPainter> (owner);   }
    else if (type == "dial")    { return std::make_unique<DialPainter> (owner);     }
    else if (type == "vu")      { return std::make_unique<VuPainter> (owner);       }
    else {
        return std::make_unique<BoxPainter> (owner);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

ObjectComponent::ObjectComponent (PatchView* view, const core::Object& object) :
    DragHandler (view),
    object_ (object),
    x_ (object.getCached<int> (Tag::Attributes, Tag::X)),
    y_ (object.getCached<int> (Tag::Attributes, Tag::Y)),
    selected_ (object.getCached<bool> (Tag::Attributes, Tag::Selected)),
    visible_ (object.getCached<bool> (Tag::Attributes, Tag::Visible, true)),
    inlets_ (object.getCached<juce::String> (Tag::Attributes, Tag::Inlets, true)),
    outlets_ (object.getCached<juce::String> (Tag::Attributes, Tag::Outlets, true)),
    label_ (object.getCached<juce::String> (Tag::Parameters, Tag::Label, true)),
    boxSelectedColour_ (Spaghettis()->getCachedColour (Tag::BoxSelected)),
    painter_ (createPainter (this, object.get<juce::String> (Tag::Attributes, Tag::Class))),
    origin_ (0, 0),
    hasResize_ (false),
    isLocked_ (object_.isLocked()),
    isInsideRunView_ (getEditView() == nullptr)
{
    jassert (view);
    
    getView()->addChildComponent (this);

    update();
    
    auto f = [this]() { update(); };
    
    visible_.attach (f);
    inlets_.attach (f);
    outlets_.attach (f);
    label_.attach (f);
    x_.attach (f);
    y_.attach (f);
    
    selected_.attach (Painter::repaint (this));
    boxSelectedColour_.attach (Painter::repaint (this));
    
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
    if (!isLocked() && object_.isGraphic() && painter_->getDimensions().has_value()) {
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

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Rectangle<int> getPaintedAreaFromBounds (const juce::Rectangle<int>& r, float f)
{
    return r.reduced (0, Painter::pinHeight (f));
}

juce::Rectangle<int> getBoundsFromPaintedArea (const juce::Rectangle<int>& r, float f)
{
    return r.expanded (0, Painter::pinHeight (f));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ObjectComponent::paint (juce::Graphics& g)
{
    const juce::Rectangle<int> bounds (getLocalBounds());
    const juce::Rectangle<int> painted (getPaintedAreaFromBounds (bounds, getScale()));
    const int w = painter_->getWidthOfPinsBackground();
    
    if (!isInsideRunView() && selected_.get()) { g.setColour (boxSelectedColour_.get()); }
    else {
        g.setColour (painter_->getPinsBackground());
    }
    
    g.fillRect (juce::Rectangle<int> (bounds.getTopLeft(), painted.getTopRight()).withWidth (w));
    g.fillRect (juce::Rectangle<int> (painted.getBottomLeft(), bounds.getBottomRight()).withWidth (w));
    
    painter_->paint (painted, g);
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

core::Point::Real ObjectComponent::getPosition() const
{
    return core::Point::Real (x_.get(), y_.get());
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
        getView()->show (this, getBoundsFromPaintedArea (painted, getScale()));
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
    const int w = Painter::pinGripX (f) * 2 + Painter::pinWidth (f);
    const int x = bounds.getX() + (index * w);
    
    bounds.setX (x);
    bounds.setWidth (Painter::pinWidth (f));
    
    if (isOutlet) {
        bounds = bounds.removeFromBottom (Painter::pinHeight (f));
    } else {
        bounds = bounds.removeFromTop (Painter::pinHeight (f));
    }
    
    return bounds.expanded (Painter::pinGripX (f), Painter::pinGripY (f));
}

juce::String getPinTooltip (const data::Data& documentation, const juce::String& type, bool isOutlet, int i)
{
    const juce::String t = Strings::firstLetterCapitalized (type);
    const juce::String k = (isOutlet ? Tag::Outlet : Tag::Inlet) + juce::String (i);
    
    if (documentation.hasParameter (Tag::Documentation, k)) {
        return t + ": " + documentation.getParameter (Tag::Documentation, k).getValueTyped<juce::String>();
    }
    
    return t;
}

std::vector<std::unique_ptr<PinComponent>> createPins (const juce::StringArray& a,
    const juce::Rectangle<int>& bounds,
    const core::Object& object,
    const data::Data& documentation,
    PatchView* view,
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
    
    const data::Data documentation (Documentation::get (object_));
    
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
