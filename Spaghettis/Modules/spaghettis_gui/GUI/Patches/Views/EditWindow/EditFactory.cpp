
/* Copyright (c) 2021 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Note that at this point the owner component is not fully constructed. */

void EditFactory::setToolbarButton (IconsButton* button)
{
    switch (button->getItemId()) {
    //
    case Icons::edit        : button->setTooltip (NEEDS_TRANS ("Open the main edit view")); break;
    case Icons::run         : button->setTooltip (NEEDS_TRANS ("Open the run view"));       break;
    case Icons::inspector   : button->setTooltip (NEEDS_TRANS ("Show or hide inspector"));  break;
    default                 : break;
    //
    }
    
    switch (button->getItemId()) {
    //
    case Icons::edit        : button->onClick = [this]() {
                                    owner_->getPatchRoot().openMainEditWindow();
                                }; break;
    case Icons::run         : button->onClick = [this]() {
                                    owner_->getPatchRoot().openMainRunWindow();
                                }; break;
    case Icons::inspector   : button->onClick = [this, button]() {
                                    if (button->getState()) { owner_->showInspector(); }
                                    else {
                                        owner_->hideInspector();
                                    }
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
