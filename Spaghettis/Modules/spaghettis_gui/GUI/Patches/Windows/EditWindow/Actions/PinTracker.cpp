
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PinComponent* getPinComponentAt (EditView* view, core::Point::Scaled pt)
{
    return dynamic_cast<PinComponent*> (view->getComponentAt (pt.getPoint()));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void unsetPointer (WeakPointer<PinComponent>& p)
{
    if (p.getComponent()) { p->setHighlighted (false); p = nullptr; }
}

void setPointer (WeakPointer<PinComponent>& p, PinComponent* pin)
{
    unsetPointer (p); p = pin; if (p.getComponent()) { p->setHighlighted (true); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool checkDisallow (PinComponent* source, PinComponent* pin)
{
    jassert (source);
    jassert (pin);
    
    const bool sOutlet = source->isOutlet();
    const bool sSignal = source->isSignal();
    const bool dOutlet = pin->isOutlet();
    const bool dSignal = pin->isSignal();
    
    if (sOutlet == dOutlet)                              { return true; }
    if (sOutlet && sSignal && !dSignal)                  { return true; }
    if (source->getIdentifier() == pin->getIdentifier()) { return true; }
    
    return false;
}

void check (PinComponent* pin, WeakPointer<PinComponent>& source, WeakPointer<PinComponent>& destination)
{
    if (destination.getComponent() != pin) {
        if (pin && checkDisallow (source.getComponent(), pin)) { setPointer (destination, nullptr); }
        else {
            setPointer (destination, pin);
        }
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PinTracker::PinTracker()
{
}

PinTracker::~PinTracker()
{
    unsetPointer (destination_);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void PinTracker::start (EditView* view, core::Point::Scaled pt)
{
    source_ = getPinComponentAt (view, pt);
}

void PinTracker::hit (EditView* view, core::Point::Scaled pt)
{
    if (source_.getComponent()) {
    //
    PinComponent* pin = getPinComponentAt (view, pt);
    
    if (pin != source_.getComponent()) { check (pin, source_, destination_); }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

std::tuple<PinComponent*, PinComponent*> PinTracker::getPins() const
{
    PinComponent* s = source_.getComponent();
    PinComponent* d = destination_.getComponent();

    if (s && !s->isOutlet()) { std::swap (s, d); }
    
    return { s, d };
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------