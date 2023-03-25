
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Note that at this point the owner component is not fully constructed. */

void ConsoleFactory::setToolbarButton (IconsButton* button)
{
    switch (button->getItemId()) {
    //
    case Icons::clear       : button->setTooltip (NEEDS_TRANS ("Clear logs"));          break;
    case Icons::restore     : button->setTooltip (NEEDS_TRANS ("Restore logs"));        break;
    case Icons::error       : button->setTooltip (NEEDS_TRANS ("Show errors"));         break;
    case Icons::message     : button->setTooltip (NEEDS_TRANS ("Show messages"));       break;
    case Icons::find        : button->setTooltip (NEEDS_TRANS ("Locate sender"));       break;
    case Icons::autoscroll  : button->setTooltip (NEEDS_TRANS ("Enable autoscroll"));   break;
    default                 : break;
    //
    }
    
    switch (button->getItemId()) {
    //
    case Icons::clear       : button->onClick = [this]() { owner_->clear(); };   break;
    case Icons::restore     : button->onClick = [this]() { owner_->restore(); }; break;
    case Icons::error       : /* Falls through. */
    case Icons::message     : button->onClick = [this, button]() {
                                    if (button->getState()) { owner_->restore(); }
                                    else {
                                        owner_->parse();
                                    }
                                }; break;
    case Icons::find        : button->onClick = [this]() { owner_->localize(); }; break;
    case Icons::autoscroll  : button->onClick = [this, button]() {
                                    if (button->getState()) { owner_->update(); }
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
