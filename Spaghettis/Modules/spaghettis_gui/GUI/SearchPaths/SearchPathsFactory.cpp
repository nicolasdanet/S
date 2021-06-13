
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -
 
/* Note that at this point the owner component is not fully constructed. */
 
void SearchPathsFactory::setToolbarButton (IconsButton* button)
{
    switch (button->getItemId()) {
    //
    case Icons::add         : button->onClick = [this]() { owner_->addPaths(); };               break;
    case Icons::remove      : button->onClick = [this]() { owner_->removeSelectedPaths(); };    break;
    case Icons::sortDown    : button->onClick = [this]() { owner_->sortPaths(); };              break;
    case Icons::sortUp      : button->onClick = [this]() { owner_->sortPaths (true); };         break;
    case Icons::synchronize : button->onClick = []() {
                                    Spaghettis()->handle (Inputs::rescan (Inputs::Logged::full));
                                }; break;
    default                 : break;
    //
    }
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
