
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
    case Icons::zoomIn      : button->setTooltip (NEEDS_TRANS ("Zoom in"));                 break;
    case Icons::zoomOut     : button->setTooltip (NEEDS_TRANS ("Zoom out"));                break;
    case Icons::zoomReset   : button->setTooltip (NEEDS_TRANS ("Reset zoom"));              break;
    case Icons::help        : button->setTooltip (NEEDS_TRANS ("Hide or show help"));       break;
    case Icons::inspector   : button->setTooltip (NEEDS_TRANS ("Hide or show inspector"));  break;
    case Icons::edit        : button->setTooltip (NEEDS_TRANS ("Open an edit view"));       break;
    case Icons::run         : button->setTooltip (NEEDS_TRANS ("Open a run view"));         break;
    default                 : break;
    //
    }
    
    switch (button->getItemId()) {
    //
    case Icons::zoomIn      : button->onClick = [this]() { owner_->zoomIn();  }; break;
    case Icons::zoomOut     : button->onClick = [this]() { owner_->zoomOut(); }; break;
    case Icons::zoomReset   : button->onClick = []() { DBG ("?"); }; break;
    case Icons::help        : button->onClick = []() { DBG ("?"); }; break;
    case Icons::inspector   : button->onClick = []() { DBG ("?"); }; break;
    case Icons::edit        : button->onClick = [this]() {
                                    owner_->getPatch().openEditWindow();
                                }; break;
    case Icons::run         : button->onClick = [this]() {
                                    owner_->getPatch().openRunWindow();
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
