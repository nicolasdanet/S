
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

void ObjectComponent::updateBounds()
{
    const juce::Rectangle<int> painted (painter_->getBounds());
    
    setBounds (showPins_ ? painted.expanded (0, pinsHeight_ * 2) : painted);
}

juce::Rectangle<int> ObjectComponent::getPaintedBounds() const
{
    const juce::Rectangle<int> bounds (getLocalBounds());
    
    return showPins_ ? bounds.reduced (0, pinsHeight_ * 2) : bounds;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ObjectComponent::updateInlets (const juce::StringArray& a)
{
    const int n = a.size();
    
    for (int i = 0; i < n; ++i) {
        //std::unique_ptr<PinComponent> p = std::make_unique<PinComponent> (owner_, a[i], i);
        //iPins.push_back (std::move (p));
    }
}

void ObjectComponent::updateInletsAndOutlets()
{
    removeInletsAndOultets();
    
    if (showPins_) {
    //
    juce::StringArray i (juce::StringArray::fromTokens (inlets_.get(), true));
    juce::StringArray o (juce::StringArray::fromTokens (outlets_.get(), true));
    
    if (!i.isEmpty()) { updateInlets (i); }
    if (!o.isEmpty()) { }
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
