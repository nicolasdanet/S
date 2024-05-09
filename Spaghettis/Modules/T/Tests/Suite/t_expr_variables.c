
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < https://github.com/codeplea/tinyexpr/blob/master/test.c > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#define expr_check(a, b)    do {   \
                                if ((b) != (b)) { break; }  \
                                err |= (test_areEquivalent (test_args ((a), lookup, 2), (b)) != 1);  \
                            } while (0)

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (exprVariables, "Expr - Variables")

    double x, y, test; te_variable lookup[] = { { "x", &x }, { "y", &y }, { "te_st", &test } };
    
    for (y = 0; y < 5; y++) {
    for (x = 0; x < 5; x++) {
    //
    test = x;
    
    TTT_EXPECT (test_areEquivalent (test_args ("cos x + sin y", lookup, 2),  cos (x) + sin (y)));
    TTT_EXPECT (test_areEquivalent (test_args ("x+x+x-y", lookup, 2),        x + x + x - y));
    TTT_EXPECT (test_areEquivalent (test_args ("x*y^3", lookup, 2),          x * y * y * y));
    TTT_EXPECT (test_areEquivalent (test_args ("te_st+5", lookup, 3),        x + 5));
    //
    }
    }
    
    TTT_EXPECT (test_args ("xx*y^3", lookup, 2)  == 0.0);
    TTT_EXPECT (test_args ("tes", lookup, 3)     == 0.0);
    TTT_EXPECT (test_args ("sinn x", lookup, 2)  == 0.0);
    TTT_EXPECT (test_args ("si x", lookup, 2)    == 0.0);
    
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
