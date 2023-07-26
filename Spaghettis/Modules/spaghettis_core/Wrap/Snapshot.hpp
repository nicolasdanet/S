
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
    SnapshotRange() : low_ (0.0), high_ (0.0), set_ (false)
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
    
    bool isSet() const
    {
        return set_;
    }
    
    juce::Range<int> getScaled (juce::Range<double> range, juce::Rectangle<int> painted) const
    {
        const double offset        = range.getEnd();
        const double valuePerPixel = range.getLength() / painted.getHeight();
        
        const int a = static_cast<int> ((offset - low_)  / valuePerPixel);
        const int b = static_cast<int> ((offset - high_) / valuePerPixel);
        
        return juce::Range<int> (b, a).getIntersectionWith (juce::Range<int> (0, painted.getHeight()));
    }
    
private:
    double low_;
    double high_;
    bool set_;
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
