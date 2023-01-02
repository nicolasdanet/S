
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

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
    case Icons::edit        : button->setTooltip (NEEDS_TRANS ("Open an edit view"));       break;
    case Icons::run         : button->setTooltip (NEEDS_TRANS ("Open a run view"));         break;
    case Icons::inspector   : button->setTooltip (NEEDS_TRANS ("Hide or show inspector"));  break;
    default                 : break;
    //
    }
    
    switch (button->getItemId()) {
    //
    case Icons::edit        : button->onClick = [this]() {
                                    owner_->getPatchRoot().openEditWindow();
                                }; break;
    case Icons::run         : button->onClick = [this]() {
                                    owner_->getPatchRoot().openRunWindow();
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
