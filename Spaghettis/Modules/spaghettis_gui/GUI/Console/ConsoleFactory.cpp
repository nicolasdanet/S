
/* Copyright (c) 2021 Nicolas Danet. */

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
    case Icons::clear       : button->onClick = [this]() { owner_->clear(); };      break;
    case Icons::restore     : button->onClick = [this]() { owner_->restore(); };    break;
    case Icons::message     : /* Falls through. */
    case Icons::error       : button->onClick = [this, button]() {
                                    if (button->getState() == false) { owner_->parse(); }
                                }; break;
    case Icons::autoscroll  : button->onClick = [this, button]() {
                                    if (button->getState()) { owner_->update(); }
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
