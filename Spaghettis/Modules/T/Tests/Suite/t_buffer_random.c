
/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void test_doSomethingWithBuffer (t_buffer *x, int m, int n)
{
    static t_symbol *sym_foo = gensym ("foo");

    if (m == 0) { buffer_clear (x);             return; }
    if (m == 1) { buffer_reserve (x, n);        return; }
    if (m == 2) { buffer_resize (x, n);         return; }
    if (m == 3) { buffer_appendSemicolon (x);   return; }
    
    buffer_appendFloat (x, n);
    buffer_appendSymbol (x, sym_foo);
    
    // char *s = buffer_toString (x); ttt_stdout (TTT_COLOR_BLUE, "%s", s); PD_MEMORY_FREE (s);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (BufferRandom, "Buffer - Random")

    int i, j;
    
    t_rand48 seed; PD_RAND48_INIT (seed);
    
    for (i = 0; i < 1000; i++) {
    //
    t_buffer *b = buffer_new();
    
    for (j = 0; j < 1000; j++) {
        int m = PD_RAND48_UINT32 (seed) % 32;
        int n = PD_RAND48_UINT32 (seed) % 256;
        test_doSomethingWithBuffer (b, m, n);
    }
    
    buffer_free (b);
    //
    }
    
TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
