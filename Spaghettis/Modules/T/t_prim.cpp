
/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include <climits>

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

// ====================================

/* Testing the Prim library. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

const int    kAgain    = 100;
const double kEpsilon  = 1E-9;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

const int kPrime1[] =
    {
        2,  3,  5,  7,  11, 13, 17, 19, 23, 29,
        31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
        73, 79, 83, 89, 97
    };

const int kPrime2[] =
    {
        101, 103, 107, 109, 113, 127, 131, 137, 139, 149,
        151, 157, 163, 167, 173, 179, 181, 191, 193, 197,
        199, 211, 223, 227, 229, 233, 239, 241, 251, 257,
        263, 269, 271, 277, 281, 283, 293, 307, 311, 313,
        317, 331, 337, 347, 349, 353, 359, 367, 373, 379,
        383, 389, 397, 401, 409, 419, 421, 431, 433, 439,
        443, 449, 457, 461, 463, 467, 479, 487, 491, 499,
        503, 509, 521, 523, 541, 547, 557, 563, 569, 571,
        577, 587, 593, 599, 601, 607, 613, 617, 619, 631,
        641, 643, 647, 653, 659, 661, 673, 677, 683, 691,
        701, 709, 719, 727, 733, 739, 743, 751, 757, 761,
        769, 773, 787, 797, 809, 811, 821, 823, 827, 829,
        839, 853, 857, 859, 863, 877, 881, 883, 887, 907,
        911, 919, 929, 937, 941, 947, 953, 967, 971, 977,
        983, 991, 997
    };

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

class Base {

public:
    Base()
    {
        ++counter_;
    }
    
    virtual ~Base()
    {
        --counter_;
    }
    
public:
    virtual std::string whoami()
    {
        std::ostringstream s; s << "Base / " << counter_; return s.str();
    }

private:
    static int counter_;
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class Derived : public Base {

public:
    Derived()
    {
        ++counter_;
    }
    
    ~Derived()
    {
        --counter_;
    }
    
public:
    std::string whoami() override
    {
        std::ostringstream s; s << "Derived / " << counter_; return s.str();
    }

private:
    static int counter_;
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class Dummy {   /* NOT polymorphic. */

public:
    Dummy()
    {
        ++counter_;
    }
    
    ~Dummy()
    {
        --counter_;
    }
    
public:
    std::string whoami()
    {
        std::ostringstream s; s << "Dummy / " << counter_; return s.str();
    }

private:
    static int counter_;
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class Widget {

public:
    Widget (int n = 0) : value_ (n)
    {
    }

private:
    Widget (const Widget&);
    Widget& operator = (const Widget&);
    
public:
    int getValue() const
    {
        return value_;
    }
    
    void setValue (int n)
    {
        value_ = n;
    }
    
public:
    bool operator == (const Widget& o)
    {
        return value_ == o.value_;
    }
    
private:
    int value_;
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

using Graph = prim::Graph<Widget>;
using Node  = prim::Graph<Widget>::Node;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int Base::counter_;
int Derived::counter_;
int Dummy::counter_;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static double getRandomDouble()
{
    static t_rand48 rnd = 0UL; if (!rnd) { PD_RAND48_INIT (rnd); }
    
    return PD_RAND48_DOUBLE (rnd);
}

static int getRandomInteger (int n = std::numeric_limits<int>::max())
{
    return static_cast<int> (getRandomDouble() * n);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if 0
void test100__types() {
#endif 

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (PrimTypes, 100, "Prim - Types")

    TTT_EXPECT (sizeof (prim::uint32)  == 4);
    TTT_EXPECT (sizeof (prim::int32)   == 4);
    TTT_EXPECT (sizeof (prim::int64)   == 8);
    TTT_EXPECT (sizeof (prim::uint64)  == 8);
    TTT_EXPECT (sizeof (prim::float32) == 4);
    TTT_EXPECT (sizeof (prim::float64) == 8);
    TTT_EXPECT (sizeof (prim::unicode) == 4);
    TTT_EXPECT (sizeof (prim::byte)    == 1);
            
TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if 0
}
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if 0
void test101__memory() {
#endif 

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (PrimMemory, 101, "Prim - Memory")

    {
    //
    prim::int32 original = 0xaabbccdd;
    
    alignas (alignof (prim::int32)) prim::byte raw[4] = { 0xdd, 0xcc, 0xbb, 0xaa };
    prim::Endian::swapIfBigEndian (reinterpret_cast<prim::int32&> (raw));
    
    void* a = static_cast<void*> (&original);
    void* b = static_cast<void*> (raw);
    
    TTT_EXPECT (std::memcmp (a, b, sizeof (prim::int32)) == 0);
    //
    }
    
    {
    //
    prim::int32 original = 0xaabbccdd;
    
    alignas (alignof (prim::int32)) prim::byte raw[4] = { 0xaa, 0xbb, 0xcc, 0xdd };
    prim::Endian::swapIfLittleEndian (reinterpret_cast<prim::int32&> (raw));
    
    void* a = static_cast<void*> (&original);
    void* b = static_cast<void*> (raw);
    
    TTT_EXPECT (std::memcmp (a, b, sizeof (prim::int32)) == 0);
    //
    }
    
    {
    //
    const int size = 16;
    
    for (int i = 0; i < kAgain; ++i) {
    //
    int a[size] = { 0 };
    int b[size] = { 0 };
    int c[size] = { 0 };
    int k[size] = { 0 };
    
    int range = getRandomInteger (size);
    
    for (int j = 0; j < size; ++j) { 
        while ((a[j] >= 0) && (a[j] <= 2)) { k[j] = a[j] = getRandomInteger(); }
        b[j] = 1; 
        c[j] = 2;
    }
    
    prim::Memory::clear (a, range);
    
    for (int j = 0; j < size; ++j) { 
        TTT_EXPECT ((a[j] == 0)    == (j < range));
        TTT_EXPECT ((a[j] == k[j]) == (j >= range));
    }
    
    prim::Memory::copy (a, b, range);
    
    for (int j = 0; j < size; ++j) { 
        TTT_EXPECT ((a[j] == 1)    == (j < range));
        TTT_EXPECT ((a[j] == k[j]) == (j >= range));
    }
    
    prim::Memory::swap (a, c, range);
    
    for (int j = 0; j < size; ++j) { 
        TTT_EXPECT ((a[j] == 2)    == (j < range));
        TTT_EXPECT ((a[j] == k[j]) == (j >= range));
        TTT_EXPECT ((c[j] == 1)    == (j < range));
        TTT_EXPECT ((c[j] == 2)    == (j >= range));
    }
    //
    }
    //
    }
            
TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if 0
}
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if 0
void test102__math() {
#endif 

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (PrimMath, 102, "Prim - Math")

    {
    //
    TTT_EXPECT (prim::Math::sign (1234)         == 1);
    TTT_EXPECT (prim::Math::sign (-1234)        == -1);
    TTT_EXPECT (prim::Math::sign (0)            == 0);
    TTT_EXPECT (prim::Math::abs (1234)          == 1234);
    TTT_EXPECT (prim::Math::abs (-1234)         == 1234);
    TTT_EXPECT (prim::Math::abs (0)             == 0);
    TTT_EXPECT (prim::Math::min (1234, 0)       == 0);
    TTT_EXPECT (prim::Math::min (0, -1234)      == -1234);
    TTT_EXPECT (prim::Math::min (1234, -1234)   == -1234);
    TTT_EXPECT (prim::Math::max (1234, 0)       == 1234);
    TTT_EXPECT (prim::Math::max (0, -1234)      == 0);
    TTT_EXPECT (prim::Math::max (1234, -1234)   == 1234);
    
    TTT_EXPECT (prim::Math::isPowerOfTwo (0)    == true);
    TTT_EXPECT (prim::Math::isPowerOfTwo (1)    == true);
    TTT_EXPECT (prim::Math::isPowerOfTwo (2)    == true);
    TTT_EXPECT (prim::Math::isPowerOfTwo (3)    == false);
    TTT_EXPECT (prim::Math::isPowerOfTwo (1024) == true);
    TTT_EXPECT (prim::Math::isPowerOfTwo (1025) == false);
    
    TTT_EXPECT (prim::Math::numberOfSetBits (0)          == 0);
    TTT_EXPECT (prim::Math::numberOfSetBits (0x7f)       == 7);
    TTT_EXPECT (prim::Math::numberOfSetBits (0x1234)     == 5);
    TTT_EXPECT (prim::Math::numberOfSetBits (0x12345678) == 13);
    
    int m = 1234;
    int n = -1234;
    
    prim::Math::ascending (m, n);
    
    TTT_EXPECT (m == -1234);
    TTT_EXPECT (n == 1234);
    
    prim::Math::descending (m, n);
    
    TTT_EXPECT (m == 1234);
    TTT_EXPECT (n == -1234);
    
    TTT_EXPECT (prim::Math::isBetween (1, 1, 9)           == true);
    TTT_EXPECT (prim::Math::isBetween (9, 1, 9)           == true);
    TTT_EXPECT (prim::Math::isBetween (0.0, -1.0, 1.0)    == true);
    TTT_EXPECT (prim::Math::isBetween (-4.0, -2.0, -6.0)  == true);
    TTT_EXPECT (prim::Math::isBetween (-8.0, -6.0, -2.0)  == false);
    TTT_EXPECT (prim::Math::isBetween (1234.0, -1.0, 1.0) == false);

    TTT_EXPECT (prim::Math::GCD (0, 0)     == 0);
    TTT_EXPECT (prim::Math::GCD (7, 0)     == 7);
    TTT_EXPECT (prim::Math::GCD (0, 7)     == 7);
    TTT_EXPECT (prim::Math::GCD (6, 35)    == 1);
    TTT_EXPECT (prim::Math::GCD (383, 127) == 1);
    TTT_EXPECT (prim::Math::GCD (812, 451) == 1);
    TTT_EXPECT (prim::Math::GCD (221, 782) == 17);
    TTT_EXPECT (prim::Math::GCD (24, 18)   == 6);
    TTT_EXPECT (prim::Math::GCD (150, 45)  == 15);
    //
    }
    
    {
    //
    const double kEpsilonRoot = 1E-5;       /* When "a" is close to zero, precision is rather bad. */
        
    double dummy;
    
    TTT_EXPECT (prim::Math::roots (0.0, 0.0, 0.0, dummy, dummy) == 0);
    
    for (int i = 0; i < kAgain; ++i) {      /* Linear equation. */
    //
    double a = (getRandomDouble() * 200.0) - 100.0;
    double b = (getRandomDouble() * 200.0) - 100.0;
    double root = 0.0;
    
    /* Substitution test. */
    
    int k = prim::Math::roots (a, b, root);
    if (k != 0) { TTT_EXPECT (prim::Math::abs (a * root + b) < kEpsilonRoot); }
    //
    }
    
    for (int i = 0; i < kAgain; ++i) {      /* Quadratic equation. */
    //
    double a = (getRandomDouble() * 200.0) - 100.0;
    double b = (getRandomDouble() * 200.0) - 100.0;
    double c = (getRandomDouble() * 200.0) - 100.0;
    double root1 = 0.0;
    double root2 = 0.0;
    
    /* Substitution test. */
    
    int k = prim::Math::roots (a, b, c, root1, root2);
    if (k != 0) { TTT_EXPECT (prim::Math::abs (a * (root1 * root1) + b * root1 + c) < kEpsilonRoot); }
    if (k == 2) { TTT_EXPECT (prim::Math::abs (a * (root2 * root2) + b * root2 + c) < kEpsilonRoot); }
    //
    }
    
    for (int i = 0; i < kAgain; ++i) {
    //
    int a = getRandomInteger (32768);
    int b = getRandomInteger (32768);
    int d = prim::Math::GCD (a, b);
    
    TTT_EXPECT (prim::Math::GCD (a / d, b / d) == 1);   /* Basic properties. */
    TTT_EXPECT (d * prim::Math::LCM (a, b) == a * b);   /* Ditto. */
    //
    }
    
    for (int i = 0; i < kAgain; ++i) {
    //
    double a = 0.0;
    double b = 0.0; 
    double phi = getRandomDouble() * prim::kPi;
    
    while (a == 0.0) { a = getRandomDouble() * 5.0; }
    while (b == 0.0) { b = getRandomDouble() * 5.0; }
    
    prim::Point pt;
    prim::Math::Ellipse::intersectionWithVerticalTangent (a, b, phi, pt);
    
    /* Substitution in the equation of an ellipse. */
    
    pt /= prim::Complex<double>::withPolar (phi, 1.0);
    
    double x2 = pt.getX() * pt.getX();
    double y2 = pt.getY() * pt.getY();
    double a2 = a * a;
    double b2 = b * b;
    
    TTT_EXPECT (prim::Math::abs (x2 / a2 + y2 / b2 - 1.0) < kEpsilon);
    //
    }
    //
    }
    
TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if 0
}
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if 0
void test103__pointer() {
#endif 

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (PrimPointer, 103, "Prim - Pointer")

    using std::swap;
    
    /* Basic. */
    
    {
    //
    prim::Pointer<Base> a (new Base());
    prim::Pointer<Base> b;
    
    TTT_EXPECT (a != nullptr);
    TTT_EXPECT (b == nullptr);
    
    b = a;
    
    TTT_EXPECT (b != nullptr);
    TTT_EXPECT (a->whoami() == std::string ("Base / 1"));
    TTT_EXPECT (b->whoami() == std::string ("Base / 1"));
    
    prim::Pointer<Base> c (b);
    
    TTT_EXPECT (c != nullptr);
    TTT_EXPECT (c->whoami() == std::string ("Base / 1"));
    
    a = nullptr;
    b = nullptr;
    c = nullptr;

    a = new Base();
    b = new Base();
    c = new Base(); 
    
    TTT_EXPECT (a->whoami() == std::string ("Base / 3"));
    TTT_EXPECT (b->whoami() == std::string ("Base / 3"));
    TTT_EXPECT (c->whoami() == std::string ("Base / 3"));
    
    a = new Base();
    
    TTT_EXPECT (a->whoami() == std::string ("Base / 3"));
    
    a = nullptr;
    b = nullptr;
    
    TTT_EXPECT (c->whoami() == std::string ("Base / 1"));
    //
    }

    /* Swappable. */
    
    {
    //
    prim::Pointer<Base> a;
    prim::Pointer<Base> b;
    
    a = new Base();
    b = new Derived();
    
    TTT_EXPECT (a->whoami() == std::string ("Base / 2"));
    TTT_EXPECT (b->whoami() == std::string ("Derived / 1"));
    
    swap (a, b);
    
    TTT_EXPECT (a->whoami() == std::string ("Derived / 1"));
    TTT_EXPECT (b->whoami() == std::string ("Base / 2"));
    //
    }
    
    /* Movable (C++11). */
    
    {
    //
    prim::Pointer<Base> t (new Base());
    prim::Pointer<Base> a;
    prim::Pointer<Base> b (std::move (t));
    
    TTT_EXPECT (b->whoami() == std::string ("Base / 1"));
    
    a = prim::Pointer<Base> (new Base());
    
    TTT_EXPECT (a->whoami() == std::string ("Base / 2"));
    
    b = prim::Pointer<Base> (new Derived());
    
    TTT_EXPECT (a->whoami() == std::string ("Base / 2"));
    TTT_EXPECT (b->whoami() == std::string ("Derived / 1"));
    //
    }
            
TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if 0
}
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if 0
void test104__complex() {
#endif 

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (PrimComplex, 104, "Prim - Complex")

    {
    //
    prim::Complex<double> a;
    prim::Complex<double> b;
    
    TTT_EXPECT (a.getX()      == 0.0);
    TTT_EXPECT (a.getY()      == 0.0);
    TTT_EXPECT (a.magnitude() == 0.0);
    TTT_EXPECT (a.angle()     == 0.0);      /* < https://fr.wikipedia.org/wiki/Atan2 > */

    a.setPolar (prim::kHalfPi, 1.0);
    b.setPolar (0.0, 1.0);
    
    prim::Complex<double> c;
                
    c = a + b;
    
    TTT_EXPECT (prim::Math::abs (c.angle() - prim::kPi / 4.0) < kEpsilon);
    TTT_EXPECT (prim::Math::abs (c.magnitude() - sqrt (2.0))  < kEpsilon);
    TTT_EXPECT (prim::Math::abs (c.getX() - 1.0) < kEpsilon);
    TTT_EXPECT (prim::Math::abs (c.getY() - 1.0) < kEpsilon);
    
    c *= -1.0;
    
    TTT_EXPECT (prim::Math::abs (c.angle() - (prim::kPi + (prim::kPi / 4.0))) < kEpsilon);
    TTT_EXPECT (prim::Math::abs (c.magnitude() - sqrt (2.0)) < kEpsilon);
    TTT_EXPECT (prim::Math::abs (c.getX() + 1.0) < kEpsilon);
    TTT_EXPECT (prim::Math::abs (c.getY() + 1.0) < kEpsilon);
    
    c = a - b;
    
    TTT_EXPECT (prim::Math::abs (c.angle() - (prim::kPi - (prim::kPi / 4.0))) < kEpsilon);
    TTT_EXPECT (prim::Math::abs (c.magnitude() - sqrt (2.0)) < kEpsilon);
    TTT_EXPECT (prim::Math::abs (c.getX() + 1.0) < kEpsilon);
    TTT_EXPECT (prim::Math::abs (c.getY() - 1.0) < kEpsilon);
    
    c *= -1.0;
    
    TTT_EXPECT (prim::Math::abs (c.angle() - (prim::kTwoPi - (prim::kPi / 4.0))) < kEpsilon);
    TTT_EXPECT (prim::Math::abs (c.magnitude() - sqrt (2.0)) < kEpsilon);
    TTT_EXPECT (prim::Math::abs (c.getX() - 1.0) < kEpsilon);
    TTT_EXPECT (prim::Math::abs (c.getY() + 1.0) < kEpsilon);
    //
    }
    
    {
    //
    for (int i = 0; i < kAgain; ++i) {
    //
    prim::Complex<double> a (getRandomDouble(), getRandomDouble());
    prim::Complex<double> b (a);
    
    TTT_EXPECT (a == b);
    
    b *= prim::Complex<double>::withPolar (prim::kPi, 1.0);
    
    TTT_EXPECT (prim::Math::abs (a.getX() + b.getX()) < kEpsilon);
    TTT_EXPECT (prim::Math::abs (a.getY() + b.getY()) < kEpsilon);
    
    b *= prim::Complex<double>::withPolar (prim::kPi, 1.0);
    
    TTT_EXPECT (prim::Math::abs (a.getX() - b.getX()) < kEpsilon);
    TTT_EXPECT (prim::Math::abs (a.getY() - b.getY()) < kEpsilon);
    
    b /= prim::Complex<double>::withPolar (prim::kHalfPi, 1.0);
    
    TTT_EXPECT (prim::Math::abs (a.getX() + b.getY()) < kEpsilon);
    TTT_EXPECT (prim::Math::abs (a.getY() - b.getX()) < kEpsilon);
    
    b /= prim::Complex<double>::withPolar (prim::kPi, 1.0);
    
    TTT_EXPECT (prim::Math::abs (a.getX() - b.getY()) < kEpsilon);
    TTT_EXPECT (prim::Math::abs (a.getY() + b.getX()) < kEpsilon);
    //
    }
    
    for (int i = 0; i < kAgain; ++i) {
    //
    prim::Complex<double> a (getRandomDouble(), getRandomDouble());
    prim::Complex<double> b = -a;
    
    a *= prim::Complex<double>::withPolar (prim::kPi, 1.0);
    
    TTT_EXPECT (prim::Math::abs (a.getX() - b.getX()) < kEpsilon);
    TTT_EXPECT (prim::Math::abs (a.getY() - b.getY()) < kEpsilon);
    //
    }
    
    for (int i = 0; i < kAgain; ++i) {   /* < https://en.wikipedia.org/wiki/Triangle_inequality > */
    //
    bool t1, t2, t3, t4;
    
    int x1 = getRandomInteger (10 + 1) - 5;
    int y1 = getRandomInteger (10 + 1) - 5;
    int x2 = getRandomInteger (10 + 1) - 5;
    int y2 = getRandomInteger (10 + 1) - 5;
    
    prim::Complex<int> a (x1, y1);
    prim::Complex<int> b (x2, y2);
    prim::Complex<int> c;
    prim::Complex<int> d;
    
    c = a + b;
    d = a - b;
    
    double mag = prim::Math::max (c.magnitude(), d.magnitude());
    double ang = prim::Math::abs (a.angle() - b.angle());
    
    t1 = (a.magnitude() + b.magnitude() - mag > kEpsilon);
    t2 = (a.angle() == 0.0) && (a.magnitude() == 0.0);
    t3 = (b.angle() == 0.0) && (b.magnitude() == 0.0);
    t4 = (ang < kEpsilon || prim::Math::abs (ang - prim::kPi) < kEpsilon);
    
    TTT_EXPECT (t1 || t2 || t3 || t4);      /* Inequality or collinearity. */
    //
    }
    
    for (int i = 0; i < kAgain; ++i) {
    //
    prim::Complex<double> a (getRandomDouble(), getRandomDouble());
    prim::Complex<double> b;
    
    double f, k = static_cast<double> (getRandomInteger (10) + 1);
    do { f = (getRandomDouble() * k) - (k / 2.0); } while (prim::Math::abs (f) < kEpsilon);
    
    b = a * f;   /* Multiply by a scalar. */
    
    double ang = prim::Math::abs (b.angle() - a.angle());
    double mag = prim::Math::abs (b.magnitude() / f);
    
    TTT_EXPECT (ang < kEpsilon || prim::Math::abs (ang - prim::kPi) < kEpsilon);
    TTT_EXPECT (mag - a.magnitude() < kEpsilon);
    //
    }
    //
    }
    
TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if 0
}
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if 0
void test105__rational() {
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (PrimRational, 105, "Prim - Rational")

    /* Basic. */
    
    {
    //
    prim::Ratio a (3, 9);
    
    TTT_EXPECT (a.getNumerator()   == 1);
    TTT_EXPECT (a.getDenominator() == 3);
    
    a = prim::Ratio (221, 782);
    
    TTT_EXPECT (a.getNumerator()   == 13);
    TTT_EXPECT (a.getDenominator() == 46);
    TTT_EXPECT (a.isNegative()     == false);
    
    a = prim::Ratio (30, -100);

    TTT_EXPECT (a.getNumerator()   == -3);
    TTT_EXPECT (a.getDenominator() == 10);
    TTT_EXPECT (a.isNegative()     == true);
    
    a = prim::Ratio (-2, -4);
    
    TTT_EXPECT (a.getNumerator()   == 1);
    TTT_EXPECT (a.getDenominator() == 2);
    TTT_EXPECT (a.isNegative()     == false);
    //
    }
    
    /* More with random. */
    
    {
    //
    for (int i = 0; i < kAgain; ++i) {
    //
    prim::int64 numerator   = kPrime1[getRandomInteger (prim::sizeOfArray (kPrime1))];
    prim::int64 denominator = kPrime2[getRandomInteger (prim::sizeOfArray (kPrime2))];
    prim::int64 n           = getRandomInteger();
    
    prim::Ratio a (numerator * n, denominator * n);
    
    TTT_EXPECT (a.getNumerator()   == numerator);
    TTT_EXPECT (a.getDenominator() == denominator);
    //
    }
    //
    }
    
    /* Zero / Undefined. */
    
    {
    //
    prim::Ratio a;
    
    TTT_EXPECT (a.isZero() == true);
    
    a = prim::Ratio (0, 0);
    
    TTT_EXPECT (PD_FLOAT64_IS_INVALID_OR_ZERO (a.asDouble()));
    
    a = prim::Ratio (0, -1234);

    TTT_EXPECT (a.getNumerator()   == 0);
    TTT_EXPECT (a.getDenominator() == 1);
    TTT_EXPECT (a.isUndefined()    == false);
    TTT_EXPECT (a.isZero()         == true);
    
    a = prim::Ratio (4, 0);

    TTT_EXPECT (a.getNumerator()   == 0);
    TTT_EXPECT (a.getDenominator() == 0);
    TTT_EXPECT (a.isUndefined()    == true);
    TTT_EXPECT (a.isZero()         == false);
    
    a = 7;
    
    TTT_EXPECT (a.getNumerator()   == 7);
    TTT_EXPECT (a.getDenominator() == 1);
    //
    }
    
    /* Math operations. */
    
    {
    //
    prim::Ratio a;
    
    a = 2 + prim::Ratio (1, 2);
    
    TTT_EXPECT (a.getNumerator()   == 5);
    TTT_EXPECT (a.getDenominator() == 2);
    
    a = prim::Ratio (1, 4) - 1;
    
    TTT_EXPECT (a.getNumerator()   == -3);
    TTT_EXPECT (a.getDenominator() == 4);
    
    a *= prim::Ratio (-2, 5);
    
    TTT_EXPECT (a.getNumerator()   == 3);
    TTT_EXPECT (a.getDenominator() == 10);
    
    a /= prim::Ratio (-6, 7);
    
    TTT_EXPECT (a.getNumerator()   == -7);
    TTT_EXPECT (a.getDenominator() == 20);
    //
    }
    
    {
    //
    prim::Ratio a (1, 2);
    prim::Ratio b (3, 10);
    prim::Ratio c (2, 3);
    
    TTT_EXPECT (b < a && a < c);
    TTT_EXPECT (c > a && a > b);
    TTT_EXPECT (b != c);
    //
    }
    
TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if 0
}
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if 0
void test106__array() {
#endif 

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (PrimArray, 106, "Prim - Array")

    /* Default constructed / Autoreleased. */
    
    {
    //
    prim::Array<int> a;
    
    a.resize (1024);
    
    for (int i = 0; i < a.size(); ++i) { TTT_EXPECT (a[i] == 0); }
    //
    }
    
    {
    //
    Dummy dummy;
    
    prim::Array<Dummy> a;
    
    a.resize (4);
    TTT_EXPECT (a.getFirst().whoami() == std::string ("Dummy / 5"));
    
    a.resize();
    TTT_EXPECT (dummy.whoami() == std::string ("Dummy / 1"));
    //
    }
    
    {
    //
    Dummy dummy;
    
    prim::Array<prim::Pointer<Dummy>> a;
    
    a.add (prim::Pointer<Dummy> (new Dummy()));
    TTT_EXPECT (a.getFirst()->whoami() == std::string ("Dummy / 2"));
    
    a.getFirst() = new Dummy();
    TTT_EXPECT (a.getFirst()->whoami() == std::string ("Dummy / 2"));
    
    a.clear();
    TTT_EXPECT (dummy.whoami() == std::string ("Dummy / 1"));
    
    a.resize (10);
    TTT_EXPECT (a.size() == 10);
    for (int i = 0; i < a.size(); ++i) { TTT_EXPECT (a[i] == nullptr); }
    //
    }
    
    {
    //
    prim::Array<prim::Pointer<Base>> a;
    prim::Array<prim::Pointer<Base>> b;
    
    prim::Pointer<Base> p (new Base());
    
    b.add (p);
    b.add (p);
    
    p = nullptr;
    
    TTT_EXPECT (b.getFirst()->whoami() == std::string ("Base / 1"));
    TTT_EXPECT (b.getLast()->whoami()  == std::string ("Base / 1"));
    
    b.clear();
    
    p = new Base();
    
    for (int i = 0; i < 4; ++i) { a.add (p); }
    
    b = a;
    prim::Array<prim::Pointer<Base>> c (b);
    
    TTT_EXPECT (b.size() == a.size());
    TTT_EXPECT (c.size() == b.size());
            
    TTT_EXPECT (p->whoami() == std::string ("Base / 1"));
            
    for (int i = 0; i < b.size(); ++i) { TTT_EXPECT (b[i] != nullptr); }
    for (int i = 0; i < c.size(); ++i) { TTT_EXPECT (c[i] != nullptr); }
    
    a.clear();
    b.clear();
    c.clear();
    
    TTT_EXPECT (p->whoami() == std::string ("Base / 1"));
    //
    }

    {
    //
    prim::Array<prim::Pointer<Base>> a;
    prim::Array<prim::Pointer<Base>> b;
    
    a.resize (4);
    
    for (int i = 0; i < a.size(); ++i) { a[i] = new Base(); }
    
    a.swapWith (b);
    
    TTT_EXPECT (a.size() == 0);
    TTT_EXPECT (b.size() == 4);
    
    b.resize (8);
    
    TTT_EXPECT (b.size() == 8);
    
    for (int i = 0; i < 4; ++i) { TTT_EXPECT (b[i] != nullptr); }
    for (int i = 4; i < 8; ++i) { TTT_EXPECT (b[i] == nullptr); }
    
    b.resize (4);
    
    TTT_EXPECT (b.getFirst()->whoami() == std::string ("Base / 4"));
    
    b.resize (3);
    
    TTT_EXPECT (b.getFirst()->whoami() == std::string ("Base / 3"));
    //
    }

    /* Polymorphism. */
    
    {
    //
    prim::Array<prim::Pointer<Base>> a;
    
    a.resize (4);
    
    a[0] = new Base();
    a[1] = new Base();
    a[2] = new Base();
    a[3] = new Derived();

    TTT_EXPECT (a[0]->whoami() == std::string ("Base / 4"));
    TTT_EXPECT (a[1]->whoami() == std::string ("Base / 4"));
    TTT_EXPECT (a[2]->whoami() == std::string ("Base / 4"));
    TTT_EXPECT (a[3]->whoami() == std::string ("Derived / 1"));
    
    a.rotate();
    
    TTT_EXPECT (a[0]->whoami() == std::string ("Derived / 1"));
    TTT_EXPECT (a[1]->whoami() == std::string ("Base / 4"));
    TTT_EXPECT (a[2]->whoami() == std::string ("Base / 4"));
    TTT_EXPECT (a[3]->whoami() == std::string ("Base / 4"));
    //
    }
    
    /* Sortable with Pointer. */
    
    {
    //
    prim::Array<prim::Pointer<Base>> a;
    
    a.sort();
    a.resize (6);
    
    prim::Pointer<Base> p (new Derived());
    
    a[1] = new Base();
    a[3] = p;
    a[5] = new Base();

    a.sort();
    
    TTT_EXPECT (a[0] == nullptr);
    TTT_EXPECT (a[1] == nullptr);
    TTT_EXPECT (a[2] == nullptr);
    TTT_EXPECT (a[3] != nullptr);
    TTT_EXPECT (a[4] != nullptr);
    TTT_EXPECT (a[5] != nullptr);
    
    int found = a.indexOfSorted (p);
    
    TTT_EXPECT (found >= 0);
    TTT_EXPECT (a[found]->whoami() == std::string ("Derived / 1"));
    //
    }
    
    /* More with random. */
    
    {
    //
    for (int i = 0; i < kAgain; ++i) {
    //
    prim::Array<int> a;
    
    TTT_EXPECT (a.size() == 0);
    
    int k = getRandomInteger();
    
    a.add (1234);
    a.add (k);
    a.add (k);
    a.add (4321);
    
    TTT_EXPECT (a.size() == 4);
    TTT_EXPECT (a.get (1) == k);
    TTT_EXPECT (a.getFirst() == 1234);
    TTT_EXPECT (a.getLast() == 4321);
    
    a.resize (10);
    
    TTT_EXPECT (a.size() == 10);
    TTT_EXPECT (a.get (2) == k);
    
    k = getRandomInteger();
    a.getLast() = k;
    
    TTT_EXPECT (a.get (a.size() - 1) == k);
    //
    }
    //
    }
    
    {
    //
    for (int i = 0; i < kAgain; ++i) {
    //
    prim::Array<int> a;
    
    int k = getRandomInteger();
    
    a.resize (10);
    a.setAll (~k);
    
    for (int j = 0; j < a.size(); ++j) { TTT_EXPECT (a[j] == ~k); }
    
    TTT_EXPECT (a.contains (k) == false);
    a.getLast() = k;
    TTT_EXPECT (a.indexOf (k) == a.size() - 1);
    a.resize (a.size() - 1);
    TTT_EXPECT (a.contains (k) == false);
    //
    }
    //
    }
    
    {
    //
    for (int i = 0; i < kAgain; ++i) {
    //
    prim::Array<int> a;
    
    a.resize (256);
    
    for (int j = 0; j < a.size(); ++j) { a[j] = getRandomInteger(); }
    
    prim::Array<int> b (a);
    prim::Array<int> c;
    prim::Array<int> d;
    
    c = b;
    d.swapWith (c);
    
    TTT_EXPECT (b.size() == a.size());
    TTT_EXPECT (c.size() == 0);
    TTT_EXPECT (d.size() == a.size());
    
    for (int j = 0; j < a.size(); ++j) { TTT_EXPECT (a[j] == d[j]); }
    //
    }
    //
    }
    
    {
    //
    for (int i = 0; i < kAgain; ++i) {
    //
    prim::Array<double> a;
    prim::Array<double> b;
    
    a.resize (16);
    for (int j = 0; j < a.size(); ++j) { a[j] = getRandomDouble(); }
    b = a;
    TTT_EXPECT (b.size() == 16);
    
    int n = getRandomInteger (b.size() * 2);
    b.resize (n);
    
    TTT_EXPECT (b.size() == n);
    
    for (int j = 0; j < prim::Math::min (a.size(), b.size()); ++j) { TTT_EXPECT (a[j] == b[j]); }
    //
    }
    //
    }
    
    {
    //
    for (int i = 0; i < kAgain; ++i) {
    //
    prim::Array<double> a;
    prim::Array<double> b;
    
    a.resize (getRandomInteger (30) + 2);
    
    for (int j = 0; j < a.size(); ++j) { a[j] = getRandomDouble(); }
    
    b = a;
    
    for (int j = 0; j < a.size(); ++j) {
        double f = a.getLast();
        a.rotate();
        TTT_EXPECT (a.getFirst() == f);
    }
    
    for (int j = 0; j < a.size(); ++j) { TTT_EXPECT (a[j] == b[j]); }
    //
    }
    //
    }
    
    {
    //
    for (int i = 0; i < kAgain; ++i) {
    //
    prim::Array<double> a;
    prim::Array<double> b;
    
    a.resize (getRandomInteger (32));
    
    for (int j = 0; j < a.size(); ++j) { a[j] = getRandomDouble(); }
    
    b = a;
    b.reverse();
    TTT_EXPECT (a.size() == b.size());
    
    for (int j = 0; j < a.size(); ++j) { TTT_EXPECT (a[j] == b[b.size() - 1 - j]); }
    
    b.reverse();
    
    for (int j = 0; j < a.size(); ++j) { TTT_EXPECT (a[j] == b[j]); }
    //
    }
    //
    }
            
TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if 0
}
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if 0
void test107__table() {
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (PrimTable, 107, "Prim - Table")

    /* Basic / Empty. */
    
    {
    //
    prim::Table<prim::String, bool> a (false);
    
    TTT_EXPECT (a.size()  == 0);
    TTT_EXPECT (a["Toto"] == false);
    TTT_EXPECT (a.size()  == 0);

    a[""] = true;
    a["Toto"] = true;
    
    TTT_EXPECT (a.size()  == 2);
    TTT_EXPECT (a[""]     == true);
    TTT_EXPECT (a["Toto"] == true);
    
    a[""] = false;
    
    TTT_EXPECT (a.size()  == 1);
    TTT_EXPECT (a[""]     == false);
    TTT_EXPECT (a["Toto"] == true);
    //
    }
    
    /* Copyable / Assignable. */
    
    {
    //
    prim::Table<prim::String, bool> a (false);
    
    a["Toto"] = true;
    a["Momo"] = true;
    a["Jojo"] = true;
    
    TTT_EXPECT (a.size() == 3);
    
    a["Momo"] = false;
    
    TTT_EXPECT (a.size()  == 2);
    TTT_EXPECT (a["Toto"] == true);
    TTT_EXPECT (a["Jojo"] == true);
    
    a.clear();
    
    TTT_EXPECT (a.size() == 0);
    
    a["Choux"]    = true;
    a["Genoux"]   = true;
    a["Hiboux"]   = true;
    a["Cailloux"] = true;
    a["Bijoux"]   = true;
    a["Joujoux"]  = true;
    a["Poux"]     = true;
    
    const prim::Table<prim::String, bool> b (a);
    
    a.clear();
    
    TTT_EXPECT (b["Cailloux"] == true);
    TTT_EXPECT (b["Zoulous"]  == false);
    
    prim::Table<prim::String, bool> c (false);
    
    c = b;
    
    TTT_EXPECT (c["Cailloux"] == true);
    
    a.swapWith (c);
    
    TTT_EXPECT (a["Cailloux"] == true);
    TTT_EXPECT (c["Cailloux"] == false);
    
    TTT_EXPECT (a.size() == 7);
    TTT_EXPECT (c.size() == 0);
    //
    }
    
    /* Keys are sorted. */
    
    {
    //
    prim::Table<int> a (-1);
    
    a[-10]  = 10;
    a[0]    = -10;
    a[4321] = 1234;
    a[1234] = 0;
    a[-1]   = 7;
    
    TTT_EXPECT (a.getAtIndex (0).getKey() == -10);
    TTT_EXPECT (a.getAtIndex (1).getKey() == -1);
    TTT_EXPECT (a.getAtIndex (2).getKey() == 0);
    TTT_EXPECT (a.getAtIndex (3).getKey() == 1234);
    TTT_EXPECT (a.getAtIndex (4).getKey() == 4321);
    
    TTT_EXPECT (a.getAtIndex (0).getValue() == 10);
    TTT_EXPECT (a.getAtIndex (1).getValue() == 7);
    TTT_EXPECT (a.getAtIndex (2).getValue() == -10);
    TTT_EXPECT (a.getAtIndex (3).getValue() == 0);
    TTT_EXPECT (a.getAtIndex (4).getValue() == 1234);

    a[-10]  = -1;
    a[0]    = -1;
    a[4321] = -1;
    a[1234] = -1;
    
    TTT_EXPECT (a.size() == 1);
    
    a[-1]   = -1;
    
    TTT_EXPECT (a.size() == 0);
    //
    }
    
    /* Pointer as values / Polymorphism. */
    
    {
    //
    prim::Table<prim::String, prim::Pointer<Base>> a ((prim::Pointer<Base> (nullptr)));
    
    a["Toto"] = new Base();
    a["Momo"] = new Base();
    a["Jojo"] = new Base();
    
    TTT_EXPECT (a["Toto"]->whoami() == std::string ("Base / 3"));
    
    a["Momo"] = nullptr;
    
    TTT_EXPECT (a["Toto"]->whoami() == std::string ("Base / 2"));
    
    prim::Table<prim::String, prim::Pointer<Base>> b (a);
    
    a.clear();
    
    TTT_EXPECT (b["Jojo"]->whoami() == std::string ("Base / 2"));

    b.clear();
    
    prim::Pointer<Base> p (new Derived());
    
    a["Toto"] = p;
    a["Momo"] = p;
    a["Jojo"] = p;
    
    TTT_EXPECT (a["Jojo"]->whoami() == std::string ("Derived / 1"));
    
    a["Toto"] = nullptr;
    a["Momo"] = nullptr;
    a["Jojo"] = nullptr;
    
    TTT_EXPECT (p->whoami() == std::string ("Derived / 1"));
    //
    }

    /* Pointer as keys / Polymorphism. */
    /* A bit weird and very nasty. */
     
    {
    //
    prim::Table<prim::Pointer<Base>, int> a (0);
    
    prim::Pointer<Base> toto (new Base());
    prim::Pointer<Base> jojo (new Derived());
    prim::Pointer<Base> momo (new Base());
    prim::Pointer<Base> zozo (nullptr);
    
    a[toto] = 1234;
    a[jojo] = -1;
    a[momo] = 4321;
    a[zozo] = 666;

    TTT_EXPECT (a[toto] == 1234);
    TTT_EXPECT (a[jojo] == -1);
    TTT_EXPECT (a[momo] == 4321);
    TTT_EXPECT (a[zozo] == 666);
    
    /* Scratch you head. */
    
    {
        prim::Pointer<Base> t (momo);
    
        TTT_EXPECT (a[t] == 4321);
    
        t = jojo;
        
        TTT_EXPECT (a[t] == -1);
        
        toto = nullptr;
    
        TTT_EXPECT (a[toto] == 666);
    }
    
    a[jojo] = 0;
    a[zozo] = 0;
    
    TTT_EXPECT (a.size() == 2);
    
    jojo = nullptr;
    
    for (int i = 0; i < a.size(); ++i) {
        TTT_EXPECT (a.getAtIndex (i).getKey()->whoami() == std::string ("Base / 2"));
    }
    //
    }

    /* Box as values. */
    
    {
    //
    prim::Table<int, prim::Box> a (prim::Box::empty());
    
    prim::Point pt1 (0.0, 0.0);
    prim::Point pt2 (2.0, 0.0);
    
    a[1] = prim::Box (pt1, pt2);
    a[2] = prim::Box (pt1, pt2);
    
    TTT_EXPECT (a.size() == 2);
    TTT_EXPECT (a[1].getBottomLeft() == pt1);
    TTT_EXPECT (a[1].getTopRight() == pt2);
    
    a[1] = prim::Box();
    a[2] = prim::Box::empty();
    
    TTT_EXPECT (a.size() == 0);
    //
    }

    /* Interval as keys. */
     
    {
    //
    prim::Table<mica::MIR::Interval, int> t (-1);

    t[mica::MIR::Interval::withNotes (mica::C5, mica::E4)] = 4;
    t[mica::MIR::Interval::withNotes (mica::B4, mica::C5)] = 3;
    t[mica::MIR::Interval::withNotes (mica::F4, mica::C5)] = 2;
    t[mica::MIR::Interval::withNotes (mica::F4, mica::G4)] = 1;
    
    TTT_EXPECT (t.size() == 4);

    TTT_EXPECT (t[mica::MIR::Interval::withNotes (mica::C5, mica::E4)] == 4);
    TTT_EXPECT (t[mica::MIR::Interval::withNotes (mica::B4, mica::C5)] == 3);
    TTT_EXPECT (t[mica::MIR::Interval::withNotes (mica::F4, mica::C5)] == 2);
    TTT_EXPECT (t[mica::MIR::Interval::withNotes (mica::F4, mica::G4)] == 1);

    t[mica::MIR::Interval::withNotes (mica::F4, mica::G4)] = -1;
    t[mica::MIR::Interval::withNotes (mica::F4, mica::C5)] = -1;
    
    TTT_EXPECT (t.size() == 2);
    //
    }

    /* Tuple as keys. */
    
    {
    //
    prim::Table<mica::MIR::Tuple<3>, int> t (-1);
    
    t[mica::MIR::Tuple<3> ( { 1, 2, 3 } )] = 123;
    t[mica::MIR::Tuple<3> ( { 4, 5, 6 } )] = 456;
    t[mica::MIR::Tuple<3> ( { 7, 8, 9 } )] = 789;
    t[mica::MIR::Tuple<3> ( { 1, 5, 9 } )] = 159;
    t[mica::MIR::Tuple<3> ( { 7, 5, 3 } )] = 753;
    t[mica::MIR::Tuple<3> ( { 1, 4, 7 } )] = 147;
    t[mica::MIR::Tuple<3> ( { 3, 6, 9 } )] = 369;
    
    TTT_EXPECT (t[mica::MIR::Tuple<3> ( { 1, 2, 3 } )] == 123);
    TTT_EXPECT (t[mica::MIR::Tuple<3> ( { 4, 5, 6 } )] == 456);
    TTT_EXPECT (t[mica::MIR::Tuple<3> ( { 7, 8, 9 } )] == 789);
    TTT_EXPECT (t[mica::MIR::Tuple<3> ( { 1, 5, 9 } )] == 159);
    TTT_EXPECT (t[mica::MIR::Tuple<3> ( { 7, 5, 3 } )] == 753);
    TTT_EXPECT (t[mica::MIR::Tuple<3> ( { 1, 4, 7 } )] == 147);
    TTT_EXPECT (t[mica::MIR::Tuple<3> ( { 3, 6, 9 } )] == 369);
    
    TTT_EXPECT (t.size() == 7);
    
    t[mica::MIR::Tuple<3> ( { 4, 5, 6 } )] = -1;
    t[mica::MIR::Tuple<3> ( { 1, 5, 9 } )] = -1;
    t[mica::MIR::Tuple<3> ( { 1, 4, 7 } )] = -1;
    
    TTT_EXPECT (t.size() == 4);
    
    TTT_EXPECT (t[mica::MIR::Tuple<3> ( { 1, 2, 3 } )] == 123);
    TTT_EXPECT (t[mica::MIR::Tuple<3> ( { 7, 8, 9 } )] == 789);
    TTT_EXPECT (t[mica::MIR::Tuple<3> ( { 7, 5, 3 } )] == 753);
    TTT_EXPECT (t[mica::MIR::Tuple<3> ( { 3, 6, 9 } )] == 369);
    //
    }
            
TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if 0
}
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if 0
void test108__string() {
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (PrimString, 108, "Prim - String")

    /* Default string. */
    
    {
    //
    prim::String s1 = prim::String ("");
    prim::String s2;
    
    TTT_EXPECT (s2 == s1);
    
    s2 = prim::String ("Cailloux");
    
    TTT_EXPECT (s2 != s1);
    
    s1 = s2;
    
    TTT_EXPECT (s1 == s2);
    //
    }
    
    /* Comparaison operations. */
    
    {
    //
    TTT_EXPECT (prim::String ("salade") > prim::String ("carottes"));
    TTT_EXPECT (prim::String ("Salade") < prim::String ("carottes"));
    
    TTT_EXPECT (prim::String ("") == prim::String (""));
    
    TTT_EXPECT (prim::String ("ABCDEFGH") == prim::String ("ABCDEFGH"));
    TTT_EXPECT (prim::String ("ABCDEFGH") != prim::String ("ABCDEFG"));
    TTT_EXPECT (prim::String ("ABCDEFGH") != prim::String ("abcDEFGH"));
    TTT_EXPECT (prim::String ("ABCDEFGH") != prim::String (""));
    
    TTT_EXPECT (prim::String ("Chocolat") != prim::String ("Chocolaterie"));
    TTT_EXPECT (prim::String ("trombone") != prim::String ("trombose"));
    TTT_EXPECT (prim::String ("trombone") != prim::String ("trombose"));
    TTT_EXPECT (prim::String ("lucioles") != prim::String ("couilles"));
    //
    }
    
TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if 0
}
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if 0
void test109__graph() {
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (PrimGraph, 109, "Prim - Graph")

    /* Add. */
    
    {
    //
    Graph graph;
    
    prim::Pointer<Node> a (new Node());
    prim::Pointer<Node> b (new Node());
    prim::Pointer<Node> c (new Node());
    prim::Pointer<Node> z (nullptr);
    
    a->getObject().setValue (1234);
    b->getObject().setValue (1234);
    c->getObject().setValue (1234);
    
    graph.add (a);
    graph.add (b); graph.add (b); graph.add (z);
    graph.add (c);
    
    TTT_EXPECT (graph.size() == 3);
    TTT_EXPECT (graph.getRoot() == a);
    TTT_EXPECT (graph.getRoot()->getObject().getValue() == 1234);
    TTT_EXPECT (graph.getAllNodes().size() == 3);
    TTT_EXPECT (graph.getAllNodes().getLast()->getObject().getValue() == 1234);
    //
    }
    
    /* Connect / Next. */
    
    {
    //
    Graph graph;
    
    prim::Pointer<Node> a (new Node());
    prim::Pointer<Node> b (new Node());
    prim::Pointer<Node> c (new Node());
    prim::Pointer<Node> d (new Node());
    prim::Pointer<Node> e (new Node());
    prim::Pointer<Node> f (new Node());
    prim::Pointer<Node> g (new Node());
    
    a->getObject().setValue (0);
    b->getObject().setValue (10);
    c->getObject().setValue (11);
    d->getObject().setValue (12);
    e->getObject().setValue (20);
    f->getObject().setValue (21);
    g->getObject().setValue (22);
    
    graph.add (a);
    graph.add (b);
    graph.add (c);
    graph.add (d);
    graph.add (e);
    graph.add (f);
    graph.add (g);
    
    graph.connect (a, b)->getObject().setValue (-1);
    graph.connect (a, c)->getObject().setValue (-2);
    graph.connect (a, d)->getObject().setValue (-3);
    
    graph.connect (c, e)->getObject().setValue (-1);
    graph.connect (c, f)->getObject().setValue (-2);
    graph.connect (c, g)->getObject().setValue (-3);
    
    a = nullptr;
    b = nullptr;
    c = nullptr;
    d = nullptr;
    e = nullptr;
    f = nullptr;
    g = nullptr;
    
    prim::Pointer<Node> root = graph.getRoot();
    prim::Pointer<Node> t = graph.nextByEdge (root, -2);
    
    TTT_EXPECT (t != nullptr);
    TTT_EXPECT (t->getObject().getValue() == 11);
    
    t = graph.nextByEdge (t, -2);
    
    TTT_EXPECT (t != nullptr);
    TTT_EXPECT (t->getObject().getValue() == 21);
    
    t = graph.previousByEdge (t, -2);
    
    TTT_EXPECT (t != nullptr);
    TTT_EXPECT (t->getObject().getValue() == 11);
    
    t = graph.previousByEdge (t, -2);
    
    TTT_EXPECT (t != nullptr);
    TTT_EXPECT (t->getObject().getValue() == 0);
    //
    }
    
    /* Connect / All next.*/
    
    {
    //
    Graph graph;
        
    prim::Pointer<Node> a (new Node());
    prim::Pointer<Node> b (new Node());
    prim::Pointer<Node> c (new Node());
    prim::Pointer<Node> d (new Node());
    prim::Pointer<Node> e (new Node());
    
    a->getObject().setValue (0);
    b->getObject().setValue (10);
    c->getObject().setValue (11);
    d->getObject().setValue (12);
    e->getObject().setValue (20);
    
    graph.add (a);
    graph.add (b);
    graph.add (c);
    graph.add (d);
    graph.add (e);
    
    graph.connect (a, b)->getObject().setValue (1);
    graph.connect (a, c)->getObject().setValue (1);
    graph.connect (a, d)->getObject().setValue (0);
    
    graph.connect (b, e)->getObject().setValue (0);
    graph.connect (c, e)->getObject().setValue (1);
    graph.connect (d, e)->getObject().setValue (1);
    
    prim::Array<prim::Pointer<Node>> next (graph.allNextByEdge (a, 1));
    prim::Array<prim::Pointer<Node>> previous (graph.allPreviousByEdge (e, 1));
    
    TTT_EXPECT (next.size() == 2);
    TTT_EXPECT (previous.size() == 2);
    
    TTT_EXPECT (graph.nextByEdge (a, 1) == nullptr);
    TTT_EXPECT (graph.previousByEdge (e, 1) == nullptr);
    //
    }
    
    /* Topologic sort. */
    
    {
    //
    Graph graph;

    prim::Pointer<Node> a (new Node());
    prim::Pointer<Node> b (new Node());
    prim::Pointer<Node> c (new Node());
    prim::Pointer<Node> d (new Node());
    prim::Pointer<Node> e (new Node());
    prim::Pointer<Node> f (new Node());
    prim::Pointer<Node> g (new Node());
    
    a->getObject().setValue (0);
    b->getObject().setValue (1);
    c->getObject().setValue (2);
    d->getObject().setValue (3);
    e->getObject().setValue (4);
    f->getObject().setValue (5);
    g->getObject().setValue (6);
    
    graph.add (a);
    graph.add (b);
    graph.add (c);
    graph.add (d);
    graph.add (e);
    graph.add (f);
    graph.add (g);
    
    graph.connect (a, b);
    graph.connect (a, c);
    graph.connect (b, d);
    graph.connect (c, d);
    graph.connect (e, c);
    graph.connect (e, f);
    graph.connect (d, g);
    graph.connect (f, g);
    
    TTT_EXPECT (graph.getAllNodesSorted().size() == graph.size());
    
    graph.connect (a, e);
    graph.connect (c, a);
    
    TTT_EXPECT (graph.getAllNodesSorted().size() == 0);
    //
    }
    
    /* Drain. */
    
    {
    //
    Graph graph1;
    Graph graph2;
    Graph graph3;
    
    prim::Pointer<Node> a (new Node());
    prim::Pointer<Node> b (new Node());
    prim::Pointer<Node> c (new Node());
    
    prim::Pointer<Node> d (new Node());
    prim::Pointer<Node> e (new Node());
    prim::Pointer<Node> f (new Node());
    
    graph1.add (a);
    graph1.add (b);
    graph1.add (c);
    
    graph2.add (d);
    graph2.add (e);
    graph2.add (f);
    
    graph1.connect (a, b);
    graph1.connect (a, c);
    
    graph2.connect (d, e);
    graph2.connect (d, f);
    
    graph1.drain (graph2);
    
    TTT_EXPECT (graph1.size() == 6);
    TTT_EXPECT (graph2.size() == 0);
    
    graph3.drain (graph1);
    
    TTT_EXPECT (graph3.size() == 6);
    TTT_EXPECT (graph1.size() == 0);
    //
    }
    
    /* More with random. */
    
    for (int i = 0; i < kAgain; ++i) {
    //
    Graph graph;
    
    /* Create a randomly connected small graph. */
    
    prim::Array<prim::Pointer<Node>> array;
    array.resize (6);
    
    for (int j = 0; j < array.size(); ++j) {
        array[j] = new Node();
        array[j]->getObject().setValue (j);
        graph.add (array[j]);
    }
    
    for (int j = 0; j < array.size(); ++j) {
    //
    int a, b;
    
    do {
    //
    a = getRandomInteger (graph.size());
    b = getRandomInteger (graph.size());
    //
    } while (b == a);
    
    graph.connect (array[a], array[b])->getObject().setValue (0);
    //
    }
    
    /* < https://en.wikipedia.org/wiki/Topological_sorting > */
    
    prim::Array<prim::Pointer<Node>> topo = graph.getAllNodesSorted();
    
    if (topo.size() == 0) { /* Find strongly connected components if cyclic? */ }
    else {
    //
    /* Test the topologic sort. */
    
    TTT_EXPECT (topo.size() == graph.size());
    for (int j = 0; j < topo.size(); ++j) {
        prim::Array<prim::Pointer<Node>> childs = graph.allNextByEdge (topo[j], 0);
        for (int t = 0; t < childs.size(); ++t) {
            TTT_EXPECT (topo.indexOf (childs[t]) > j);
        }
    }
    //
    }
    //
    }
            
TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if 0
}
#endif
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if 0
void test110__utils() {
#endif 

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (PrimUtils, 110, "Prim - Utils")

    std::string s;
    
    s = prim::Utils::asHex (0);
    TTT_EXPECT (s == std::string ("0"));
    TTT_EXPECT (prim::Utils::withHex (s) == 0);
    s = prim::Utils::asHex (1234);
    TTT_EXPECT (s == std::string ("4d2"));
    TTT_EXPECT (prim::Utils::withHex (s) == 1234);
    s = prim::Utils::asHex (-1234);
    TTT_EXPECT (s == std::string ("fffffffffffffb2e"));
    TTT_EXPECT (prim::Utils::withHex (s) == -1234);
    s = prim::Utils::asHex (65535);
    TTT_EXPECT (s == std::string ("ffff"));
    TTT_EXPECT (prim::Utils::withHex (s) == 65535);
    
    s = prim::Utils::paddedLeft ("7", 10);
    TTT_EXPECT (s == std::string ("         7"));
    s = prim::Utils::paddedLeft ("o", 10, 'x');
    TTT_EXPECT (s == std::string ("xxxxxxxxxo"));
    s = prim::Utils::paddedLeft ("123456789", 4, '?');
    TTT_EXPECT (s == std::string ("123456789"));
    s = prim::Utils::paddedLeft ("123456789", -1234, '?');
    TTT_EXPECT (s == std::string ("123456789"));
    
TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if 0
}
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if 0
void test111__rectangle() {
#endif 

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (PrimRectangle, 111, "Prim - Rectangle")

    /* Basic. */
    
    {
    //
    prim::Box rect1;
    
    TTT_EXPECT (rect1.isEmpty() == true);
    TTT_EXPECT (rect1 == prim::Box::empty());
    
    prim::Point a (2.0, 2.0);
    prim::Point b (6.0, 4.0);
    
    rect1 = prim::Box (a, b);
    
    TTT_EXPECT (rect1.getWidth()  == 4.0);
    TTT_EXPECT (rect1.getHeight() == 2.0);
    
    prim::Point c (-1.0, -3.0);
    
    prim::Box rect2 (a, c);
    
    TTT_EXPECT (rect2.getLeft()   == -1.0);
    TTT_EXPECT (rect2.getRight()  == 2.0);
    TTT_EXPECT (rect2.getTop()    == 2.0);
    TTT_EXPECT (rect2.getBottom() == -3.0);
    TTT_EXPECT (rect2.getWidth()  == 3.0);
    TTT_EXPECT (rect2.getHeight() == 5.0);
    
    rect2.setTopRight (b - a);
    
    TTT_EXPECT (rect2.getRight()  == 4.0);
    TTT_EXPECT (rect2.getTop()    == 2.0);
    
    prim::Box bound = rect1 + rect2;
    
    TTT_EXPECT (bound.getLeft()   == -1.0);
    TTT_EXPECT (bound.getRight()  == 6.0);
    TTT_EXPECT (bound.getTop()    == 4.0);
    TTT_EXPECT (bound.getBottom() == -3.0);
    
    bound += prim::Point (-2.0, -2.0);
    
    TTT_EXPECT (bound.getLeft()   == -2.0);
    TTT_EXPECT (bound.getWidth()  == 8.0);
    
    TTT_EXPECT (bound.contains (prim::Point (-2.0, -2.0)) == true);
    TTT_EXPECT (bound.contains (prim::Point (1.0, 1.0))   == true);
    TTT_EXPECT (bound.contains (prim::Point (-3.0, -2.0)) == false);
    TTT_EXPECT (bound.contains (prim::Point (-1.0, 7.0))  == false);
    //
    }
    
    /* Empty / Equality. */
    
    {
    //
    prim::Box rect1, rect2;
    TTT_EXPECT (rect1 == rect2);
    
    rect1 = prim::Box (prim::Point (0.0, 0.0));
    TTT_EXPECT (rect1 != rect2);
    
    rect2 = rect1;
    TTT_EXPECT (rect1 == rect2);
    //
    }
    
    /* More with random. */
    
    for (int i = 0; i < kAgain; ++i) {
    //
    int x1 = getRandomInteger (10 + 1) - 5;
    int y1 = getRandomInteger (10 + 1) - 5;
    int x2 = getRandomInteger (10 + 1) - 5;
    int y2 = getRandomInteger (10 + 1) - 5;
    
    prim::Point a (x1, y1);
    prim::Point b (x2, y2);
    
    prim::Box box (a, b);
    prim::Point center = box.getCenter();

    TTT_EXPECT ((box.getTopLeft() + box.getBottomRight() - center - center).magnitude() < kEpsilon);
    TTT_EXPECT ((box.getTopRight() + box.getBottomLeft() - center - center).magnitude() < kEpsilon);
    //
    }
            
TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if 0
}
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if 0
void test112__line() {
#endif 

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (PrimLine, 112, "Prim - Line")

    /* Segment geometry. */
    
    {
    //
    prim::Line l1 (0.0, 0.0, 1.0, 1.0);
    prim::Line l2 (1.0, 1.0, 0.0, 0.0);
    
    TTT_EXPECT (prim::Math::abs (l1.length() - sqrt (2.0)) < kEpsilon);
    TTT_EXPECT (prim::Math::abs (l1.angle() - (prim::kHalfPi / 2.0)) < kEpsilon);
    TTT_EXPECT (prim::Math::abs (l2.length() - sqrt (2.0)) < kEpsilon);
    TTT_EXPECT (prim::Math::abs (l2.angle() - (prim::kPi + (prim::kHalfPi / 2.0))) < kEpsilon);
    
    double d;
    l1.horizontalDistanceFromPoint (prim::Point (0.0, 0.5), d);
    TTT_EXPECT (prim::Math::abs (d + 0.5) < kEpsilon);
    l1.horizontalDistanceFromPoint (prim::Point (1.0, 0.5), d);
    TTT_EXPECT (prim::Math::abs (d - 0.5) < kEpsilon);
    
    prim::Line l3 = prim::Line::makeClipped (l1, 0.0, sqrt (2.0) / 2);
    prim::Line l4 = prim::Line::makeClipped (l2, sqrt (2.0) / 2, 0.0);
    
    TTT_EXPECT (prim::Math::abs (l3.length() - 1.0) < kEpsilon);
    TTT_EXPECT (prim::Math::abs (l4.length() - 1.0) < kEpsilon);
    
    TTT_EXPECT (prim::Math::abs (l3.getStart().getX() - l4.getEnd().getX()) < kEpsilon);
    TTT_EXPECT (prim::Math::abs (l3.getStart().getY() - l4.getEnd().getY()) < kEpsilon);
    //
    }

    /* Clockwise order. */
    
    {
    //
    prim::Point a (1.0, 1.0);
    prim::Point b (2.0, 2.0);
    prim::Point c (0.0, 3.0);
    
    prim::Point t (c);
    
    TTT_EXPECT (clockwiseOrder (a, b, t) < 0);      /* Counterclockwise. */
    t *= -1.0;
    TTT_EXPECT (clockwiseOrder (a, b, t) > 0);      /* Clockwise. */
    t *= 0.0;
    TTT_EXPECT (clockwiseOrder (a, b, t) == 0);     /* Collinear. */
    
    TTT_EXPECT (clockwiseOrder (a, a, t) == 0);     /* Special cases. */
    TTT_EXPECT (clockwiseOrder (a, t, t) == 0);
    TTT_EXPECT (clockwiseOrder (t, t, t) == 0);
    //
    }
    
    /* Intersections. */
    
    {
    //
    prim::Point a (1.0,  1.0);
    prim::Point b (2.0,  2.0);
    prim::Point c (0.0,  3.0);
    prim::Point d (-1.0, 2.0);
    
    prim::Line l1 (a, c);
    prim::Line l2 (b, d);
    
    TTT_EXPECT (l1.intersects (l2));
    TTT_EXPECT (l2.intersects (l1));
    
    l1 = prim::Line (a, b);
    l2 = prim::Line (c, d);
    
    TTT_EXPECT (l1.intersects (l2) == false);
    TTT_EXPECT (l2.intersects (l1) == false);
    
    l1 = prim::Line (a, b);
    l2 = prim::Line (c, d);
    
    TTT_EXPECT (l1.intersects (l2) == false);
    
    /* Special cases. */
    
    prim::Point e (0.0, 0.0);
    prim::Point f (0.0, 2.0);
    
    l1 = prim::Line (b, d);
    l2 = prim::Line (e, f);
    
    TTT_EXPECT (l1.intersects (l2) == true);
    
    l2 = prim::Line (b, a);
    
    TTT_EXPECT (l1.intersects (l2) == true);
    
    l1 = prim::Line (d, c);
    l2 = prim::Line (a, f);
    
    TTT_EXPECT (l1.intersects (l2) == false);
    
    l2 = prim::Line (f, f);
    
    TTT_EXPECT (l1.intersects (l2) == false);
    
    prim::Point g (-4.0, -4.0);
    
    l1 = prim::Line (a, b);
    l2 = prim::Line (e, g);
    
    TTT_EXPECT (l1.intersects (l2) == false);
    
    l1 = prim::Line (e, b);
    l2 = prim::Line (a, g);
    
    TTT_EXPECT (l1.intersects (l2) == true);
    //
    }
    
    {
    //
    prim::Line l1 (0.0, 0.0, 1.0, 1.0);
    
    prim::Point p1;
    prim::Point p2;
    
    bool b1 = l1.intersectionWithVertical (0.5, p1);
    bool b2 = l1.intersectionWithHorizontal (0.5, p2);
    
    TTT_EXPECT (b1 = true);
    TTT_EXPECT (b2 = true);
    TTT_EXPECT (p1.getX() - p2.getX() < kEpsilon);
    TTT_EXPECT (p1.getY() - p2.getY() < kEpsilon);
    
    TTT_EXPECT (l1.intersectionWithVertical (2.0, p1)    == false);
    TTT_EXPECT (l1.intersectionWithVertical (0.0, p2)    == true);
    
    TTT_EXPECT (l1.intersectionWithHorizontal (1.0, p1)  == true);
    TTT_EXPECT (l1.intersectionWithHorizontal (-2.0, p2) == false);
    
    prim::Line l2 (0.0, 0.0, 0.0, 0.0);
    
    p1 = p2 = prim::Point (1.0, 1.0);
    
    TTT_EXPECT (l2.intersectionWithVertical (0.0, p1)    == true);
    TTT_EXPECT (l2.intersectionWithHorizontal (0.0, p2)  == true);
    
    TTT_EXPECT (p1.getX() < kEpsilon);
    TTT_EXPECT (p1.getY() < kEpsilon);
    TTT_EXPECT (p2.getX() < kEpsilon);
    TTT_EXPECT (p2.getY() < kEpsilon);
    
    TTT_EXPECT (l2.intersectionWithVertical (1.0, p1)    == false);
    TTT_EXPECT (l2.intersectionWithHorizontal (1.0, p2)  == false);
    //
    }
    
    /* More with random. */
    
    for (int i = 0; i < kAgain; ++i) {
    //
    double x1 = getRandomInteger (10 + 1) - 5;
    double y1 = getRandomInteger (10 + 1) - 5;
    double x2 = getRandomInteger (10 + 1) - 5;
    double y2 = getRandomInteger (10 + 1) - 5;
    double x3 = getRandomInteger (10 + 1) - 5;
    double y3 = getRandomInteger (10 + 1) - 5;
    
    prim::Point a (x1, y1);
    prim::Point b (x2, y2);
    prim::Point c (x3, y3);
    
    int ccw = clockwiseOrder (a, b, c);
    
    prim::Vector v1 (b - a);
    prim::Vector v2 (c - a);
    double ang = 0.0;
    
    if ((v1.magnitude() != 0.0) && (v2.magnitude() != 0.0)) { ang = (v2 / v1).angle(); }

    if (ccw < 0)      { TTT_EXPECT (ang < prim::kPi); }     /* Counterclockwise. */
    else if (ccw > 0) { TTT_EXPECT (ang > prim::kPi); }     /* Clockwise. */
    else { 
        TTT_EXPECT (ang < kEpsilon || prim::Math::abs (ang - prim::kPi) < kEpsilon);  /* Collinear. */
    }
    //
    }
    
    /* < http://www.mathopenref.com/chordsintersecting.html > */
    
    for (int i = 0; i < kAgain; ++i) {
    //
    prim::Point a = prim::Point::withPolar (getRandomDouble() * prim::kTwoPi, 1.0);
    prim::Point b = prim::Point::withPolar (getRandomDouble() * prim::kTwoPi, 1.0);
    prim::Point c = prim::Point::withPolar (getRandomDouble() * prim::kTwoPi, 1.0);
    prim::Point d = prim::Point::withPolar (getRandomDouble() * prim::kTwoPi, 1.0);
    prim::Point p;
    
    prim::Line l1 (a, b);
    prim::Line l2 (c, d);
    
    if (l1.intersects (l2, p)) {
    //
    double pa = (a - p).magnitude();
    double pb = (b - p).magnitude();
    double pc = (c - p).magnitude();
    double pd = (d - p).magnitude();
    
    TTT_EXPECT (prim::Math::abs ((pa * pb) - (pc * pd)) < kEpsilon);
    //
    }
    //
    }
            
TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if 0
}
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if 0
void test113__polygon() {
#endif 

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (PrimPolygon, 113, "Prim - Polygon")

    /* Basic. */
    
    {
    //
    prim::Polygon a;
    prim::Polygon b;
    
    TTT_EXPECT (a.size() == 0);
    
    a.add (prim::Point (1.0,  2.0));
    a.add (prim::Point (-2.0, 3.0));
    
    b.swapWith (a);
    
    TTT_EXPECT (a.size() == 0);
    TTT_EXPECT (b.size() == 2);
    
    prim::Point pt = b.get (1);
    
    TTT_EXPECT (pt.getX() == -2.0);
    TTT_EXPECT (pt.getY() == 3.0);
    
    b.clear();
    
    TTT_EXPECT (b.size() == 0);
    //
    }
    
    /* Hull. */
    
    {
    //
    prim::Polygon a;
    
    a.add (prim::Point (0.0, -1.0));
    a.add (prim::Point (0.0,  1.0));
    a.add (prim::Point (-1.0, 2.0));
    a.add (prim::Point (3.0,  1.0));
    a.add (prim::Point (1.0,  1.0));
    a.add (prim::Point (-1.0, 0.0));
    a.add (prim::Point (2.0,  2.0));
    
    a.convertToConvexHull();
    
    TTT_EXPECT (a.size() == 5);
    
    TTT_EXPECT (a.get (0) == prim::Point (0.0, -1.0));
    TTT_EXPECT (a.get (1) == prim::Point (3.0,  1.0));
    TTT_EXPECT (a.get (2) == prim::Point (2.0,  2.0));
    TTT_EXPECT (a.get (3) == prim::Point (-1.0, 2.0));
    TTT_EXPECT (a.get (4) == prim::Point (-1.0, 0.0));
    //
    }
    
    {
    //
    prim::Polygon a;
    
    a.add (prim::Point (0.0, 1.0));
    a.add (prim::Point (0.0, 2.0));
    a.add (prim::Point (0.0, 0.0)); a.add (prim::Point (0.0, 0.0));
    
    a.convertToConvexHull();
    
    TTT_EXPECT (a.size() == 2);
    
    TTT_EXPECT (a.get (0) == prim::Point (0.0, 0.0));
    TTT_EXPECT (a.get (1) == prim::Point (0.0, 2.0));
    //
    }
    
    {
    //
    prim::Polygon a;
    
    a.add (prim::Point (-1.0, 0.0));
    a.add (prim::Point (-2.0, 0.0));
    a.add (prim::Point (-4.0, 0.0));
    a.add (prim::Point (4.0,  0.0));
    a.add (prim::Point (2.0,  0.0));
    a.add (prim::Point (1.0,  0.0));
    
    a.convertToConvexHull();
    
    TTT_EXPECT (a.size() == 2);
    
    TTT_EXPECT (a.get (0) == prim::Point (-4.0, 0.0));
    TTT_EXPECT (a.get (1) == prim::Point (4.0,  0.0));
    //
    }
    
    /* More with random. */
    
    for (int i = 0; i < kAgain; ++i) {
    //
    prim::Polygon a;
    
    for (int j = 0; j < 100; ++j) {
        a.add (prim::Point ((getRandomDouble() * 10.0) - 5.0, (getRandomDouble() * 10.0) - 5.0));
    }
    
    prim::Array<prim::Point> removed; a.convertToConvexHull (&removed);
    
    TTT_EXPECT (a.size() + removed.size() == 100);
    
    /* Any line between two removed points must NOT cross any side of the hull. */
    
    if (removed.size() > 1) {
    //
    for (int j = 0; j < removed.size(); ++j) {
    //
    int m, n;
    prim::Point p0, p1;
    
    do {
    //
    m = getRandomInteger (removed.size());
    n = getRandomInteger (removed.size());
    p0 = removed[m];
    p1 = removed[n];
    //
    } while (m == n);
    
    for (int t = 1; t < a.size(); ++t) {
    //
    prim::Point p2 (a.get (t - 1));
    prim::Point p3 (a.get (t));
    
    int t1 = clockwiseOrder (p0, p1, p2);
    int t2 = clockwiseOrder (p0, p1, p3);
    int t3 = clockwiseOrder (p2, p3, p0);
    int t4 = clockwiseOrder (p2, p3, p1);
    
    bool intersection = ((t1 * t2 < 0) && (t3 * t4 < 0));
    
    TTT_EXPECT (intersection == false);
    }
    //
    }
    //
    }
    //
    }
            
TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if 0
}
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if 0
void test114__sort() {
#endif 

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (PrimSort, 114, "Prim - Sort")
    
    {
    //
    for (int i = 0; i < kAgain; ++i) {
    //
    prim::Array<int> a;
    prim::Array<int> b;
        
    for (int j = 0; j < 100; ++j) { a.add (getRandomInteger()); }
        
    b = a;
    a.sort();
        
    for (int j = 0; j < b.size(); ++j) { TTT_EXPECT (a.indexOfSorted (b[j]) != -1); }
    //
    }
    //
    }
    
    {
    //
    for (int i = 0; i < kAgain; ++i) {
    //
    prim::Array<int> a;
    
    a.resize (100);
    
    for (int j = 0; j < a.size(); ++j) { a[j] = getRandomInteger (200 + 1) - 100; }
    
    int first  = a.getFirst();
    int last   = a.getLast();
    int random = a.get (getRandomInteger (a.size()));
    
    a.sort();
    
    for (int j = 0; j < a.size() - 1; ++j) { TTT_EXPECT (a[j] <= a[j + 1]); }
    
    TTT_EXPECT (a.contains (first));
    TTT_EXPECT (a.contains (last));
    TTT_EXPECT (a.contains (random));
    //
    }
    //
    }
    
    {
    //
    for (int i = 0; i < kAgain; ++i) {
    //
    prim::Array<int> a;
    
    a.resize (100);
    
    for (int j = 0; j < a.size() - 1; ++j) { 
        do { a[j] = getRandomInteger (200 + 1) - 100; } while (a[j] == 0);
    }
    
    a.getLast() = 0;
    a.sort();
    
    TTT_EXPECT (a.indexOfSorted (0) != -1);
    TTT_EXPECT (a[a.indexOfSorted (0)] == 0);
    //
    }
    //
    }
            
TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if 0
}
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
