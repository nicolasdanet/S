
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

class EditComponent;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class EditFactory : public IconsFactory {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit EditFactory (EditComponent* owner) : owner_ (owner)
    {
        jassertquiet (owner_);
    }
    
    ~EditFactory() = default;

public:
    void getAllToolbarItemIds (juce::Array<int>& ids) override
    {
        ids.add (Icons::edit);
        ids.add (Icons::run);
        // ids.add (ToolbarItemFactory::spacerId);
        ids.add (Icons::inspector);
    }

    void getDefaultItemSet (juce::Array<int>& ids) override
    {
        getAllToolbarItemIds (ids);
    }
    
    void setToolbarButton (IconsButton* button) override;

private:
    EditComponent* owner_;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EditFactory)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

using EditFactoryHelper = IconsFactoryHelper<EditFactory, EditComponent>;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
