
/* 
    Copyright (c) 2007-2013 Andi Brae.
    Copyright (c) 2013 Jojo and others.
    
*/

/* < http://opensource.org/licenses/BSD-2-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef BELLE_FREETYPE_HPP_
#define BELLE_FREETYPE_HPP_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < http://www.freetype.org/freetype2/docs/reference/ft2-index.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Tested with 2.10.4 version. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include <ft2build.h>

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include FT_FREETYPE_H 
#include FT_GLYPH_H
#include FT_OUTLINE_H
#include FT_ADVANCES_H

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace belle {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

struct FTCallback {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

struct Data {

public:
    double normalize_;
    Glyph* glyph_;
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static int moveTo (const FT_Vector* end, void* p)
{
    Data* data = reinterpret_cast<Data*> (p);
    Point pt (static_cast<double> (end->x), static_cast<double> (end->y));
    pt *= data->normalize_;
    data->glyph_->path_.add (Instruction::moveTo (pt));
    return 0;
}

static int lineTo (const FT_Vector* end, void* p)
{
    Data* data = reinterpret_cast<Data*> (p);
    Point pt (static_cast<double> (end->x), static_cast<double> (end->y));
    pt *= data->normalize_;
    data->glyph_->path_.add (Instruction::lineTo (pt));
    return 0;
}

static int conicTo (const FT_Vector* cp, const FT_Vector* end, void* p)
{
    Data* data = reinterpret_cast<Data*> (p);
  
    Point p0;
    Point p1 (static_cast<double> (cp->x), static_cast<double> (cp->y));
    Point p2 (static_cast<double> (end->x), static_cast<double> (end->y));
    
    if (data->glyph_->path_.getInstructions().size()) { 
    //
    p0 = data->glyph_->path_.getInstructions().getLast().getEnd();
    //
    }
  
    p1 *= data->normalize_;
    p2 *= data->normalize_;
    
    /* Make a cubic from a quadratic. */
    
    Bezier b (p0, p1, p2);
    data->glyph_->path_.add (Instruction::withBezier (b));

    return 0;
}

static int cubicTo (const FT_Vector* cp1, const FT_Vector* cp2, const FT_Vector* end, void* p)
{
    Data* data = reinterpret_cast<Data*> (p);
    
    Point p0 (static_cast<double> (cp1->x), static_cast<double> (cp1->y));
    Point p1 (static_cast<double> (cp2->x), static_cast<double> (cp2->y));
    Point p2 (static_cast<double> (end->x), static_cast<double> (end->y));
    
    p0 *= data->normalize_;
    p1 *= data->normalize_;
    p2 *= data->normalize_;
    
    data->glyph_->path_.add (Instruction::cubicTo (p0, p1, p2));
    
    return 0;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#ifdef BELLE_COMPILE_INLINE

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool Typeface::importFont (const char* filename)
{
    Array<byte> rawData;
    bool err = (File::readToArray (filename, rawData) == false);
    
    if (!err) {
    //
    FT_Library library;
    err |= (FT_Init_FreeType (&library) != 0);
    
    if (!err) {
    //
    FT_Face face;
    err |= (FT_New_Memory_Face (library, &rawData[0], rawData.size(), 0, &face) != 0);
    
    if (!err) {
    
        Typeface scoped;
        
        /* Callbacks for outline decomposition. */
        
        FT_Outline_Funcs callbacks;
        
        callbacks.shift    = 0;
        callbacks.delta    = 0;
        callbacks.move_to  = FTCallback::moveTo;
        callbacks.line_to  = FTCallback::lineTo;
        callbacks.conic_to = FTCallback::conicTo;
        callbacks.cubic_to = FTCallback::cubicTo;
      
        /* Typeface values. */
        
        double normalize = 1.0;
        
        if (face->units_per_EM) { normalize /= static_cast<double> (face->units_per_EM); }
    
        scoped.height_    = normalize * face->height;
        scoped.ascender_  = normalize * face->ascender;
        scoped.descender_ = normalize * face->descender;
    
        /* Load glyphs if possible. */
        
        FT_UInt index = 0;
        FT_ULong code = 0;
        
        code = FT_Get_First_Char (face, &index);
        
        do {
        //
        FT_Int32 flags = 0;
        
        flags |= static_cast<FT_Int32> (FT_LOAD_NO_SCALE);
        flags |= static_cast<FT_Int32> (FT_LOAD_NO_BITMAP);
        flags |= static_cast<FT_Int32> (FT_LOAD_IGNORE_TRANSFORM);
        flags |= static_cast<FT_Int32> (FT_LOAD_NO_HINTING);
        
        if (!FT_Load_Glyph (face, index, flags) && (face->glyph->format == FT_GLYPH_FORMAT_OUTLINE)) {
        //
        FT_Glyph glyph;
        FT_Fixed advance = 0;
        
        if (!FT_Get_Advance (face, index, flags, &advance) && !FT_Get_Glyph (face->glyph, &glyph)) {
        //
        scoped.glyphs_.add (GlyphPointer (static_cast<unicode> (code)));
        scoped.glyphs_.getLast().getRawPointer()->advance_ = advance * normalize;
        
        /* Build path from outlines. */
        
        FT_Outline *outline = &(reinterpret_cast<FT_OutlineGlyph> (glyph)->outline);
        
        FTCallback::Data data;
        data.normalize_ = normalize;
        data.glyph_ = scoped.glyphs_.getLast().getRawPointer();
        
        err |= (FT_Outline_Decompose (outline, &callbacks, static_cast<void*> (&data)) != 0);
        
        FT_Done_Glyph (glyph);
        //
        }
        //
        }
        //
        } while ((code = FT_Get_Next_Char (face, code, &index)));
        
        scoped.glyphs_.sort();
        
        /* Find kerning pairs. */
        
        if (FT_HAS_KERNING (face)) {
        //
        for (int i = 0; i < scoped.glyphs_.size(); ++i) {
        //
        FT_UInt left  = FT_Get_Char_Index (face, scoped.glyphs_[i].getRawPointer()->character_);
        
        for (int j = 0; j < scoped.glyphs_.size(); ++j) {
        //
        FT_UInt right = FT_Get_Char_Index (face, scoped.glyphs_[j].getRawPointer()->character_);
        
        FT_Vector kerning;
        FT_Get_Kerning (face, left, right, FT_KERNING_UNSCALED, &kerning);
        
        if (kerning.x != 0) {
            unicode character = scoped.glyphs_[j].getRawPointer()->character_;
            double advance = static_cast<double> (kerning.x) * normalize; 
            scoped.glyphs_[i].getRawPointer()->kerning_.add (Glyph::Kerning (character, advance));
        }
        //
        }
        //
        }
        //
        }
        
        err |= (FT_Done_Face (face) != 0);
        
        if (!err) { scoped.swapWith (*this); }
        
    }
    
    err |= (FT_Done_FreeType (library) != 0);
    //
    }
    //
    }
    
    return (err == false);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // BELLE_COMPILE_INLINE

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace belle 

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // BELLE_FREETYPE_HPP_
