
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Sync::Sync (const core::Item& item) : source_ (item), data_ (Documentation::getCopyOfDataExtended (item))
{
    observer_ = nullptr;
}

Sync::Sync (Sync&& o) : source_ (std::move (o.source_)), data_ (std::move (o.data_))
{
    observer_ = nullptr;            /* ValueTree listeners are removed on move. */
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

core::Data Sync::getData() const
{
    return data_;
}

core::UniqueId Sync::getIdentifier() const
{
    return source_.getIdentifier();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Sync::bind (InspectorView* view)
{
    jassert (observer_ == nullptr);
    
    data_.addObserver (view); source_.addObserver (this);
    
    observer_ = view;
}

void Sync::unbind (InspectorView* view)
{
    jassert (observer_ == view);
    
    source_.removeObserver (this); data_.removeObserver (view);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Sync::parameterHasChanged (const core::Group& group, const core::Parameter& parameter)
{
    data_.removeObserver (observer_);
    data_.changeValue (group.getName(), parameter.getKey(), parameter.getValue());
    data_.addObserver (observer_);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
