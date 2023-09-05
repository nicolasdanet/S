
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

RadioPainter::RadioPainter (ObjectComponent* owner) :
    PainterPolicy (owner),
    radioBackgroundColour_ (Spaghettis()->getCachedColour (Tag::RadioBackground)),
    radioButtonColour_ (Spaghettis()->getCachedColour (Tag::RadioButton)),
    value_ (object_.getCached<double> (Tag::Parameters, Tag::Value)),
    isVertical_ (object_.getCached<bool> (Tag::Parameters, Tag::Vertical)),
    isMultiple_ (object_.getCached<bool> (Tag::Parameters, Tag::Multiple)),
    buttons_ (object_.getCached<int> (Tag::Parameters, Tag::Buttons)),
    width_ (object_.getCached<int> (Tag::Parameters, Tag::Width))
{
    radioBackgroundColour_.attach (repaint (component_));
    radioButtonColour_.attach (repaint (component_));
    value_.attach (repaint (component_));
    isMultiple_.attach (repaint (component_));
    isVertical_.attach (resized (component_));
    buttons_.attach (resized (component_));
    width_.attach (resized (component_));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

juce::Rectangle<float> getSelectorPosition (const juce::Rectangle<int>& r, bool vertical, float width, int i)
{
    const float top     = r.getY();
    const float bottom  = r.getBottom();
    const float left    = r.getX();
    const float right   = r.getRight();
    
    if (vertical) {
        return juce::Rectangle<float> (left, (i * width) + top, right - left, width);
    } else {
        return juce::Rectangle<float> ((i * width) + left, top, width, bottom - top);
    }
}

void paintStateAt (const juce::Rectangle<int>& r, juce::Graphics& g, bool vertical, float width, int i)
{
    LookAndFeel::drawTick (g, getSelectorPosition (r, vertical, width, i).toNearestInt().reduced (3));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int RadioPainter::getSelectorAt (juce::Point<int> pt)
{
    if (painted_.contains (pt)) {
    //
    const bool v  = isVertical_.get();
    const float w = width_.get() * getScale();
    const int n   = buttons_.get();
    
    for (int i = 0; i < n; ++i) {
    //
    if (getSelectorPosition (painted_, v, w, i).contains (pt.toFloat())) { return i; }
    //
    }
    //
    }
    
    return -1;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void RadioPainter::setStateProceed (double f)
{
    Spaghettis()->handle (Inputs::sendObjectFloat (getIdentifier(), f));
}

void RadioPainter::setStateSingle (int n)
{
    setStateProceed (static_cast<double> (n));
}

void RadioPainter::setStateMultiple (int n)
{
    juce::BigInteger t (static_cast<int64_t> (value_.get()));
    
    t.setBit (n, !t[n]);
    
    setStateProceed (static_cast<double> (t.toInt64()));
}

void RadioPainter::setState (int n)
{
    if (n >= 0) {
        if (isMultiple_.get()) { setStateMultiple (n); }
        else {
            setStateSingle (n);
        }
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void RadioPainter::mouseDown (const juce::MouseEvent& e)
{
    setState (getSelectorAt (e.getMouseDownPosition()));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void RadioPainter::paintStateSingle (const juce::Rectangle<int>& r, juce::Graphics& g)
{
    const bool v  = isVertical_.get();
    const float w = width_.get() * getScale();
    const int n   = buttons_.get();
    
    const int t = juce::jlimit (0, n, static_cast<int> (value_.get()));
    
    for (int i = 0; i < n; ++i) {
        if (i == t) {
            paintStateAt (r, g, v, w, i);
        }
    }
}

void RadioPainter::paintStateMultiple (const juce::Rectangle<int>& r, juce::Graphics& g)
{
    const bool v  = isVertical_.get();
    const float w = width_.get() * getScale();
    const int n   = buttons_.get();
    
    const juce::BigInteger t (static_cast<int64_t> (value_.get()));

    for (int i = 0; i < n; ++i) {
        if (t[i]) {
            paintStateAt (r, g, v, w, i);
        }
    }
}

void RadioPainter::paintBackground (const juce::Rectangle<int>& r, juce::Graphics& g)
{
    const bool v  = isVertical_.get();
    const float w = width_.get() * getScale();
    const int n   = buttons_.get();
    
    for (int i = 1; i < n; ++i) {
    //
    const juce::Rectangle<float> t = getSelectorPosition (r, v, w, i);
    
    if (v) { g.fillRect (t.withHeight (1.0f).translated (0.0f, -0.5f)); }
    else {
        g.fillRect (t.withWidth (1.0f).translated (-0.5f, 0.0f));
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void RadioPainter::paintObject (juce::Rectangle<int> r, juce::Graphics& g)
{
    const juce::Colour c (radioBackgroundColour_.get());
    
    g.setColour (c);
    g.fillRect (r);
    g.setColour (c.contrasting (0.1f));
    
    paintBackground (r, g);
    
    g.setColour (radioButtonColour_.get());
    
    if (isMultiple_.get()) { paintStateMultiple (r, g); } else { paintStateSingle (r, g); }
    
    painted_ = r;
}

juce::Rectangle<int> RadioPainter::getRequiredBoundsForObject()
{
    return getRequiredBoundsForObjectFromDimensions();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

std::optional<core::Vector::Real> RadioPainter::getDimensions()
{
    const int w = width_.get();
    const int h = w * buttons_.get();
    
    if (isVertical_.get()) { return core::Vector::Real (w, h); }
    else {
        return core::Vector::Real (h, w);
    }
}

void RadioPainter::setDimensions (core::Vector::Real v)
{
    // setDimensionsByParameters (v);
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
