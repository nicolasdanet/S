
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
    case Icons::help        : button->setTooltip (NEEDS_TRANS ("Hide or show help"));       break;
    case Icons::inspector   : button->setTooltip (NEEDS_TRANS ("Hide or show inspector"));  break;
    case Icons::edit        : button->setTooltip (NEEDS_TRANS ("Open edit view"));          break;
    case Icons::run         : button->setTooltip (NEEDS_TRANS ("Open run view"));           break;
    default                 : break;
    //
    }
    
    switch (button->getItemId()) {
    //
    case Icons::zoomIn      : button->onClick = []() { DBG ("Zoom In"); };      break;
    case Icons::zoomOut     : button->onClick = []() { DBG ("Zoom Out"); };     break;
    case Icons::help        : button->onClick = []() { DBG ("Help"); };         break;
    case Icons::inspector   : button->onClick = []() { DBG ("Inspector"); };    break;
    case Icons::edit        : button->onClick = []() { DBG ("Edit"); };         break;
    case Icons::run         : button->onClick = []() { DBG ("Run"); };          break;
    default                 : break;
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
