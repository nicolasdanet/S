
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
    SnapshotRange() : low_ (0.0), high_ (0.0), set_ (false), r_()
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
        low_  = juce::jmin (f, low_);
        high_ = juce::jmax (f, high_);
        set_  = true;
    }
    
    void draw (double offset, double valuePerPixel)
    {
        const int a = static_cast<int> ((offset - low_)  / valuePerPixel);
        const int b = static_cast<int> ((offset - high_) / valuePerPixel);
        
        r_ = juce::Rectangle<int> (juce::Point<int> (0, b), juce::Point<int> (1, a));
    }
    
    void expand()
    {
        r_.setWidth (r_.getWidth() + 1);
    }
    
    bool isSet() const
    {
        return set_;
    }
    
    juce::Rectangle<int> getRectangle() const
    {
        return r_;
    }
    
private:
    double low_;
    double high_;
    bool set_;

private:
    juce::Rectangle<int> r_;
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class Snapshot {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

friend class Snapshots;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit Snapshot (juce::Range<int>, juce::Range<double>, juce::Rectangle<int>);
    
    ~Snapshot() = default;

public:
    Snapshot (const Snapshot&) = default;
    Snapshot (Snapshot&&) = default;
    Snapshot& operator = (const Snapshot&) = default;
    Snapshot& operator = (Snapshot&&) = default;

public:
    void paint (juce::Graphics&);
    
private:
    void fetch (void*, int);
    
private:
    juce::Range<int> domain_;
    juce::Range<double> range_;
    juce::Rectangle<int> painted_;
    std::vector<SnapshotRange> v_;
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

private:
    JUCE_LEAK_DETECTOR (Snapshot)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
