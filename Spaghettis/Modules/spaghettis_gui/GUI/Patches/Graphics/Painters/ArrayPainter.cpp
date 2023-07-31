
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

ArrayPainter::ArrayPainter (ObjectComponent* owner) :
    PainterPolicy (owner),
    arrayBackgroundColour_ (Spaghettis()->getCachedColour (Tag::ArrayBackground)),
    arrayValueColour_ (Spaghettis()->getCachedColour (Tag::ArrayValue)),
    name_ (object_.getCached<juce::String> (Tag::Parameters, Tag::Name)),
    width_ (object_.getCached<int> (Tag::Parameters, Tag::Width)),
    height_ (object_.getCached<int> (Tag::Parameters, Tag::Height)),
    size_ (object_.getCached<int> (Tag::Parameters, Tag::Size)),
    start_ (object_.getCached<int> (Tag::Parameters, Tag::Start)),
    end_ (object_.getCached<int> (Tag::Parameters, Tag::End)),
    low_ (object_.getCached<double> (Tag::Parameters, Tag::Low)),
    high_ (object_.getCached<double> (Tag::Parameters, Tag::High))
{
    arrayBackgroundColour_.attach (repaint (component_));
    arrayValueColour_.attach (repaint (component_));
            
    width_.attach (resized (component_));
    height_.attach (resized (component_));
    
    name_.attach (repaint (component_));
    size_.attach (repaint (component_));
    start_.attach (repaint (component_));
    end_.attach (repaint (component_));
    low_.attach (repaint (component_));
    high_.attach (repaint (component_));
    
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
    const juce::Range<int> r = juce::Range<int> (start_.get(), end_.get());
    
    return r.isEmpty() ? juce::Range<int> (0, size_.get()) : r;
}

juce::Range<double> ArrayPainter::getRange() const
{
    const juce::Range<double> r = juce::Range<double> (low_.get(), high_.get());
    
    return r.isEmpty() ? juce::Range<double> (-1.0, 1.0) : r;
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
    return getRequiredBoundsForObjectFromDimensions();
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
