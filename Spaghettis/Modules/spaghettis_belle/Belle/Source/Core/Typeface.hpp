
/* 
    Copyright (c) 2007-2013 Andi Brae.
    Copyright (c) 2013 Jojo and others.
    
*/

/* < http://opensource.org/licenses/BSD-2-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* A sorted collection of Glyphs. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < http://www.freetype.org/freetype2/docs/glyphs/index.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace belle {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class Typeface {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    class GlyphPointer {    /* Sortable by unicode. */
    
    public:
        GlyphPointer (unicode character = 0x0000) : glyph_ (new Glyph())
        {
            if (character) { glyph_->character_ = character; }
        }
    
        GlyphPointer (const GlyphPointer&) = default;
        GlyphPointer (GlyphPointer&&) = default;
        GlyphPointer& operator = (const GlyphPointer&) = default;
        GlyphPointer& operator = (GlyphPointer&&) = default;

    public:
        void swapWith (GlyphPointer& o)
        {
            glyph_.swapWith (o.glyph_);
        }
        
    public:
        Glyph* getRawPointer() const
        {
            return glyph_.get();
        }
        
    public:
        bool operator < (const GlyphPointer& o) const
        {
            return glyph_->character_ < o.glyph_->character_;
        }

        bool operator > (const GlyphPointer& o) const
        {
            return glyph_->character_ > o.glyph_->character_;
        }

        bool operator == (const GlyphPointer& o) const
        {
            return glyph_->character_ == o.glyph_->character_;
        }
    
    private:
        Pointer<Glyph> glyph_;
    
    private:
        PRIM_LEAK_DETECTOR (GlyphPointer)
    };

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Note that glyphs must always been kept sorted. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Typeface() : height_ (0.0), ascender_ (0.0), descender_ (0.0), undefined_ (new Glyph())
    {
        undefined_->advance_ = 0.5;
        undefined_->path_.addRectangle (Box (Point (0.1, 0.1), Point (0.4, 0.4)));
    }
    
public:
    Typeface (const Typeface&) = delete;
    Typeface& operator = (const Typeface&) = delete;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void swapWith (Typeface& o)
    {
        using std::swap;
        
        glyphs_.swapWith (o.glyphs_);
        
        swap (height_, o.height_);
        swap (ascender_, o.ascender_);
        swap (descender_, o.descender_);
        swap (undefined_, o.undefined_);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    int size() const
    {
        return glyphs_.size();
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    double getHeight() const
    {
        return height_;
    }
    
    double getAscender() const
    {
        return ascender_;
    }
    
    double getDescender() const
    {
        return descender_;
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -
    
public:
    const Glyph* getGlyph (unicode character) const
    {
        return getGlyphAtIndex (glyphs_.indexOfSorted (character));
    }

private:
    const Glyph* getGlyphAtIndex (int i) const
    {
        if (i < 0 || i >= glyphs_.size()) { return undefined_.get(); }
        else {
            return glyphs_[i].getRawPointer();
        }
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    bool importFont (const char* filename);
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void importBinary (const byte* data)            /* No security check. */
    {
        if (data != nullptr) {
        //
        Typeface scoped;
        
        const byte* p = data;
        
        /* Magic number. */
        
        if (nextRead<int32> (p) != 49285378) { return; }
        
        /* Number of glyphs. */
        
        int length = nextRead<int32> (p);
        
        PRIM_ASSERT (length >= 0);
                
        scoped.glyphs_.resize (length);
        
        /* Typographic properties. */
        
        scoped.height_    = nextRead<float32> (p);
        scoped.ascender_  = nextRead<float32> (p);
        scoped.descender_ = nextRead<float32> (p);
        
        for (int i = 0; i < length; ++i) {
        //
        Glyph* glyph = scoped.glyphs_[i].getRawPointer();
        
        /* Glyph values. */
        
        glyph->character_ = nextRead<unicode> (p);
        glyph->advance_   = nextRead<float32> (p);
        
        /* Kerning array. */
        
        int kSize = nextRead<int32> (p);
        
        PRIM_ASSERT (kSize >= 0);
        
        glyph->kerning_.resize (kSize);
        
        for (int j = 0; j < kSize; ++j) {
        //
        glyph->kerning_[j].next_       = nextRead<unicode> (p);
        glyph->kerning_[j].horizontal_ = nextRead<float32> (p);
        //
        }
        
        /* Path instructions. */
        
        int iSize = nextRead<int32> (p);
        
        PRIM_ASSERT (iSize >= 0);
        
        for (int j = 0; j < iSize; ++j) {
        //
        byte type = nextRead<byte> (p);
        
        if (type == Instruction::ClosePath)   { glyph->path_.add (Instruction::closePath()); }
        else {
        //
        Point end;
        end.getX() = nextRead<float32> (p);
        end.getY() = nextRead<float32> (p);
        
        if (type == Instruction::MoveTo)      { glyph->path_.add (Instruction::moveTo (end)); }
        else if (type == Instruction::LineTo) { glyph->path_.add (Instruction::lineTo (end)); }
        else {
            Point cp1;
            Point cp2;
            cp1.getX() = nextRead<float32> (p);
            cp1.getY() = nextRead<float32> (p);
            cp2.getX() = nextRead<float32> (p);
            cp2.getY() = nextRead<float32> (p);
            
            glyph->path_.add (Instruction::cubicTo (cp1, cp2, end));
        }
        //
        }
        //
        }
        //
        }

        scoped.swapWith (*this);
        //
        }
    }

    void exportBinary (Array<byte> & data)
    {
        Array<byte> scoped;
        
        /* Magic number. */
        
        nextWrite<int32> (scoped, 49285378);
        
        /* Number of glyphs. */
        
        nextWrite<int32> (scoped, glyphs_.size());
        
        /* Typographic properties. */
        
        nextWrite<float32> (scoped, static_cast<float32> (height_));
        nextWrite<float32> (scoped, static_cast<float32> (ascender_));
        nextWrite<float32> (scoped, static_cast<float32> (descender_));
        
        for (int i = 0; i < glyphs_.size(); ++i) {
        //
        Glyph* glyph = glyphs_[i].getRawPointer();
        
        /* Glyph values. */
        
        nextWrite<unicode> (scoped, static_cast<unicode> (glyph->character_));
        nextWrite<float32> (scoped, static_cast<float32> (glyph->advance_));
        
        /* Kerning array. */
        
        int32 kSize = static_cast<int32> (glyph->kerning_.size());
        nextWrite<int32> (scoped, kSize);
        
        for (int j = 0; j < kSize; ++j) {
        //
        nextWrite<unicode> (scoped, static_cast<unicode> (glyph->kerning_[j].next_));
        nextWrite<float32> (scoped, static_cast<float32> (glyph->kerning_[j].horizontal_));
        //
        }
        
        /* Path instructions. */
        
        int32 iSize = static_cast<int32> (glyph->path_.getInstructions().size());
        nextWrite<int32> (scoped, iSize);
        
        for (int j = 0; j < iSize; ++j) {
        //
        const Instruction& instruction = glyph->path_.getInstructions().get (j);
        nextWrite<byte> (scoped, static_cast<byte> (instruction.getType()));
        
        if (instruction.hasEnd()) {
        //
        nextWrite<float32> (scoped, static_cast<float32> (instruction.getEnd().getX()));
        nextWrite<float32> (scoped, static_cast<float32> (instruction.getEnd().getY()));
        //
        }
      
        if (instruction.hasControls()) {
        //
        nextWrite<float32> (scoped, static_cast<float32> (instruction.getControl1().getX()));
        nextWrite<float32> (scoped, static_cast<float32> (instruction.getControl1().getY()));
        nextWrite<float32> (scoped, static_cast<float32> (instruction.getControl2().getX()));
        nextWrite<float32> (scoped, static_cast<float32> (instruction.getControl2().getY()));
        //
        }
        //
        }
        //
        }
        
        scoped.swapWith (data);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void crop (int first, int last = -1)                    /* Last included. */
    {
        if (last < 0)  { last = glyphs_.size() - 1; }
        if (first < 0) { first = 0; }
        
        Math::ascending (first, last);
        
        if (last < glyphs_.size()) {
            Array<GlyphPointer> scoped;
            for (int i = first; i <= last; ++i) { scoped.add (glyphs_[i]); }
            scoped.swapWith (glyphs_);
        }
        
        for (int i = 0; i < glyphs_.size(); ++i) {          /* Remove obsolete kernings. */
        //
        Glyph* glyph = glyphs_[i].getRawPointer();
        
        if (glyph->kerning_.size()) {
        //
        Array<Glyph::Kerning> scoped;
        
        for (int j = 0; j < glyphs_.size(); ++j) { 
            unicode character = getGlyphAtIndex (j)->character_;
            double horizontal = glyph->getKerningWith (character);
            if (horizontal != 0.0) { 
                scoped.add (Glyph::Kerning (character, horizontal)); 
            }
        }
        
        scoped.swapWith (glyph->kerning_);
        //
        }
        //
        }
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    template <class T> static T nextRead (const byte*& p)      /* Bellefont is little-endian. */
    {
        T v; 
        std::memcpy (&v, p, sizeof (T));
        Endian::swapIfBigEndian (v);
        p += sizeof (T);
        return v;
    }
    
    template <class T> static void nextWrite (Array<byte> & data, const T& value)
    {
        T v (value);
        Endian::swapIfBigEndian (v);
        data.resize (static_cast<int> (data.size() + sizeof (T)));
        byte* d = &data.getLast();
        d -= (sizeof (T) - 1);
        std::memcpy (d, &v, sizeof (T));
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

friend void swap (GlyphPointer&, GlyphPointer&);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    Array<GlyphPointer> glyphs_;
    double height_;
    double ascender_;
    double descender_;

private:
    Pointer<Glyph> undefined_;

private:
    PRIM_LEAK_DETECTOR (Typeface)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void swap (Typeface& a, Typeface& b);
void swap (Typeface::GlyphPointer& a, Typeface::GlyphPointer& b);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#ifdef BELLE_COMPILE_INLINE

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void swap (Typeface& a, Typeface& b)
{
    a.swapWith (b);
}

void swap (Typeface::GlyphPointer& a, Typeface::GlyphPointer& b)
{
    a.swapWith (b);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // BELLE_COMPILE_INLINE

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace belle

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
