
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -
 
void SearchPathsFactory::setCallback (int itemId, juce::ToolbarButton* button)
{
    if (itemId == Icons::add) {
        button->onClick = [p = juce::Component::SafePointer<SearchPathsComponent> (owner_)]() {
            if (SearchPathsComponent* c = p.getComponent()) { c->addPaths(); }
        };

    } else if (itemId == Icons::remove) {
        button->onClick = [p = juce::Component::SafePointer<SearchPathsComponent> (owner_)]() {
            if (SearchPathsComponent* c = p.getComponent()) { c->removeSelectedPaths(); }
        };
    }
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
