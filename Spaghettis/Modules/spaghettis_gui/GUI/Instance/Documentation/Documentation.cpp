
/* Copyright (c) 2022 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

auto findResource (const juce::String& key)
{
    const juce::String name  = key + juce::String ("_pdinfo");
    int n = 0; const char* p = BinaryData::getNamedResource (name.toRawUTF8(), n);
    
    return std::tuple<int, const char*> (n, p);
}

auto hasSameKey (const juce::String& key)
{
    return [s = key](const DocumentationElement& e)
    {
        return (e.getKey() == s);
    };
}

std::optional<data::Data> findCached (std::vector<DocumentationElement>& v, const juce::String& key)
{
    auto r = std::find_if (v.cbegin(), v.cend(), hasSameKey (key));
        
    if (r != v.cend()) { return r->getData(); }
    else {
        return std::nullopt;
    }
}

std::optional<data::Data> addDocumentation (std::vector<DocumentationElement>& v,
    const juce::String& key,
    const juce::String& data)
{
    v.emplace_back (key, data); return v.back().getData();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

std::optional<data::Data> fetchDocumentation (std::vector<DocumentationElement>& v, const juce::String& key)
{
    if (key.isNotEmpty()) {
    //
    auto [n, p] = findResource (key);
    
    if (n && p) {
    //
    std::optional<data::Data> documentation (findCached (v, key));

    if (documentation.has_value()) { return documentation; }
    else {
        return addDocumentation (v, key, juce::String::createStringFromData (p, n));
    }
    //
    }
    //
    }
    
    return std::nullopt;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

std::optional<data::Data> Documentation::get (const juce::String& s)
{
    return fetchDocumentation (cache_, s);
}

std::optional<data::Data> Documentation::get (const core::Item& i)
{
    if (i.hasParameter (Tag::Attributes, Tag::Class)) {
        return get (i.get<juce::String> (Tag::Attributes, Tag::Class));
    }
    
    return std::nullopt;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

data::Data Documentation::makeCopyWithDocumentation (const core::Item& item)
{
    data::Data data (data::Data::makeCopy (item.getData()));
    
    const std::optional<data::Data> documentation (Spaghettis()->getDocumentation().get (item));
    
    if (documentation.has_value()) {
        data.copyGroup (documentation.value(), Tag::Documentation);
    }
    
    return data;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
