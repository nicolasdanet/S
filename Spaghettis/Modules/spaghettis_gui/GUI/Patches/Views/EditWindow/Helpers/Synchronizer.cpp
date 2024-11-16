
/* Copyright (c) 2023 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* ??? */

Synchronizer::Synchronizer (const core::Item& item) :
    source_ (item),
    data_ (data::Data::makeCopy (item.getData()))
{
    observer_ = nullptr;
}

Synchronizer::Synchronizer (Synchronizer&& o) : source_ (std::move (o.source_)), data_ (std::move (o.data_))
{
    observer_ = nullptr;            /* ValueTree listeners are removed on move. */
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

data::Data Synchronizer::getData() const
{
    return data_;
}

core::UniqueId Synchronizer::getIdentifier() const
{
    return source_.getIdentifier();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Synchronizer::bind (InspectorView* view)
{
    jassert (observer_ == nullptr);
    
    data_.addObserver (view); source_.addObserver (this);
    
    observer_ = view;
}

void Synchronizer::unbind (InspectorView* view)
{
    jassert (observer_ == view);
    
    source_.removeObserver (this); data_.removeObserver (view);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class ScopedNoObserver {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    ScopedNoObserver (data::Data& data, data::Observer* observer) : data_ (data), observer_ (observer)
    {
        data_.removeObserver (observer_);
    }
    
    ~ScopedNoObserver()
    {
        data_.addObserver (observer_);
    }
    
public:
    data::Data& data_;
    data::Observer* observer_;
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Called when source object is modified (e.g. from core commands). */

void Synchronizer::parameterHasChanged (const data::Group& group, const data::Parameter& parameter)
{
    ScopedNoObserver suspend (data_, observer_);        /* Break any possible feedback-loop. */
    
    data_.changeValue (group.getName(), parameter.getKey(), parameter.getValue());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
