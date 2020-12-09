
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < https://github.com/codeplea/tinyexpr/blob/master/test.c > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

// ====================================

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if 0
void test80__base() {
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (exprBase, 80, "Expr - Base")

    TTT_EXPECT (math_areEquivalent (te_interpret ("1"),                             1));
    TTT_EXPECT (math_areEquivalent (te_interpret ("1 "),                            1));
    TTT_EXPECT (math_areEquivalent (te_interpret ("(1)"),                           1));

    TTT_EXPECT (math_areEquivalent (te_interpret ("pi"),                            PD_PI));
    TTT_EXPECT (math_areEquivalent (te_interpret ("atan(1)*4 - pi"),                0));
    TTT_EXPECT (math_areEquivalent (te_interpret ("e"),                             PD_E));

    TTT_EXPECT (math_areEquivalent (te_interpret ("2+1"),                           2 + 1));
    TTT_EXPECT (math_areEquivalent (te_interpret ("(((2+(1))))"),                   2 + 1));
    TTT_EXPECT (math_areEquivalent (te_interpret ("3+2"),                           3 + 2));

    TTT_EXPECT (math_areEquivalent (te_interpret ("3+2+4"),                         3 + 2 + 4));
    TTT_EXPECT (math_areEquivalent (te_interpret ("(3+2)+4"),                       3 + 2 + 4));
    TTT_EXPECT (math_areEquivalent (te_interpret ("3+(2+4)"),                       3 + 2 + 4));
    TTT_EXPECT (math_areEquivalent (te_interpret ("(3+2+4)"),                       3 + 2 + 4));

    TTT_EXPECT (math_areEquivalent (te_interpret ("3*2*4"),                         3 * 2 * 4));
    TTT_EXPECT (math_areEquivalent (te_interpret ("(3*2)*4"),                       3 * 2 * 4));
    TTT_EXPECT (math_areEquivalent (te_interpret ("3*(2*4)"),                       3 * 2 * 4));
    TTT_EXPECT (math_areEquivalent (te_interpret ("(3*2*4)"),                       3 * 2 * 4));

    TTT_EXPECT (math_areEquivalent (te_interpret ("3-2-4"),                         3 - 2 - 4));
    TTT_EXPECT (math_areEquivalent (te_interpret ("(3-2)-4"),                       (3 - 2) - 4));
    TTT_EXPECT (math_areEquivalent (te_interpret ("3-(2-4)"),                       3 - (2 - 4)));
    TTT_EXPECT (math_areEquivalent (te_interpret ("(3-2-4)"),                       3 - 2 - 4));

    TTT_EXPECT (math_areEquivalent (te_interpret ("3/2/4"),                         3.0 / 2.0 / 4.0));
    TTT_EXPECT (math_areEquivalent (te_interpret ("(3/2)/4"),                       (3.0 / 2.0) / 4.0));
    TTT_EXPECT (math_areEquivalent (te_interpret ("3/(2/4)"),                       3.0 / (2.0 / 4.0)));
    TTT_EXPECT (math_areEquivalent (te_interpret ("(3/2/4)"),                       3.0 /2.0 / 4.0));

    TTT_EXPECT (math_areEquivalent (te_interpret ("(3*2/4)"),                       3.0 * 2.0 / 4.0));
    TTT_EXPECT (math_areEquivalent (te_interpret ("(3/2*4)"),                       3.0 / 2.0 * 4.0));
    TTT_EXPECT (math_areEquivalent (te_interpret ("3*(2/4)"),                       3.0 * (2.0 / 4.0)));

    TTT_EXPECT (math_areEquivalent (te_interpret ("asin sin .5"),                   0.5));
    TTT_EXPECT (math_areEquivalent (te_interpret ("sin asin .5"),                   0.5));
    TTT_EXPECT (math_areEquivalent (te_interpret ("ln exp .5"),                     0.5));
    TTT_EXPECT (math_areEquivalent (te_interpret ("exp ln .5"),                     0.5));

    TTT_EXPECT (math_areEquivalent (te_interpret ("asin sin-.5"),                   -0.5));
    TTT_EXPECT (math_areEquivalent (te_interpret ("asin sin-0.5"),                  -0.5));
    TTT_EXPECT (math_areEquivalent (te_interpret ("asin sin -0.5"),                 -0.5));
    TTT_EXPECT (math_areEquivalent (te_interpret ("asin (sin -0.5)"),               -0.5));
    TTT_EXPECT (math_areEquivalent (te_interpret ("asin (sin (-0.5))"),             -0.5));
    TTT_EXPECT (math_areEquivalent (te_interpret ("asin sin (-0.5)"),               -0.5));
    TTT_EXPECT (math_areEquivalent (te_interpret ("(asin sin (-0.5))"),             -0.5));

    TTT_EXPECT (math_areEquivalent (te_interpret ("log10 1000"),                    3));
    TTT_EXPECT (math_areEquivalent (te_interpret ("log10 1e3"),                     3));
    TTT_EXPECT (math_areEquivalent (te_interpret ("log10 1000"),                    3));
    TTT_EXPECT (math_areEquivalent (te_interpret ("log10 1e3"),                     3));
    TTT_EXPECT (math_areEquivalent (te_interpret ("log10(1000)"),                   3));
    TTT_EXPECT (math_areEquivalent (te_interpret ("log10(1e3)"),                    3));
    TTT_EXPECT (math_areEquivalent (te_interpret ("log10 1.0e3"),                   3));
    TTT_EXPECT (math_areEquivalent (te_interpret ("10^5*5e-5"),                     5));

    TTT_EXPECT (math_areEquivalent (te_interpret ("log 1000"),                      3));

    TTT_EXPECT (math_areEquivalent (te_interpret ("ln (e^10)"),                     10));
    TTT_EXPECT (math_areEquivalent (te_interpret ("100^.5+1"),                      11));
    TTT_EXPECT (math_areEquivalent (te_interpret ("100 ^.5+1"),                     11));
    TTT_EXPECT (math_areEquivalent (te_interpret ("100^+.5+1"),                     11));
    TTT_EXPECT (math_areEquivalent (te_interpret ("100^--.5+1"),                    11));
    TTT_EXPECT (math_areEquivalent (te_interpret ("100^---+-++---++-+-+-.5+1"),     11));

    TTT_EXPECT (math_areEquivalent (te_interpret ("100^-.5+1"),                     1.1));
    TTT_EXPECT (math_areEquivalent (te_interpret ("100^---.5+1"),                   1.1));
    TTT_EXPECT (math_areEquivalent (te_interpret ("100^+---.5+1"),                  1.1));
    TTT_EXPECT (math_areEquivalent (te_interpret ("1e2^+---.5e0+1e0"),              1.1));
    TTT_EXPECT (math_areEquivalent (te_interpret ("--(1e2^(+(-(-(-.5e0))))+1e0)"),  1.1));

    TTT_EXPECT (math_areEquivalent (te_interpret ("sqrt 100 + 7"),                  17));
    TTT_EXPECT (math_areEquivalent (te_interpret ("sqrt 100 * 7"),                  70));
    TTT_EXPECT (math_areEquivalent (te_interpret ("sqrt (100 * 100)"),              100));

    TTT_EXPECT (math_areEquivalent (te_interpret ("1,2"),                           2));
    TTT_EXPECT (math_areEquivalent (te_interpret ("1,2+1"),                         3));
    TTT_EXPECT (math_areEquivalent (te_interpret ("1+1,2+2,2+1"),                   3));
    TTT_EXPECT (math_areEquivalent (te_interpret ("1,2,3"),                         3));
    TTT_EXPECT (math_areEquivalent (te_interpret ("(1,2),3"),                       3));
    TTT_EXPECT (math_areEquivalent (te_interpret ("1,(2,3)"),                       3));
    TTT_EXPECT (math_areEquivalent (te_interpret ("-(1,(2,3))"),                    -3));

    TTT_EXPECT (math_areEquivalent (te_interpret ("2^2"),                           4));
    TTT_EXPECT (math_areEquivalent (te_interpret ("pow(2,2)"),                      4));

    TTT_EXPECT (math_areEquivalent (te_interpret ("atan2(1,1)"),                    0.7853981633974483));
    TTT_EXPECT (math_areEquivalent (te_interpret ("atan2(1,2)"),                    0.4636476090008061));
    TTT_EXPECT (math_areEquivalent (te_interpret ("atan2(2,1)"),                    1.1071487177940904));
    TTT_EXPECT (math_areEquivalent (te_interpret ("atan2(3,4)"),                    0.6435011087932844));
    TTT_EXPECT (math_areEquivalent (te_interpret ("atan2(3+3,4*2)"),                0.6435011087932844));
    TTT_EXPECT (math_areEquivalent (te_interpret ("atan2(3+3,(4*2))"),              0.6435011087932844));
    TTT_EXPECT (math_areEquivalent (te_interpret ("atan2((3+3),4*2)"),              0.6435011087932844));
    TTT_EXPECT (math_areEquivalent (te_interpret ("atan2((3+3),(4*2))"),            0.6435011087932844));
    
    TTT_EXPECT (math_areEquivalent (te_interpret ("fac(0)"),                        1));
    TTT_EXPECT (math_areEquivalent (te_interpret ("fac(0.2)"),                      1));
    TTT_EXPECT (math_areEquivalent (te_interpret ("fac(1)"),                        1));
    TTT_EXPECT (math_areEquivalent (te_interpret ("fac(2)"),                        2));
    TTT_EXPECT (math_areEquivalent (te_interpret ("fac(3)"),                        6));
    TTT_EXPECT (math_areEquivalent (te_interpret ("fac(4.8)"),                      24));
    TTT_EXPECT (math_areEquivalent (te_interpret ("fac(10)"),                       3628800));

    TTT_EXPECT (math_areEquivalent (te_interpret ("ncr(0,0)"),                      1));
    TTT_EXPECT (math_areEquivalent (te_interpret ("ncr(10,1)"),                     10));
    TTT_EXPECT (math_areEquivalent (te_interpret ("ncr(10,0)"),                     1));
    TTT_EXPECT (math_areEquivalent (te_interpret ("ncr(10,10)"),                    1));
    TTT_EXPECT (math_areEquivalent (te_interpret ("ncr(16,7)"),                     11440));
    TTT_EXPECT (math_areEquivalent (te_interpret ("ncr(16,9)"),                     11440));
    TTT_EXPECT (math_areEquivalent (te_interpret ("ncr(100,95)"),                   75287520));

    TTT_EXPECT (math_areEquivalent (te_interpret ("npr(0,0)"),                      1));
    TTT_EXPECT (math_areEquivalent (te_interpret ("npr(10,1)"),                     10));
    TTT_EXPECT (math_areEquivalent (te_interpret ("npr(10,0)"),                     1));
    TTT_EXPECT (math_areEquivalent (te_interpret ("npr(10,10)"),                    3628800));
    TTT_EXPECT (math_areEquivalent (te_interpret ("npr(20,5)"),                     1860480));
    TTT_EXPECT (math_areEquivalent (te_interpret ("npr(100,4)"),                    94109400));
    
    /* Left to right power. */
    
    TTT_EXPECT (math_areEquivalent (te_interpret ("2^3^4"),         te_interpret ("(2^3)^4")));
    TTT_EXPECT (math_areEquivalent (te_interpret ("-2^2"),          te_interpret ("(-2)^2")));
    TTT_EXPECT (math_areEquivalent (te_interpret ("--2^2"),         te_interpret ("2^2")));
    TTT_EXPECT (math_areEquivalent (te_interpret ("---2^2"),        te_interpret ("(-2)^2")));
    TTT_EXPECT (math_areEquivalent (te_interpret ("-2^2"),          te_interpret ("4")));
    TTT_EXPECT (math_areEquivalent (te_interpret ("2^1.1^1.2^1.3"), te_interpret ("((2^1.1)^1.2)^1.3")));
    
    /* Syntax error. */
    
    TTT_EXPECT (te_interpret ("")                           == 0.0);
    TTT_EXPECT (te_interpret ("1+")                         == 0.0);
    TTT_EXPECT (te_interpret ("1)")                         == 0.0);
    TTT_EXPECT (te_interpret ("(1")                         == 0.0);
    TTT_EXPECT (te_interpret ("1**1")                       == 0.0);
    TTT_EXPECT (te_interpret ("1*2(+4")                     == 0.0);
    TTT_EXPECT (te_interpret ("1*2(1+4")                    == 0.0);
    TTT_EXPECT (te_interpret ("a+5")                        == 0.0);
    TTT_EXPECT (te_interpret ("A+5")                        == 0.0);
    TTT_EXPECT (te_interpret ("Aa+5")                       == 0.0);
    TTT_EXPECT (te_interpret ("1^^5")                       == 0.0);
    TTT_EXPECT (te_interpret ("1**5")                       == 0.0);
    TTT_EXPECT (te_interpret ("sin(cos5")                   == 0.0);
    
    /* Infinity. */
    
    TTT_EXPECT (PD_FLOAT64_IS_INVALID_OR_ZERO (te_interpret ("1/0")));
    TTT_EXPECT (PD_FLOAT64_IS_INVALID_OR_ZERO (te_interpret ("log(0)")));
    TTT_EXPECT (PD_FLOAT64_IS_INVALID_OR_ZERO (te_interpret ("pow(2,10000000)")));
    TTT_EXPECT (PD_FLOAT64_IS_INVALID_OR_ZERO (te_interpret ("fac(300)")));
    TTT_EXPECT (PD_FLOAT64_IS_INVALID_OR_ZERO (te_interpret ("ncr(300,100)")));
    TTT_EXPECT (PD_FLOAT64_IS_INVALID_OR_ZERO (te_interpret ("ncr(300000,100)")));
    TTT_EXPECT (PD_FLOAT64_IS_INVALID_OR_ZERO (te_interpret ("ncr(300000,100)*8")));
    TTT_EXPECT (PD_FLOAT64_IS_INVALID_OR_ZERO (te_interpret ("npr(3,2)*ncr(300000,100)")));
    TTT_EXPECT (PD_FLOAT64_IS_INVALID_OR_ZERO (te_interpret ("npr(100,90)")));
    TTT_EXPECT (PD_FLOAT64_IS_INVALID_OR_ZERO (te_interpret ("npr(30,25)")));
    
    /* Nan. */
    
    TTT_EXPECT (PD_FLOAT64_IS_INVALID_OR_ZERO (te_interpret ("0/0")));
    TTT_EXPECT (PD_FLOAT64_IS_INVALID_OR_ZERO (te_interpret ("1%0")));
    TTT_EXPECT (PD_FLOAT64_IS_INVALID_OR_ZERO (te_interpret ("1%(1%0)")));
    TTT_EXPECT (PD_FLOAT64_IS_INVALID_OR_ZERO (te_interpret ("(1%0)%1")));
    TTT_EXPECT (PD_FLOAT64_IS_INVALID_OR_ZERO (te_interpret ("fac(-1)")));
    TTT_EXPECT (PD_FLOAT64_IS_INVALID_OR_ZERO (te_interpret ("ncr(2, 4)")));
    TTT_EXPECT (PD_FLOAT64_IS_INVALID_OR_ZERO (te_interpret ("ncr(-2, 4)")));
    TTT_EXPECT (PD_FLOAT64_IS_INVALID_OR_ZERO (te_interpret ("ncr(2, -4)")));
    TTT_EXPECT (PD_FLOAT64_IS_INVALID_OR_ZERO (te_interpret ("npr(2, 4)")));
    TTT_EXPECT (PD_FLOAT64_IS_INVALID_OR_ZERO (te_interpret ("npr(-2, 4)")));
    TTT_EXPECT (PD_FLOAT64_IS_INVALID_OR_ZERO (te_interpret ("npr(2, -4)")));
    
    /* Classify. */
    
    TTT_EXPECT (math_areEquivalent (te_interpret ("isinf(1/0)"),                        1));
    TTT_EXPECT (math_areEquivalent (te_interpret ("isinf(log(0))"),                     1));
    TTT_EXPECT (math_areEquivalent (te_interpret ("isinf(pow(2,10000000))"),            1));
    TTT_EXPECT (math_areEquivalent (te_interpret ("isinf(fac(300))"),                   1));
    TTT_EXPECT (math_areEquivalent (te_interpret ("isinf(ncr(300,100))"),               1));
    TTT_EXPECT (math_areEquivalent (te_interpret ("isinf(ncr(300000,100))"),            1));
    TTT_EXPECT (math_areEquivalent (te_interpret ("isinf(ncr(300000,100)*8)"),          1));
    TTT_EXPECT (math_areEquivalent (te_interpret ("isinf(npr(3,2)*ncr(300000,100))"),   1));
    TTT_EXPECT (math_areEquivalent (te_interpret ("isinf(npr(100,90))"),                1));
    TTT_EXPECT (math_areEquivalent (te_interpret ("isinf(npr(30,25))"),                 1));
    
    TTT_EXPECT (math_areEquivalent (te_interpret ("isinf(0)"),                          0));
    TTT_EXPECT (math_areEquivalent (te_interpret ("isinf(3.14)"),                       0));
    
    TTT_EXPECT (math_areEquivalent (te_interpret ("isnan(0/0)"),                        1));
    TTT_EXPECT (math_areEquivalent (te_interpret ("isnan(1%0)"),                        1));
    TTT_EXPECT (math_areEquivalent (te_interpret ("isnan(1%(1%0))"),                    1));
    TTT_EXPECT (math_areEquivalent (te_interpret ("isnan((1%0)%1)"),                    1));
    TTT_EXPECT (math_areEquivalent (te_interpret ("isnan(fac(-1))"),                    1));
    TTT_EXPECT (math_areEquivalent (te_interpret ("isnan(ncr(2, 4))"),                  1));
    TTT_EXPECT (math_areEquivalent (te_interpret ("isnan(ncr(-2, 4))"),                 1));
    TTT_EXPECT (math_areEquivalent (te_interpret ("isnan(ncr(2, -4))"),                 1));
    TTT_EXPECT (math_areEquivalent (te_interpret ("isnan(npr(2, 4))"),                  1));
    TTT_EXPECT (math_areEquivalent (te_interpret ("isnan(npr(-2, 4))"),                 1));
    TTT_EXPECT (math_areEquivalent (te_interpret ("isnan(npr(2, -4))"),                 1));
    
    TTT_EXPECT (math_areEquivalent (te_interpret ("isnan(0)"),                          0));
    TTT_EXPECT (math_areEquivalent (te_interpret ("isnan(3.14)"),                       0));
    
    /* FTZ. */
    
    TTT_EXPECT (math_areEquivalent (te_interpret ("ftz(1/0)"),                          0));
    TTT_EXPECT (math_areEquivalent (te_interpret ("ftz(log(0))"),                       0));
    TTT_EXPECT (math_areEquivalent (te_interpret ("ftz(pow(2,10000000))"),              0));
    TTT_EXPECT (math_areEquivalent (te_interpret ("ftz(fac(300))"),                     0));

    TTT_EXPECT (math_areEquivalent (te_interpret ("ftz(0/0)"),                          0));
    TTT_EXPECT (math_areEquivalent (te_interpret ("ftz(1%0)"),                          0));
    TTT_EXPECT (math_areEquivalent (te_interpret ("ftz(1%(1%0))"),                      0));
    TTT_EXPECT (math_areEquivalent (te_interpret ("ftz((1%0)%1)"),                      0));
    TTT_EXPECT (math_areEquivalent (te_interpret ("ftz(fac(-1))"),                      0));
    
    TTT_EXPECT (math_areEquivalent (te_interpret ("ftz(0)"),                            0));
    TTT_EXPECT (math_areEquivalent (te_interpret ("ftz(3.14)"),                         3.14));
    
TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if 0
}
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define expr_check(a, b)    do {   \
                                if ((b) != (b)) { break; }  \
                                err |= (math_areEquivalent (te_interpret ((a), lookup, 2), (b)) != 1);  \
                            } while (0)

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if 0
void test81__variables() {
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (exprVariables, 81, "Expr - Variables")

    double x, y, test; te_variable lookup[] = { { "x", &x }, { "y", &y }, { "te_st", &test } };
    
    for (y = 0; y < 5; y++) {
    for (x = 0; x < 5; x++) {
    //
    test = x;
    
    TTT_EXPECT (math_areEquivalent (te_interpret ("cos x + sin y", lookup, 2),  cos (x) + sin (y)));
    TTT_EXPECT (math_areEquivalent (te_interpret ("x+x+x-y", lookup, 2),        x + x + x - y));
    TTT_EXPECT (math_areEquivalent (te_interpret ("x*y^3", lookup, 2),          x * y * y * y));
    TTT_EXPECT (math_areEquivalent (te_interpret ("te_st+5", lookup, 3),        x + 5));
    //
    }
    }
    
    TTT_EXPECT (te_interpret ("xx*y^3", lookup, 2)  == 0.0);
    TTT_EXPECT (te_interpret ("tes", lookup, 3)     == 0.0);
    TTT_EXPECT (te_interpret ("sinn x", lookup, 2)  == 0.0);
    TTT_EXPECT (te_interpret ("si x", lookup, 2)    == 0.0);
    
    t_error err = PD_ERROR_NONE;
    
    for (x = -5; x < 5; x += .2) {
    //
    expr_check ("abs x",       fabs (x));
    expr_check ("acos x",      acos (x));
    expr_check ("asin x",      asin (x));
    expr_check ("atan x",      atan (x));
    expr_check ("ceil x",      ceil (x));
    expr_check ("cos x",       cos (x));
    expr_check ("cosh x",      cosh (x));
    expr_check ("exp x",       exp (x));
    expr_check ("floor x",     floor (x));
    expr_check ("ln x",        log (x));
    expr_check ("log10 x",     log10 (x));
    expr_check ("sin x",       sin (x));
    expr_check ("sinh x",      sinh (x));
    expr_check ("sqrt x",      sqrt (x));
    expr_check ("tan x",       tan (x));
    expr_check ("tanh x",      tanh (x));

    for (y = -2; y < 2; y += .2) {
    //
    expr_check ("min(x,y)",    PD_MIN (x, y));
    expr_check ("max(x,y)",    PD_MAX (x, y));
    expr_check ("eq(x,y)",     x == y);
    expr_check ("ne(x,y)",     x != y);
    expr_check ("lt(x,y)",     x < y);
    expr_check ("le(x,y)",     x <= y);
    expr_check ("gt(x,y)",     x > y);
    expr_check ("ge(x,y)",     x >= y);
    if (fabs (x) < 0.01) { break; }
    expr_check ("atan2(x,y)",  atan2 (x, y));
    expr_check ("pow(x,y)",    pow (x, y));
    //
    }
    //
    }
    
    TTT_EXPECT (err == PD_ERROR_NONE);
    
TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if 0
}
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static double expr_sum0 (void)
{
    return 6;
}

static double expr_sum1 (double a)
{
    return a * 2;
}

static double expr_sum2 (double a, double b)
{
    return a + b;
}

static double expr_sum3 (double a, double b, double c)
{
    return a + b + c;
}

static double expr_sum4 (double a, double b, double c, double d)
{
    return a + b + c + d;
}

static double expr_sum5 (double a, double b, double c, double d, double e)
{
    return a + b + c + d + e;
}

static double expr_sum6 (double a, double b, double c, double d, double e, double f)
{
    return a + b + c + d + e + f;
}

static double expr_sum7 (double a, double b, double c, double d, double e, double f, double g)
{
    return a + b + c + d + e + f + g;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if 0
void test82__functions() {
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (exprFunctions, 82, "Expr - Functions")

    double x = 2;
    double f = 5;
    
    te_variable lookup[] =
        {
            { "x", &x},
            { "f", &f},
            { "sum0", (const void *)expr_sum0, TE_FUNCTION0 },
            { "sum1", (const void *)expr_sum1, TE_FUNCTION1 },
            { "sum2", (const void *)expr_sum2, TE_FUNCTION2 },
            { "sum3", (const void *)expr_sum3, TE_FUNCTION3 },
            { "sum4", (const void *)expr_sum4, TE_FUNCTION4 },
            { "sum5", (const void *)expr_sum5, TE_FUNCTION5 },
            { "sum6", (const void *)expr_sum6, TE_FUNCTION6 },
            { "sum7", (const void *)expr_sum7, TE_FUNCTION7 },
        };
    
    TTT_EXPECT (math_areEquivalent (te_interpret ("x",                      lookup, 10), 2));
    TTT_EXPECT (math_areEquivalent (te_interpret ("f+x",                    lookup, 10), 7));
    TTT_EXPECT (math_areEquivalent (te_interpret ("x+x",                    lookup, 10), 4));
    TTT_EXPECT (math_areEquivalent (te_interpret ("x+f",                    lookup, 10), 7));
    TTT_EXPECT (math_areEquivalent (te_interpret ("f+f",                    lookup, 10), 10));
    TTT_EXPECT (math_areEquivalent (te_interpret ("f+sum0",                 lookup, 10), 11));
    TTT_EXPECT (math_areEquivalent (te_interpret ("sum0+sum0",              lookup, 10), 12));
    TTT_EXPECT (math_areEquivalent (te_interpret ("sum0()+sum0",            lookup, 10), 12));
    TTT_EXPECT (math_areEquivalent (te_interpret ("sum0+sum0()",            lookup, 10), 12));
    TTT_EXPECT (math_areEquivalent (te_interpret ("sum0()+(0)+sum0()",      lookup, 10), 12));
    TTT_EXPECT (math_areEquivalent (te_interpret ("sum1 sum0",              lookup, 10), 12));
    TTT_EXPECT (math_areEquivalent (te_interpret ("sum1(sum0)",             lookup, 10), 12));
    TTT_EXPECT (math_areEquivalent (te_interpret ("sum1 f",                 lookup, 10), 10));
    TTT_EXPECT (math_areEquivalent (te_interpret ("sum1 x",                 lookup, 10), 4));
    TTT_EXPECT (math_areEquivalent (te_interpret ("sum2 (sum0, x)",         lookup, 10), 8));
    TTT_EXPECT (math_areEquivalent (te_interpret ("sum3 (sum0, x, 2)",      lookup, 10), 10));
    TTT_EXPECT (math_areEquivalent (te_interpret ("sum2(2,3)",              lookup, 10), 5));
    TTT_EXPECT (math_areEquivalent (te_interpret ("sum3(2,3,4)",            lookup, 10), 9));
    TTT_EXPECT (math_areEquivalent (te_interpret ("sum4(2,3,4,5)",          lookup, 10), 14));
    TTT_EXPECT (math_areEquivalent (te_interpret ("sum5(2,3,4,5,6)",        lookup, 10), 20));
    TTT_EXPECT (math_areEquivalent (te_interpret ("sum6(2,3,4,5,6,7)",      lookup, 10), 27));
    TTT_EXPECT (math_areEquivalent (te_interpret ("sum7(2,3,4,5,6,7,8)",    lookup, 10), 35));
    
TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if 0
}
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if 0
void test83__optimize() {
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (exprOptimize, 83, "Expr - Optimize")

    TTT_EXPECT (math_areEquivalent (te_raw ("5+5"),         10));
    TTT_EXPECT (math_areEquivalent (te_raw ("pow(2,2)"),    4));
    TTT_EXPECT (math_areEquivalent (te_raw ("sqrt 100"),    10));
    TTT_EXPECT (math_areEquivalent (te_raw ("pi * 2"),      PD_TWO_PI));
    
TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if 0
}
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
