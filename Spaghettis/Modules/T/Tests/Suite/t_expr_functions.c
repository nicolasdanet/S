
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < https://github.com/codeplea/tinyexpr/blob/master/test.c > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

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

TTT_BEGIN (exprFunctions, "Expr - Functions")

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
    
    TTT_EXPECT (test_areEquivalent (test_args ("x",                      lookup, 10), 2));
    TTT_EXPECT (test_areEquivalent (test_args ("f+x",                    lookup, 10), 7));
    TTT_EXPECT (test_areEquivalent (test_args ("x+x",                    lookup, 10), 4));
    TTT_EXPECT (test_areEquivalent (test_args ("x+f",                    lookup, 10), 7));
    TTT_EXPECT (test_areEquivalent (test_args ("f+f",                    lookup, 10), 10));
    TTT_EXPECT (test_areEquivalent (test_args ("f+sum0",                 lookup, 10), 11));
    TTT_EXPECT (test_areEquivalent (test_args ("sum0+sum0",              lookup, 10), 12));
    TTT_EXPECT (test_areEquivalent (test_args ("sum0()+sum0",            lookup, 10), 12));
    TTT_EXPECT (test_areEquivalent (test_args ("sum0+sum0()",            lookup, 10), 12));
    TTT_EXPECT (test_areEquivalent (test_args ("sum0()+(0)+sum0()",      lookup, 10), 12));
    TTT_EXPECT (test_areEquivalent (test_args ("sum1 sum0",              lookup, 10), 12));
    TTT_EXPECT (test_areEquivalent (test_args ("sum1(sum0)",             lookup, 10), 12));
    TTT_EXPECT (test_areEquivalent (test_args ("sum1 f",                 lookup, 10), 10));
    TTT_EXPECT (test_areEquivalent (test_args ("sum1 x",                 lookup, 10), 4));
    TTT_EXPECT (test_areEquivalent (test_args ("sum2 (sum0, x)",         lookup, 10), 8));
    TTT_EXPECT (test_areEquivalent (test_args ("sum3 (sum0, x, 2)",      lookup, 10), 10));
    TTT_EXPECT (test_areEquivalent (test_args ("sum2(2,3)",              lookup, 10), 5));
    TTT_EXPECT (test_areEquivalent (test_args ("sum3(2,3,4)",            lookup, 10), 9));
    TTT_EXPECT (test_areEquivalent (test_args ("sum4(2,3,4,5)",          lookup, 10), 14));
    TTT_EXPECT (test_areEquivalent (test_args ("sum5(2,3,4,5,6)",        lookup, 10), 20));
    TTT_EXPECT (test_areEquivalent (test_args ("sum6(2,3,4,5,6,7)",      lookup, 10), 27));
    TTT_EXPECT (test_areEquivalent (test_args ("sum7(2,3,4,5,6,7,8)",    lookup, 10), 35));
    
TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
