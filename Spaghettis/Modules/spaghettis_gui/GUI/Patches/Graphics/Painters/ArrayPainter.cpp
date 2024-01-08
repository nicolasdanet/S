
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

ArrayPainter::ArrayPainter (ObjectComponent* owner) :
    PainterStrategy (owner),
    arrayBackgroundColour_ (Spaghettis()->getCachedColour (Tag::ArrayBackground)),
    arrayValueColour_ (Spaghettis()->getCachedColour (Tag::ArrayValue)),
    name_ (object_.getCached<juce::String> (Tag::Parameters, Tag::Name)),
    width_ (object_.getCached<int> (Tag::Parameters, Tag::Width)),
    height_ (object_.getCached<int> (Tag::Parameters, Tag::Height)),
    size_ (object_.getCached<int> (Tag::Parameters, Tag::Size)),
    count_ (object_.getCached<int> (Tag::Parameters, Tag::Count))
{
    arrayBackgroundColour_.attach (repaint (component_));
    arrayValueColour_.attach (repaint (component_));
          
    name_.attach (repaint (component_));
    width_.attach (resized (component_));
    height_.attach (resized (component_));
    size_.attach (repaint (component_));
    
    count_.attach (repaint (component_));       /* Repaint when garray is updated. */
    
    /* Repaint from time to time to in case of missed (e.g. content changed from DSP). */
    /* Use random period to spread the draws. */
    
    startTimer (juce::Random::getSystemRandom().nextInt (juce::Range<int> (900, 1100)));
}

ArrayPainter::~ArrayPainter()
{
    stopTimer();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ArrayPainter::timerCallback()
{
    component_->repaint();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Range<int> ArrayPainter::getDomain() const
{
    return juce::Range<int> (0, size_.get());
}

juce::Range<double> ArrayPainter::getRange() const
{
    return juce::Range<double> (-1.0, 1.0);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ArrayPainter::paintSignal (juce::Rectangle<int> r, juce::Graphics& g)
{
    Snapshot t (Spaghettis()->getSnapshots().get (getIdentifier(), getDomain(), getRange(), r));
    
    g.setColour (arrayValueColour_.get());
    
    t.paint (g);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ArrayPainter::paintObject (juce::Rectangle<int> r, juce::Graphics& g)
{
    g.setColour (arrayBackgroundColour_.get());
    g.fillRect (r);
    
    paintSignal (r, g);
}

juce::Rectangle<int> ArrayPainter::getRequiredBoundsForObject()
{
    return getRequiredBoundsFromDimensions();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

std::optional<core::Vector::Real> ArrayPainter::getDimensions()
{
    const int w = width_.get();
    const int h = height_.get();
    
    return core::Vector::Real (w, h);
}

void ArrayPainter::setDimensions (core::Vector::Real v)
{
    setDimensionsByParameters (v);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
