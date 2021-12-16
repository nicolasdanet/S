
/* 
    Copyright (c) 2007-2013 Andi Brae.
    Copyright (c) 2013 Jojo and others.
    
*/

/* < http://opensource.org/licenses/BSD-2-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* A simple reference counted (RCO) pointer. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace prim {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class PointerOwner {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

template <class T> friend class Pointer;

private:
    explicit PointerOwner (void* toOwn) : count_ (1), raw_ (toOwn) 
    { 
    }

public:
    PointerOwner (const PointerOwner&) = delete;
    PointerOwner& operator = (const PointerOwner&) = delete;

private:
    int count_;
    void* raw_;     // -- 5.2.9[expr.static.cast]13
    
private:
    PRIM_LEAK_DETECTOR (PointerOwner)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

template <class T> class Pointer {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Pointer() : reference_ (nullptr)
    { 
    }
    
    explicit Pointer (T* toOwn) : reference_ (nullptr)
    {
        if (toOwn) { reference_ = new PointerOwner (static_cast<void*> (toOwn)); }
    }

    Pointer (const Pointer<T>& toShare) : reference_ (nullptr)
    {
        share (toShare);
    }
    
    Pointer<T>& operator = (const Pointer<T>& toShare)
    {
        if (this != &toShare) { share (toShare); }
        
        return *this;
    }

    Pointer<T>& operator = (T* toOwn)
    {
        unshare();
        
        if (toOwn) { reference_ = new PointerOwner (static_cast<void*> (toOwn)); }
        
        return *this;
    }

public:
    Pointer (Pointer<T>&& toMove) : reference_ (toMove.reference_)
    {
        toMove.reference_ = nullptr;
    }
    
    Pointer<T>& operator = (Pointer<T>&& toMove)
    {
        if (this != &toMove) { unshare(); toMove.swapWith (*this); }
        
        return *this;
    }

public:
    ~Pointer()
    {
        unshare();
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void swapWith (Pointer<T>& o) noexcept
    {
        using std::swap;
        
        swap (reference_, o.reference_);
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    T* get() const
    {
        if (reference_) { return static_cast<T*> (reference_->raw_); } else { return nullptr; }
    }
    
    T* operator ->() const
    {
        return get();
    }
    
    T& operator *() const
    {
        return *get();
    }

    operator const void*() const
    {
        return static_cast<const void*> (get());
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Make the Pointer class sortable. */

public:
    bool operator < (const Pointer<T>& o) const
    {
        return get() < o.get();
    }
    
    bool operator > (const Pointer<T>& o) const
    {
        return get() > o.get();
    }

    bool operator == (const Pointer<T>& o) const
    {
        return get() == o.get();
    }
        
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void share (const Pointer<T>& toShare)
    {
        unshare();
      
        reference_ = toShare.reference_;
        
        if (reference_) {
            reference_->count_++;
        }
    }
    
    void unshare()
    {
        if (reference_) {
        //
        if ((--reference_->count_) == 0) {
        //
        delete static_cast<T*> (reference_->raw_);
        reference_->raw_ = nullptr;
        delete reference_;
        //
        }
              
        reference_ = nullptr;
        //
        }
    }
    
private:
    PointerOwner* reference_;

private:
    PRIM_LEAK_DETECTOR (Pointer)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

template <class T> void swap (Pointer<T>& a, Pointer<T>& b) noexcept
{
    a.swapWith (b);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace prim

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
