
/* 
    Copyright (c) 2007-2013 William Andrew Burnson.
    Copyright (c) 2013 Nicolas Danet.
    
*/

/* < http://opensource.org/licenses/BSD-2-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Doubly linked list optimized for traversal by index. */
/* Element must be default constructible and assignable. */
/* Copy constructors and comparaison may also be needed to sort items. */
/* Consider to implement an efficient swap in that case. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace prim {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

template < class T > class List {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    class Link {
    
    public:
        Link() : next_ (nullptr), previous_ (nullptr) 
        { 
        }

    public:
        Link (const Link&) = delete;
        Link& operator = (const Link&) = delete;

    public:
        T data_;
        Link* next_;
        Link* previous_;
    
    private:
        PRIM_LEAK_DETECTOR (Link)
    };  

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    List() : head_ (nullptr), tail_ (nullptr), pointer_ (nullptr), index_ (0), size_ (0) 
    { 
    }

public:
    List < T > (const List < T > &) = delete;
    List < T > & operator = (const List < T > &) = delete;

public:
    ~List()
    {
        clear();
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void swapWith (List < T > & o)
    {
        using std::swap;
            
        swap (head_, o.head_);
        swap (tail_, o.tail_);
        swap (pointer_, o.pointer_);
        swap (index_, o.index_);
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
        while (size_ > 0) { remove (0); }
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    T& getFirst() 
    { 
        PRIM_ASSERT (size_ > 0);
        
        return head_->data_;
    }
    
    const T& getFirst() const 
    {
        PRIM_ASSERT (size_ > 0);
        
        return head_->data_;
    }

    T& getLast() 
    {
        PRIM_ASSERT (size_ > 0);
        
        return tail_->data_;
    }
    
    const T& getLast() const
    {
        PRIM_ASSERT (size_ > 0);
        
        return tail_->data_;
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    T& get (int i)
    {
        return const_cast < T& > (static_cast < const List < T > & > (*this).get (i));
    }
    
    const T& get (int i) const
    {
        return traverseToFetch (i)->data_;
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
    void add (const T& x)
    {
        Link* newLink = new Link();
  
        if (!size_) { tail_ = head_ = newLink; }
        else {
            newLink->previous_   = tail_;
            tail_ = tail_->next_ = newLink;
        }
  
        size_++;
  
        tail_->data_ = x;
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void remove (int i)
    {
        Link* p = traverseToFetch (i);

        if (p->previous_) { pointer_ = p->previous_; index_ = i - 1; }
        else {
            pointer_ = nullptr; index_ = 0;
        }
        
        if (!p->previous_ && !p->next_) { head_ = tail_ = nullptr; }
        else {
        //
        if (p->previous_ == nullptr) { head_ = p->next_; }
        else {
            p->previous_->next_ = p->next_;
        }
        
        if (p->next_ == nullptr) { tail_ = p->previous_; }
        else {
            p->next_->previous_ = p->previous_;
        }
        //
        }

        delete p; size_--;
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    Link* traverseToFetch (int i) const
    {
        PRIM_ASSERT ((i >= 0) && (i < size_));
        
        int j; 
        Link* p = nullptr;
        
        if (pointer_ && i >= index_) { p = pointer_; j = index_; }
        else {
            p = head_; j = 0;
        }
        
        while (j != i) { p = p->next_; j++; }
        
        pointer_ = p; index_ = j;
        
        return p;
    }

private:
    Link* head_;
    Link* tail_;
    mutable Link* pointer_;     /* Optimisation for traversals. */
    mutable int index_;         /* Ditto. */
    int size_;

private:
    PRIM_LEAK_DETECTOR (List)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

template < class T > void swap (List < T > & a, List < T > & b)
{
    a.swapWith (b);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace prim

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
