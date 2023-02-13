
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
// MARK: -

#if 0
void test91__levenshteinKey() {     /* Levenshtein key. */
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (LevenshteinKey, 91, "Levenshtein - Key")

    spaghettis::Levenshtein<std::string> levenshtein;
    
    std::vector<std::string> items = {
        "!=",
        "%",
        "&",
        "&&",
        "*",
        "*~",
        "+",
        "+~",
        "-",
        "-~",
        "/",
        "/~",
        "<",
        "<<",
        "<=",
        "<~",
        "==",
        ">",
        ">=",
        ">>",
        ">~",
        "|",
        "||",
        "abs",
        "abs~",
        "adc~",
        "amptodb",
        "amptodb~",
        "arguments",
        "array",
        "array get",
        "array max",
        "array min",
        "array quantile",
        "array random",
        "array set",
        "array size",
        "array sum",
        "atan",
        "atan2",
        "b",
        "bag",
        "bang",
        "bang~",
        "bendin",
        "bendout",
        "biquad~",
        "block~",
        "blocksize",
        "bng",
        "bp~",
        "catch~",
        "change",
        "clip",
        "clip~",
        "closebang",
        "comment",
        "cos",
        "cos~",
        "counter",
        "cpole~",
        "ctlin",
        "ctlout",
        "czero_rev~",
        "czero~",
        "dac~",
        "dbtoamp",
        "dbtoamp~",
        "dbtopow",
        "dbtopow~",
        "dbtorms",
        "dbtorms~",
        "delay",
        "delread~",
        "delwrite~",
        "dial",
        "directories",
        "div",
        "dspstatus",
        "env~",
        "exp",
        "exp~",
        "expr",
        "f",
        "fft~",
        "float",
        "framp~",
        "freeze",
        "ftom",
        "ftom~",
        "garray",
        "gatom",
        "hip~",
        "hradio",
        "hslider",
        "i",
        "ifft~",
        "inlet",
        "inlet~",
        "int",
        "line",
        "line~",
        "list",
        "list append",
        "list change",
        "list equal",
        "list fromsymbol",
        "list group",
        "list iterate",
        "list length",
        "list prepend",
        "list reverse",
        "list rotate",
        "list scramble",
        "list sort",
        "list split",
        "list store",
        "list stream",
        "list sum",
        "list tosymbol",
        "list trim",
        "loadbang",
        "loadmess",
        "log",
        "log~",
        "lop~",
        "lrshift~",
        "mag~",
        "makefilename",
        "makenote",
        "max",
        "max~",
        "menubutton",
        "message",
        "metro",
        "midiports",
        "midisystemin",
        "midisystemout",
        "min",
        "min~",
        "mod",
        "moses",
        "mtof",
        "mtof~",
        "netreceive",
        "netsend",
        "noise~",
        "notein",
        "noteout",
        "osc~",
        "oscbundle",
        "oscformat",
        "oscparse",
        "oscstream",
        "outlet",
        "outlet~",
        "pack",
        "pak",
        "panel",
        "pd",
        "pgmin",
        "pgmout",
        "phasor~",
        "pipe",
        "poly",
        "polytouchin",
        "polytouchout",
        "pow",
        "pow~",
        "powtodb",
        "powtodb~",
        "prepend",
        "print",
        "print~",
        "qlist",
        "r",
        "r~",
        "random",
        "readsf~",
        "realtime",
        "receive",
        "receive~",
        "rfft~",
        "rifft~",
        "rmag~",
        "rmstodb",
        "rmstodb~",
        "route",
        "rpole~",
        "rsqrt~",
        "rzero_rev~",
        "rzero~",
        "s",
        "s~",
        "samphold~",
        "samplerate",
        "select",
        "send",
        "send~",
        "sig~",
        "sigmund~",
        "sin",
        "snapshot~",
        "soundfiler",
        "soundinfo",
        "spigot",
        "sqrt",
        "sqrt~",
        "stripnote",
        "swap",
        "symbol",
        "sysexin",
        "sysexout",
        "t",
        "tabosc4~",
        "tabplay~",
        "tabread",
        "tabread~",
        "tabread4",
        "tabread4~",
        "tabreceive~",
        "tabsend~",
        "tabwrite",
        "tabwrite~",
        "tan",
        "text",
        "text define",
        "text delete",
        "text fromlist",
        "text get",
        "text insert",
        "text search",
        "text sequence",
        "text set",
        "text size",
        "text tolist",
        "textfile",
        "tgl",
        "threshold~",
        "throw~",
        "timer",
        "timestamp",
        "touchin",
        "touchout",
        "trigger",
        "unpack",
        "until",
        "urn",
        "uzi",
        "v",
        "value",
        "vcf~",
        "vd~",
        "vexpr",
        "vradio",
        "vslider",
        "vu",
        "wrap",
        "wrap~",
        "writesf~"
    };
    
    std::vector<int> results = {
        3,
        3,
        3,
        3,
        3,
        3,
        3,
        3,
        3,
        3,
        3,
        3,
        3,
        3,
        3,
        3,
        3,
        3,
        3,
        3,
        3,
        3,
        3,
        3,
        3,
        3,
        7,
        7,
        9,
        5,
        9,
        9,
        9,
        14,
        11,
        8,
        9,
        8,
        4,
        5,
        3,
        3,
        4,
        5,
        5,
        6,
        7,
        6,
        8,
        3,
        3,
        6,
        6,
        4,
        5,
        8,
        7,
        3,
        3,
        7,
        6,
        5,
        6,
        10,
        6,
        3,
        6,
        7,
        6,
        7,
        6,
        6,
        4,
        7,
        8,
        3,
        10,
        2,
        7,
        4,
        3,
        4,
        4,
        2,
        4,
        5,
        6,
        6,
        4,
        5,
        6,
        5,
        4,
        5,
        6,
        3,
        4,
        5,
        6,
        3,
        4,
        5,
        3,
        10,
        10,
        9,
        13,
        9,
        11,
        10,
        11,
        11,
        10,
        12,
        8,
        9,
        9,
        10,
        7,
        12,
        8,
        7,
        6,
        3,
        4,
        4,
        6,
        4,
        11,
        8,
        3,
        4,
        10,
        6,
        5,
        8,
        10,
        11,
        3,
        4,
        3,
        4,
        3,
        4,
        10,
        6,
        5,
        6,
        7,
        3,
        8,
        7,
        7,
        8,
        6,
        7,
        4,
        3,
        5,
        3,
        5,
        6,
        6,
        4,
        4,
        11,
        12,
        3,
        4,
        7,
        7,
        7,
        5,
        6,
        4,
        3,
        3,
        5,
        4,
        8,
        7,
        8,
        4,
        5,
        5,
        6,
        7,
        5,
        6,
        5,
        10,
        6,
        2,
        2,
        9,
        10,
        6,
        4,
        5,
        4,
        8,
        3,
        8,
        9,
        7,
        6,
        4,
        5,
        9,
        4,
        6,
        6,
        7,
        3,
        7,
        8,
        7,
        8,
        8,
        8,
        11,
        7,
        8,
        9,
        3,
        4,
        9,
        10,
        12,
        8,
        10,
        10,
        12,
        7,
        8,
        10,
        7,
        3,
        9,
        6,
        5,
        8,
        7,
        8,
        7,
        6,
        5,
        3,
        3,
        3,
        5,
        3,
        3,
        5,
        5,
        6,
        3,
        4,
        5,
        6
    };
    
    levenshtein.setKey ("dsf");
    
    const int m = items.size();
    const int n = results.size();
    
    TTT_EXPECT (m == n);    /* Check at compile-time? */
    
    for (int i = 0; i < m; ++i) {
        TTT_EXPECT (levenshtein.distanceToKey (items[i]) == results[i]);
    }
    
TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if 0
}
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
