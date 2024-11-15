
/* Copyright (c) 2022 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void moveAllPinsFront (ObjectComponent *c,
        std::vector<std::unique_ptr<PinComponent>>& iPins,
        std::vector<std::unique_ptr<PinComponent>>& oPins)
{
    for (const auto& i : iPins) { i->toBehind (c); }
    for (const auto& o : oPins) { o->toBehind (c); }
    
    if (!iPins.empty())      { c->toBehind (iPins.front().get()); }
    else if (!oPins.empty()) { c->toBehind (oPins.front().get()); }
}

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

juce::String getPinTooltip (const data::Data& d, const juce::String& type, bool isOutlet, int i)
{
    const juce::String k = (isOutlet ? Tag::Outlet : Tag::Inlet) + juce::String (i);
    
    if (d.hasParameter (Tag::Documentation, k)) {
    //
    return d.getParameter (Tag::Documentation, k).getValueTyped<TextBlock>().toString();
    //
    }
    
    return Strings::firstLetterCapitalized (type);
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

void ObjectComponent::createInletsAndOutlets()
{
    const float scale = getView()->getScale();
        
    const juce::StringArray i (juce::StringArray::fromTokens (inlets_.get(), true));
    const juce::StringArray o (juce::StringArray::fromTokens (outlets_.get(), true));
    
    const juce::Rectangle<int> bounds (getBounds());
    
    const data::Data documentation (Spaghettis()->getDocumentation().get (object_));
    
    if (!i.isEmpty()) { iPins_ = createPins (i, bounds, object_, documentation, getView(), scale, false); }
    if (!o.isEmpty()) { oPins_ = createPins (o, bounds, object_, documentation, getView(), scale, true);  }
    
    moveAllPinsFront (this, iPins_, oPins_);
}

void ObjectComponent::removeInletsAndOultets()
{
    iPins_.clear();
    oPins_.clear();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ObjectComponent::updateLabel()
{
    if (isInsideRunView() && isIncluded()) { getRunView()->updatePresets(); }
}

void ObjectComponent::updateTooltip()
{
    setTooltip (label_.get());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ObjectComponent::update (bool notify)
{
    const bool isRunView = isInsideRunView();
    const bool isVisible = isRunView ? (isIncluded() && visible_.get()) : visible_.get();
    
    removeInletsAndOultets();
    
    if (isVisible) {
        getView()->show (this, painter_->getRequiredBounds());
        if (!isRunView) {
            createInletsAndOutlets();
        }
    } else {
        getView()->hide (this);
    }
    
    updateTooltip();
    
    if (notify) { sendChangeMessage(); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
