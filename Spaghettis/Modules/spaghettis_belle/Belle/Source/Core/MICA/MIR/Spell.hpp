
/* Copyright (c) 2013 Jojo and others. */

/* < http://opensource.org/licenses/BSD-2-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace MIR {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Pitch spelling based on the algorithm from Emilios Cambouropoulos. */

/* < http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.385.8305&rep=rep1&type=pdf > */
/* < http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.331.5363&rep=rep1&type=pdf > */
/* < http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.368.271&rep=rep1&type=pdf > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class Spell {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static const int kWindowSize        = 6;            /* Must be a multiple of three. */
static const int kOneThird          = 2;            /* One third of the window size. */
static const int kTwoThird          = 4;            /* Two thirds of the window size. */
static const int kTurnMaximum       = 2;
static const int kPenaltyAccidental = 1;
static const int kPenaltyEnharmonic = 2;
static const int kPenaltyClassA     = 0;
static const int kPenaltyClassB     = 0;
static const int kPenaltyClassC     = 1;
static const int kPenaltyClassD     = 2;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

using Window = Tuple<kWindowSize>;                  /* Cacheable group of concepts. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

struct HashWindow {

    std::size_t operator()(const Window& w) const
    {
        return w.hash();
    }
};

struct HashInterval {

    std::size_t operator()(const Interval& i) const
    {
        return i.hash();
    }
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Spell (Concept key = mica::CMajor)
    {
        keySignature_ = map (key, mica::KeySignature);
        accidental_   = map (keySignature_, mica::Accidental);
        accidentals_  = Utils::getAccidentalsByLetters (keySignature_);
    }

public:
    Spell (const Spell&) = delete;
    Spell& operator = (const Spell&) = delete;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    static bool keyIsValid (Concept c)
    {
        return !mica::map (c, mica::KeySignature).isUndefined();
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    bool isValid() const
    {
        return (accidentals_.size() != 0);
    }

    void setKey (Concept key)
    {
        Concept keySignature = map (key, mica::KeySignature);
        
        if (keySignature_ != keySignature) {
        //
        intervals_.clear();     /* Clear the caches that contain wrong values now. */
        windows_.clear();
        
        keySignature_ = keySignature;
        accidental_   = map (keySignature_, mica::Accidental);
        accidentals_  = Utils::getAccidentalsByLetters (keySignature_);
        //
        }
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    prim::Array<Concept> getSpelling (const prim::Array<int>& numbers) const
    {
        prim::Array<Concept> result; vectorCompute (numbers, result);
        
        return result;
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void vectorCompute (const prim::Array<int>& numbers, prim::Array<Concept> & chromatics) const
    {
        chromatics.resize (numbers.size());
    
        Window wFirst (numbers); vectorFindBest (wFirst);
        
        /* First step. */
        
        if (vectorSetResult (chromatics, wFirst) == false) { return; }              /* Stop if all done. */
            
        /* Third overlapping. */
        
        int k = kOneThird;
                                             
        while (k < numbers.size()) {                   
            Window wOverlap (numbers, k);
            for (int i = 0; i < kOneThird; ++i) { wOverlap[i] = chromatics[k + i]; }
            vectorFindBest (wOverlap);
            if (vectorSetResult (chromatics, wOverlap, k) == false) { return; }     /* Stop if all done. */
            k += kOneThird;
        }
    }

    void vectorFindBest (Window& w) const
    {
        auto iter = windows_.find (w);

        if (iter != windows_.end()) { w = iter->second; }
        else {
            Window result (w);
            vectorCombineAndSet (result.getRaw());
            windows_.insert (std::pair< Window, Window> (w, result));
            w = result;
        } 
    }

    static bool vectorSetResult (prim::Array<Concept> & chromatics, const Window& w, int k = 0)
    {
        const int start = (k == 0) ? 0 : kOneThird;
        
        for (int i = start; i < kTwoThird; ++i) {
            if (w[i] == mica::Undefined) { return false; } 
            else { 
                PRIM_ASSERT ((k + i) < chromatics.size()); chromatics[k + i] = w[i]; 
            }
        }
        
        return ((w[kTwoThird] == mica::Undefined) ? false : true);
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    template <int N> void vectorCombineAndSet (Concept (&a)[N]) const
    {
        /* Predefined chromatics pitch are retained. */
        /* Undefined concepts are ignored. */
        
        Concept base[N];
        
        prim::Array<int> turn;       /* Indexes of pitches modified during combination. */
        
        for (int i = N - 1; i >= 0; --i) { 
        //
        base[i] = a[i]; 

        if (base[i].isInteger()) {
            base[i] = map (base[i], accidental_ == mica::Flat ? mica::Flat : mica::Sharp); 
            turn.add (i);
        }
        //
        }
        
        /* Initialise. */
        
        Concept result[N], testing[N];
            
        for (int i = 0; i < N; ++i) { result[i] = testing[i] = base[i]; }
        
        int minimum = -1;
        
        /* Combine. */
            
        static_assert (N < 32, "");
        
        int size = turn.size();
        
        for (int k = 0; k < 2; ++k) {                                   
        //
        Concept enharmony[N];   /* Alternate sharps or flats. */
        
        for (int i = 0; i < size; ++i) { enharmony[turn[i]] = item (base[turn[i]], k == 0 ? 1 : -1); }
        
        /* Distribute with binary representation. */
        
        for (prim::uint32 i = 0; i < (1U << size); ++i) {
        //
        if (minimum == 0) { break; }                                /* Stop if sequence is optimum. */
        
        if (prim::Math::numberOfSetBits (i) <= kTurnMaximum) {      /* Limit for efficiency. */
        //
        for (int j = 0; j < size; ++j) {
            if ((i & (1U << j)) != 0U) { testing[turn[j]] = enharmony[turn[j]]; }
            else {
                testing[turn[j]] = base[turn[j]];
            }
        }
        
        int penalty = vectorPenalty (testing);
        
        if (minimum == -1 || penalty < minimum) { 
            prim::Memory::copy (result, testing, N); minimum = penalty; 
        }
        //
        }
        //
        }
        //
        }
        
        prim::Memory::swap (result, a, N);
    }
    
    template <int N> int vectorPenalty (Concept (&a)[N]) const
    {
        int penalty = 0;
        
        for (int i = 0; i < N - 1; ++i) {
        //
        for (int j = i + 1; j < N; ++j) {
        //
        if (!a[i].isUndefined() && !a[j].isUndefined()) { penalty += getPenalty (Interval (a[i], a[j])); }
        //
        }
        //
        }
        
        return penalty;
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Public for debugging purpose. */

public:
    int getPenalty (const Interval& interval) const
    {
        auto iter = intervals_.find (interval);
    
        if (iter != intervals_.end()) { return iter->second; }
        else {
            int penalty = intervalPenalty (interval);
            intervals_.insert (std::pair<Interval, int> (interval, penalty));
            return penalty;
        }
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    int intervalPenalty (const Interval& interval) const
    {
        int penalty = 0;
    
        /* Notational parsimony. */

        penalty += intervalPenaltyIsEnharmonic (interval.first()) ? kPenaltyEnharmonic : 0;
        penalty += intervalPenaltyIsEnharmonic (interval.last())  ? kPenaltyEnharmonic : 0;

        penalty += intervalPenaltyIsAccidental (interval.first()) ? kPenaltyAccidental : 0;
        penalty += intervalPenaltyIsAccidental (interval.last())  ? kPenaltyAccidental : 0;
        
        /* Interval optimisation. */

        penalty += intervalPenaltyByClass (interval.getName());
                
        return penalty;
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

private:
    bool intervalPenaltyIsEnharmonic (Concept chromatic) const
    {
        static const Concept enharmonics[14] = 
            { 
                BSharp, 
                DDoubleFlat,
                FDoubleSharp,
                ADoubleFlat,
                ADoubleSharp,
                CFlat,
                FDoubleSharp,
                ADoubleFlat,
                ESharp,
                GDoubleFlat,
                DDoubleSharp,
                FFlat,
                CDoubleSharp,
                EDoubleFlat
            };
        
        Concept note = map (chromatic, mica::ChromaticNote);

        for (int i = 0; i < 14; ++i) { if (note == enharmonics[i]) { return true; } }
        
        return false;
    }

    bool intervalPenaltyIsAccidental (Concept chromatic) const
    {
        Concept accidental = map (chromatic, mica::Accidental);
        Concept letter = map (chromatic, mica::Letter);
        
        if (accidentals_[letter] == accidental) { return false; }
        else {
            return true;
        }
    }

    int intervalPenaltyByClass (Concept interval) const
    {
        static const Concept intervals[20] = 
            { 
                PerfectUnison,
                PerfectOctave,
                PerfectFourth, 
                PerfectFifth,
                //
                MinorSecond, 
                MajorSeventh, 
                MajorSecond, 
                MinorSeventh, 
                MinorThird, 
                MajorSixth, 
                MajorThird, 
                MinorSixth,
                //
                AugmentedSecond, 
                DiminishedSeventh,
                DiminishedThird,
                AugmentedSixth,
                DiminishedFourth,
                AugmentedFifth,
                AugmentedFourth,
                DiminishedFifth
            };
        
        for (int i = 0; i < 20; ++i) { 
        //
        if (interval == intervals[i]) { 
        //
        if (i < 4)       { return kPenaltyClassA; }
        else if (i < 12) { return kPenaltyClassB; }
        else             { return kPenaltyClassC; }
        //
        }
        //
        }
        
        return kPenaltyClassD;
    }

private:
    Concept keySignature_;
    Concept accidental_;
    prim::Table<Concept> accidentals_;

/* Results are cached for efficiency. */

private:
    mutable std::unordered_map<Interval, int, HashInterval> intervals_;
    mutable std::unordered_map<Window, Window, HashWindow> windows_;

private:
    PRIM_LEAK_DETECTOR (Spell)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace MIR

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
