
/* 
    Copyright (c) 2007-2013 Andi Brae.
    Copyright (c) 2013 Jojo and others.
    
*/

/* < http://opensource.org/licenses/BSD-2-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < https://en.wikipedia.org/wiki/Transformation_matrix#Affine_transformations > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace belle {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class Affine {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Affine() : a_ (1.0), b_ (0.0), c_ (0.0), d_ (1.0), e_ (0.0), f_ (0.0)       /* Identity. */
    {
        // | a  b  0 |
        // | c  d  0 |
        // | e  f  1 |
    }
    
    Affine (double a, double b, double c, double d, double e, double f) :
        a_ (a), 
        b_ (b), 
        c_ (c), 
        d_ (d), 
        e_ (e), 
        f_ (f) 
        {
        }

    Affine (const Affine&) = default;
    Affine (Affine&&) = default;
    Affine& operator = (const Affine&) = default;
    Affine& operator = (Affine&&) = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Vector appliedTo (const Vector& v) const
    {
        double x = v.getX();
        double y = v.getY();

        return Vector (a_ * x + c_ * y + e_, b_ * x + d_ * y + f_);
    }
    
    Box appliedTo (const Box& b) const
    {
        if (!b.isEmpty()) {
        //
        Point pt1 (b.getBottomLeft());
        Point pt2 (b.getTopRight());
        return Box (appliedTo (pt1), appliedTo (pt2));
        //
        }
        
        return Box::empty();
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    std::string asPDFString() const
    {
        std::ostringstream s; s.setf (std::ios::fixed, std::ios::floatfield);
        
        s << "    ";
        s << a_ << " ";
        s << b_ << " ";
        s << c_ << " ";
        s << d_ << " ";
        s << e_ << " "; 
        s << f_ << " cm" << newLine;
        
        return s.str();
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Affine operator * (const Affine& o) const
    {
        double a = o.a_ * a_ + o.b_ * c_;
        double b = o.a_ * b_ + o.b_ * d_;
        double c = o.c_ * a_ + o.d_ * c_;
        double d = o.c_ * b_ + o.d_ * d_;
        double e = o.e_ * a_ + o.f_ * c_ + e_;
        double f = o.e_ * b_ + o.f_ * d_ + f_;
      
        return Affine (a, b, c, d, e, f);
    }

    void operator *= (const Affine& o)
    {
        *this = (*this) * o;
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    static Affine identity()
    {
        return Affine();
    }
    
    static Affine flipHorizontal()
    {
        return Affine (1.0, 0.0, 0.0, -1.0, 0.0, 0.0);
    }

    /* < http://www.dr-lex.be/random/matrix_inv.html > */

    static Affine inversed (const Affine& affine) 
    {
        double determinant = (affine.a_ * affine.d_ - affine.b_ * affine.c_);

        if (!determinant) { return Affine (0.0, 0.0, 0.0, 0.0, 0.0, 0.0); }
        else {
        //
        double k = (1.0 / determinant);
        
        double a = affine.d_;
        double b = -affine.b_;
        double c = -affine.c_;
        double d = affine.a_;
        double e = affine.c_ * affine.f_ - affine.d_ * affine.e_;
        double f = -affine.a_ * affine.f_ + affine.b_ * affine.e_;

        return Affine (k * a, k * b, k * c, k * d, k * e, k * f);
        //
        }
    }
    
    static Affine translation (Vector translation)
    {
        return Affine (1.0, 0.0, 0.0, 1.0, translation.getX(), translation.getY());
    }

    static Affine scale (Vector scale)
    {
        return Affine (scale.getX(), 0.0, 0.0, scale.getY(), 0.0, 0.0);
    }
    
    static Affine rotation (double rotation)
    {
        double t1 = std::cos (rotation);
        double t2 = std::sin (rotation);
        return Affine (t1, t2, -t2, t1, 0.0, 0.0);
    }
    
private:
    double a_;
    double b_;
    double c_;      
    double d_;
    double e_;
    double f_;

private:
    PRIM_LEAK_DETECTOR (Affine)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Note that the affine multiplication is NOT commutative. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class AffineStack {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    AffineStack()
    {
        stack_.add (Affine::identity());
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

public:
    AffineStack (const AffineStack&) = delete;
    AffineStack& operator = (const AffineStack&) = delete;
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void pop()
    {
        stack_.resize (stack_.size() - 1);
    }
    
    void push (const Affine& affine)
    {
        stack_.add (affine);
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Affine collapsed() const
    {
        Affine affine; for (int i = 0; i < stack_.size(); ++i) { affine *= stack_[i]; }
        return affine;
    };

private:
    Array<Affine> stack_;

private:
    PRIM_LEAK_DETECTOR (AffineStack)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace belle

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
