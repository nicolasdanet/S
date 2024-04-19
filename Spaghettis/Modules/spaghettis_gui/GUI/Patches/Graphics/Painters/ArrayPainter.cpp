
/* Copyright (c) 2023 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

ArrayPainter::ArrayPainter (ObjectComponent* owner) :
    PainterStrategy (owner),
    arrayBackgroundColour_ (Painted (Spaghettis()->getCachedColour (Tag::ArrayBackground), getOwner())),
    arrayValueColour_ (Painted (Spaghettis()->getCachedColour (Tag::ArrayValue), getOwner())),
    name_ (Painted (getObject().getCached<juce::String> (Tag::Parameters, Tag::Name), getOwner())),
    width_ (Resized (getObject().getCached<int> (Tag::Parameters, Tag::Width), getOwner())),
    height_ (Resized (getObject().getCached<int> (Tag::Parameters, Tag::Height), getOwner())),
    size_ (Painted (getObject().getCached<int> (Tag::Parameters, Tag::Size), getOwner())),
    count_ (Painted (getObject().getCached<int> (Tag::Parameters, Tag::Count), getOwner()))
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
    getOwner()->repaint();
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
    return PainterHelpers::getRequiredBoundsFromDimensions (*this);
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
    PainterHelpers::setDimensionsByParameters (*this, v);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
