
/* 
    Copyright (c) 2007-2013 Andi Brae.
    Copyright (c) 2013 Jojo and others.
    
*/

/* < http://opensource.org/licenses/BSD-2-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Implementation of the engravers. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace belle { 

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#ifdef BELLE_COMPILE_INLINE

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Engravers::Dummy::engrave (Music&, NodePtr, const Pointer<Stamp>&)
{
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void Engravers::Chord::engrave (Music& music, NodePtr token, const Pointer<Stamp>& stamp)
{
    Shapes::Chord shape;
    
    shape.setDuration (token->getObject().getAttribute (mica::Value));
    shape.setBeam (token->getObject().getAttribute (mica::Beam));
    shape.setDirection (token->getObject().getAttribute (mica::Stem));
    
    /* Notes and accidentals. */
    
    Array<NodePtr> notes = music.getAllTokensNext (token);
    
    for (int i = 0; i < notes.size(); ++i) {
    //
    mica::Concept tie = notes[i]->getObject().getAttribute (mica::Tie);
    mica::Concept chromatic = notes[i]->getObject().getAttribute (mica::Value);
    mica::Concept linespace = music.getClefState().getLinespace (chromatic);
    
    if (tie == mica::Middle || tie == mica::End) { 
        shape.addNote (linespace, mica::Undefined, chromatic, true); 
    } else {
        shape.addNote (linespace, music.getClefState().getAccidental (chromatic), chromatic, false);
    }
    //
    }
    
    /* Engrave the shape. */
    
    Color color = isSelected() ? House::kColorSelected : House::kColorDefault;
    stamp->add (Pointer<Graphic> (new Graphic (Path (shape), color)));
    
    /* Update the stem direction. */
    
    token->getObject().setAttribute (mica::Stem, shape.getDirection());
    
    /* Set the weight for spacing. */
    
    stamp->setWeight (Utils::getWeight (Tupletting::getNoteValue (token)));
    
    /* Ensure extra care for spacing of bracket in mixed tuplets. */
    
    mica::Concept tuplet = token->getObject().getAttribute (mica::Tuplet);
    mica::Concept nested = token->getObject().getAttribute (mica::NestedTuplet);
    
    if (tuplet == mica::End || nested == mica::End) { 
    //
    Array<NodePtr> tupletted (music.getGroupState().reclaimTokens());
    
    if (Tupletting::isMixed (tupletted)) {
        int instant = music.getIsland (tupletted.getFirst())->getObject().getStamp()->getInstant();
        double unit = music.getInstants().get (instant).getUnit();
        double weight = Utils::getWeight (Tupletting::getNoteValue (token));
        Affine affine = Affine::translation (Vector ((weight * unit) - House::kNoteHeadWidth, 0.0));
        stamp->add (Pointer<Graphic> (new Graphic (Path (Shapes::Extra (affine)), color)));
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void Engravers::Flags::engrave (Music&, NodePtr token, const Pointer<Stamp>& stamp) 
{
    Ratio duration = token->getObject().getAttribute (mica::Value).toRatio();
    
    if (!duration.isUndefined() && (duration < Ratio (1, 4))) {
    //
    Box stem = stamp->getBox (mica::Stem);
    int flags = Utils::countFlags (duration);
    
    if (flags && (stem != Box::empty())) { 
    //
    Path path;
    
    if (token->getObject().getAttribute (mica::Stem) == mica::Up) {
        Affine offset = Affine::translation (Vector (0.0, -House::kFlagDistance));
        Affine origin = Affine::translation (stem.getTopRight());
        for (int i = 0; i < flags; ++i) { 
            path.addShape (Shapes::FlagStemUp(), origin); 
            origin *= offset; 
        }
        
    } else {
        Affine offset = Affine::translation (Vector (0.0, House::kFlagDistance));
        Affine origin = Affine::translation (stem.getBottomRight());
        for (int i = 0; i < flags; ++i) { 
            path.addShape (Shapes::FlagStemDown(), origin); 
            origin *= offset; 
        }
    }
    
    /* Engrave the shape. */
    
    Color color = isSelected() ? House::kColorSelected : House::kColorDefault;
    
    stamp->add (Pointer<Graphic> (new Graphic (std::move (path), color)));
    //
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void Engravers::Tie::engrave (Music& music, NodePtr token, const Pointer<Stamp>& stamp) 
{
    /* The token parameter is the note. */
    
    mica::Concept chromatic = token->getObject().getAttribute (mica::Value);
    
    Box b1 = music.getChordState().getBox (chromatic);
    Box b2 = stamp->getBox (chromatic);
    
    if (b1 != Box::empty() && b2 != Box::empty()) {
    //
    NodePtr chord = music.getPreviousToken (token);
    
    Affine affine = Affine::inversed (stamp->getContext());
    Shapes::Tie tie (affine.appliedTo (b1), b2);
    
    int size1 = (music.getChordState().getAttribute (mica::Size)).getNumerator();
    int size2 = (chord->getObject().getAttribute (mica::Size)).getNumerator();
    
    mica::Concept stem1 = music.getChordState().getAttribute (mica::Stem);
    mica::Concept stem2 = chord->getObject().getAttribute (mica::Stem);
        
    tie.setDirections (stem1, stem2);
    
    if (size1 > 1 || size2 > 1) { 
        tie.setPosition (stamp->getProperty (chromatic));      /* Determined by Shapes::Chord. */
        tie.isChord (true);
        
    } else {
        if (stem1 == mica::Up && stem2 == mica::Up) { tie.setPosition (mica::Below); }
    }
    
    if (b1.getWidth() > House::kTieAdjustThresholdHead) { 
        tie.setPathAtStart (Path (music.getChordState().getPath (chromatic), affine)); 
    }
    
    if (b2.getWidth() > House::kTieAdjustThresholdHead) {  
        tie.setPathAtEnd (stamp->getPath());
    }
    
    /* Engrave the shape. */
    
    Color color = isSelected() ? House::kColorSelected : House::kColorDefault;
    stamp->add (Pointer<Graphic> (new Graphic (Path (tie), color)));
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void Engravers::Barline::engrave (Music& music, NodePtr token, const Pointer<Stamp>& stamp)
{
    NodePtr tokenBelow = music.getToken (music.getIslandBelow (token));
    
    mica::Concept type = token->getObject().getAttribute (mica::Value);
        
    Path path;
    
    if ((tokenBelow != nullptr) && (type == tokenBelow->getObject().getAttribute (mica::Value))) {
    //
    token->getObject().setAttribute (mica::Large, mica::True);
    
    double extra = music.getParts().get (stamp->getPart()).getExtra();
    
    if (type == mica::DoubleBarline)         { path.addShape (Shapes::DoubleBarlineLarge (extra)); }
    else if (type == mica::EndBarline)       { path.addShape (Shapes::EndBarlineLarge (extra)); }
    else if (type == mica::EndRepeatBarline) { path.addShape (Shapes::EndRepeatBarlineLarge (extra)); }
    else                                     { path.addShape (Shapes::BarlineLarge (extra)); }
    //
    } else {
    //
    if (type == mica::DoubleBarline)         { path.addShape (Shapes::DoubleBarline()); }
    else if (type == mica::EndBarline)       { path.addShape (Shapes::EndBarline()); }
    else if (type == mica::EndRepeatBarline) { path.addShape (Shapes::EndRepeatBarline()); }
    else                                     { path.addShape (Shapes::Barline()); }
    //
    }
    
    /* Engrave the shape. */
    
    Color color = isSelected() ? House::kColorSelected : House::kColorDefault;
    
    stamp->add (Pointer<Graphic> (new Graphic (std::move (path), color)));
    
    /* Set the weight for spacing. */
    
    if (type != mica::BeginBarline) { stamp->setWeight (Utils::getWeight (House::kSpacingBarline)); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void Engravers::Clef::engrave (Music&, NodePtr token, const Pointer<Stamp>& stamp)
{
    mica::Concept type = token->getObject().getAttribute (mica::Value);
    mica::Concept symbol = mica::map (type, mica::Symbol);
    
    /* Engrave the shape. */
    
    Color color = isSelected() ? House::kColorSelected : House::kColorDefault;
    
    if (symbol == mica::GClef) { 
        stamp->add (Pointer<Graphic> (new Graphic (Path (Shapes::GClef()), color)));
    } else { 
        stamp->add (Pointer<Graphic> (new Graphic (Path (Shapes::FClef()), color)));
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void Engravers::TimeSignature::engrave (Music&, NodePtr token, const Pointer<Stamp>& stamp)
{
    mica::Concept type = token->getObject().getAttribute (mica::Value);
    
    Path path;
    
    if (type == mica::RegularTimeSignature) {
        int n = token->getObject().getAttribute (mica::Beats).getNumerator();
        int d = token->getObject().getAttribute (mica::NoteValue).getDenominator();
        path.addShape (Shapes::RegularTime (n, d));

    } else {
        if (type == mica::CommonTime)   { path.addShape (Shapes::CommonTime()); }
        else if (type == mica::CutTime) { path.addShape (Shapes::CutTime()); }
    }

    /* Engrave the shape. */
    
    Color color = isSelected() ? House::kColorSelected : House::kColorDefault;
    
    stamp->add (Pointer<Graphic> (new Graphic (std::move (path), color)));
    
    /* Set the weight for spacing. */
    
    stamp->setWeight (Utils::getWeight (House::kSpacingTimeSignature));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void Engravers::KeySignature::engrave (Music& music, NodePtr token, const Pointer<Stamp>& stamp) 
{
    mica::Concept key = token->getObject().getAttribute (mica::Value);
    mica::Concept clef = music.getClefState().getAttribute (mica::Clef);
    
    /* Engrave the shape. */
    
    Color color = isSelected() ? House::kColorSelected : House::kColorDefault;
    stamp->add (Pointer<Graphic> (new Graphic (Path (Shapes::KeySignature (key, clef)), color)));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void Engravers::Beam::engrave (Music& music, NodePtr token, const Pointer<Stamp>& stamp) 
{
    Array<NodePtr> beamed (music.getGroupState().reclaimTokens());
    
    /* At this point all notes in the group must have stems in the same direction. */
    
    mica::Concept direction = token->getObject().getAttribute (mica::Stem);
    double slant = Beaming::getSlant (music, beamed, direction);
    Array<Box> stems (Stems::getBoxes (music, beamed, stamp->getContext()));
    
    /* Consider secondary beams. */
    
    Array<int> flags (Stems::getFlags (beamed));
    
    /* Due to path filling artifacts, the engraving needs two passes. */ 
    
    Shapes::Beam beam (stems, direction);

    Color color = isSelected() ? House::kColorSelected : House::kColorDefault;
    stamp->add (Pointer<Graphic> (new Graphic (Path (beam.setSlant (slant)), color)));
    stamp->add (Pointer<Graphic> (new Graphic (Path (beam.setFlags (flags)), color)));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void Engravers::Tuplet::engrave (Music& music, NodePtr token, const Pointer<Stamp>& stamp) 
{
    Array<NodePtr> tupletted (music.getGroupState().reclaimTokens());
    
    /* Get the tuplet value. */
    
    int n = token->getObject().getAttribute (mica::Integer).getNumerator();
    
    /* Determine the tuplet type. */
    
    bool hasBeam = false;
    bool hasRest = false;
    
    if (tupletted.size()) {
    //
    hasBeam |= (tupletted.getFirst()->getObject().getAttribute (mica::Beam) == mica::Beginning);
    hasBeam &= (tupletted.getLast()->getObject().getAttribute (mica::Beam) == mica::End);
    
    hasRest |= (tupletted.getFirst()->getObject().getAttribute (mica::Size).getNumerator() == 0);
    hasRest |= (tupletted.getLast()->getObject().getAttribute (mica::Size).getNumerator() == 0);
    //
    }
    
    for (int i = 0; i < tupletted.size(); ++i) {
        if (tupletted[i]->getObject().getAttribute (mica::Beam) == mica::Undefined) { 
            if (tupletted[i]->getObject().getAttribute (mica::Size).getNumerator() != 0) { 
                hasBeam = false; break; 
            }
        }
    }
    
    bool forceBracket = false;      /* Avoid collision with staff lines. */
    
    Color color = isSelected() ? House::kColorSelected : House::kColorDefault;
        
    /* Numeral case. */ 
    
    if (hasBeam && !hasRest) {
    //
    mica::Concept direction = token->getObject().getAttribute (mica::Stem);
    Box beam = stamp->getBox (mica::Beam);
    int linespace = Utils::getLinespace (beam.getCenter().getY());
    
    forceBracket |= (direction == mica::Up) && (linespace <= 0);
    forceBracket |= (direction == mica::Down) && (linespace >= 0);
    
    if (!forceBracket) {
        Shapes::Tuplet::Numeral numeral (beam, direction, n);
        stamp->add (Pointer<Graphic> (new Graphic (Path (numeral), color)));
    }    
    //
    }
    
    /* Bracket case. */ 
    
    if (!hasBeam || hasRest || forceBracket) {
    //
    Array<Box> stems (Stems::getBoxes (music, tupletted, stamp->getContext()));
    Array<mica::Concept> directions (Stems::getDirections (tupletted));
    
    Polygon hull (Hull::getHull (music, tupletted, stamp->getContext()));

    mica::Concept position;
    double angle = 0.0;
    double weight = 0.0;        /* Extra space for mixed tuplet (e.g. with rest or unequal values). */
    
    if (forceBracket) {
        mica::Concept direction = token->getObject().getAttribute (mica::Stem);
        Box beam = stamp->getBox (mica::Beam);
        angle = Beaming::getAngle (beam, stamp->getProperty (mica::Beam));
        position = ((direction == mica::Down) ? mica::Above : mica::Below);
        
    } else {
        angle = Tupletting::getAngle (stems, directions);
        position = Tupletting::getPosition (tupletted, directions);
        weight = Tupletting::getWeight (tupletted);
    }
    
    /* Spacing unit at the first instant of the tuplet. */
    
    int instant = music.getIsland (tupletted.getFirst())->getObject().getStamp()->getInstant();
    double unit = music.getInstants().get (instant).getUnit();
    
    Shapes::Tuplet::Bracket bracket (stems, directions, hull, n);
    bracket.setAngle (angle);
    bracket.setPosition (position);
    bracket.setUnit (unit);
    bracket.setWeight (weight);
    stamp->add (Pointer<Graphic> (new Graphic (Path (bracket), color)));
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
