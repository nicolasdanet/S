
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include <numeric>
#include <string>
#include <vector>

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "Levenshtein.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < https://github.com/maxbachmann/rapidfuzz-cpp > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if 0
void test90__levenshtein() {        /* Levenshtein distance. */
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (LevenshteinDistance, 90, "Levenshtein - Distance")

    spaghettis::Levenshtein<std::string> levenshtein;
    
    TTT_EXPECT (levenshtein.distance ("rosettacode", "raisethysword")   == 8);
    TTT_EXPECT (levenshtein.distance ("South Korea", "North Korea")     == 2);
    
    TTT_EXPECT (levenshtein.distance ("", "")                           == 0);
    TTT_EXPECT (levenshtein.distance ("aaaa", "")                       == 4);
    TTT_EXPECT (levenshtein.distance ("", "aaaa")                       == 4);

    TTT_EXPECT (levenshtein.distance ("aaaa", "aaaa")                   == 0);
    TTT_EXPECT (levenshtein.distance ("aaaa", "aaa")                    == 1);
    TTT_EXPECT (levenshtein.distance ("abaa", "baaa")                   == 2);
    TTT_EXPECT (levenshtein.distance ("aaaa", "aaab")                   == 1);
    TTT_EXPECT (levenshtein.distance ("aaaa", "bbbb")                   == 4);
    TTT_EXPECT (levenshtein.distance ("aabc", "cccd")                   == 4);

    {
        std::string s1 =    "kkkkbbbbfkkkkkkibfkkkafakkfekgkkkkkkkkkkbdbbddddddddddafkkkekkkhkk";
        std::string s2 =    "khddddddddkkkkdgkdikkccccckcckkkekkkkdddddddddddafkkhckkkkkdckkkcc";
        
        TTT_EXPECT (levenshtein.distance (s1, s2) == 36);
    }

    {
        std::string s1 =    "ccddcddddddddddddddddddddddddddddddddddddddddddddddddddddaaaaaaaaaaa";
        std::string s2 =    "aaaaaaaaaaaaaadddddddddbddddddddddddddddddddddddddddddddddbddddddddd";
        
        TTT_EXPECT (levenshtein.distance (s1, s2) == 26);
    }

    {
        std::string s1 =    "accccccccccaaaaaaaccccccccccccccccccccccccccccccacccccccccccccccccccccccccccccc"
                            "ccccccccccccccccccccaaaaaaaaaaaaacccccccccccccccccccccc";
        std::string s2 =    "ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
                            "ccccccccccccccccccccccccccccccccccccbcccb";
                            
        TTT_EXPECT (levenshtein.distance (s1, s2) == 24);
    }

    {
        std::string s1 =    "miiiiiiiiiiliiiiiiibghiiaaaaaaaaaaaaaaacccfccccedddaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                            "aaaaaaaaaaaaa";
        std::string s2 =    "aaaaaaajaaaaaaaabghiiaaaaaaaaaaaaaaacccfccccedddaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                            "aajjdim";
                            
        TTT_EXPECT (levenshtein.distance (s1, s2) == 27);
    }

    {
        std::string s1 =    "lllllfllllllllllllllllllllllllllllllllllllllllllllllllglllllilldcaaaaaaaaaaaaaa"
                            "aaaaadbbllllllllllhllllllllllllllllllllllllllgl";
        std::string s2 =    "aaaaaaaaaaaaaadbbllllllllllllllelllllllllllllllllllllllllllllllglllllilldcaaaaa"
                            "aaaaaaaaaaaaaadbbllllllllllllllellllllllllllllhlllllllllill";
                            
        TTT_EXPECT (levenshtein.distance (s1, s2) == 23);
    }

    {
        std::string s1 =    "llccacaaaaaaaaaccccccccccccccccddffaccccaccecccggggclallhcccccljif";
        std::string s2 =    "bddcbllllllbcccccccccccccccccddffccccccccebcccggggclbllhcccccljifbddcccccc";

        TTT_EXPECT (levenshtein.distance (s1, s2) == 27);
    }
    
    {
        spaghettis::Levenshtein<std::wstring> t;
        
        std::wstring s1 = L"héllö wôrld";
        std::wstring s2 = L"hello world";
        
        TTT_EXPECT (t.distance (s1, s2) == 3);
        TTT_EXPECT (t.distance (s1, s1) == 0);
    }
            
TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if 0
}
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
