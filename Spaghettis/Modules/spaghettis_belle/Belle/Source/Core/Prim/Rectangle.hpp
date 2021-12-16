
/* 
    Copyright (c) 2007-2013 Andi Brae.
    Copyright (c) 2013 Jojo and others.
    
*/

/* < http://opensource.org/licenses/BSD-2-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace prim { 

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

using Point  = Complex<double>;
using Vector = Complex<double>;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

template <class T> class Rectangle {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Rectangle() : isEmpty_ (true)
    { 
    }
    
    Rectangle (Complex<T> pt) : isEmpty_ (false), a_ (pt), b_ (pt)
    {
    }
    
    Rectangle (Complex<T> bLeft, Complex<T> tRight) : isEmpty_ (false), a_ (bLeft), b_ (tRight) 
    { 
        sortCoordinates();
    }

    Rectangle (const Rectangle<T>&) = default;
    Rectangle (Rectangle<T>&&) = default;
    Rectangle<T>& operator = (const Rectangle<T>&) = default;
    Rectangle<T>& operator = (Rectangle<T>&&) = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    bool isEmpty() const
    {
        return isEmpty_;
    }
    
    bool contains (const Complex<T>& pt) const
    {
        if (isEmpty() == false) {
            T x = pt.getX();
            T y = pt.getY();
            return (x >= getLeft() && y >= getBottom() && x <= getRight() && y <= getTop());
        }
        
        return false;
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    T getWidth() const
    {
        return (b_.getX() - a_.getX());
    }

    T getHeight() const
    {
        return (b_.getY() - a_.getY());
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

public:
    Complex<T> getCenter() const
    {
        return (a_ + b_) / 2.0;
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    T getLeft() const 
    {
        return a_.getX();
    }

    T getBottom() const 
    {
        return a_.getY();
    }

    T getRight() const 
    {
        return b_.getX();
    }

    T getTop() const 
    {
        return b_.getY();
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

public:
    Complex<T> getBottomLeft() const
    {
        return Complex<T> (getLeft(), getBottom());
    }
    
    Complex<T> getBottomRight() const
    {
        return Complex<T> (getRight(), getBottom());
    }
    
    Complex<T> getTopLeft() const
    {
        return Complex<T> (getLeft(), getTop());
    }
    
    Complex<T> getTopRight() const
    {
        return Complex<T> (getRight(), getTop());
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void setLeft (T x1)
    {
        if (isEmpty() == false) { a_.getX() = x1; sortCoordinates(); }
    }

    void setBottom (T y1)
    {
        if (isEmpty() == false) { a_.getY() = y1; sortCoordinates(); }
    }

    void setRight (T x2)
    {
        if (isEmpty() == false) { b_.getX() = x2; sortCoordinates(); }
    }

    void setTop (T y2)
    {
        if (isEmpty() == false) { b_.getY() = y2; sortCoordinates(); }
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

public:
    void setBottomLeft (const Complex<T>& pt)
    {
        setLeft (pt.getX()); setBottom (pt.getY());
    }
    
    void setBottomRight (const Complex<T>& pt)
    {
        setRight (pt.getX()); setBottom (pt.getY());
    }
    
    void setTopLeft (const Complex<T>& pt)
    {
        setLeft (pt.getX()); setTop (pt.getY());
    }
    
    void setTopRight (const Complex<T>& pt)
    {
        setRight (pt.getX()); setTop (pt.getY());
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    bool operator != (const Rectangle<T>& rect) const
    {
        return !(*this == rect);
    }
    
    bool operator == (const Rectangle<T>& rect) const
    {
        if (isEmpty_ && rect.isEmpty_)      { return true;  }
        else if (isEmpty_ != rect.isEmpty_) { return false; }
        else {
            return (a_ == rect.a_) && (b_ == rect.b_);
        }
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* < https://en.wikipedia.org/wiki/Minimum_bounding_rectangle > */

public:
    Rectangle<T>& operator += (const Rectangle<T>& rect)
    {
        *this = minimumBoundingRectangle (*this, rect);
        return *this;
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void sortCoordinates()
    {
        Math::ascending (a_.getX(), b_.getX());
        Math::ascending (a_.getY(), b_.getY());
    }

    static Rectangle<T> minimumBoundingRectangle (Rectangle<T> rect1, Rectangle<T> rect2)
    {
        if (rect1.isEmpty() && rect2.isEmpty()) { return Rectangle(); }
        else if (rect1.isEmpty()) { return rect2; }
        else if (rect2.isEmpty()) { return rect1; }
      
        T tblx = rect1.a_.getX();
        T oblx = rect2.a_.getX();
        T tbly = rect1.a_.getY();
        T obly = rect2.a_.getY();
        T ttrx = rect1.b_.getX();
        T otrx = rect2.b_.getX();
        T ttry = rect1.b_.getY();
        T otry = rect2.b_.getY();
      
        Complex<T> a (Math::min (tblx, oblx), Math::min (tbly, obly));
        Complex<T> b (Math::max (ttrx, otrx), Math::max (ttry, otry));
      
        return Rectangle (a, b);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    static const Rectangle<T> empty()
    {
        return Rectangle<T>();
    }
    
private:
    bool isEmpty_;
    Complex<T> a_;
    Complex<T> b_;

private:
    PRIM_LEAK_DETECTOR (Rectangle)
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

friend const Rectangle<T> operator + (Rectangle<T> rect1, const Rectangle<T>& rect2)
{
    rect1 += rect2; return rect1;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

using Box = Rectangle<double>;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace prim

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
