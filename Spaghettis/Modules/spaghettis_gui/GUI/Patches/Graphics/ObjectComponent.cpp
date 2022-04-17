
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
    visible_ (object.getCachedAttribute<bool> (Tags::Visible)),
    inlets_ (object.getCachedAttribute<juce::String> (Tags::Inlets)),
    outlets_ (object.getCachedAttribute<juce::String> (Tags::Outlets)),
    background_ (Spaghettis()->getCachedColour (Tags::PinBackground)),
    painter_ (createPainter (*this, object)),
    showPins_ (true)
{
    setOpaque (true); setPaintingIsUnclipped (true);
    
    owner_.addChildComponent (this);

    update();
    
    auto f = [this]() { update(); };
    
    visible_.attach (f);
    inlets_.attach  (f);
    outlets_.attach (f);
    
    background_.attach (PainterPolicy::repainter (this));
}

ObjectComponent::~ObjectComponent()
{
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

void ObjectComponent::paint (juce::Graphics& g)
{
    if (showPins_) { g.setColour (background_.get()); g.fillRect (getLocalBounds()); }
    
    painter_->paint (getPaintedBounds(), g);
}
    
void ObjectComponent::resized()
{

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ObjectComponent::update()
{
    setVisible (visible_.get());
    updateBounds();
    updateInletsAndOutlets();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

juce::Rectangle<int> getPinBounds (juce::Rectangle<int> bounds, int index, bool isOutlet)
{
    const int x = bounds.getX() + (index * (PainterPolicy::pinSpace() + PainterPolicy::pinWidth()));
    
    bounds.setX (x);
    bounds.setWidth (PainterPolicy::pinWidth());
    
    if (isOutlet) { return bounds.removeFromBottom (PainterPolicy::pinHeight()); }
    else {
        return bounds.removeFromTop (PainterPolicy::pinHeight());
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ObjectComponent::updateBounds()
{
    juce::Rectangle<int> painted (painter_->getBounds());
    
    setBounds (showPins_ ? painted.expanded (0, PainterPolicy::pinHeight()) : painted);
}

juce::Rectangle<int> ObjectComponent::getPaintedBounds() const
{
    const juce::Rectangle<int> bounds (getLocalBounds());
    
    return showPins_ ? bounds.reduced (0, PainterPolicy::pinHeight()) : bounds;
}

juce::Rectangle<int> ObjectComponent::getInletBounds (int index) const
{
    return getPinBounds (getBounds(), index, false);
}

juce::Rectangle<int> ObjectComponent::getOutletBounds (int index) const
{
    return getPinBounds (getBounds(), index, true);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/*
std::vector<std::unique_ptr<PinComponent>> ObjectComponent::updatePins (const juce::StringArray& a, bool b)
{
    const juce::Rectangle<int> bounds (getBounds());
    
    const int n = a.size();
    
    std::vector<std::unique_ptr<PinComponent>> pins;
    
    for (int i = 0; i < n; ++i) {
        std::unique_ptr<PinComponent> p = std::make_unique<PinComponent> (owner_, a[i], i);
        p->setBounds (getPinBounds (bounds, i, b));
        p->setVisible (true);
        pins.push_back (std::move (p));
    }
    
    return pins;
}
*/

void ObjectComponent::updateInlets (const juce::StringArray& a)
{
    const int n = a.size();
    
    for (int i = 0; i < n; ++i) {
        std::unique_ptr<PinComponent> p = std::make_unique<PinComponent> (owner_, a[i], i);
        p->setBounds (getInletBounds (i));
        p->setVisible (true);
        iPins_.push_back (std::move (p));
    }
}

void ObjectComponent::updateOutlets (const juce::StringArray& a)
{
    const int n = a.size();
    
    for (int i = 0; i < n; ++i) {
        std::unique_ptr<PinComponent> p = std::make_unique<PinComponent> (owner_, a[i], i);
        p->setBounds (getOutletBounds (i));
        p->setVisible (true);
        oPins_.push_back (std::move (p));
    }
}

void ObjectComponent::updateInletsAndOutlets()
{
    removeInletsAndOultets();
    
    if (showPins_ && visible_.get()) {
    //
    const juce::StringArray i (juce::StringArray::fromTokens (inlets_.get(), true));
    const juce::StringArray o (juce::StringArray::fromTokens (outlets_.get(), true));
    
    if (!i.isEmpty()) { updateInlets (i);  }
    if (!o.isEmpty()) { updateOutlets (o); }
    //
    }
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
