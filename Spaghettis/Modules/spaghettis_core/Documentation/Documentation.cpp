
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

// TODO: Cache documentation for efficiency?

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

auto findResource (const juce::String& c)
{
    const juce::String name  = juce::String ("info_") + c + juce::String ("_xml");
    int n = 0; const char* p = BinaryData::getNamedResource (name.toRawUTF8(), n);
    
    return std::tuple<int, const char*> (n, p);
}

bool hasResource (const juce::String& c)
{
    auto [n, p] = findResource (c); return (n && p);
}

void findDocumentationForClass (core::Data& data, const juce::String& c)
{
    if (c.isNotEmpty()) {
    //
    auto [n, p] = findResource (c);
    
    if (n && p) {
    //
    data.addParametersFromXmlDocumentation (juce::String::createStringFromData (p, n));
    //
    }
    //
    }
}

void addDocumentation (core::Data& data, const juce::String& c)
{
    findDocumentationForClass (data, c);
}

void addDocumentation (core::Data& data, const core::Item& i)
{
    if (i.has (Tag::Attributes, Tag::Class)) {
    //
    addDocumentation (data, i.get<juce::String> (Tag::Attributes, Tag::Class));
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool Documentation::has (const juce::String& c)
{
    return hasResource (c);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

core::Data Documentation::get (const juce::String& c)
{
    core::Data data (Id::DOCUMENTATION);
    
    addDocumentation (data, c);
    
    return data;
}

core::Data Documentation::get (const core::Item& item)
{
    core::Data data (Id::DOCUMENTATION);
    
    addDocumentation (data, item);
    
    return data;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

core::Data Documentation::getCopyWithDocumentation (const core::Item& item)
{
    core::Data data (core::Data::makeCopy (item.getData()));

    addDocumentation (data, item);
    
    return data;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
