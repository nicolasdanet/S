
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
    juce::String t (object.getAttribute<juce::String> (Tags::Class));
    
    if (t == "bng") { return std::make_unique<BangPainter> (owner, object); }
    else {
        jassert (object.isGUI() == false); return std::make_unique<BoxPainter> (owner, object);
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
    x_ (object.getCachedAttribute<int> (Tags::X)),
    y_ (object.getCachedAttribute<int> (Tags::Y)),
    selected_ (object.getCachedAttribute<bool> (Tags::Selected)),
    visible_ (object.getCachedAttribute<bool> (Tags::Visible, true)),
    inlets_ (object.getCachedAttribute<juce::String> (Tags::Inlets, true)),
    outlets_ (object.getCachedAttribute<juce::String> (Tags::Outlets, true)),
    label_ (object.getCachedParameter<juce::String> (Tags::Label, true)),
    boxPinBackgroundColour_ (Spaghettis()->getCachedColour (Tags::BoxPinBackground)),
    boxSelectedColour_ (Spaghettis()->getCachedColour (Tags::BoxSelected)),
    painter_ (createPainter (this, object)),
    isRunView_ (View::isRunView (view_))
{
    jassert (view);
    
    setOpaque (true); setPaintingIsUnclipped (true);
    
    view_->addChildComponent (this);

    update();
    
    auto f = [this]() { update(); };
    
    visible_.attach (f);
    inlets_.attach (f);
    outlets_.attach (f);
    
    if (isInsideRunView()) { label_.attach (f); }
    else {
        x_.attach (f);
        y_.attach (f);
    }
    
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
    if (o.isPatch()) { v->getPatch().openSubPatchWindow (o); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ObjectComponent::mouseMove (const juce::MouseEvent&)
{
    if (!isInsideRunView()) {
    //
    //
    }
}

void ObjectComponent::mouseEnter (const juce::MouseEvent&)
{
    if (!isInsideRunView()) {
    //
    //
    }
}

void ObjectComponent::mouseExit (const juce::MouseEvent&)
{
    if (!isInsideRunView()) {
    //
    //
    }
}

void ObjectComponent::mouseDown (const juce::MouseEvent& e)
{
    if (!isInsideRunView()) {
    //
    if (Mouse::isCommandClick (e))    { painter_->mouseDown (e); }
    else {
    //
    // view_->dragStart();
    
    if (Mouse::isDoubleClick (e))     { openSubPatch (object_, view_);  }
    else if (Mouse::isShiftClick (e)) { setSelected (!selected_.get()); }
    else if (Mouse::isClick (e))      { setSelected (true); }
    //
    }
    //
    }
}

void ObjectComponent::mouseDrag (const juce::MouseEvent& e)
{
    if (!isInsideRunView()) {
    //

    //
    }
}

void ObjectComponent::mouseUp (const juce::MouseEvent&)
{
    if (!isInsideRunView()) {
    //
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ObjectComponent::dragStart()
{
    origin_ = juce::Point<int> (getPositionX(), getPositionY());
}

void ObjectComponent::drag (juce::Point<int> pt)
{
    // juce::Point<int> pt (e.getOffsetFromDragStart());
    
    // DBG (juce::String (pt.x) + " / " + juce::String (pt.y));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

core::UniqueId ObjectComponent::getIdentifier() const
{
    return object_.getIdentifier();
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

int ObjectComponent::getPositionX() const
{
    return x_.get();
}

int ObjectComponent::getPositionY() const
{
    return y_.get();
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
    }
}

bool ObjectComponent::isSelected() const
{
    return selected_.get();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ObjectComponent::snap()
{
    EditCommands::snap (object_.getIdentifier());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool ObjectComponent::isInsideRunView() const
{
    return isRunView_;
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
    const bool isVisible = isInsideRunView() ? (hasLabel() && visible_.get()) : visible_.get();
    
    removeInletsAndOultets();
    
    if (isVisible) {
        const juce::Rectangle<int> painted (painter_->getRequiredBounds());
        view_->show (this, view_->getBoundsFromPaintedArea (painted));
        if (!isInsideRunView()) {
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
    View* view,
    float scale,
    bool isOutlet)
{
    const int n = a.size();
    
    std::vector<std::unique_ptr<PinComponent>> pins;
    
    for (int i = 0; i < n; ++i) {
        std::unique_ptr<PinComponent> p = std::make_unique<PinComponent> (view, object, a[i]);
        p->setBounds (getPinBounds (bounds, i, scale, isOutlet));
        p->setVisible (true);
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
    
    if (!i.isEmpty()) { iPins_ = createPins (i, bounds, object_, view_, scale, false); }
    if (!o.isEmpty()) { oPins_ = createPins (o, bounds, object_, view_, scale, true);  }
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
