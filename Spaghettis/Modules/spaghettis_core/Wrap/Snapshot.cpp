
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

void Snapshot::paint (juce::Graphics& g)
{
    const double offset        = range_.getEnd();
    const double valuePerPixel = range_.getLength() / painted_.getHeight();
    
    const int n = static_cast<int> (v_.size());
    
    int t = 0;
    
    for (int i = 0; i < n; ++i) {
        if (v_[i].isSet())      { v_[i].draw (offset, valuePerPixel); t = i; }
        else if (v_[t].isSet()) { v_[t].expand(); }
    }
    
    for (auto& v : v_) {
        if (v.isSet()) {
            juce::Rectangle<int> r = v.getRectangle();
            DBG (r.toString());
        }
    }
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
