
/*
    Copyright (c) 2007-2013 Andi Brae.
    Copyright (c) 2013 Jojo and others.
    
*/

/* < http://opensource.org/licenses/BSD-2-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

// ====================================

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Basic implementation of the PDF standard. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < http://www.adobe.com/content/dam/Adobe/en/devnet/acrobat/pdfs/pdf_reference_1-7.pdf > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace belle { 

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class Pdf : public Painter {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class Object {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Object (int label) : label_ (label), offset_ (0) 
    {
    }

public:
    Object (const Object&) = delete;
    Object& operator = (const Object&) = delete;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void setOffset (std::size_t offset)
    {
        offset_ = offset;
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    std::string getOffset() const
    {
        return prim::Utils::paddedLeft (offset_, 10, '0') +  " 00000 n ";
    }
    
    int getLabel() const
    {
        return label_;
    }
    
    const std::string& getContent() const
    {
        return content_;
    }

    const std::string& getDictionary() const
    {
        return dictionary_;
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    std::string asReference() const
    {
        std::ostringstream s; s << label_ << " 0 R";
        return s.str();
    } 
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void addToContent (const std::string& s)
    {
        content_ += s;
        content_ += newLine;
    }
    
    void addToDictionary (const std::string& s)
    {
        dictionary_ += s;
        dictionary_ += newLine;
    }

private:
    int label_;
    std::size_t offset_;
    std::string content_;
    std::string dictionary_;
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Pdf (const std::string& filename) : filename_ (filename), size_ (Paper::portrait (Paper::A4()))
    {
    }

public:
    Pdf (const Pdf&) = delete;
    Pdf& operator = (const Pdf&) = delete;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    bool isPageable() const override
    {
        return true;
    }

    Points getPageSize() override
    {
        return size_;
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void setState (const Raster& state) override
    {
        state_ = state;
        
        if (stream_) { 
            stream_->addToContent (state_.asPDFString());
        }
    }
    
    void pushAffine (const Affine& affine) override
    {
        if (stream_) {
        //
        std::string s;
        
        s += "    " "q";
        s += newLine;
        s += affine.asPDFString();
        
        stream_->addToContent (s);
        //
        }
    }
    
    void popAffine (int n) override
    {
        if (stream_) {
        //
        std::string s;
        
        for (int i = 0; i < n; ++i) {
            s += "    " "Q";
            s += newLine;
        }
        
        stream_->addToContent (s);
        //
        }
    }
    
    void draw (const Path& path) override
    {
        if (stream_) {
        //
        std::string s (path.asPDFString());
        
        bool fill   = (state_.getFillColor().getAlpha() > 0.0);
        bool stroke = (state_.getStrokeColor().getAlpha() > 0.0) && (state_.getWidth() > 0.0);
    
        s += "    ";
    
        if (fill && stroke) { s += "B"; }
        else if (fill)      { s += "f"; }
        else if (stroke)    { s += "S"; }
        else { 
            s += "n";
        }
    
        s += newLine;

        stream_->addToContent (s);
        //
        }
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void setActivePage (int n) override
    {
        if (n < 0 || n >= contents_.size()) { stream_ = nullptr; }
        else {
        //
        stream_ = contents_[n];
            
        std::string s;
            
        s += newLine;
        s += "    " "/DeviceRGB cs";
        s += newLine;
        s += "    " "/DeviceRGB CS";
        s += newLine;
            
        stream_->addToContent (s);
        //
        }
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void performPaint (Paintable& toPaint) override
    {
        writeBegin (toPaint);
        writePaint (toPaint);
        writeClose (toPaint);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void writeBegin (Paintable& toPaint)
    {
        const int catalog = 0;
        const int info = 1;
        const int tree = 2;
        
        int count = 1;
        int n = Math::max (toPaint.getNumberOfPages (*this), 1);
        
        headers_.add (Pointer<Object> (new Object (count++)));
        headers_.add (Pointer<Object> (new Object (count++)));
        headers_.add (Pointer<Object> (new Object (count++)));
        
        for (int i = 0; i < n; ++i) { pages_.add (Pointer<Object> (new Object (count++))); }
        for (int i = 0; i < n; ++i) { contents_.add (Pointer<Object> (new Object (count++))); }
            
        headers_[catalog]->addToDictionary ("/Type /Catalog");
        headers_[catalog]->addToDictionary ("    " "/Pages " + headers_[tree].get()->asReference());
        
        headers_[info]->addToDictionary ("/Producer (Belle, Bonne, Sage)");
        
        headers_[tree]->addToDictionary ("/Type /Pages");
        headers_[tree]->addToDictionary ("    " "/Kids [ " + pages_[0].get()->asReference());
        for (int i = 1; i < n; ++i) { 
        headers_[tree]->addToDictionary ("    " "        " + pages_[i].get()->asReference());
        }
        headers_[tree]->addToDictionary ("    " "      ]");
        headers_[tree]->addToDictionary ("    " "/Count " + prim::Utils::paddedLeft (pages_.size()));
        
        for (int i = 0; i < n; ++i) {
        //
        std::string sizeX = prim::Utils::paddedLeft (size_.width());
        std::string sizeY = prim::Utils::paddedLeft (size_.height());
        
        pages_[i]->addToDictionary ("/Type /Page");
        pages_[i]->addToDictionary ("    " "/Parent " + headers_[tree].get()->asReference());
        pages_[i]->addToDictionary ("    " "/Contents " + contents_[i].get()->asReference());
        pages_[i]->addToDictionary ("    " "/MediaBox [ 0 0 " + sizeX + " " + sizeY + " ]");
        pages_[i]->addToDictionary ("    " "/Resources " "<<  >>");
        //
        }
    }
    
    void writePaint (Paintable& toPaint)
    {
        setActivePage (0);
        toPaint.paint (*this);
        stream_ = nullptr;
    }
    
    void writeClose (Paintable&)
    {
        std::ostringstream output;
        
        output << "%PDF-1.3" << newLine;
        output << "%" "\xc2\xa5\xc2\xb1\xc3\xab" << newLine;
        output << newLine;
        
        int size = headers_.size() + pages_.size() + contents_.size() + 1;
        
        for (int i = 0; i < headers_.size(); ++i)  { writeObject (headers_[i], output);  }
        for (int i = 0; i < pages_.size(); ++i)    { writeObject (pages_[i], output);    }
        for (int i = 0; i < contents_.size(); ++i) { writeObject (contents_[i], output); }
    
        std::size_t XRefLocation = output.str().length();
        
        output << "xref" << newLine;
        output << "0 " << size << newLine;
        output << "0000000000 65535 f " << newLine;

        for (int i = 0; i < headers_.size(); ++i)  { output << headers_[i]->getOffset() << newLine;  }
        for (int i = 0; i < pages_.size(); ++i)    { output << pages_[i]->getOffset() << newLine;    }
        for (int i = 0; i < contents_.size(); ++i) { output << contents_[i]->getOffset() << newLine; }
      
        std::string unique = prim::Utils::nextID();
        
        output << newLine;
        output << "trailer" << newLine;
        output << " << " "/Root " << headers_[0]->asReference() << newLine;
        output << "    " "/Info " << headers_[1]->asReference() << newLine;
        output << "    " "/Size " << size << newLine;
        output << "    " "/ID [ " << "<" << unique << ">" << newLine;
        output << "    " "      " << "<" << unique << ">" << newLine;
        output << "    " "    ]" << newLine;
        output << " >>" << newLine;
        output << "startxref" << newLine;
        output << XRefLocation << newLine;
        output << "%%EOF" << newLine;
        
        headers_.clear();
        pages_.clear();
        contents_.clear();
        
        if (filename_.length()) { File::writeByString (filename_.c_str(), output.str().c_str()); }
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    static void writeObject (Pointer<Object> object, std::ostringstream& output)
    {
        object->setOffset (output.str().length());

        output << object->getLabel() << " 0 obj" << newLine;
        output << " << " << object->getDictionary().c_str();
        
        if (object->getContent().length() == 0) { output << " >>" << newLine; }
        else {
            if (object->getDictionary().length()) { output << "    "; }
            output << "/Length " << object->getContent().length() << newLine;
            output << " >>" << newLine;
            output << "stream" << newLine;
            output << object->getContent() << newLine;
            output << "endstream" << newLine;
        }
        
        output << "endobj" << newLine;
        output << newLine;
    }
    
private:
    std::string filename_;
    Points size_;
    Raster state_;
    Pointer<Object> stream_;
    Array<Pointer<Object>> headers_;
    Array<Pointer<Object>> pages_;
    Array<Pointer<Object>> contents_;

private:
    PRIM_LEAK_DETECTOR (Pdf)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace belle

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
