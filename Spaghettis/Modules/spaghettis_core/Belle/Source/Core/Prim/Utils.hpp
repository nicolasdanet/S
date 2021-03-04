
/* 
    Copyright (c) 2007-2013 William Andrew Burnson.
    Copyright (c) 2013 Nicolas Danet.
    
*/

/* < http://opensource.org/licenses/BSD-2-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace prim {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

struct Utils {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    template < class T > static std::string paddedLeft (T v, int n = 0, char c = ' ')
    {
        std::ostringstream s; s.setf (std::ios::fixed, std::ios::floatfield);
    
        s << v;
    
        std::string padded;
    
        const int k = Math::max (n - static_cast < int > (s.str().length()), 0);
        
        for (int i = 0; i < k; ++i) { padded += c; }
    
        padded += s.str();
    
        return padded;
    }

    static std::string asHex (uint64 v)
    {
        static const char hex[] = "0123456789abcdef";
        const int size = 32;
        char buffer[size] = { 0 };
    
        int i = size - 1;

        do {
        //
        buffer[--i] = hex[static_cast < std::size_t > (v & 15)];
        v >>= 4;
        //
        } while (v != 0);
    
        return std::string (buffer + i);
    }
    
    static uint64 withHex (const std::string& s)
    {
        std::istringstream converter (s); uint64 v; converter >> std::hex >> v; return v;
    }
    
    static std::string nextID()
    {
        static std::mt19937 prng (std::random_device{}());      // --
        static std::uniform_int_distribution < uint64 > d;
        
        return asHex (d (prng));
    }
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace prim

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
