
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace juce {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

template<> struct VariantConverter<Colour> {

static Colour fromVar (const var& v)
{
    return Colour (spaghettis::Colours::getColourFromString (v.toString()));
}
    
static var toVar (const juce::Colour& c)
{
    return var (spaghettis::Colours::getColourAsString (c));
}

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

template<> struct VariantConverter<Rectangle<int>> {

static Rectangle<int> fromVar (const var& v)
{
    return Rectangle<int>::fromString (v.toString());
}
    
static var toVar (const Rectangle<int>& r)
{
    return var (r.toString());
}

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace juce
