
/* 
    Copyright (c) 2007-2013 Andi Brae.
    Copyright (c) 2013 Jojo and others.
    
*/

/* < http://opensource.org/licenses/BSD-2-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Engrave and arrange stamps. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace belle { 

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

struct Typesetters {        /* Rely on implicit special member functions. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class Debug : public Typesetter::Partwise {

public:
    void start (Music& music) const override;
    void end (Music& music) const override;
    void endPart (Music& music) const override;
    
public:
    void perform (Music& music, NodePtr island) const override;
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

class Allocate : public Typesetter::Partwise {

public:
    void start (Music& music) const override;
    void end (Music& music) const override;
    void endPart (Music& music) const override;

public:
    void perform (Music& music, NodePtr island) const override;
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

class Parts : public Typesetter::Partwise {

public:
    void start (Music& music) const override;
    void end (Music& music) const override;
    void endPart (Music& music) const override;
    void perform (Music& music, NodePtr island) const override;

private:
    mutable int count_;     /* Initialized in the start method. */
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

class Instants : public Typesetter::Instantwise {

public:
    void start (Music& music) const override;
    void end (Music& music) const override;
    void endInstant (Music& music) const override;
    void perform (Music& music, NodePtr island) const override;

private:
    mutable int count_;     /* Initialized in the start method. */
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

class Pack : public Typesetter::Partwise {

public:
    void start (Music& music) const override;
    void end (Music& music) const override;
    void endPart (Music& music) const override;
    
public:
    void perform (Music& music, NodePtr island) const override;
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

class Spacing : public Typesetter::Partwise {

public:
    void start (Music& music) const override;
    void end (Music& music) const override;
    void endPart (Music& music) const override;
    
public:
    void perform (Music& music, NodePtr island) const override;
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

class Stems : public Typesetter::Partwise {

public:
    void start (Music& music) const override;
    void end (Music& music) const override;
    void endPart (Music& music) const override;

public:
    void perform (Music& music, NodePtr island) const override;
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

class Engrave : public Typesetter::Partwise {
    
public:
    void start (Music& music) const override;
    void end (Music& music) const override;
    void endPart (Music& music) const override;

public:
    void perform (Music& music, NodePtr island) const override;
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

class Flags : public Typesetter::Partwise {

public:
    void start (Music& music) const override;
    void end (Music& music) const override;
    void endPart (Music& music) const override;
    
public:
    void perform (Music& music, NodePtr island) const override;
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

class Beams : public Typesetter::Partwise {

public:
    void start (Music& music) const override;
    void end (Music& music) const override;
    void endPart (Music& music) const override;

public:
    void perform (Music& music, NodePtr island) const override;
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

class Ties : public Typesetter::Partwise {
    
public:
    void start (Music& music) const override;
    void end (Music& music) const override;
    void endPart (Music& music) const override;

public:
    void perform (Music& music, NodePtr island) const override;
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

class Enlarge : public Typesetter::Partwise {

public:
    void start (Music& music) const override;
    void end (Music& music) const override;
    void endPart (Music& music) const override;
    
public:
    void perform (Music& music, NodePtr island) const override;
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

class Rest : public Typesetter::Partwise {
    
public:
    void start (Music& music) const override;
    void end (Music& music) const override;
    void endPart (Music& music) const override;

public:
    void perform (Music& music, NodePtr island) const override;
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

class Tuplets : public Typesetter::Partwise {

public:
    void start (Music& music) const override;
    void end (Music& music) const override;
    void endPart (Music& music) const override;

public:
    void perform (Music& music, NodePtr island) const override;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

private:
    Tuplets (mica::Concept t, mica::Concept n)
    {
        tuplet_ = t;
        number_ = n;
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

public:
    static Tuplets inner()
    {
        return Tuplets (mica::NestedTuplet, mica::NestedTupletNumber);
    }
    
    static Tuplets outer()
    {
        return Tuplets (mica::Tuplet, mica::TupletNumber);
    }
    
private:
    mica::Concept tuplet_;
    mica::Concept number_;
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace belle

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
