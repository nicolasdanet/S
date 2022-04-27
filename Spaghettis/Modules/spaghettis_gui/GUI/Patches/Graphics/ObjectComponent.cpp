
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

std::unique_ptr<PainterPolicy> createPainter (juce::Component& owner, const core::Object& object)
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

ObjectComponent::ObjectComponent (juce::Component& owner, const core::Object& object) :
    owner_ (owner),
    object_ (object),
    visible_ (object.getCachedAttribute<bool> (Tags::Visible, true)),
    inlets_ (object.getCachedAttribute<juce::String> (Tags::Inlets, true)),
    outlets_ (object.getCachedAttribute<juce::String> (Tags::Outlets, true)),
    background_ (Spaghettis()->getCachedColour (Tags::PinBackground)),
    painter_ (createPainter (*this, object)),
    showPins_ (true)
{
    setOpaque (true); setPaintingIsUnclipped (true);
    
    owner_.addChildComponent (this);

    update();
    
    auto f = [this]() { update(); };
    
    inlets_.attach  (f);
    outlets_.attach (f);
    visible_.attach (f);
    
    background_.attach (PainterPolicy::repainter (this));
}

ObjectComponent::~ObjectComponent()
{
    removeAllChangeListeners();
    removeInletsAndOultets();
    
    owner_.removeChildComponent (this);
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
        return nullptr;     /* If not visible or ill-formed (e.g. subpatch two-step creation). */
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
        
    if (showPins_) {
        g.setColour (background_.get());
        g.fillRect (bounds);
        painter_->paint (bounds.reduced (0, PainterPolicy::pinHeight()), g);
    } else {
        painter_->paint (bounds, g);
    }
}
    
void ObjectComponent::resized()
{

}

void ObjectComponent::update()
{
    const bool isVisible = visible_.get();
    
    removeInletsAndOultets();
    
    if (isVisible) {
        juce::Rectangle<int> painted (painter_->getBounds());
        setBounds (showPins_ ? painted.expanded (0, PainterPolicy::pinHeight()) : painted);
        setVisible (true);
        if (showPins_) { createInletsAndOutlets(); }
    } else {
        setVisible (false);
    }
    
    sendChangeMessage();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

juce::Rectangle<int> getPinBounds (juce::Rectangle<int> bounds, int index, bool isOutlet)
{
    const int x = bounds.getX() + (index * ((PainterPolicy::pinGripX() * 2) + PainterPolicy::pinWidth()));
    
    bounds.setX (x);
    bounds.setWidth (PainterPolicy::pinWidth());
    
    if (isOutlet) {
        bounds = bounds.removeFromBottom (PainterPolicy::pinHeight());
    } else {
        bounds = bounds.removeFromTop (PainterPolicy::pinHeight());
    }
    
    return bounds.expanded (PainterPolicy::pinGripX(), PainterPolicy::pinGripY());
}

std::vector<std::unique_ptr<PinComponent>> createPins (const juce::StringArray& a,
    const juce::Rectangle<int>& bounds,
    juce::Component& owner,
    bool isOutlet)
{
    const int n = a.size();
    
    std::vector<std::unique_ptr<PinComponent>> pins;
    
    for (int i = 0; i < n; ++i) {
        std::unique_ptr<PinComponent> p = std::make_unique<PinComponent> (owner, a[i], i);
        p->setBounds (getPinBounds (bounds, i, isOutlet));
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
    const juce::StringArray i (juce::StringArray::fromTokens (inlets_.get(), true));
    const juce::StringArray o (juce::StringArray::fromTokens (outlets_.get(), true));
    
    const juce::Rectangle<int> bounds (getBounds());
    
    if (!i.isEmpty()) { iPins_ = createPins (i, bounds, owner_, false); }
    if (!o.isEmpty()) { oPins_ = createPins (o, bounds, owner_, true);  }
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
