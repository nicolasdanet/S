
/* 
    Copyright (c) 2007-2013 Andi Brae.
    Copyright (c) 2013 Jojo and others.
    
*/

/* < http://opensource.org/licenses/BSD-2-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace internal {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

using namespace mica;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class KeyToConcept {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    bool operator < (const KeyToConcept& o) const
    {
        return (std::strcmp (key_, o.key_) < 0);
    }
    
    bool operator > (const KeyToConcept& o) const
    {
        return (std::strcmp (key_, o.key_) > 0);
    }

public:
    static UUID find (const char* key);

public:
    const char* key_;       /* Do NOT change order of members. */
    UUID concept_;
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class ConceptToLength {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    bool operator < (const ConceptToLength& o) const
    {
        return (concept_ < o.concept_);
    }
    
    bool operator > (const ConceptToLength& o) const
    {
        return (concept_ > o.concept_);
    }

public:
    static int64 find (UUID concept);

public:
    UUID concept_;          /* Do NOT change order of members. */
    int64 length_;
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class SequenceConceptToIndex {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    bool operator < (const SequenceConceptToIndex& o) const
    {
        return (sequence_ < o.sequence_ || (sequence_ == o.sequence_ && concept_ < o.concept_));
    }
    
    bool operator > (const SequenceConceptToIndex& o) const
    {
        return (sequence_ > o.sequence_ || (sequence_ == o.sequence_ && concept_ > o.concept_));
    }

public:
    static UUID find (UUID sequence, UUID concept);
    
public:
    UUID sequence_;         /* Do NOT change order of members. */
    UUID concept_;
    int64 index_;
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class SequenceIndexToConcept {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    bool operator < (const SequenceIndexToConcept& o) const
    {
        return (sequence_ < o.sequence_ || (sequence_ == o.sequence_ && index_ < o.index_));
    }
    
    bool operator > (const SequenceIndexToConcept& o) const
    {
        return (sequence_ > o.sequence_ || (sequence_ == o.sequence_ && index_ > o.index_));
    }

public:
    static UUID find (UUID sequence, int64 index);

public:
    UUID sequence_;         /* Do NOT change order of members. */
    int64 index_;
    UUID concept_;
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class ConceptLanguageToKey {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    bool operator < (const ConceptLanguageToKey& o) const
    {
        bool cmp = (std::strcmp (language_, o.language_) < 0);
        return (concept_ < o.concept_ || (concept_ == o.concept_ && cmp));
    }
    
    bool operator > (const ConceptLanguageToKey& o) const
    {
        bool cmp = (std::strcmp (language_, o.language_) > 0);
        return (concept_ > o.concept_ || (concept_ == o.concept_ && cmp));
    }

public:
    static const char* find (UUID concept, const char* language);

public:
    UUID concept_;          /* Do NOT change order of members. */
    const char* language_;
    const char* key_;
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class MapToConcept {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    bool operator < (const MapToConcept& o) const
    {
        return (map_ < o.map_);
    }
    
    bool operator > (const MapToConcept& o) const
    {
        return (map_ > o.map_);
    }

public:
    static UUID find (UUID hash);
    
public:
    UUID map_;              /* Do NOT change order of members. */
    UUID concept_;
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace internal

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "Data/MicaData_1.hpp"
#include "Data/MicaData_2.hpp"
#include "Data/MicaData_3.hpp"
#include "Data/MicaData_4.hpp"
#include "Data/MicaData_5.hpp"
#include "Data/MicaData_6.hpp"
#include "Data/MicaData_7.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace internal {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < https://en.wikipedia.org/wiki/Binary_search_algorithm > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

template <class T> int64 search (const T* source, int64 size, const T& needle)
{
    int64 low = 0;
    int64 high = size - 1;
    int64 middle;
    
    while (low <= high) {
    //
    middle = (high - low) / 2 + low;
    const T& v = source[middle];
      
    if (v < needle) { low = middle + 1; }
    else if (v > needle) { high = middle - 1; }
    else {
        return middle;
    }
    //
    }
    
    return -1;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

UUID KeyToConcept::find (const char* key)
{
    KeyToConcept needle = { key, UUID::Undefined };
    int64 k = search (keyToConceptLookup, keyToConceptSize, needle);
    
    if (k < 0) { return UUID::Undefined; }
    else {
        return keyToConceptLookup[k].concept_;
    }
}

int64 ConceptToLength::find (UUID concept)
{
    ConceptToLength needle = { concept, 0 };
    int64 k = search (conceptToLengthLookup, conceptToLengthSize, needle);
    
    if (k < 0) { return 0; }
    else {
        return conceptToLengthLookup[k].length_;
    }
}

UUID SequenceConceptToIndex::find (UUID sequence, UUID concept)
{
    SequenceConceptToIndex needle = { sequence, concept, 0 };
    int64 k = search (sequenceConceptToIndexLookup, sequenceConceptToIndexSize, needle);
    
    if (k < 0) { return UUID::Undefined; }
    else {
        UUID uuid = { static_cast<uint64> (sequenceConceptToIndexLookup[k].index_), 1ULL };
        return uuid;
    }
}

UUID SequenceIndexToConcept::find (UUID sequence, int64 index)
{
    SequenceIndexToConcept needle = { sequence, index, UUID::Undefined };
    int64 k = search (sequenceIndexToConceptLookup, sequenceIndexToConceptSize, needle);
    
    if (k < 0) { return UUID::Undefined; }
    else {
        return sequenceIndexToConceptLookup[k].concept_;
    }
}

const char* ConceptLanguageToKey::find (UUID concept, const char* language)
{
    ConceptLanguageToKey needle = { concept, language, "" };
    int64 k = search (conceptLanguageToKeyLookup, conceptLanguageToKeySize, needle);
    
    if (k < 0) { return ""; }
    else {
        return conceptLanguageToKeyLookup[k].key_;
    }
}

UUID MapToConcept::find (UUID hash)
{
    MapToConcept needle = { hash, UUID::Undefined };
    int64 k = search (mapToConceptLookup, mapToConceptSize, needle);
    
    if (k < 0) { return UUID::Undefined; }
    else {
        return mapToConceptLookup[k].concept_;
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace internal

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
