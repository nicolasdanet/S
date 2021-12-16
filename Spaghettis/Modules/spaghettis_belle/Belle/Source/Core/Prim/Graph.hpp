
/* 
    Copyright (c) 2007-2013 Andi Brae.
    Copyright (c) 2013 Jojo and others.
    
*/

/* < http://opensource.org/licenses/BSD-2-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Directed labeled multigraph. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < https://en.wikipedia.org/wiki/Multigraph > */
/* < https://en.wikipedia.org/wiki/Directed_acyclic_graph > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Rely on a valid implementation of the equality. */
/* Notice that sharing nodes between graphs is undefined. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace prim {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

template <class T> class Graph {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    class Node {
    
    template <class U> friend class Graph;
    
    public:
        Node() : incoming_ (0), edges_ (false), from_ (nullptr), to_ (nullptr)
        {
        }
    
    public:
        Node (const Node&) = delete;
        Node& operator = (const Node&) = delete;
        
    public:
        T& getObject()
        {
            return object_;
        }

        const T& getObject() const
        {
            return object_;
        }
    
    private:
        T object_;
        mutable int incoming_;                  /* For topologic sort. */
        Table<Pointer<Node>, bool> edges_;
        Pointer<Node> from_;                    /* Only when Node is an edge. */
        Pointer<Node> to_;                      /* Ditto. */
    
    private:
        PRIM_LEAK_DETECTOR (Node)
    };

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Proxy to prevent misleading use of an edge as a node. */

public:
    class Edge {
        
    public:
        Edge (const Pointer<Node>& p) : ptr_ (p)
        {
        }
    
    public:
        Edge (const Edge&) = default;
        Edge (Edge&&) = default;
        Edge& operator = (const Edge&) = default;
        Edge& operator = (Edge&&) = default;
        
    public:
        Pointer<Node> operator ->() const
        {
            return ptr_;
        }
    
    private:
        Pointer<Node> ptr_;
    
    private:
        PRIM_LEAK_DETECTOR (Edge)
    };

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Graph() : root_ (nullptr), nodes_ (false), hasOwnership_ (true) 
    {
    }
    
    virtual ~Graph() 
    {
        clear();
    }

public:
    Graph<T> (const Graph<T>&) = delete;
    Graph<T> & operator = (const Graph<T>&) = delete;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    int size()
    {
        return nodes_.size();
    }
    
    void clear()
    {
        root_ = nullptr;
        
        if (hasOwnership_) {
            for (int i = nodes_.size() - 1; i >= 0; --i) { removeEdges (nodes_.getAtIndex (i).getKey()); }
        }
        
        nodes_.clear();
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void add (const Pointer<Node>& p)
    {
        if (p && (contains (p) == false)) {
        //
        if (!root_) { root_ = p; }
        
        nodes_[p] = true;
        //
        }
    }

    Edge connect (const Pointer<Node>& a, const Pointer<Node>& b)
    {
        if (contains (a) && contains (b)) {
        //
        Pointer<Node> edge (new Node());
        edge->from_ = a;
        edge->to_ = b;
      
        a->edges_[edge] = true;
        b->edges_[edge] = true;
      
        return edge;
        //
        }
        
        return Pointer<Node> (nullptr);
    }

    bool contains (const Pointer<Node>& p) const
    {
        if (p) { return nodes_[p]; }
        else {
            return false;
        }
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Pointer<Node> getRoot() const
    {
        return root_;
    }

    Array<Pointer<Node>> getAllNodes() const
    {
        Array<Pointer<Node>> nodes;
        nodes.resize (nodes_.size());
        for (int i = 0; i < nodes.size(); ++i) { nodes[i] = nodes_.getAtIndex (i).getKey(); }
        return nodes;
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < http://cs.brown.edu/courses/cs016/lectures/14%20DAGS%20and%20Top%20Sort.pdf > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

public:
    Array<Pointer<Node>> getAllNodesSorted() const
    {
        Array<Pointer<Node>> sorted;
        
        for (int i = 0; i < nodes_.size(); ++i) { 
            Pointer<Node> p = nodes_.getAtIndex (i).getKey();
            for (int j = 0; j < p->edges_.size(); ++j) {
                Pointer<Node> e = p->edges_.getAtIndex (j).getKey();
                if (e->to_ == p) { p->incoming_++; }
            }
        }
        
        std::queue<Pointer<Node>> queue;
        
        for (int i = 0; i < nodes_.size(); ++i) { 
            Pointer<Node> p = nodes_.getAtIndex (i).getKey();
            if (p->incoming_ == 0) { queue.push (std::move (p)); }
        }
        
        while (!queue.empty()) {
        //
        Pointer<Node> p = queue.front(); queue.pop();
        sorted.add (p);
        
        for (int i = 0; i < p->edges_.size(); ++i) {
            Pointer<Node> e = p->edges_.getAtIndex (i).getKey();
            if (e->from_ == p) { 
                e->to_->incoming_--;
                if (e->to_->incoming_ == 0) { queue.push (e->to_); }
            }
        }
        //
        }
        
        if (sorted.size() != nodes_.size()) { sorted.clear(); }
        
        return sorted;
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    bool isCyclic()
    {
        return (getAllNodesSorted().size() == 0);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Array<Pointer<Node>> allNextByEdge (const Pointer<Node>& p, const T& v)
    {
        Array<Pointer<Node>> t;
      
        if (contains (p)) {
        //
        for (int i = 0; i < p->edges_.size(); ++i) {
            Pointer<Node> edge = p->edges_.getAtIndex (i).getKey();
            if ((edge->from_ == p) && (edge->object_ == v)) { t.add (edge->to_); }
        }
        //
        }
        
        return t;
    }
    
    Array<Pointer<Node>> allPreviousByEdge (const Pointer<Node>& p, const T& v)
    {
        Array<Pointer<Node>> t;
      
        if (contains (p)) {
        //
        for (int i = 0; i < p->edges_.size(); ++i) {
            Pointer<Node> edge = p->edges_.getAtIndex (i).getKey();
            if ((edge->to_ == p) && (edge->object_ == v)) { t.add (edge->from_); }
        }
        //
        }
        
        return t;
    }
    
    Pointer<Node> nextByEdge (const Pointer<Node>& p, const T& v)
    {
        Array<Pointer<Node>> next = allNextByEdge (p, v);
        
        if (next.size() == 1) { return next.getFirst(); }
        else {
            return Pointer<Node> (nullptr);
        }
    }

    Pointer<Node> previousByEdge (const Pointer<Node>& p, const T& v)
    {
        Array<Pointer<Node>> previous = allPreviousByEdge (p, v);
        
        if (previous.size() == 1) { return previous.getFirst(); }
        else {
            return Pointer<Node> (nullptr);
        }
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void drain (Graph& o)        /* Take ownership of (aka move) the nodes. */
    {
        if ((this != &o) && (o.size())) {
        //
        if (nodes_.size()) { 
        //
        Table<Pointer<Node>, bool> scoped (nodes_);
        for (int i = 0; i < o.nodes_.size(); ++i) { scoped[o.nodes_.getAtIndex (i).getKey()] = true; }
        nodes_.swapWith (scoped);
        //
        } else {
        //
        root_.swapWith (o.root_); 
        nodes_.swapWith (o.nodes_); 
        //
        } 
        
        o.hasOwnership_ = false; 
        o.clear();
        o.hasOwnership_ = true;
        //
        }
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void removeEdges (const Pointer<Node>& p)
    {
        if (contains (p)) {
        //
        Table<Pointer<Node>, bool> scoped (p->edges_);
        
        for (int i = scoped.size() - 1; i >= 0; --i) {   
        //
        Pointer<Node> e = scoped.getAtIndex (i).getKey();
        e->from_->edges_[e] = false;
        e->to_->edges_[e] = false;
        e->from_->edges_.sortAndPruneNecessary();
        e->to_->edges_.sortAndPruneNecessary();
        e->from_ = nullptr;
        e->to_ = nullptr;
        //
        }
        //
        }
    }
    
private:
    Pointer<Node> root_;
    Table<Pointer<Node>, bool> nodes_;
    bool hasOwnership_;

private:
    PRIM_LEAK_DETECTOR (Graph)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace prim

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
