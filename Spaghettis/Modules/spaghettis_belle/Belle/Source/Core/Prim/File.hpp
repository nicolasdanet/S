
/* 
    Copyright (c) 2007-2013 Andi Brae.
    Copyright (c) 2013 Jojo and others.
    
*/

/* < http://opensource.org/licenses/BSD-2-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace prim {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

struct File {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    static bool write (const char* filename, const char* s, std::streamsize count)
    {
        bool b = false;
        
        std::ofstream stream; stream.open (filename, std::ios::out | std::ios::trunc | std::ios::binary);
        
        if (stream.is_open()) {
        //
        stream.write (s, count);
        b = stream.good();
        stream.close();
        //
        }
        
        return b;
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    static bool writeByString (const char* filename, const std::string& data)
    {
        return File::write (filename, data.c_str(), data.length());
    }

    template <class T> static bool writeByArray (const char* filename, const Array<T>& data)
    {
        return File::write (filename, reinterpret_cast<const char*> (&data[0]), data.size() * sizeof (T));
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    template <class T> static bool readToArray (const char* filename, Array<T>& data)
    {
        bool b = false;
        
        std::ifstream stream; stream.open (filename, std::ios::in | std::ios::binary);

        if (stream.is_open()) {
        //
        stream.seekg (0, std::ios_base::end);
        int size = static_cast<int> (stream.tellg());
        stream.seekg (0, std::ios_base::beg);
        
        data.resize (static_cast<int> (size / sizeof (T)));
        
        stream.read (reinterpret_cast<char*> (&data[0]), size);
        b = stream.good();
        stream.close();
        //
        }

        return b;
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace prim

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
