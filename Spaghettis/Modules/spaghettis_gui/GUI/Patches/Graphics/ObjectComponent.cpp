
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
        return std::make_unique<BoxPainter> (owner, object);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

ObjectComponent::ObjectComponent (EditView* view, const core::Object& object) :
    view_ (view),
    object_ (object),
    visible_ (object.getCachedAttribute<bool> (Tags::Visible, true)),
    inlets_ (object.getCachedAttribute<juce::String> (Tags::Inlets, true)),
    outlets_ (object.getCachedAttribute<juce::String> (Tags::Outlets, true)),
    backgroundColour_ (Spaghettis()->getCachedColour (Tags::PinBackground)),
    painter_ (createPainter (this, object)),
    showPins_ (true)
{
    jassert (view);
    
    setOpaque (true); setPaintingIsUnclipped (true);
    
    view_->addChildComponent (this);

    update();
    
    auto f = [this]() { update(); };
    
    inlets_.attach  (f);
    outlets_.attach (f);
    visible_.attach (f);
    
    backgroundColour_.attach (PainterPolicy::repaint (this));
}

ObjectComponent::~ObjectComponent()
{
    removeAllChangeListeners();
    removeInletsAndOultets();
    
    view_->removeChildComponent (this);
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
    
    g.setColour (backgroundColour_.get());
    g.fillRect (bounds);
    
    painter_->paint (showPins_ ? bounds.reduced (0, PainterPolicy::pinHeight (getScale())) : bounds, g);
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

void ObjectComponent::update (bool notify)
{
    const bool isVisible = visible_.get();
    
    removeInletsAndOultets();
    
    if (isVisible) {
        const juce::Rectangle<int> painted (painter_->getRequiredBounds().toNearestInt());
        setBounds (showPins_ ? painted.expanded (0, PainterPolicy::pinHeight (getScale())) : painted);
        setVisible (true);
        if (showPins_) { createInletsAndOutlets(); }
    } else {
        setVisible (false);
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
    const int offset = index * ((PainterPolicy::pinGripX (f) * 2) + PainterPolicy::pinWidth (f));
    const int x = bounds.getX() + offset;
    
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
    EditView* view,
    float scale,
    bool isOutlet)
{
    const int n = a.size();
    
    std::vector<std::unique_ptr<PinComponent>> pins;
    
    for (int i = 0; i < n; ++i) {
        std::unique_ptr<PinComponent> p = std::make_unique<PinComponent> (view, a[i]);
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

void ObjectComponent::createInletsAndOutlets()
{
    const float scale = getScale();
        
    const juce::StringArray i (juce::StringArray::fromTokens (inlets_.get(), true));
    const juce::StringArray o (juce::StringArray::fromTokens (outlets_.get(), true));
    
    const juce::Rectangle<int> bounds (getBounds());
    
    if (!i.isEmpty()) { iPins_ = createPins (i, bounds, view_, scale, false); }
    if (!o.isEmpty()) { oPins_ = createPins (o, bounds, view_, scale, true);  }
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
