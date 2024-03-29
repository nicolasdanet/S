
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

class DevicesComponent;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class DevicesFactory : public IconsFactory {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit DevicesFactory (DevicesComponent*)
    {
    }
    
    ~DevicesFactory() = default;

public:
    void getAllToolbarItemIds (juce::Array<int>&) override
    {
    }

    void getDefaultItemSet (juce::Array<int>&) override
    {
    }
    
    void setToolbarButton (IconsButton*) override
    {
    }
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DevicesFactory)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

using DevicesFactoryHelper = IconsFactoryHelper<DevicesFactory, DevicesComponent>;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

