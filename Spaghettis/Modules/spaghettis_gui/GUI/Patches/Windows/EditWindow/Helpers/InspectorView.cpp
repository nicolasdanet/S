
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

InspectorView::InspectorView (Sync&& s, int w) :
    ParameterView (s.getData(),
        ParameterBase (Fonts::getFont(), Fonts::getMonospacedFont(), w)),
    sync_ (std::move (s))
{
    sync_.bind (this);
}

InspectorView::~InspectorView()
{
    sync_.unbind (this);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void InspectorView::parameterHasChanged (const data::Group& group, const data::Parameter& parameter)
{
    if (group.getName() == Tag::Parameters) {
    //
    Broadcast::parameters (sync_.getIdentifier(), data::Group::makeCopy (group));
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------