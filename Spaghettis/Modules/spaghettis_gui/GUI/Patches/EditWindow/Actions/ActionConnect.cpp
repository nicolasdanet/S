
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

ActionConnect::ActionConnect (EditView* view) : view_ (view)
{
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ActionConnect::createComponent (const juce::Point<int>& pt)
{
    connectComponent_ = std::make_unique<ActionConnectComponent> (pt);
    
    view_->addAndMakeVisible (connectComponent_.get());
}

void ActionConnect::updateComponent (const juce::Point<int>& pt)
{
    if (connectComponent_) { tracker_.hit (view_, pt); connectComponent_->set (pt); }
    else {
        tracker_.start (view_, pt); createComponent (pt);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ActionConnect::connect()
{
    const auto [o, i] = tracker_.getPins();
    
    if (o && i) {
    //
    const core::UniqueId u = o->getIdentifier();
    const core::UniqueId v = i->getIdentifier();
    const int m = o->getIndex();
    const int n = i->getIndex();
    
    DBG (juce::String (u) + " " + juce::String (m) + " / " + juce::String (v) + " " + juce::String (n));
    EditCommands::connect (u, m, v, n);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ActionConnect::mouseDrag (const juce::MouseEvent& e)
{
    updateComponent (e.getPosition());
}

void ActionConnect::mouseUp (const juce::MouseEvent& e)
{
    if (connectComponent_) {
        view_->removeChildComponent (connectComponent_.get());
        connect();
        connectComponent_ = nullptr;
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
