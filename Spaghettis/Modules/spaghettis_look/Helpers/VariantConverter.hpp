
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace juce {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* t_id */

template<> struct VariantConverter<uint64_t> {

static uint64_t fromVar (const var& v)
{
    return static_cast<uint64_t> (static_cast<int64> (v));
}
    
static var toVar (const uint64_t& i)
{
    return var (static_cast<int64> (i));
}

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

template<> struct VariantConverter<Colour> {

static Colour fromVar (const var& v)
{
    return Colour (spaghettis::Colour::getColourFromString (v.toString()));
}
    
static var toVar (const juce::Colour& c)
{
    return var (spaghettis::Colour::getColourAsString (c));
}

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

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
