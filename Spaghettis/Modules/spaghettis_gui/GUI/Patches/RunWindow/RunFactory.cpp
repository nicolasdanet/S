
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
    case Icons::presets : button->setTooltip (NEEDS_TRANS ("Hide or show presets"));    break;
    case Icons::edit    : button->setTooltip (NEEDS_TRANS ("Open edit view"));          break;
    case Icons::run     : button->setTooltip (NEEDS_TRANS ("Open run view"));           break;
    default             : break;
    //
    }
    
    switch (button->getItemId()) {
    //
    case Icons::presets : button->onClick = []() { DBG ("Presets"); };  break;
    case Icons::edit    : button->onClick = []() { DBG ("Edit"); };     break;
    case Icons::run     : button->onClick = []() { DBG ("Run"); };      break;
    default             : break;
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
