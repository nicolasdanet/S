
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

class SearchPathsComponent;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class SearchPathsFactory : public IconsFactory {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    SearchPathsFactory (SearchPathsComponent* owner) : owner_ (owner)
    {
    }
    
    ~SearchPathsFactory() = default;

public:
    void getAllToolbarItemIds (juce::Array<int>& ids) override
    {
        ids.add (Icons::add);
        ids.add (Icons::remove);
        ids.add (Icons::synchronize);
        ids.add (ToolbarItemFactory::spacerId);
        ids.add (Icons::sortDown);
        ids.add (Icons::sortUp);
    }

    void getDefaultItemSet (juce::Array<int>& ids) override
    {
        getAllToolbarItemIds (ids);
    }
    
    void setToolbarButton (int itemId, bool isToggle, juce::ToolbarButton* button) override;

private:
    SearchPathsComponent* owner_;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SearchPathsFactory)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

using SearchPathsFactoryHelper = IconsFactoryHelper<SearchPathsFactory, SearchPathsComponent>;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

