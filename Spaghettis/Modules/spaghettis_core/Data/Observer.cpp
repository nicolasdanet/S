
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis::core {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Observer::addParameterHandler (const juce::String& key, std::function<void (const Parameter&)> f)
{
    handlers_.emplace_back (key, f);
}

void Observer::valueTreePropertyChanged (juce::ValueTree& tree, const juce::Identifier&)
{
    if (tree.hasType (Id::PARAMETER)) {
    //
    const Parameter p (tree);
    
    callParameterHandlers (p); parameterHasChanged (core::Group::getFromParameter (p), p);
    //
    }
}

void Observer::callParameterHandlers (const Parameter& parameter)
{
    const juce::String key (parameter.getKey());
    
    for (const auto& h : handlers_) { if (h.key_ == key) { h.f_ (parameter); } }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
