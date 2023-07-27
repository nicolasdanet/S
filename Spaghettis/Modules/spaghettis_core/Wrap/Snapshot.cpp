
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
    const int n = static_cast<int> (v_.size());
    
    std::vector<juce::Rectangle<int>> t;
    
    for (int i = 0; i < n; ++i) {
        if (v_[i].isSet()) {
            const juce::Range<int> r = v_[i].getScaled (range_, painted_);
            const juce::Point<int> a = juce::Point<int> (i, r.getStart());
            const juce::Point<int> b = juce::Point<int> (i + 1, r.getEnd());
            t.emplace_back (a, b);
        } else if (!t.empty()) {
            t.back().setWidth (t.back().getWidth() + 1);
        }
    }
    
    for (auto rect : t) { DBG (rect.toString()); }
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
