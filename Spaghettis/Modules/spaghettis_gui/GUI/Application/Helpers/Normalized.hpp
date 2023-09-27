
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class Normalized {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit Normalized (bool isLogarithmic, double min, double max, double interval = 0.0) :
        isLogarithmic_ (isLogarithmic),
        r_ (min, max),
        interval_ (interval)
    {
    }
    
    ~Normalized() = default;

public:
    Normalized (const Normalized&) = delete;
    Normalized (Normalized&&) = delete;
    Normalized& operator = (const Normalized&) = delete;
    Normalized& operator = (Normalized&&) = delete;
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    double convertLinear (double v) const
    {
        const auto n = juce::NormalisableRange<double> (r_.getStart(), r_.getEnd(), interval_);

        return n.convertTo0to1 (n.snapToLegalValue (v));
    }

    double convertLogarithmic (double v) const
    {
        const auto n = juce::NormalisableRange<double> (r_.getStart(), r_.getEnd(), interval_, skew_);

        return n.convertTo0to1 (n.snapToLegalValue (v));
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    double mapLinear (double v) const
    {
        const auto n = juce::NormalisableRange<double> (r_.getStart(), r_.getEnd(), interval_);

        return n.snapToLegalValue (n.convertFrom0to1 (v));
    }

    double mapLogarithmic (double v) const
    {
        const auto n = juce::NormalisableRange<double> (r_.getStart(), r_.getEnd(), interval_, skew_);

        return n.snapToLegalValue (n.convertFrom0to1 (v));
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    double convert (double v) const
    {
        return isLogarithmic_ ? convertLogarithmic (v) : convertLinear (v);
    }
    
    double map (double v) const
    {
        return isLogarithmic_ ? mapLogarithmic (v) : mapLinear (v);
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

private:
    bool isLogarithmic_;
    juce::Range<double> r_;
    double interval_;
    
private:
    static constexpr double skew_ = 0.25;
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

