
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
    juce::String t (object.get<juce::String> (Tags::Attributes, Tags::Class));
    
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
    view_ (view),
    object_ (object),
    x_ (object.getCached<int> (Tags::Attributes, Tags::X)),
    y_ (object.getCached<int> (Tags::Attributes, Tags::Y)),
    selected_ (object.getCached<bool> (Tags::Attributes, Tags::Selected)),
    visible_ (object.getCached<bool> (Tags::Attributes, Tags::Visible, true)),
    inlets_ (object.getCached<juce::String> (Tags::Attributes, Tags::Inlets, true)),
    outlets_ (object.getCached<juce::String> (Tags::Attributes, Tags::Outlets, true)),
    label_ (object.getCached<juce::String> (Tags::Parameters, Tags::Label, true)),
    boxPinBackgroundColour_ (Spaghettis()->getCachedColour (Tags::BoxPinBackground)),
    boxSelectedColour_ (Spaghettis()->getCachedColour (Tags::BoxSelected)),
    painter_ (createPainter (this, object)),
    documentation_ (object)
{
    jassert (view);
    
    setOpaque (true); setPaintingIsUnclipped (true);
    
    view_->addChildComponent (this);

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
    
    view_->hide (this);
    view_->removeChildComponent (this);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void openSubPatch (const core::Object& o, View* v)
{
    if (o.isPatch()) { v->getPatch().openSubPatchWindow (o.getIdentifier()); }
}

EditView* asEditView (View* view)
{
    return dynamic_cast<EditView*> (view);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ObjectComponent::mouseMove (const juce::MouseEvent&)
{

}

void ObjectComponent::mouseEnter (const juce::MouseEvent&)
{

}

void ObjectComponent::mouseExit (const juce::MouseEvent&)
{

}

void ObjectComponent::mouseDown (const juce::MouseEvent& e)
{
    if (auto view = asEditView (view_)) {
    //
    if (Mouse::isCommandClick (e))     { painter_->mouseDown (e); }
    else {
    //
    view->dragStart();
    
    if (Mouse::isDoubleClick (e))      { openSubPatch (object_, view); }
    else if (Mouse::isShiftClick (e))  { setSelected (!isSelected());  }
    else if (Mouse::isSimpleClick (e)) {
        if (!isSelected()) {
            view->deselectAll(); setSelected (true);
        }
    }
    //
    }
    //
    }
}

void ObjectComponent::mouseDrag (const juce::MouseEvent& e)
{
    if (auto view = asEditView (view_)) {
    //
    if (isSelected()) { view->drag (PainterPolicy::unscaled (e.getOffsetFromDragStart(), getScale())); }
    //
    }
}

void ObjectComponent::mouseUp (const juce::MouseEvent&)
{

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ObjectComponent::dragStart()
{
    origin_ = getLocalPosition();
}

void ObjectComponent::drag (juce::Point<int> offset)
{
    EditCommands::position (object_.getIdentifier(), origin_ + offset);
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
    
    painter_->paint (view_->getPaintedAreaFromBounds (bounds), g);
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
    return view_->getScale();
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
    EditCommands::moveBack (object_.getIdentifier());
}

void ObjectComponent::moveFront()
{
    EditCommands::moveFront (object_.getIdentifier());
}

void ObjectComponent::snap()
{
    EditCommands::snap (object_.getIdentifier());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool ObjectComponent::isInsideRunView() const
{
    return (dynamic_cast<RunView*> (view_) != nullptr);
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
        view_->show (this, view_->getBoundsFromPaintedArea (painted));
        if (!isRunView) {
            setTooltip (getLabel()); createInletsAndOutlets();
        }
    } else {
        view_->hide (this);
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

std::vector<std::unique_ptr<PinComponent>> createPins (const juce::StringArray& a,
    const juce::Rectangle<int>& bounds,
    const core::Object& object,
    const Documentation& documentation,
    View* view,
    float scale,
    bool isOutlet)
{
    const int n = a.size();
    
    std::vector<std::unique_ptr<PinComponent>> pins;
    
    for (int i = 0; i < n; ++i) {
        const juce::String type (a[i]);
        std::unique_ptr<PinComponent> p = std::make_unique<PinComponent> (view, object, type);
        p->setBounds (getPinBounds (bounds, i, scale, isOutlet));
        p->setVisible (true);
        p->setTooltip (documentation.getPinTooltip (type, isOutlet, i));
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
    
    if (!i.isEmpty()) { iPins_ = createPins (i, bounds, object_, documentation_, view_, scale, false); }
    if (!o.isEmpty()) { oPins_ = createPins (o, bounds, object_, documentation_, view_, scale, true);  }
    
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
