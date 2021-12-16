
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

#ifdef BELLE_COMPILE_INLINE

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Music::proceed (const Typesetter::Partwise& f)
{
    f.start (*this);
    
    NodePtr n = graph_.getRoot();
         
    while (n) {
    //
    NodePtr m = n;
              
    while (m) {
    //
    f.perform (*this, m);
    m = graph_.nextByEdge (m, Labels::partwise());
    //
    }
           
    n = graph_.nextByEdge (n, Labels::instantwise());
    f.endPart (*this);
    //
    }
    
    f.end (*this);
}

void Music::proceed (const Typesetter::Instantwise& f)
{
    f.start (*this);
    
    Array<NodePtr> front (getAllIslandsByInstantwise (graph_.getRoot()));
    
    for (int i = 0; i < front.size(); ++i) { f.perform (*this, front[i]); }
    
    f.endInstant (*this);
  
    while (front.size()) {
    //
    NodePtr n = graph_.nextByEdge (front.getLast(), Labels::partwise());
    
    if (n == nullptr) { front.resize (front.size() - 1); }
    else {
    //
    Array<NodePtr> beyond = getAllIslandsByInstantwise (n);
    Array<NodePtr> previous;
    
    for (int i = 0; i < beyond.size(); ++i) { 
        previous.add (graph_.previousByEdge (beyond[i], Labels::partwise()));
    }
    
    bool isNext = true;
    
    for (int i = 0; i < previous.size(); ++i) { 
        if (front.contains (previous[i]) == false) { isNext = false; break; }
    }
    
    if (isNext == false) { front.rotate(); }
    else {
    //
    for (int i = 0; i < previous.size(); ++i) { 
        for (int j = 0; j < front.size(); ++j) {
            if (front[j] == previous[i]) { front[j] = beyond[i]; f.perform (*this, front[j]); break; }
        }
    }
    
    f.endInstant (*this);
    //
    }
    //
    }
    //
    }
    
    f.end (*this);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Typesetters::Allocate::start (Music& music) const
{
    music.getParts().clear();
    music.getInstants().clear();
}

void Typesetters::Allocate::end (Music&) const
{
}

void Typesetters::Allocate::endPart (Music&) const
{
}

void Typesetters::Allocate::perform (Music&, NodePtr island) const
{
    island->getObject().setStamp (Pointer<Stamp> (new Stamp()));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Typesetters::Parts::start (Music&) const
{
    count_ = 0; 
}

void Typesetters::Parts::end (Music& music) const
{
    music.getParts().resize (count_);
}

void Typesetters::Parts::endPart (Music&) const
{
    count_++;
}

void Typesetters::Parts::perform (Music&, NodePtr island) const
{
    island->getObject().getStamp()->setPart (count_);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Typesetters::Instants::start (Music&) const
{
    count_ = 0;
}

void Typesetters::Instants::end (Music& music) const
{
    music.getInstants().resize (count_);
}

void Typesetters::Instants::endInstant (Music&) const
{
    count_++;
}

void Typesetters::Instants::perform (Music&, NodePtr island) const
{
    island->getObject().getStamp()->setInstant (count_);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Typesetters::Pack::start (Music&) const
{
}

void Typesetters::Pack::end (Music&) const
{
}

void Typesetters::Pack::endPart (Music&) const
{
}

void Typesetters::Pack::perform (Music& music, NodePtr island) const
{
    int i = island->getObject().getStamp()->getInstant();
    
    music.getInstants().get (i).addStamp (island->getObject().getStamp());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Typesetters::Spacing::start (Music& music) const
{
    music.getGroupState().reset();
}

void Typesetters::Spacing::end (Music&) const
{
}

void Typesetters::Spacing::endPart (Music&) const
{
}

void Typesetters::Spacing::perform (Music& music, NodePtr island) const
{
    NodePtr token = music.getToken (island);
    mica::Concept kind = token->getObject().getAttribute (mica::Kind);
    
    if (kind == mica::TimeSignature) { music.getGroupState().reset(); }
    else if (kind != mica::Barline)  { music.getGroupState().add (token); }
    else {
        Array<NodePtr> events (music.getGroupState().reclaimTokens());
        if (Density::needExtraSpace (music, events)) {
            for (int i = 0; i < events.size(); ++i) {
                int instant = music.getIsland (events[i])->getObject().getStamp()->getInstant();
                double old  = music.getInstants().get (instant).getUnit();
                double unit = Density::getUnit (music, events);
                music.getInstants().get (instant).setUnit (Math::max (old, unit));
            }
        }
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Typesetters::Stems::start (Music& music) const
{
    music.getClefState().reset();
    music.getGroupState().reset();
}

void Typesetters::Stems::end (Music&) const
{
}

void Typesetters::Stems::endPart (Music&) const
{
}

void Typesetters::Stems::perform (Music& music, NodePtr island) const
{
    NodePtr token = music.getToken (island);
    
    music.getClefState().parse (token);
        
    if (token->getObject().getAttribute (mica::Kind) == mica::Chord) { 
    //
    if (token->getObject().getAttribute (mica::Size).getNumerator()) { 
    //
    mica::Concept beam = token->getObject().getAttribute (mica::Beam);
    
    if (beam != mica::Undefined) { music.getGroupState().add (token); }
    if (beam == mica::End)       { Direction::setStems (music);       }
    //
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Typesetters::Engrave::start (Music& music) const
{
    music.getClefState().reset();
    music.getGroupState().reset();
}

void Typesetters::Engrave::end (Music&) const
{
}

void Typesetters::Engrave::endPart (Music&) const
{
}

void Typesetters::Engrave::perform (Music& music, NodePtr island) const
{
    Pointer<Stamp> stamp = island->getObject().getStamp();
    NodePtr token = music.getToken (island);
    
    /* Requiered to determine accidentals in chords. */
    
    music.getClefState().parse (token);
    
    /* Ensure extra care for spacing of bracket in mixed tuplets. */
    
    if (token->getObject().getAttribute (mica::Kind) == mica::Chord) { 
    //
    mica::Concept tuplet = token->getObject().getAttribute (mica::Tuplet);
    mica::Concept nested = token->getObject().getAttribute (mica::NestedTuplet);
    
    if (tuplet != mica::Undefined || nested != mica::Undefined) { music.getGroupState().add (token); }
    else {
        music.getGroupState().reset();
    }
    //
    }
    
    /* Engrave the symbol associated with the token. */
    
    Pointer<Engraver> engraver = token->getObject().getEngraver();
    engraver->setSelected (Select::isSelected (island));
    engraver->engrave (music, token, stamp);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Typesetters::Flags::start (Music&) const
{
}

void Typesetters::Flags::end (Music&) const
{
}

void Typesetters::Flags::endPart (Music&) const
{
}

void Typesetters::Flags::perform (Music& music, NodePtr island) const
{
    Pointer<Stamp> stamp = island->getObject().getStamp();
    NodePtr token = music.getToken (island);
    
    if (token->getObject().getAttribute (mica::Kind) == mica::Chord) {
        if (token->getObject().getAttribute (mica::Beam) == mica::Undefined) {
            Engravers::Flags().setSelected (Select::isSelected (island)).engrave (music, token, stamp); 
        }
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Typesetters::Beams::start (Music& music) const
{
    music.getClefState().reset();
    music.getGroupState().reset();
}

void Typesetters::Beams::end (Music&) const
{
}

void Typesetters::Beams::endPart (Music&) const
{
}

void Typesetters::Beams::perform (Music& music, NodePtr island) const
{
    Pointer<Stamp> stamp = island->getObject().getStamp();
    NodePtr token = music.getToken (island);
    
    music.getClefState().parse (token);
        
    if (token->getObject().getAttribute (mica::Kind) == mica::Chord) { 
    //
    if (token->getObject().getAttribute (mica::Size).getNumerator()) {
    //
    mica::Concept beam = token->getObject().getAttribute (mica::Beam);
    
    if (beam != mica::Undefined) { music.getGroupState().add (token); }
    else {
        music.getGroupState().reset();
    }
    
    if (beam == mica::End) { 
        Engravers::Beam().setSelected (Select::isGroupSelected (music)).engrave (music, token, stamp); 
    }
    //
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Typesetters::Ties::start (Music& music) const
{
    music.getChordState().reset();
}

void Typesetters::Ties::end (Music&) const
{
}

void Typesetters::Ties::endPart (Music&) const
{
}

void Typesetters::Ties::perform (Music& music, NodePtr island) const
{
    Pointer<Stamp> stamp = island->getObject().getStamp();
    NodePtr token = music.getToken (island);
    
    if (token->getObject().getAttribute (mica::Kind) == mica::Chord) {
    //
    if (token->getObject().getAttribute (mica::Size).getNumerator()) { 
    //
    Array<NodePtr> notes = music.getAllTokensNext (token);
    
    for (int i = 0; i < notes.size(); ++i) {
        mica::Concept tie = notes[i]->getObject().getAttribute (mica::Tie);
        if (tie == mica::Middle || tie == mica::End) { 
            bool selected = Select::isPreviousSelected (music, island);
            Engravers::Tie().setSelected (selected).engrave (music, notes[i], stamp); 
        }
        music.getChordState().parse (island, notes[i], stamp);
    }
    
    music.getChordState().parse (island, token, stamp);
    //
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Typesetters::Enlarge::start (Music&) const
{
}

void Typesetters::Enlarge::end (Music&) const
{
}

void Typesetters::Enlarge::endPart (Music&) const
{
}

void Typesetters::Enlarge::perform (Music& music, NodePtr island) const
{
    Pointer<Stamp> stamp = island->getObject().getStamp();
    NodePtr token = music.getToken (island);
    
    /* Enlarge barlines if needed. */
    /* At this point extra spaces for parts have been defined. */
    
    if (token->getObject().getAttribute (mica::Kind) == mica::Barline) {
        if (token->getObject().getAttribute (mica::Large) == mica::True) {
            stamp->erase();
            token->getObject().getEngraver()->engrave (music, token, stamp);
        }
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Typesetters::Rest::start (Music& music) const
{
    music.getGroupState().reset();
}

void Typesetters::Rest::end (Music&) const
{
}

void Typesetters::Rest::endPart (Music&) const
{
}

void Typesetters::Rest::perform (Music& music, NodePtr island) const
{
    NodePtr token = music.getToken (island);
    mica::Concept kind = token->getObject().getAttribute (mica::Kind);
    
    if (kind == mica::Chord || kind == mica::Barline) { music.getGroupState().add (token); }
    if (kind == mica::Barline) { Adjust::rests (music); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Typesetters::Tuplets::start (Music& music) const
{
    music.getGroupState().reset();
}

void Typesetters::Tuplets::end (Music&) const
{
}

void Typesetters::Tuplets::endPart (Music&) const
{
}

void Typesetters::Tuplets::perform (Music& music, NodePtr island) const
{
    Pointer<Stamp> stamp = island->getObject().getStamp();
    NodePtr token = music.getToken (island);
            
    if (token->getObject().getAttribute (mica::Kind) == mica::Chord) { 
    //
    mica::Concept tuplet = token->getObject().getAttribute (tuplet_);
    
    if (tuplet != mica::Undefined) { music.getGroupState().add (token); }
    else {
        music.getGroupState().reset();
    }
    
    if (tuplet == mica::End) { 
        token->getObject().setAttribute (mica::Integer, token->getObject().getAttribute (number_));
        Engravers::Tuplet().setSelected (Select::isGroupSelected (music)).engrave (music, token, stamp); 
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // BELLE_COMPILE_INLINE

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace belle

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
