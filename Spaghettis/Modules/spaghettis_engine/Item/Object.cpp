
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis::core {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

std::optional<PresetElement> Object::getPresetElement() const
{
    const juce::String s (get<juce::String> (Tag::Parameters, Tag::Label));
    
    if (hasParameter (Tag::Parameters, Tag::Value)) {
        return PresetElement (s, getParameter (Tag::Parameters, Tag::Value));
    }
    
    if (hasParameter (Tag::Parameters, Tag::State)) {
        return PresetElement (s, getParameter (Tag::Parameters, Tag::State));
    }
    
    return std::nullopt;
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

data::Group Object::getCopyOfParameters() const
{
    jassert (isGraphic());
    
    return data::Group::makeCopy (data_.getGroup (Tag::Parameters));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
