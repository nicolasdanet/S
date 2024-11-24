
/* Copyright (c) 2022 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

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
// MARK: -

template<> struct VariantConverter<spaghettis::Directory> {

    static spaghettis::Directory fromVar (const var& v)
    {
        return spaghettis::Directory (v.toString());
    }
        
    static var toVar (const spaghettis::Directory& f)
    {
        return var (f.toString());
    }

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

template<> struct VariantConverter<spaghettis::Device> {

    static spaghettis::Device fromVar (const var& v)
    {
        return spaghettis::Device (v.toString());
    }
        
    static var toVar (const spaghettis::Device& d)
    {
        return var (d.toString());
    }

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

template<> struct VariantConverter<spaghettis::CodeBlock> {

    static spaghettis::CodeBlock fromVar (const var& v)
    {
        return spaghettis::CodeBlock (v.toString());
    }
        
    static var toVar (const spaghettis::CodeBlock& b)
    {
        return var (b.toString());
    }

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace juce
