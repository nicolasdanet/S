
/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define TEST_BUFFER_LOOP    1000

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if 0
void test70__basic() {
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (BufferBasic, 70, "Buffer - Basic")

t_buffer *b = buffer_new();
t_buffer *t = buffer_new();

{
    buffer_appendSymbol (t, gensym ("Monsieur"));
    buffer_appendSymbol (t, gensym ("Jack"));
    buffer_appendComma (t);
    buffer_appendSymbol (t, gensym ("vous"));
    buffer_appendSymbol (t, gensym ("dactylographiez"));
    buffer_appendSymbol (t, gensym ("bien"));
    buffer_appendSymbol (t, gensym ("mieux"));
    buffer_appendSymbol (t, gensym ("que"));
    buffer_appendSymbol (t, gensym ("votre"));
    buffer_appendSymbol (t, gensym ("ami"));
    buffer_appendSymbol (t, gensym ("Wolf"));
    buffer_appendComma (t);
    buffer_appendFloat (t, 1);
    buffer_appendFloat (t, 2);
    buffer_appendFloat (t, 3);
    buffer_appendFloat (t, 4);

    buffer_appendBuffer (b, t);

    TTT_EXPECT (buffer_getSize (t) == 16);
    TTT_EXPECT (buffer_getSize (b) == 16);

    TTT_EXPECT (strcmp (symbol_getName (atom_getSymbol (buffer_getAtomAtIndex (b, 1))),  "Jack") == 0);
    TTT_EXPECT (strcmp (symbol_getName (atom_getSymbol (buffer_getAtomAtIndex (b, 10))), "Wolf") == 0);
}

{
    char *s = NULL;
    
    s = buffer_toString (b);
    TTT_EXPECT (!strcmp (s, "Monsieur Jack, vous dactylographiez bien mieux que votre ami Wolf, 1 2 3 4"));

    buffer_extend (b, 11, 16, 0);

    PD_MEMORY_FREE (s); s = buffer_toString (b);
    TTT_EXPECT (!strcmp (s, "Monsieur Jack, vous dactylographiez bien mieux que votre ami Wolf"));
    
    buffer_extend (b, 3, 5, 4);
    
    TTT_EXPECT (IS_NULL (buffer_getAtomAtIndex (b, 5)));
    TTT_EXPECT (IS_NULL (buffer_getAtomAtIndex (b, 6)));
    
    buffer_setSymbolAtIndex (b, 5, gensym ("et"));
    buffer_setSymbolAtIndex (b, 6, gensym ("patchez"));

    PD_MEMORY_FREE (s); s = buffer_toString (b);
    TTT_EXPECT (!strcmp (s, "Monsieur Jack, vous dactylographiez et patchez bien mieux que votre ami Wolf"));
    
    buffer_extend (b, 4, 6, 0);
    buffer_extend (b, 1, 2, 0);
    
    PD_MEMORY_FREE (s); s = buffer_toString (b);
    TTT_EXPECT (!strcmp (s, "Monsieur, vous patchez bien mieux que votre ami Wolf"));
    
    buffer_extend (b, 1, 1, 2);
    
    buffer_setSymbolAtIndex (b, 1, gensym ("Miller"));
    buffer_setSymbolAtIndex (b, 2, gensym ("Puckette"));
    
    PD_MEMORY_FREE (s); s = buffer_toString (b);
    TTT_EXPECT (!strcmp (s, "Monsieur Miller Puckette, vous patchez bien mieux que votre ami Wolf"));
    
    buffer_extend (b, 0, 7, 6);
    
    PD_MEMORY_FREE (s); s = buffer_toString (b);
    TTT_EXPECT (!strcmp (s, "Monsieur Miller Puckette, vous patchez mieux que votre ami Wolf"));
    PD_MEMORY_FREE (s);
}

{
    char *s = NULL;
    
    t_atom numbers[4];
    
    SET_SYMBOL (numbers + 0, gensym ("ga"));
    SET_SYMBOL (numbers + 1, gensym ("bu"));
    SET_SYMBOL (numbers + 2, gensym ("zo"));
    SET_SYMBOL (numbers + 3, gensym ("meu"));
    
    buffer_clear (b); buffer_replace (b, 0, 0, 4, numbers);
    
    s = buffer_toString (b);
    TTT_EXPECT (!strcmp (s, "ga bu zo meu"));

    buffer_replace (b, 3, 4, 3, numbers);
    
    PD_MEMORY_FREE (s); s = buffer_toString (b);
    TTT_EXPECT (!strcmp (s, "ga bu zo ga bu zo"));
    
    buffer_replace (b, 0, 3, 4, numbers);
    
    PD_MEMORY_FREE (s); s = buffer_toString (b);
    TTT_EXPECT (!strcmp (s, "ga bu zo meu ga bu zo"));
    
    buffer_replace (b, 4, 4, 4, numbers);
    
    PD_MEMORY_FREE (s); s = buffer_toString (b);
    TTT_EXPECT (!strcmp (s, "ga bu zo meu ga bu zo meu ga bu zo"));
    
    buffer_replace (b, 2, 10, 2, numbers);
    
    PD_MEMORY_FREE (s); s = buffer_toString (b);
    TTT_EXPECT (!strcmp (s, "ga bu ga bu zo"));
    
    buffer_replace (b, 4, 5, 2, numbers);
    
    PD_MEMORY_FREE (s); s = buffer_toString (b);
    TTT_EXPECT (!strcmp (s, "ga bu ga bu ga bu"));
    
    PD_MEMORY_FREE (s);
}
    
{
    char *s = NULL;
    
    t_atom menu[4];
    
    SET_SYMBOL (menu + 0, gensym ("spam"));
    SET_SYMBOL (menu + 1, gensym ("bacon"));
    SET_SYMBOL (menu + 2, gensym ("egg"));
    SET_SYMBOL (menu + 3, gensym ("and"));
    
    buffer_clear (b);
    
    buffer_prepend (b, 1,       menu + 1);
    buffer_append (b, 1,        menu + 2);
    buffer_prepend (b, 1,       menu + 0);
    buffer_insertAtIndex (b, 2, menu + 3);
    
    s = buffer_toString (b);
    TTT_EXPECT (!strcmp (s, "spam bacon and egg"));
    
    buffer_insertAtIndex (b, 0, menu + 0);
    buffer_insertAtIndex (b, 3, menu + 0);
    
    PD_MEMORY_FREE (s); s = buffer_toString (b);
    TTT_EXPECT (!strcmp (s, "spam spam bacon spam and egg"));
    
    buffer_prepend (b, 4, menu);
    
    PD_MEMORY_FREE (s); s = buffer_toString (b);
    TTT_EXPECT (!strcmp (s, "spam bacon egg and spam spam bacon spam and egg"));
    PD_MEMORY_FREE (s);
    
    t_atom a;
    
    TTT_EXPECT (buffer_pop (b, &a)  == PD_ERROR_NONE);
    TTT_EXPECT (atom_getSymbol (&a) == GET_SYMBOL (menu + 2));
    TTT_EXPECT (buffer_pop (b, &a)  == PD_ERROR_NONE);
    TTT_EXPECT (atom_getSymbol (&a) == GET_SYMBOL (menu + 3));
    TTT_EXPECT (buffer_pop (b, &a)  == PD_ERROR_NONE);
    TTT_EXPECT (atom_getSymbol (&a) == GET_SYMBOL (menu + 0));
    TTT_EXPECT (buffer_pop (b, &a)  == PD_ERROR_NONE);
    TTT_EXPECT (atom_getSymbol (&a) == GET_SYMBOL (menu + 1));
    
    buffer_clear (b);
    
    TTT_EXPECT (buffer_pop (b, &a) == PD_ERROR);
}

buffer_free (t);
buffer_free (b);
    
TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if 0
}
#endif

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
// MARK: -

#if 0
void test71__random() {
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (BufferRandom, 71, "Buffer - Random")

    int i, j;
    
    t_rand48 seed; PD_RAND48_INIT (seed);
    
    for (i = 0; i < TEST_BUFFER_LOOP; i++) {
    //
    t_buffer *b = buffer_new();
    
    for (j = 0; j < TEST_BUFFER_LOOP; j++) {
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

#if 0
}
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
