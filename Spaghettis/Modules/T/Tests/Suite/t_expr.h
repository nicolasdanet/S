
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static inline double test_args (const char *expression, const te_variable *variables, int count)
{
    te_expr *x = te_compile (expression, variables, count);
    
    if (x) { double f = te_eval (x); te_free (x); return f; }
    
    return 0;
}

static inline double test_expr (const char *expression)
{
    return test_args (expression, NULL, 0);
}

static inline double test_raw (const char *expression)
{
    te_expr *x = te_compile (expression, NULL, 0);
    
    if (x) { double f = x->te_value; te_free (x); return f; }
    
    return 0;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
