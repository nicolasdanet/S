
/* 
    Copyright (c) 2007-2013 Andi Brae.
    Copyright (c) 2013 Jojo and others.
    
*/

/* < http://opensource.org/licenses/BSD-2-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Operations with undefined rationals are not supported. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace prim {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

template <class T> class Rational {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Rational (T numerator, T denominator) : n_ (numerator), d_ (denominator)
    {
        if (d_ < 0)  { n_ = -n_; d_ = -d_; }
        if (d_ == 0) { n_ = 0; }
        else if (n_ == 0) { d_ = 1; }
        else {
        //
        T g = Math::GCD (n_, d_);
        
        if (g < 1) { n_ = d_ = 0; }
        else {
            n_ = n_ / g;
            d_ = d_ / g;
        }
        //
        }
    }
    
    Rational (T n = 0) : n_ (n), d_ (1)
    {
    }

    Rational (const Rational<T>&) = default;
    Rational (Rational<T>&&) = default;
    Rational<T>& operator = (const Rational<T>&) = default;
    Rational<T>& operator = (Rational<T>&&) = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    bool isNegative() const
    {
        return (n_ < 0) && !isUndefined();
    }
    
    bool isZero() const
    {
        return (n_ == 0) && !isUndefined();
    }
    
    bool isUndefined() const
    {
        return (d_ <= 0);
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    T getNumerator() const
    {
        return n_;
    }

    T getDenominator() const
    {
        return d_;
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    double asDouble() const
    {
        if (isUndefined()) { return std::numeric_limits<double>::quiet_NaN(); }
        else {
            return static_cast<double> (getNumerator()) / static_cast<double> (getDenominator());
        }
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Rational<T>& operator += (const Rational<T>& r)
    {
        if ((*this).isUndefined() || r.isUndefined()) { throw std::domain_error ("rational undefined"); }
        
        T lcm = Math::LCM (d_, r.d_);
        T d = lcm;
        T n = n_ * (lcm / d_) + r.n_ * (lcm / r.d_);
        
        *this = Rational<T> (n, d);
        
        return *this;
    }

    Rational<T>& operator -= (const Rational<T>& r)
    {
        if ((*this).isUndefined() || r.isUndefined()) { throw std::domain_error ("rational undefined"); }
        
        T lcm = Math::LCM (d_, r.d_);
        T d = lcm;
        T n = n_ * (lcm / d_) - r.n_ * (lcm / r.d_);
        
        *this = Rational<T> (n, d);
        
        return *this;
    }
    
    Rational<T>& operator *= (const Rational<T>& r)
    {
        if ((*this).isUndefined() || r.isUndefined()) { throw std::domain_error ("rational undefined"); }
        
        T n = n_ * r.n_;
        T d = d_ * r.d_;
        
        *this = Rational<T> (n, d);
        
        return *this;
    }
    
    Rational<T>& operator /= (const Rational<T>& r)
    {
        if ((*this).isUndefined() || r.isUndefined()) { throw std::domain_error ("rational undefined"); }
        
        T n = n_ * r.d_;
        T d = d_ * r.n_;
        
        *this = Rational<T> (n, d);
        
        return *this;
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    static const Rational<T> undefined()
    {
        return Rational<T> (0, 0);
    }
    
private:
    T n_;
    T d_;

private:
    PRIM_LEAK_DETECTOR (Rational)
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

friend bool operator != (const Rational<T>& a, const Rational<T>& b)
{
    return !(a == b);
}

friend bool operator == (const Rational<T>& a, const Rational<T>& b)
{
    if (a.isUndefined() || b.isUndefined()) { throw std::domain_error ("rational undefined"); }
    
    return (a.n_ == b.n_ && a.d_ == b.d_);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

friend bool operator > (const Rational<T>& a, const Rational<T>& b)
{
    return ((a - b).n_ > 0);
}

friend bool operator >= (const Rational<T>& a, const Rational<T>& b)
{
    return ((a - b).n_ >= 0);
}
    
friend bool operator < (const Rational<T>& a, const Rational<T>& b)
{
    return ((a - b).n_ < 0);
}

friend bool operator <= (const Rational<T>& a, const Rational<T>& b)
{
    return ((a - b).n_ <= 0);
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

friend Rational<T> operator + (Rational<T> a, const Rational<T>& b)
{
    a += b; return a;
}

friend Rational<T> operator - (Rational<T> a, const Rational<T>& b)
{
    a -= b; return a;
}

friend Rational<T> operator * (Rational<T> a, const Rational<T>& b)
{
    a *= b; return a;
}

friend Rational<T> operator / (Rational<T> a, const Rational<T>& b)
{
    a /= b; return a;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

using Ratio = Rational<int64>;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace prim

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
