
/* Copyright (c) 2021 Jojo and others. */

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
    case Icons::add         : button->setTooltip (NEEDS_TRANS ("Add folders"));         break;
    case Icons::sort        : button->setTooltip (NEEDS_TRANS ("Sort alphabetically")); break;
    case Icons::synchronize : button->setTooltip (NEEDS_TRANS ("Rescan"));              break;
    default                 : break;
    //
    }
    
    switch (button->getItemId()) {
    //
    case Icons::add         : button->onClick = [this]() { owner_->addPaths();  };               break;
    case Icons::sort        : button->onClick = [this]() { owner_->sortPaths(); };              break;
    case Icons::synchronize : button->onClick = []() {
                                    Spaghettis()->handle (Inputs::rescanSearchPaths (Inputs::Logged::base));
                                }; break;
    default                 : break;
    //
    }
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
