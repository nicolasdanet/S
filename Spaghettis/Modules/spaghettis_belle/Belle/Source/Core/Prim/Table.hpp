
/* 
    Copyright (c) 2007-2013 Andi Brae.
    Copyright (c) 2013 Jojo and others.
    
*/

/* < http://opensource.org/licenses/BSD-2-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Key must be sortable. */ 
/* A Key set to undefined value is deleted. */
/* Key and Value must be trivially copyable (see Array). */
/* Key and Value should have an efficient swap. */
/* Note that no exception safety is guaranteed if Key and/or Value throw in copy constructors. */
/* Key addition is NOT efficient in large tables. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace prim {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

template <class K, class V> class KeyValue {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    KeyValue() : key_(), value_()
    { 
    }
    
    explicit KeyValue (const K& k, const V& v = V()) : key_ (k), value_ (v) 
    { 
    }
    
    KeyValue<K, V>& operator = (const KeyValue<K, V>& o)
    {
        if (this != &o) {
            KeyValue scoped (o); scoped.swapWith (*this);
        }
        
        return *this;
    }
    
    KeyValue (const KeyValue<K, V>&) = default;
    KeyValue (KeyValue<K, V>&&) = default;
    KeyValue<K, V>& operator = (KeyValue<K, V>&&) = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void swapWith (KeyValue<K, V>& o)
    {
        using std::swap;
        
        swap (key_, o.key_);
        swap (value_, o.value_);
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    const K& getKey() const
    {
        return key_;
    }
    
    V& getValue()
    {
        return value_;
    }
    
    const V& getValue() const
    {
        return value_;
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    bool operator < (const KeyValue<K, V>& kv) const
    {
        return key_ < kv.key_;
    }
    
    bool operator > (const KeyValue<K, V>& kv) const
    {
        return key_ > kv.key_;
    }
    
    bool operator == (const KeyValue<K, V>& kv) const
    { 
        return key_ == kv.key_;
    }

private:
    K key_;
    V value_;

private:
    PRIM_LEAK_DETECTOR (KeyValue)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

template <class K, class V = K> class Table {

template <class U> friend class Graph;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit Table (const V& undefined = V()) : isSorted_ (true), isPruned_ (true), undefined_ (undefined) 
    {
    }

    Table<K, V>& operator = (const Table<K, V>& o)
    {
        if (this != &o) {
            Table scoped (o); scoped.swapWith (*this);
        }
        
        return *this;
    }
    
    Table (const Table<K, V>&) = default;
    Table (Table<K, V>&&) = default;
    Table<K, V>& operator = (Table<K, V>&&) = default;

    ~Table()
    {
        clear();
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void swapWith (Table<K, V>& o)
    {
        using std::swap;
        
        swap (isSorted_, o.isSorted_);
        swap (isPruned_, o.isPruned_);
        
        data_.swapWith (o.data_);
        
        swap (undefined_, o.undefined_);
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    int size() const
    {
        sortAndPruneNecessary(); return data_.size();
    }
    
    void clear()
    {
        data_.clear(); isPruned_ = isSorted_ = true;
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    V& get (const K& key)
    {
        sortAndPruneNecessary();
        int i = data_.indexOfSorted (KeyValue<K, V> (key));
        
        isPruned_ = false; 
        
        if (i >= 0) { return data_[i].getValue(); }
        else {
            KeyValue<K, V> kv (key, undefined_);
            data_.add (kv);
            isSorted_ = false;
            return data_.getLast().getValue();
        }
    }

    const V& get (const K& key) const
    {
        sortAndPruneNecessary();
        int i = data_.indexOfSorted (KeyValue<K, V> (key));
    
        if (i >= 0) { return data_[i].getValue(); }
        else {
            return undefined_;
        }
    }

    V& operator[] (const K& key)
    {
        return get (key);
    }

    const V& operator[] (const K& key) const
    {
        return get (key);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    const KeyValue<K, V>& getAtIndex (int i) const
    {
        sortAndPruneNecessary(); return data_[i];
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void sortAndPruneNecessary() const
    {
        if (isPruned_ == false) {
        //
        int j = 0;
        for (int i = 0; i < data_.size(); ++i) {
            if (data_[i].getValue() != undefined_) { 
                if (i != j) { swap (data_[j], data_[i]); } 
                ++j; 
            }
        }
        data_.resize (j);
        isPruned_ = true;
        //
        }
        
        if (isSorted_ == false) { data_.sort(); isSorted_ = true; }
    }
    
private:
    mutable bool isSorted_;
    mutable bool isPruned_;
    mutable Array<KeyValue<K, V>> data_;
    V undefined_;

private:
    PRIM_LEAK_DETECTOR (Table)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

template <class K, class V> void swap (KeyValue<K, V>& a, KeyValue<K, V>& b)
{
    a.swapWith (b);
}

template <class K, class V> void swap (Table<K, V>& a, Table<K, V>& b)
{
    a.swapWith (b);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace prim

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
