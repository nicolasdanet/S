
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < https://github.com/codeplea/tinyexpr/blob/master/test.c > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "t_expr.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (exprBase, "Expr - Base")

    TTT_EXPECT (test_areEquivalent (test_expr ("1"),                             1));
    TTT_EXPECT (test_areEquivalent (test_expr ("1 "),                            1));
    TTT_EXPECT (test_areEquivalent (test_expr ("(1)"),                           1));

    TTT_EXPECT (test_areEquivalent (test_expr ("pi"),                            PD_PI));
    TTT_EXPECT (test_areEquivalent (test_expr ("atan(1)*4 - pi"),                0));
    TTT_EXPECT (test_areEquivalent (test_expr ("e"),                             PD_E));

    TTT_EXPECT (test_areEquivalent (test_expr ("2+1"),                           2 + 1));
    TTT_EXPECT (test_areEquivalent (test_expr ("(((2+(1))))"),                   2 + 1));
    TTT_EXPECT (test_areEquivalent (test_expr ("3+2"),                           3 + 2));

    TTT_EXPECT (test_areEquivalent (test_expr ("3+2+4"),                         3 + 2 + 4));
    TTT_EXPECT (test_areEquivalent (test_expr ("(3+2)+4"),                       3 + 2 + 4));
    TTT_EXPECT (test_areEquivalent (test_expr ("3+(2+4)"),                       3 + 2 + 4));
    TTT_EXPECT (test_areEquivalent (test_expr ("(3+2+4)"),                       3 + 2 + 4));

    TTT_EXPECT (test_areEquivalent (test_expr ("3*2*4"),                         3 * 2 * 4));
    TTT_EXPECT (test_areEquivalent (test_expr ("(3*2)*4"),                       3 * 2 * 4));
    TTT_EXPECT (test_areEquivalent (test_expr ("3*(2*4)"),                       3 * 2 * 4));
    TTT_EXPECT (test_areEquivalent (test_expr ("(3*2*4)"),                       3 * 2 * 4));

    TTT_EXPECT (test_areEquivalent (test_expr ("3-2-4"),                         3 - 2 - 4));
    TTT_EXPECT (test_areEquivalent (test_expr ("(3-2)-4"),                       (3 - 2) - 4));
    TTT_EXPECT (test_areEquivalent (test_expr ("3-(2-4)"),                       3 - (2 - 4)));
    TTT_EXPECT (test_areEquivalent (test_expr ("(3-2-4)"),                       3 - 2 - 4));

    TTT_EXPECT (test_areEquivalent (test_expr ("3/2/4"),                         3.0 / 2.0 / 4.0));
    TTT_EXPECT (test_areEquivalent (test_expr ("(3/2)/4"),                       (3.0 / 2.0) / 4.0));
    TTT_EXPECT (test_areEquivalent (test_expr ("3/(2/4)"),                       3.0 / (2.0 / 4.0)));
    TTT_EXPECT (test_areEquivalent (test_expr ("(3/2/4)"),                       3.0 /2.0 / 4.0));

    TTT_EXPECT (test_areEquivalent (test_expr ("(3*2/4)"),                       3.0 * 2.0 / 4.0));
    TTT_EXPECT (test_areEquivalent (test_expr ("(3/2*4)"),                       3.0 / 2.0 * 4.0));
    TTT_EXPECT (test_areEquivalent (test_expr ("3*(2/4)"),                       3.0 * (2.0 / 4.0)));

    TTT_EXPECT (test_areEquivalent (test_expr ("asin sin .5"),                   0.5));
    TTT_EXPECT (test_areEquivalent (test_expr ("sin asin .5"),                   0.5));
    TTT_EXPECT (test_areEquivalent (test_expr ("ln exp .5"),                     0.5));
    TTT_EXPECT (test_areEquivalent (test_expr ("exp ln .5"),                     0.5));

    TTT_EXPECT (test_areEquivalent (test_expr ("asin sin-.5"),                   -0.5));
    TTT_EXPECT (test_areEquivalent (test_expr ("asin sin-0.5"),                  -0.5));
    TTT_EXPECT (test_areEquivalent (test_expr ("asin sin -0.5"),                 -0.5));
    TTT_EXPECT (test_areEquivalent (test_expr ("asin (sin -0.5)"),               -0.5));
    TTT_EXPECT (test_areEquivalent (test_expr ("asin (sin (-0.5))"),             -0.5));
    TTT_EXPECT (test_areEquivalent (test_expr ("asin sin (-0.5)"),               -0.5));
    TTT_EXPECT (test_areEquivalent (test_expr ("(asin sin (-0.5))"),             -0.5));

    TTT_EXPECT (test_areEquivalent (test_expr ("log10 1000"),                    3));
    TTT_EXPECT (test_areEquivalent (test_expr ("log10 1e3"),                     3));
    TTT_EXPECT (test_areEquivalent (test_expr ("log10 1000"),                    3));
    TTT_EXPECT (test_areEquivalent (test_expr ("log10 1e3"),                     3));
    TTT_EXPECT (test_areEquivalent (test_expr ("log10(1000)"),                   3));
    TTT_EXPECT (test_areEquivalent (test_expr ("log10(1e3)"),                    3));
    TTT_EXPECT (test_areEquivalent (test_expr ("log10 1.0e3"),                   3));
    TTT_EXPECT (test_areEquivalent (test_expr ("10^5*5e-5"),                     5));

    TTT_EXPECT (test_areEquivalent (test_expr ("log 1000"),                      3));

    TTT_EXPECT (test_areEquivalent (test_expr ("ln (e^10)"),                     10));
    TTT_EXPECT (test_areEquivalent (test_expr ("100^.5+1"),                      11));
    TTT_EXPECT (test_areEquivalent (test_expr ("100 ^.5+1"),                     11));
    TTT_EXPECT (test_areEquivalent (test_expr ("100^+.5+1"),                     11));
    TTT_EXPECT (test_areEquivalent (test_expr ("100^--.5+1"),                    11));
    TTT_EXPECT (test_areEquivalent (test_expr ("100^---+-++---++-+-+-.5+1"),     11));

    TTT_EXPECT (test_areEquivalent (test_expr ("100^-.5+1"),                     1.1));
    TTT_EXPECT (test_areEquivalent (test_expr ("100^---.5+1"),                   1.1));
    TTT_EXPECT (test_areEquivalent (test_expr ("100^+---.5+1"),                  1.1));
    TTT_EXPECT (test_areEquivalent (test_expr ("1e2^+---.5e0+1e0"),              1.1));
    TTT_EXPECT (test_areEquivalent (test_expr ("--(1e2^(+(-(-(-.5e0))))+1e0)"),  1.1));

    TTT_EXPECT (test_areEquivalent (test_expr ("sqrt 100 + 7"),                  17));
    TTT_EXPECT (test_areEquivalent (test_expr ("sqrt 100 * 7"),                  70));
    TTT_EXPECT (test_areEquivalent (test_expr ("sqrt (100 * 100)"),              100));

    TTT_EXPECT (test_areEquivalent (test_expr ("1,2"),                           2));
    TTT_EXPECT (test_areEquivalent (test_expr ("1,2+1"),                         3));
    TTT_EXPECT (test_areEquivalent (test_expr ("1+1,2+2,2+1"),                   3));
    TTT_EXPECT (test_areEquivalent (test_expr ("1,2,3"),                         3));
    TTT_EXPECT (test_areEquivalent (test_expr ("(1,2),3"),                       3));
    TTT_EXPECT (test_areEquivalent (test_expr ("1,(2,3)"),                       3));
    TTT_EXPECT (test_areEquivalent (test_expr ("-(1,(2,3))"),                    -3));

    TTT_EXPECT (test_areEquivalent (test_expr ("2^2"),                           4));
    TTT_EXPECT (test_areEquivalent (test_expr ("pow(2,2)"),                      4));

    TTT_EXPECT (test_areEquivalent (test_expr ("atan2(1,1)"),                    0.7853981633974483));
    TTT_EXPECT (test_areEquivalent (test_expr ("atan2(1,2)"),                    0.4636476090008061));
    TTT_EXPECT (test_areEquivalent (test_expr ("atan2(2,1)"),                    1.1071487177940904));
    TTT_EXPECT (test_areEquivalent (test_expr ("atan2(3,4)"),                    0.6435011087932844));
    TTT_EXPECT (test_areEquivalent (test_expr ("atan2(3+3,4*2)"),                0.6435011087932844));
    TTT_EXPECT (test_areEquivalent (test_expr ("atan2(3+3,(4*2))"),              0.6435011087932844));
    TTT_EXPECT (test_areEquivalent (test_expr ("atan2((3+3),4*2)"),              0.6435011087932844));
    TTT_EXPECT (test_areEquivalent (test_expr ("atan2((3+3),(4*2))"),            0.6435011087932844));
    
    TTT_EXPECT (test_areEquivalent (test_expr ("fac(0)"),                        1));
    TTT_EXPECT (test_areEquivalent (test_expr ("fac(0.2)"),                      1));
    TTT_EXPECT (test_areEquivalent (test_expr ("fac(1)"),                        1));
    TTT_EXPECT (test_areEquivalent (test_expr ("fac(2)"),                        2));
    TTT_EXPECT (test_areEquivalent (test_expr ("fac(3)"),                        6));
    TTT_EXPECT (test_areEquivalent (test_expr ("fac(4.8)"),                      24));
    TTT_EXPECT (test_areEquivalent (test_expr ("fac(10)"),                       3628800));

    TTT_EXPECT (test_areEquivalent (test_expr ("ncr(0,0)"),                      1));
    TTT_EXPECT (test_areEquivalent (test_expr ("ncr(10,1)"),                     10));
    TTT_EXPECT (test_areEquivalent (test_expr ("ncr(10,0)"),                     1));
    TTT_EXPECT (test_areEquivalent (test_expr ("ncr(10,10)"),                    1));
    TTT_EXPECT (test_areEquivalent (test_expr ("ncr(16,7)"),                     11440));
    TTT_EXPECT (test_areEquivalent (test_expr ("ncr(16,9)"),                     11440));
    TTT_EXPECT (test_areEquivalent (test_expr ("ncr(100,95)"),                   75287520));

    TTT_EXPECT (test_areEquivalent (test_expr ("npr(0,0)"),                      1));
    TTT_EXPECT (test_areEquivalent (test_expr ("npr(10,1)"),                     10));
    TTT_EXPECT (test_areEquivalent (test_expr ("npr(10,0)"),                     1));
    TTT_EXPECT (test_areEquivalent (test_expr ("npr(10,10)"),                    3628800));
    TTT_EXPECT (test_areEquivalent (test_expr ("npr(20,5)"),                     1860480));
    TTT_EXPECT (test_areEquivalent (test_expr ("npr(100,4)"),                    94109400));
    
    /* Left to right power. */
    
    TTT_EXPECT (test_areEquivalent (test_expr ("2^3^4"),            test_expr ("(2^3)^4")));
    TTT_EXPECT (test_areEquivalent (test_expr ("-2^2"),             test_expr ("(-2)^2")));
    TTT_EXPECT (test_areEquivalent (test_expr ("--2^2"),            test_expr ("2^2")));
    TTT_EXPECT (test_areEquivalent (test_expr ("---2^2"),           test_expr ("(-2)^2")));
    TTT_EXPECT (test_areEquivalent (test_expr ("-2^2"),             test_expr ("4")));
    TTT_EXPECT (test_areEquivalent (test_expr ("2^1.1^1.2^1.3"),    test_expr ("((2^1.1)^1.2)^1.3")));
    
    /* Syntax error. */
    
    TTT_EXPECT (test_expr ("")          == 0.0);
    TTT_EXPECT (test_expr ("1+")        == 0.0);
    TTT_EXPECT (test_expr ("1)")        == 0.0);
    TTT_EXPECT (test_expr ("(1")        == 0.0);
    TTT_EXPECT (test_expr ("1**1")      == 0.0);
    TTT_EXPECT (test_expr ("1*2(+4")    == 0.0);
    TTT_EXPECT (test_expr ("1*2(1+4")   == 0.0);
    TTT_EXPECT (test_expr ("a+5")       == 0.0);
    TTT_EXPECT (test_expr ("A+5")       == 0.0);
    TTT_EXPECT (test_expr ("Aa+5")      == 0.0);
    TTT_EXPECT (test_expr ("1^^5")      == 0.0);
    TTT_EXPECT (test_expr ("1**5")      == 0.0);
    TTT_EXPECT (test_expr ("sin(cos5")  == 0.0);
    
    /* Infinity. */
    
    TTT_EXPECT (PD_FLOAT64_IS_INVALID_OR_ZERO (test_expr ("1/0")));
    TTT_EXPECT (PD_FLOAT64_IS_INVALID_OR_ZERO (test_expr ("log(0)")));
    TTT_EXPECT (PD_FLOAT64_IS_INVALID_OR_ZERO (test_expr ("pow(2,10000000)")));
    TTT_EXPECT (PD_FLOAT64_IS_INVALID_OR_ZERO (test_expr ("fac(300)")));
    TTT_EXPECT (PD_FLOAT64_IS_INVALID_OR_ZERO (test_expr ("ncr(300,100)")));
    TTT_EXPECT (PD_FLOAT64_IS_INVALID_OR_ZERO (test_expr ("ncr(300000,100)")));
    TTT_EXPECT (PD_FLOAT64_IS_INVALID_OR_ZERO (test_expr ("ncr(300000,100)*8")));
    TTT_EXPECT (PD_FLOAT64_IS_INVALID_OR_ZERO (test_expr ("npr(3,2)*ncr(300000,100)")));
    TTT_EXPECT (PD_FLOAT64_IS_INVALID_OR_ZERO (test_expr ("npr(100,90)")));
    TTT_EXPECT (PD_FLOAT64_IS_INVALID_OR_ZERO (test_expr ("npr(30,25)")));
    
    /* Nan. */
    
    TTT_EXPECT (PD_FLOAT64_IS_INVALID_OR_ZERO (test_expr ("0/0")));
    TTT_EXPECT (PD_FLOAT64_IS_INVALID_OR_ZERO (test_expr ("1%0")));
    TTT_EXPECT (PD_FLOAT64_IS_INVALID_OR_ZERO (test_expr ("1%(1%0)")));
    TTT_EXPECT (PD_FLOAT64_IS_INVALID_OR_ZERO (test_expr ("(1%0)%1")));
    TTT_EXPECT (PD_FLOAT64_IS_INVALID_OR_ZERO (test_expr ("fac(-1)")));
    TTT_EXPECT (PD_FLOAT64_IS_INVALID_OR_ZERO (test_expr ("ncr(2, 4)")));
    TTT_EXPECT (PD_FLOAT64_IS_INVALID_OR_ZERO (test_expr ("ncr(-2, 4)")));
    TTT_EXPECT (PD_FLOAT64_IS_INVALID_OR_ZERO (test_expr ("ncr(2, -4)")));
    TTT_EXPECT (PD_FLOAT64_IS_INVALID_OR_ZERO (test_expr ("npr(2, 4)")));
    TTT_EXPECT (PD_FLOAT64_IS_INVALID_OR_ZERO (test_expr ("npr(-2, 4)")));
    TTT_EXPECT (PD_FLOAT64_IS_INVALID_OR_ZERO (test_expr ("npr(2, -4)")));
    
    /* Classify. */
    
    TTT_EXPECT (test_areEquivalent (test_expr ("isinf(1/0)"),                        1));
    TTT_EXPECT (test_areEquivalent (test_expr ("isinf(log(0))"),                     1));
    TTT_EXPECT (test_areEquivalent (test_expr ("isinf(pow(2,10000000))"),            1));
    TTT_EXPECT (test_areEquivalent (test_expr ("isinf(fac(300))"),                   1));
    TTT_EXPECT (test_areEquivalent (test_expr ("isinf(ncr(300,100))"),               1));
    TTT_EXPECT (test_areEquivalent (test_expr ("isinf(ncr(300000,100))"),            1));
    TTT_EXPECT (test_areEquivalent (test_expr ("isinf(ncr(300000,100)*8)"),          1));
    TTT_EXPECT (test_areEquivalent (test_expr ("isinf(npr(3,2)*ncr(300000,100))"),   1));
    TTT_EXPECT (test_areEquivalent (test_expr ("isinf(npr(100,90))"),                1));
    TTT_EXPECT (test_areEquivalent (test_expr ("isinf(npr(30,25))"),                 1));
    
    TTT_EXPECT (test_areEquivalent (test_expr ("isinf(0)"),                          0));
    TTT_EXPECT (test_areEquivalent (test_expr ("isinf(3.14)"),                       0));
    
    TTT_EXPECT (test_areEquivalent (test_expr ("isnan(0/0)"),                        1));
    TTT_EXPECT (test_areEquivalent (test_expr ("isnan(1%0)"),                        1));
    TTT_EXPECT (test_areEquivalent (test_expr ("isnan(1%(1%0))"),                    1));
    TTT_EXPECT (test_areEquivalent (test_expr ("isnan((1%0)%1)"),                    1));
    TTT_EXPECT (test_areEquivalent (test_expr ("isnan(fac(-1))"),                    1));
    TTT_EXPECT (test_areEquivalent (test_expr ("isnan(ncr(2, 4))"),                  1));
    TTT_EXPECT (test_areEquivalent (test_expr ("isnan(ncr(-2, 4))"),                 1));
    TTT_EXPECT (test_areEquivalent (test_expr ("isnan(ncr(2, -4))"),                 1));
    TTT_EXPECT (test_areEquivalent (test_expr ("isnan(npr(2, 4))"),                  1));
    TTT_EXPECT (test_areEquivalent (test_expr ("isnan(npr(-2, 4))"),                 1));
    TTT_EXPECT (test_areEquivalent (test_expr ("isnan(npr(2, -4))"),                 1));
    
    TTT_EXPECT (test_areEquivalent (test_expr ("isnan(0)"),                          0));
    TTT_EXPECT (test_areEquivalent (test_expr ("isnan(3.14)"),                       0));
    
    /* FTZ. */
    
    TTT_EXPECT (test_areEquivalent (test_expr ("ftz(1/0)"),                          0));
    TTT_EXPECT (test_areEquivalent (test_expr ("ftz(log(0))"),                       0));
    TTT_EXPECT (test_areEquivalent (test_expr ("ftz(pow(2,10000000))"),              0));
    TTT_EXPECT (test_areEquivalent (test_expr ("ftz(fac(300))"),                     0));

    TTT_EXPECT (test_areEquivalent (test_expr ("ftz(0/0)"),                          0));
    TTT_EXPECT (test_areEquivalent (test_expr ("ftz(1%0)"),                          0));
    TTT_EXPECT (test_areEquivalent (test_expr ("ftz(1%(1%0))"),                      0));
    TTT_EXPECT (test_areEquivalent (test_expr ("ftz((1%0)%1)"),                      0));
    TTT_EXPECT (test_areEquivalent (test_expr ("ftz(fac(-1))"),                      0));
    
    TTT_EXPECT (test_areEquivalent (test_expr ("ftz(0)"),                            0));
    TTT_EXPECT (test_areEquivalent (test_expr ("ftz(3.14)"),                         3.14));
    
TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
