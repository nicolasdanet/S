
/* 
    Copyright (c) 2007-2013 Andi Brae.
    Copyright (c) 2013 Jojo and others.
    
*/

/* < http://opensource.org/licenses/BSD-2-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace belle { 

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Music is an acyclic directed labeled graph of islands. */
/* It forms a grid with horizontal partwise and vertical instantwise connexions. */
/* The root is the topmost left island. */
/* An island contains a tree of tokens. */
/* First token of the tree may owned an engraver policy. */
/* Each island allocate a stamp on which to engrave graphics. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class MusicBase {

friend class Music;
friend class GrandStaff;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

public:

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class Chord : public MusicGraph {   /* A temporary graph to be drained by the music graph. */

friend class MusicBase;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Chord (mica::Concept duration) : duration_ (duration)
    {
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Chord& note (mica::Concept chromatic, mica::Concept tie = mica::Undefined)
    {
        begin();
        
        NodePtr t = makeToken (mica::Note, Pointer<Engraver> (new Engravers::Dummy()));
        t->getObject().setAttribute (mica::Value, chromatic);
        if (!tie.isUndefined()) { t->getObject().setAttribute (mica::Tie, tie); }
        add (t); connect (getRoot(), t)->getObject().setAttribute (mica::Type, mica::Token);
        
        return *this;
    }

    Chord& rest()
    {
        return *this;
    }
    
    Chord& beam (mica::Concept concept)
    {
        begin();
        
        if (getRoot()->getObject().getAttribute (mica::Value).toRatio() < Ratio (1, 4)) {
            getRoot()->getObject().setAttribute (mica::Beam, concept);
        }
        
        return *this;
    }

    Chord& tuplet (mica::Concept concept, int n = House::kTupletDefault)
    {
        begin();
        
        getRoot()->getObject().setAttribute (mica::Tuplet, concept);
        getRoot()->getObject().setAttribute (mica::TupletNumber, n);
        
        return *this;
    }
    
    Chord& tupletNested (mica::Concept concept, int n = House::kTupletDefault)
    {
        begin();
        
        getRoot()->getObject().setAttribute (mica::NestedTuplet, concept);
        getRoot()->getObject().setAttribute (mica::NestedTupletNumber, n);
        
        return *this;
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Chord& notes (mica::Concept a, mica::Concept b)
    {
        return note (a).note (b);
    }
    
    Chord& notes (mica::Concept a, mica::Concept b, mica::Concept c)
    {
        return note (a).note (b).note (c);
    }
    
    Chord& notes (mica::Concept a, mica::Concept b, mica::Concept c, mica::Concept d)
    {
        return note (a).note (b).note (c).note (d);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Chord& tied (mica::Concept a, mica::Concept b, mica::Concept tie)
    {
        return note (a, tie).note (b, tie);
    }
    
    Chord& tied (mica::Concept a, mica::Concept b, mica::Concept c, mica::Concept tie)
    {
        return note (a, tie).note (b, tie).note (c, tie);
    }
    
    Chord& tied (mica::Concept a, mica::Concept b, mica::Concept c, mica::Concept d, mica::Concept tie)
    {
        return note (a, tie).note (b, tie).note (c, tie).note (d, tie);
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void begin()        /* Initialize the root token. */
    {
        if (getRoot() == nullptr) {
        //
        NodePtr t = makeToken (mica::Chord, Pointer<Engraver> (new Engravers::Chord()));
        t->getObject().setAttribute (mica::Value, duration_);
        add (t);
        //
        }
    }
    
    void close()        /* A chord without any notes is a rest. */
    {
        begin(); getRoot()->getObject().setAttribute (mica::Size, size() - 1);
    }
    
private:
    mica::Concept duration_;
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    MusicBase()
    {
    }

public:
    MusicBase (const MusicBase&) = delete;
    MusicBase& operator = (const MusicBase&) = delete;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void with (MusicBase& toDrain)
    {
        graph_.clear();
        graph_.drain (toDrain.graph_);
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void clear()
    {
        graph_.clear();
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    NodePtr addBarline (mica::Concept barlineType)
    {
        NodePtr t = makeToken (mica::Barline, Pointer<Engraver> (new Engravers::Barline()));
        t->getObject().setAttribute (mica::Value, barlineType);
        return makeIsland (t);
    }

    NodePtr addClef (mica::Concept clefType)
    {
        NodePtr t = makeToken (mica::Clef, Pointer<Engraver> (new Engravers::Clef()));
        t->getObject().setAttribute (mica::Value, clefType);
        return makeIsland (t);
    }

    NodePtr addKey (mica::Concept keyType)
    {
        return addKeySignature (mica::map (keyType, mica::KeySignature));
    }
    
    NodePtr addTime (int beats, int noteValue)
    {
        return addRegularTimeSignature (beats, noteValue);
    }
    
    NodePtr addTime (mica::Concept timeType)
    {
        if (timeType == mica::CutTime) { return addCutTimeSignature(); }
        else {
            return addCommonTimeSignature();
        }
    }
    
    NodePtr addChord (Chord& chord)     /* A chord with no note is a rest. */
    {
        chord.close(); return makeIsland (chord);
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    NodePtr addKeySignature (mica::Concept keySignature)    /* Argument could be mica::Undefined. */
    {   
        NodePtr t = makeToken (mica::KeySignature, Pointer<Engraver> (new Engravers::KeySignature()));
        t->getObject().setAttribute (mica::Value, keySignature);
        return makeIsland (t);
    }

    NodePtr addRegularTimeSignature (int beats, int noteValue)
    {
        NodePtr t = makeToken (mica::TimeSignature, Pointer<Engraver> (new Engravers::TimeSignature()));
        t->getObject().setAttribute (mica::Value, mica::RegularTimeSignature);
        t->getObject().setAttribute (mica::Beats, Ratio (beats, 1));
        t->getObject().setAttribute (mica::NoteValue, Ratio (1, noteValue));
        return makeIsland (t);
    }
    
    NodePtr addCommonTimeSignature()
    {
        NodePtr t = makeToken (mica::TimeSignature, Pointer<Engraver> (new Engravers::TimeSignature()));
        t->getObject().setAttribute (mica::Value, mica::CommonTime);
        return makeIsland (t);
    }
    
    NodePtr addCutTimeSignature()
    {
        NodePtr t = makeToken (mica::TimeSignature, Pointer<Engraver> (new Engravers::TimeSignature()));
        t->getObject().setAttribute (mica::Value, mica::CutTime);
        return makeIsland (t);
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void connect (NodePtr a, NodePtr b, mica::Concept concept)
    {
        graph_.connect (a, b)->getObject().setAttribute (mica::Type, concept);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

protected:
    NodePtr getToken (NodePtr island)
    {
        return graph_.nextByEdge (island, Labels::token());
    }

    NodePtr getIsland (NodePtr token)
    {
        NodePtr n (token);
        NodePtr previous (nullptr);
        
        while ((previous = graph_.previousByEdge (n, Labels::token()))) { n = previous; }
        
        return n;
    }

    NodePtr getIslandBelow (NodePtr token)
    {
        return graph_.nextByEdge (getIsland (token), Labels::instantwise());
    }
    
    Array<NodePtr> getAllTokensNext (NodePtr token)
    {
        return graph_.allNextByEdge (token, Labels::token());
    }
    
    NodePtr getPreviousToken (NodePtr token)
    {
        return graph_.previousByEdge (token, Labels::token());
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    static NodePtr makeToken (mica::Concept kind, const Pointer<Engraver>& engraver)
    {
        NodePtr t (new Graph<Label>::Node());
        t->getObject().setAttribute (mica::Type, mica::Token);
        t->getObject().setAttribute (mica::Kind, kind);
        t->getObject().setEngraver (engraver);
        return t;
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    NodePtr makeIsland (NodePtr token)
    {
        NodePtr i (new Graph<Label>::Node());
        i->getObject().setAttribute (mica::Type, mica::Island);
        graph_.add (i);
        graph_.add (token);
        graph_.connect (i, token)->getObject().setAttribute (mica::Type, mica::Token);
        return i;
    }
    
    NodePtr makeIsland (MusicGraph& graph)
    {
        NodePtr token = graph.getRoot();
        NodePtr i (new Graph<Label>::Node());
        i->getObject().setAttribute (mica::Type, mica::Island);
        graph_.add (i);
        graph_.drain (graph);
        graph_.connect (i, token)->getObject().setAttribute (mica::Type, mica::Token);
        return i;
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    NodePtr cloneIsland (NodePtr island)
    {
        MusicGraph scoped;
        
        NodePtr token = graph_.nextByEdge (island, Labels::token());
        NodePtr cloned = cloneToken (token);
        scoped.add (cloned);
        cloneTokenRecursive (scoped, cloned, token); 
        
        return makeIsland (scoped);
    }
    
    static NodePtr cloneToken (NodePtr token)
    {
        NodePtr t (new Graph<Label>::Node());
        t->getObject().setAttributes (token->getObject().getAttributes());
        t->getObject().setEngraver (token->getObject().getEngraver()->clone());
        return t;
    }
    
    void cloneTokenRecursive (MusicGraph& scoped, NodePtr cloned, NodePtr token)
    {
        Array<NodePtr> tokens = graph_.allNextByEdge (token, Labels::token());
        
        for (int i = 0; i < tokens.size(); ++i) {
            NodePtr nextCloned = cloneToken (tokens[i]);
            scoped.add (nextCloned);
            scoped.connect (cloned, nextCloned)->getObject().setAttribute (mica::Type, mica::Token);
            cloneTokenRecursive (scoped, nextCloned, tokens[i]);
        }
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    Array<NodePtr> getAllIslandsByInstantwise (NodePtr island)
    {
        Array<NodePtr> nodes;
        
        NodePtr n (island);
        NodePtr previous (nullptr);
        while ((previous = graph_.previousByEdge (n, Labels::instantwise()))) { n = previous; }
        nodes.add (n);
        while ((n = graph_.nextByEdge (n, Labels::instantwise()))) { nodes.add (n); }
        
        return nodes;
    }
    
private:
    MusicGraph graph_;

private:
    PRIM_LEAK_DETECTOR (MusicBase)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace belle

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
