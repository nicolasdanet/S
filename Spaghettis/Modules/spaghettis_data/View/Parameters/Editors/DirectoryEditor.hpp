
/* Copyright (c) 2024 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class DirectoryEditor : public  juce::Component,
                        private juce::Value::Listener {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit DirectoryEditor (const juce::Value&);

    ~DirectoryEditor() = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void resized() override;
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void updateValue();
    void updateLabel();
    
private:
    void valueChanged (juce::Value&) override;

private:
    juce::Value value_;
    juce::Label label_;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DirectoryEditor)
};
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

