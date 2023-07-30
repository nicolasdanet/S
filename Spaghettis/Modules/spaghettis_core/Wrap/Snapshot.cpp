
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

Snapshot::Snapshot (juce::Range<int> domain, juce::Range<double> range, juce::Rectangle<int> painted) :
    domain_ (domain),
    range_ (range),
    painted_ (painted),
    v_ (painted.getWidth())
{
    jassert (!domain_.isEmpty());
    jassert (!range_.isEmpty());
    jassert (painted_.getWidth()  > 0);
    jassert (painted_.getHeight() > 0);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

double fetchValue (void* p, int size, int i)
{
    core::t_word* w = static_cast<core::t_word*> (p);
    
    if (i < 0 || i >= size) { return 0.0; }

    return core::w_getFloat (w + i);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Note that a mutex is hold during that call. */

void Snapshot::fetch (void* p, int size)
{
    const int start  = domain_.getStart();
    const int end    = domain_.getEnd();
    const int length = domain_.getLength();
    const int n      = static_cast<int> (v_.size());
    
    jassert (n && length);

    const double increment = n / static_cast<double> (length);
    
    double i = 0.0;
    
    for (int j = start; j < end; ++j) {

        const double f = fetchValue (p, size, j);
        const int k    = static_cast<int> (i);
        
        jassert (k >= 0 && k < n);
        
        v_[k].set (f);
        
        i += increment;
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

juce::Range<int> getRectangleRange (SnapshotRange s, juce::Range<double> range, juce::Rectangle<int> painted)
{
    const double offset        = range.getEnd();
    const double valuePerPixel = range.getLength() / painted.getHeight();
    
    const int a = static_cast<int> ((offset - s.getLow())  / valuePerPixel);
    const int b = static_cast<int> ((offset - s.getHigh()) / valuePerPixel);
    
    return juce::Range<int> (b, a).getIntersectionWith (juce::Range<int> (0, painted.getHeight()));
}

juce::Rectangle<float> getRectangle (SnapshotRange s, juce::Range<double> range, juce::Rectangle<int> painted)
{
    const juce::Range<int> r   = getRectangleRange (s, range, painted);
    const juce::Point<float> a = juce::Point<float> (0, r.getStart());
    const juce::Point<float> b = juce::Point<float> (s.getWidth(), r.getEnd());
    
    return juce::Rectangle<float> (a, b);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Snapshot::paint (juce::Graphics& g)
{
    const int n = static_cast<int> (v_.size());
    
    int t = 0;
    
    for (int i = 0; i < n; ++i) {
        if (v_[i].isSet()) { t = i; }
        else {
            v_[t].enlarge();
        }
    }
    
    juce::RectangleList<float> r;
    
    for (int i = 0; i < n; ++i) {
        if (v_[i].isSet()) { DBG (getRectangle (v_[i], range_, painted_).withX (i).toString()); }
    }
    
    // fillRectList
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
