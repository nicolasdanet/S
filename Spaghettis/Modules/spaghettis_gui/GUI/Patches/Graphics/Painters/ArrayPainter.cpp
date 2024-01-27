
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
    arrayBackgroundColour_ (Painted (Spaghettis()->getCachedColour (Tag::ArrayBackground), component_)),
    arrayValueColour_ (Painted (Spaghettis()->getCachedColour (Tag::ArrayValue), component_)),
    name_ (Painted (object_.getCached<juce::String> (Tag::Parameters, Tag::Name), component_)),
    width_ (Resized (object_.getCached<int> (Tag::Parameters, Tag::Width), component_)),
    height_ (Resized (object_.getCached<int> (Tag::Parameters, Tag::Height), component_)),
    size_ (Painted (object_.getCached<int> (Tag::Parameters, Tag::Size), component_)),
    count_ (Painted (object_.getCached<int> (Tag::Parameters, Tag::Count), component_))
{
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

void ArrayPainter::paintWidget (juce::Rectangle<int> r, juce::Graphics& g)
{
    g.setColour (arrayBackgroundColour_.get());
    g.fillRect (r);
    
    paintSignal (r, g);
}

juce::Rectangle<int> ArrayPainter::getRequiredBoundsForWidget()
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
