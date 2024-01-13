
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

class SnapshotRange {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    SnapshotRange() : low_ (0.0), high_ (0.0), width_ (0)
    {
    }
    
    ~SnapshotRange() = default;

public:
    SnapshotRange (const SnapshotRange&) = default;
    SnapshotRange (SnapshotRange&&) = default;
    SnapshotRange& operator = (const SnapshotRange&) = default;
    SnapshotRange& operator = (SnapshotRange&&) = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void set (double f)
    {
        low_    = juce::jmin (f, low_);
        high_   = juce::jmax (f, high_);
        width_  = 1;
    }

    void enlarge()
    {
        ++width_;
    }
    
    void scale (juce::Range<double> range, juce::Rectangle<int> painted)
    {
        const double offset        = range.getEnd();
        const double valuePerPixel = range.getLength() / painted.getHeight();
        
        const double a = (offset - high_) / valuePerPixel;
        const double b = (offset - low_)  / valuePerPixel;
        
        r_ = juce::Range<int> (static_cast<int> (std::round (a)), static_cast<int> (std::round (b)));
    }
    
    bool collapse (SnapshotRange& s)
    {
        if (r_ == s.r_) { width_ += s.width_; s.width_ = 0; return true; }
        
        return false;
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    juce::Rectangle<float> getRectangle() const
    {
        juce::Point<float> a = juce::Point<float> (0, r_.getStart());
        juce::Point<float> b = juce::Point<float> (getWidth(), r_.getEnd());
    
        if (high_ == 0.0) { a = a.translated (0.0, -0.5f); }
        if (low_  == 0.0) { b = b.translated (0.0,  0.5f); }
        
        return juce::Rectangle<float> (a, b);
    }

    int getWidth() const
    {
        return width_;
    }
    
    bool isSet() const
    {
        return (width_ > 0);
    }
    
private:
    double low_;
    double high_;
    int width_;

private:
    juce::Range<int> r_;
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
