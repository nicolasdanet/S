
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis::core {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class Observer : public juce::ValueTree::Listener {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class Handler {
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit Handler (const juce::String& key, std::function<void (const Parameter&)> f) : key_ (key), f_ (f)
    {
    }
    
    ~Handler() = default;

public:
    Handler (const Handler&) = default;
    Handler (Handler&&) = default;
    Handler& operator = (const Handler&) = default;
    Handler& operator = (Handler&&) = default;
    
public:
    juce::String key_;
    std::function<void (const Parameter&)> f_;
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

public:
    Observer()  = default;
    ~Observer() = default;

public:
    Observer (const Observer&) = default;
    Observer (Observer&&) = default;
    Observer& operator = (const Observer&) = default;
    Observer& operator = (Observer&&) = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void valueTreePropertyChanged (juce::ValueTree&, const juce::Identifier&) override;
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void addParameterHandler (const juce::String& key, std::function<void (const Parameter&)> f);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void callParameterHandlers (const Parameter&);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    virtual void parameterHasChanged (const core::Group&, const core::Parameter&)
    {
    }
    
private:
    std::vector<Handler> handlers_;

private:
    JUCE_LEAK_DETECTOR (Observer)
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------