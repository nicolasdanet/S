
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -
 
/* Note that at this point the owner component is not fully constructed. */
 
void SearchPathsFactory::setToolbarButton (int itemId, bool isToggle, juce::ToolbarButton* button)
{
    switch (itemId) {
    //
    case Icons::add         : button->onClick = [this]() { owner_->addPaths(); };                       break;
    case Icons::remove      : button->onClick = [this]() { owner_->removeSelectedPaths(); };            break;
    case Icons::synchronize : button->onClick = []() { Spaghettis()->handle (Inputs::rescan (true)); }; break;
    default                 : break;
    //
    }
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
