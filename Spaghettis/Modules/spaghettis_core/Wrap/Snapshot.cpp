
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
    
    if (length) {
    //
    jassert (n);

    const double increment = n / static_cast<double> (length);
    
    double i = 0.0;
    
    for (int j = start; j < end; ++j) {

        const double f = fetchValue (p, size, j);
        const int k    = static_cast<int> (i);
        
        jassert (k >= 0 && k < n);
        
        v_[k].set (f);
        
        i += increment;
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Snapshot::paintCompute()
{
    const int n = static_cast<int> (v_.size());
    
    int t = -1;
    
    for (int i = 0; i < n; ++i) {
        if (v_[i].isSet())  { v_[i].scale (range_, painted_); t = i; }
        else if (t >= 0)    { v_[t].enlarge(); }
    }
}

/* Collapse adjacent rectangles. */
/* Does it worth the cost? */

void Snapshot::paintCollapse()
{
    const int n = static_cast<int> (v_.size());
    
    int t = -1;
    
    for (int i = 0; i < n; ++i) {
        if (v_[i].isSet()) {
            if (t < 0 || !v_[t].collapse (v_[i])) {
                t = i;
            }
        }
    }
}

void Snapshot::paintProceed (juce::Graphics& g)
{
    const int n = static_cast<int> (v_.size());
    
    juce::RectangleList<float> list;
    
    for (int i = 0; i < n; ++i) {
        if (v_[i].isSet()) {
            list.addWithoutMerging (v_[i].getRectangle().translated (i, painted_.getY()));
        }
    }
    
    list.clipTo (painted_.toFloat());
    
    g.fillRectList (list);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Snapshot::paint (juce::Graphics& g)
{
    paintCompute();
    paintCollapse();
    paintProceed (g);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
