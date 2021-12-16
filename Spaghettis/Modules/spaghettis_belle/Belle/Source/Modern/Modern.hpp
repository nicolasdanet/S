
/* 
    Copyright (c) 2007-2013 Andi Brae.
    Copyright (c) 2013 Jojo and others.
    
*/

/* < http://opensource.org/licenses/BSD-2-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Modern western music notation. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < http://icking-music-archive.org/lists/sottisier/notation.pdf > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* For now multiple voices and layers are NOT implemented. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "Music/Label.hpp"
#include "Music/Graphic.hpp"
#include "Music/Stamp.hpp"
#include "Music/Instant.hpp"
#include "Music/Part.hpp"
#include "Music/State.hpp"
#include "Functors/Abstract.hpp"
#include "Functors/Typesetters.hpp"
#include "Functors/Engravers.hpp"
#include "Music/Base.hpp"
#include "Music/Chords.hpp"
#include "Music/Music.hpp"
#include "Music/Paint.hpp"
#include "Helpers/Direction.hpp"
#include "Helpers/Stems.hpp"
#include "Helpers/Hull.hpp"
#include "Helpers/Beaming.hpp"
#include "Helpers/Tupletting.hpp"
#include "Helpers/Adjust.hpp"
#include "Helpers/Spacing.hpp"
#include "Helpers/Density.hpp"
#include "Helpers/Select.hpp"
#include "Functors/Typesetting.hpp"
#include "Functors/Engraving.hpp"
#include "Builders/GrandStaff.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
