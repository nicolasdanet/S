
/* 
    Copyright (c) 2007-2013 Andi Brae.
    Copyright (c) 2013 Jojo and others.
    
*/

/* < http://opensource.org/licenses/BSD-2-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Element MUST be default constructible and NOT polymorphic (trivially copyable). */
/* I.e. implementation-compatible with memcpy relocation. */
/* No bound check is performed. */
/* Assignment is used while adding an element. */
/* Copy constructors and comparaison may also be required. */
/* Efficient swap should be implemented in case of sort. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace prim { 

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

template <class T, int N> constexpr int sizeOfArray (T (&)[N])
{
    return N;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class PowerOfTwoGrowth {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

template <class T, class GM> friend class Array;

private:
    static std::size_t getMemorySize (int size)
    {
        if (size <= 0) { return 0; }
        else {
            std::size_t k = 1;
            while (k < static_cast<std::size_t> (size)) { k <<= 1; }
            return k;
        }
    }
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

template <class T, class GM = PowerOfTwoGrowth> class Array {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Array() : data_ (nullptr), size_ (0)
    {
    }

    Array (const Array<T>& o) : data_ (nullptr), size_ (0)
    {
        resize (o.size_);
        
        for (int i = 0; i < size_; ++i) { data_[i] = o.data_[i]; }
    }

    Array<T>& operator = (const Array<T>& o)
    {
        if (this != &o) {
            Array<T> scoped (o); scoped.swapWith (*this);
        }
        
        return *this;
    }

    Array (Array<T>&& toMove) : data_ (toMove.data_), size_ (toMove.size_)
    {
        toMove.data_ = nullptr;
        toMove.size_ = 0;
    }
    
    Array<T>& operator = (Array<T>&& toMove)
    {
        if (this != &toMove) {
            resize(); toMove.swapWith (*this);
        }
        
        return *this;
    }

    ~Array() 
    { 
        resize(); 
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void swapWith (Array<T>& o)
    {
        using std::swap;
        
        swap (data_, o.data_);
        swap (size_, o.size_);
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    int size() const
    {
        return size_;
    }
    
    void clear()
    {
        resize();
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* < http://stackoverflow.com/a/2204380 > */

public:
    void resize (int newSize = 0)
    {
        PRIM_ASSERT (newSize >= 0);
        
        using std::swap;
        
        if (newSize == size_) { return; }

        if (newSize <= 0) {
        //
        delete[] data_;
        data_ = nullptr;
        size_ = 0;
        return;
        //
        }
        
        std::size_t oldMemorySize = GM::getMemorySize (size_);
        std::size_t newMemorySize = GM::getMemorySize (newSize);
          
        if (oldMemorySize == newMemorySize) { 
        //
        if (newSize < size_) {
        //
        int elementsToRelease = (size_ - newSize);
        T* scoped = new T[elementsToRelease]();
        Memory::swap (&data_[newSize], scoped, elementsToRelease);
        delete[] scoped;
        //
        }
        size_ = newSize; 
        //
        } else {
        //
        T* scoped = new T[newMemorySize]();
        int elementsInCommon = (newSize > size_) ? size_ : newSize;
        Memory::swap (data_, scoped, elementsInCommon);
        swap (data_, scoped);
        swap (size_, newSize);
        delete[] scoped;
        //
        }
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    T& getFirst()
    {
        return get (0);
    }

    const T& getFirst() const
    {
        return get (0);
    }

    T& getLast()
    {
        return get (size_ - 1);
    }

    const T& getLast() const
    {
        return get (size_ - 1);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    T& get (int i)
    {
        PRIM_ASSERT ((i >= 0) && (i < size_));
        
        return data_[i];
    }

    const T& get (int i) const
    {
        PRIM_ASSERT ((i >= 0) && (i < size_));
        
        return static_cast<const T&> (data_[i]);
    }
    
    T& operator[] (int i)
    {   
        return get (i);
    }
    
    const T& operator[] (int i) const
    {
        return get (i);
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void add (const T& v)
    {
        if (size_ < std::numeric_limits<int>::max()) { resize (size_ + 1); getLast() = v; }
        else {
            throw std::length_error ("array overflow");       /* Throw something else? */
        }
    }

    void setAll (const T& v)
    {
        for (int i = 0; i < size_; ++i) { data_[i] = v; }
    }

    int indexOf (const T& v) const
    {
        for (int i = 0; i < size_; ++i) { if (get (i) == v) { return i; } }
    
        return -1;
    }
    
    bool contains (const T& v) const
    {
        return (indexOf (v) != -1);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void rotate()
    {
        using std::swap;
        
        if (size_ > 1) { for (int i = size_ - 1; i > 0; --i) { swap (data_[i - 1], data_[i]); } }
    }
    
    void reverse()
    {
        using std::swap;
        
        int m = size_ / 2; for (int i = 0; i < m; ++i) { swap (data_[i], data_[size_ - 1 - i]); }
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void sort()
    {
        quicksortShuffle();                             /* Avoid worst-case scenario. */
        quicksortRecursive (data_, 0, size_ - 1);       /* Bentley-McIlroy 3-way partitioning. */
    }
    
    /* < https://en.wikipedia.org/wiki/Binary_search_algorithm > */
    
    int indexOfSorted (const T& v) const                /* Undefined with data not sorted. */
    {
        int low = 0;
        int high = size_ - 1;
        
        while (low <= high) {
        //
        int m = (high - low) / 2 + low;
        const T& t = get (m);
      
        if (t < v) { low = m + 1; }
        else if (t > v) { high = m - 1; }
        else {
            return m;
        }
        //
        }
        
        return -1;
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < https://www.cs.princeton.edu/~rs/talks/QuicksortIsOptimal.pdf > */
/* < http://en.wikipedia.org/wiki/RANDU > */
/* < https://en.wikipedia.org/wiki/Fisher-Yates_shuffle > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void quicksortShuffle()     /* Basic shuffle with infamously bad RANDU generator. */
    {
        using std::swap;
        
        static uint32 seed = 1;
        
        for (int i = size_ - 1; i > 0; --i) {
        //
        seed = (65539 * seed) & 0x7fffffff;
        double f = static_cast<double> (seed) * (1.0 / 2147483648.0);
        int rand = static_cast<int> (f * static_cast<double> (i + 1));
        swap (data_[i], data_[rand]);
        //
        }
    }
    
    static void quicksortRecursive (T* data, int left, int right)
    {
        using std::swap;
        
        if (right <= left) { return; }
        
        int i = left - 1;
        int j = right;
        int p = left - 1;
        int q = right; 
        
        T v = data[right];
        
        while (1) {
        // 
        while (data[++i] < v) { }
        while (v < data[--j]) { if (j == left) { break; } }
        
        if (i >= j) { break; }
        
        swap (data[i], data[j]);
        
        if (data[i] == v) { p++; swap (data[p], data[i]); }
        if (v == data[j]) { q--; swap (data[j], data[q]); }
        //
        } 
        
        swap (data[i], data[right]); 
        
        j = i - 1; 
        i = i + 1;
        
        for (int k = left; k < p; ++k, --j) { swap (data[k], data[j]); }
        for (int k = right - 1; k > q; --k, ++i) { swap (data[i], data[k]); }
        
        quicksortRecursive (data, left, j); 
        quicksortRecursive (data, i, right);
    }

private:
    T* data_;
    int size_;

private:
    PRIM_LEAK_DETECTOR (Array)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

template <class T> void swap (Array<T>& a, Array<T>& b)
{
    a.swapWith (b);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace prim

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
