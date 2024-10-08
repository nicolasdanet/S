
/* Copyright (c) 2022 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

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
    const juce::String name  = c + juce::String ("_pdinfo");
    int n = 0; const char* p = BinaryData::getNamedResource (name.toRawUTF8(), n);
    
    return std::tuple<int, const char*> (n, p);
}

bool hasResource (const juce::String& c)
{
    auto [n, p] = findResource (c); return (n && p);
}

void findDocumentationForClass (data::Data& data, const juce::String& c)
{
    if (c.isNotEmpty()) {
    //
    auto [n, p] = findResource (c);
    
    if (n && p) {
    //
    data.setValuesFromXmlDocumentation (juce::String::createStringFromData (p, n));
    //
    }
    //
    }
}

void addDocumentation (data::Data& data, const juce::String& c)
{
    findDocumentationForClass (data, c);
}

void addDocumentation (data::Data& data, const core::Item& i)
{
    if (i.hasParameter (Tag::Attributes, Tag::Class)) {
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

data::Data Documentation::get (const juce::String& c)
{
    data::Data data (Id::DOCUMENTATION);
    
    addDocumentation (data, c);
    
    return data;
}

data::Data Documentation::get (const core::Item& item)
{
    data::Data data (Id::DOCUMENTATION);
    
    addDocumentation (data, item);
    
    return data;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

data::Data Documentation::makeCopy (const core::Item& item)
{
    data::Data data (data::Data::makeCopy (item.getData()));

    addDocumentation (data, item);
    
    return data;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
