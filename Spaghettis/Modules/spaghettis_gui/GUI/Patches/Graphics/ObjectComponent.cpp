
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
    juce::Rectangle<int> painted (painter_->getBounds());
    
    setVisible (visible_.get());
    setBounds (showPins_ ? painted.expanded (0, PainterPolicy::pinHeight()) : painted);
    
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

std::vector<std::unique_ptr<PinComponent>> updatePins (const juce::StringArray& a,
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

void ObjectComponent::updateInletsAndOutlets()
{
    removeInletsAndOultets();
    
    if (showPins_ && visible_.get()) {
    //
    const juce::StringArray i (juce::StringArray::fromTokens (inlets_.get(), true));
    const juce::StringArray o (juce::StringArray::fromTokens (outlets_.get(), true));
    
    const juce::Rectangle<int> bounds (getBounds());
    
    if (!i.isEmpty()) { iPins_ = updatePins (i, bounds, owner_, false); }
    if (!o.isEmpty()) { oPins_ = updatePins (o, bounds, owner_, true);  }
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
