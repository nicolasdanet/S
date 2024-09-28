
/* Copyright (c) 2023 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

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

void ActionConnect::createComponent (core::Point::Scaled pt)
{
    connectComponent_ = std::make_unique<ActionConnectComponent> (pt);
    
    view_->addAndMakeVisible (connectComponent_.get());
}

void ActionConnect::updateComponent (core::Point::Scaled pt)
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
    
    Broadcast::connect (u, m, v, n);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ActionConnect::mouseDrag (const juce::MouseEvent& e)
{
    updateComponent (core::Point::Scaled (e.getPosition(), view_->getScale()));
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
