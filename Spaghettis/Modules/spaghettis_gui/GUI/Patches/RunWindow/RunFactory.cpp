
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Note that at this point the owner component is not fully constructed. */

void RunFactory::setToolbarButton (IconsButton* button)
{
    switch (button->getItemId()) {
    //
    case Icons::presets : button->setTooltip (NEEDS_TRANS ("Show or hide presets")); break;
    case Icons::edit    : button->setTooltip (NEEDS_TRANS ("Open an edit view"));    break;
    case Icons::run     : button->setTooltip (NEEDS_TRANS ("Open a run view"));      break;
    default             : break;
    //
    }
    
    switch (button->getItemId()) {
    //
    case Icons::presets : button->onClick = [this, button]() {
                                    if (button->getState()) { owner_->showPresets(); }
                                    else {
                                        owner_->hidePresets();
                                    }
                                }; break;
    case Icons::edit    : button->onClick = [this]() {
                                owner_->getPatchRoot().openEditWindow();
                            }; break;
    case Icons::run     : button->onClick = [this]() {
                                owner_->getPatchRoot().openRunWindow();
                            }; break;
    default             : break;
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
