
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
    backgroundColour_ (Spaghettis()->getCachedColour (Tags::PinBackground)),
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
    
    backgroundColour_.attach (PainterPolicy::repainter (this));
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
    const juce::Rectangle<int> bounds (getLocalBounds());
    
    if (showPins_) { g.setColour (backgroundColour_.get()); g.fillRect (bounds); }
    
    painter_->paint (getPainted (bounds), g);
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
    updateBounds (painter_->getBounds());
    updateInletsAndOutlets();
}

void ObjectComponent::updateBounds (const juce::Rectangle<int>& painted)
{
    setBounds (showPins_ ? painted.expanded (0, pinsHeight_ * 2) : painted);
}

juce::Rectangle<int> ObjectComponent::getPainted (const juce::Rectangle<int>& bounds) const
{
    return showPins_ ? bounds.reduced (0, pinsHeight_ * 2) : bounds;
}

void ObjectComponent::updateInletsAndOutlets()
{
    DBG ("? I/O");
}

void ObjectComponent::removeInletsAndOultets()
{
    DBG ("! I/O");
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
