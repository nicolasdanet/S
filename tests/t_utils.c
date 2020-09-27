
/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if 0
void test75__string() {
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (UtilsString, 75, "Utils - String")

    char t[64] = { 0 };
    
    TTT_EXPECT (string_copy (t, 64, "Trois cents millions de papillons") == PD_ERROR_NONE);
    TTT_EXPECT (strcmp (t, "Trois cents millions de papillons") == 0);
    TTT_EXPECT (string_copy (t, 64, "Quand Martin, Martin, Martin") == PD_ERROR_NONE);
    TTT_EXPECT (strcmp (t, "Quand Martin, Martin, Martin") == 0);
    
    string_clear (t, 64); TTT_EXPECT (strcmp (t, "") == 0);
    
    TTT_EXPECT (string_copy (t, 16, "Trois cents millions de papillons") == PD_ERROR);
    TTT_EXPECT (strcmp (t, "Trois cents mil") == 0);
    TTT_EXPECT (string_copy (t, 16, "Trois cents mill") == PD_ERROR);
    TTT_EXPECT (strcmp (t, "Trois cents mil") == 0);
    TTT_EXPECT (string_copy (t, 16, "Trois cents mil") == PD_ERROR_NONE);
    TTT_EXPECT (strcmp (t, "Trois cents mil") == 0);
    
    TTT_EXPECT (string_copy (t, 64, "Quand Martin") == PD_ERROR_NONE);
    TTT_EXPECT (strcmp (t, "Quand Martin") == 0);
    TTT_EXPECT (string_add (t, 64, ", Martin") == PD_ERROR_NONE);
    TTT_EXPECT (string_add (t, 64, ", Martin") == PD_ERROR_NONE);
    TTT_EXPECT (strcmp (t, "Quand Martin, Martin, Martin") == 0);
    TTT_EXPECT (string_add (t, 64, ", Martin") == PD_ERROR_NONE);
    TTT_EXPECT (string_add (t, 64, ", Martin") == PD_ERROR_NONE);
    TTT_EXPECT (string_add (t, 64, ", Martin") == PD_ERROR_NONE);
    TTT_EXPECT (string_add (t, 64, ", Martin") == PD_ERROR_NONE);
    TTT_EXPECT (string_add (t, 64, ", Martin") == PD_ERROR);
    TTT_EXPECT (strcmp (t, "Quand Martin, Martin, Martin, Martin, Martin, Martin, Martin, M") == 0);
    
    string_clear (t, 16);
    TTT_EXPECT (string_add (t, 16, "Trois cents millions de papillons") == PD_ERROR);
    TTT_EXPECT (strcmp (t, "Trois cents mil") == 0);
    string_clear (t, 16);
    TTT_EXPECT (string_add (t, 16, "Quand Martin") == PD_ERROR_NONE);
    TTT_EXPECT (strcmp (t, "Quand Martin") == 0);
    
    string_clear (t, 16);
    TTT_EXPECT (string_append (t, 16, "Trois cents millions de papillons", 5) == PD_ERROR_NONE);
    TTT_EXPECT (strcmp (t, "Trois") == 0);
    TTT_EXPECT (string_append (t, 16, " Martin", 0) == PD_ERROR_NONE);
    TTT_EXPECT (strcmp (t, "Trois") == 0);
    TTT_EXPECT (string_append (t, 16, " Martin", 1234) == PD_ERROR_NONE);
    TTT_EXPECT (strcmp (t, "Trois Martin") == 0);
    TTT_EXPECT (string_append (t, 16, " Martin", 5) == PD_ERROR);
    TTT_EXPECT (strcmp (t, "Trois Martin Ma") == 0);
    TTT_EXPECT (string_append (t, 16, " Martin", 5) == PD_ERROR);
    TTT_EXPECT (strcmp (t, "Trois Martin Ma") == 0);
    
    string_clear (t, 16);
    TTT_EXPECT (string_append (t, 16, "abcdefgh", 1) == PD_ERROR_NONE);
    TTT_EXPECT (string_append (t, 16, "abcdefgh", 2) == PD_ERROR_NONE);
    TTT_EXPECT (string_append (t, 16, "abcdefgh", 3) == PD_ERROR_NONE);
    TTT_EXPECT (string_append (t, 16, "abcdefgh", 4) == PD_ERROR_NONE);
    TTT_EXPECT (string_append (t, 16, "abcdefgh", 5) == PD_ERROR_NONE);
    TTT_EXPECT (string_append (t, 16, "abcdefgh", 6) == PD_ERROR);
    TTT_EXPECT (strcmp (t, "aababcabcdabcde") == 0);
    
    TTT_EXPECT (string_sprintf (t, 16, "%d %s", 3, "Martin") == PD_ERROR_NONE);
    TTT_EXPECT (strcmp (t, "3 Martin") == 0);
    TTT_EXPECT (string_sprintf (t, 16, "%d %s", 3, "cents millions de papillons") == PD_ERROR);
    TTT_EXPECT (strcmp (t, "3 cents million") == 0);
    TTT_EXPECT (string_sprintf (t, 16, "%s%s%s", "Trois", "Trois", "Trois") == PD_ERROR_NONE);
    TTT_EXPECT (strcmp (t, "TroisTroisTrois") == 0);
    
    string_clear (t, 64);
    TTT_EXPECT (string_addSprintf (t, 64, "%d %s", 3, "cents millions de papillons") == PD_ERROR_NONE);
    TTT_EXPECT (strcmp (t, "3 cents millions de papillons") == 0);
    
    string_clear (t, 64);
    TTT_EXPECT (string_addSprintf (t, 64, "%s", "Quand Martin") == PD_ERROR_NONE);
    TTT_EXPECT (strcmp (t, "Quand Martin") == 0);
    TTT_EXPECT (string_addSprintf (t, 64, "%s", ", Martin") == PD_ERROR_NONE);
    TTT_EXPECT (string_addSprintf (t, 64, "%s", ", Martin") == PD_ERROR_NONE);
    TTT_EXPECT (strcmp (t, "Quand Martin, Martin, Martin") == 0);
    TTT_EXPECT (string_addSprintf (t, 64, "%s", ", Martin") == PD_ERROR_NONE);
    TTT_EXPECT (string_addSprintf (t, 64, "%s", ", Martin") == PD_ERROR_NONE);
    TTT_EXPECT (string_addSprintf (t, 64, "%s", ", Martin") == PD_ERROR_NONE);
    TTT_EXPECT (string_addSprintf (t, 64, "%s", ", Martin") == PD_ERROR_NONE);
    TTT_EXPECT (string_addSprintf (t, 64, "%s", ", Martin") == PD_ERROR);
    TTT_EXPECT (strcmp (t, "Quand Martin, Martin, Martin, Martin, Martin, Martin, Martin, M") == 0);
    
TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if 0
}
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if 0
void test76__string() {
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (UtilsHeap, 76, "Utils - Heap")

    {
    t_heapstring *t = heapstring_new (1);
    
    TTT_EXPECT (heapstring_add (t, "Trois cents millions de papillons") == PD_ERROR_NONE);
    TTT_EXPECT (strcmp (heapstring_getRaw (t), "Trois cents millions de papillons") == 0);
    TTT_EXPECT (heapstring_getSize (t) == 33);
    
    TTT_EXPECT (heapstring_add (t, " et Martin") == PD_ERROR_NONE);
    TTT_EXPECT (strcmp (heapstring_getRaw (t), "Trois cents millions de papillons et Martin") == 0);
    TTT_EXPECT (heapstring_getSize (t) == 43);
    
    heapstring_free (t);
    }
    
    {
    t_heapstring *t = heapstring_new (1);
    
    TTT_EXPECT (heapstring_add (t, "Quand Martin") == PD_ERROR_NONE);
    TTT_EXPECT (strcmp (heapstring_getRaw (t), "Quand Martin") == 0);
    TTT_EXPECT (heapstring_add (t, ", Martin") == PD_ERROR_NONE);
    TTT_EXPECT (heapstring_add (t, ", Martin") == PD_ERROR_NONE);
    TTT_EXPECT (strcmp (heapstring_getRaw (t), "Quand Martin, Martin, Martin") == 0);
    TTT_EXPECT (heapstring_add (t, ", Martin") == PD_ERROR_NONE);
    TTT_EXPECT (heapstring_add (t, ", Martin") == PD_ERROR_NONE);
    TTT_EXPECT (heapstring_add (t, ", Martin") == PD_ERROR_NONE);
    TTT_EXPECT (strcmp (heapstring_getRaw (t), "Quand Martin, Martin, Martin, Martin, Martin, Martin") == 0);
    TTT_EXPECT (heapstring_getSize (t) == 52);
    
    heapstring_free (t);
    }
    
    {
    t_heapstring *t = heapstring_new (1);
    
    TTT_EXPECT (heapstring_append (t, "Trois cents millions de papillons", 5) == PD_ERROR_NONE);
    TTT_EXPECT (strcmp (heapstring_getRaw (t), "Trois") == 0);
    TTT_EXPECT (heapstring_append (t, " Martin", 0) == PD_ERROR_NONE);
    TTT_EXPECT (strcmp (heapstring_getRaw (t), "Trois") == 0);
    TTT_EXPECT (heapstring_append (t, " Martin", 1234) == PD_ERROR_NONE);
    TTT_EXPECT (strcmp (heapstring_getRaw (t), "Trois Martin") == 0);
    
    heapstring_clear (t);
    TTT_EXPECT (strcmp (heapstring_getRaw (t), "") == 0);
    TTT_EXPECT (heapstring_getSize (t) == 0);
    
    TTT_EXPECT (heapstring_append (t, "abcdefgh", 1) == PD_ERROR_NONE);
    TTT_EXPECT (heapstring_append (t, "abcdefgh", 2) == PD_ERROR_NONE);
    TTT_EXPECT (heapstring_append (t, "abcdefgh", 3) == PD_ERROR_NONE);
    TTT_EXPECT (heapstring_append (t, "abcdefgh", 4) == PD_ERROR_NONE);
    TTT_EXPECT (heapstring_append (t, "abcdefgh", 5) == PD_ERROR_NONE);
    TTT_EXPECT (heapstring_append (t, "abcdefgh", 1) == PD_ERROR_NONE);
    TTT_EXPECT (heapstring_append (t, "abcdefgh", 2) == PD_ERROR_NONE);
    TTT_EXPECT (heapstring_append (t, "abcdefgh", 3) == PD_ERROR_NONE);
    TTT_EXPECT (heapstring_append (t, "abcdefgh", 4) == PD_ERROR_NONE);
    TTT_EXPECT (heapstring_append (t, "abcdefgh", 5) == PD_ERROR_NONE);
    TTT_EXPECT (strcmp (heapstring_getRaw (t), "aababcabcdabcdeaababcabcdabcde") == 0);
    
    heapstring_free (t);
    }
    
    {
    t_heapstring *t = heapstring_new (1);
    
    TTT_EXPECT (heapstring_addSprintf (t, "%d %s", 3, "Martin") == PD_ERROR_NONE);
    TTT_EXPECT (strcmp (heapstring_getRaw (t), "3 Martin") == 0);
    TTT_EXPECT (heapstring_addSprintf (t, " et %d %s", 3, "papillons") == PD_ERROR_NONE);
    TTT_EXPECT (strcmp (heapstring_getRaw (t), "3 Martin et 3 papillons") == 0);
    TTT_EXPECT (heapstring_addSprintf (t, " %s%s%s", "Trois", "Trois", "Trois") == PD_ERROR_NONE);
    TTT_EXPECT (strcmp (heapstring_getRaw (t), "3 Martin et 3 papillons TroisTroisTrois") == 0);
    
    heapstring_free (t);
    }
    
    {
    t_heapstring *t = heapstring_new (1);
    
    TTT_EXPECT (heapstring_addSprintf (t, "%s", "Quand Martin") == PD_ERROR_NONE);
    TTT_EXPECT (strcmp (heapstring_getRaw (t), "Quand Martin") == 0);
    TTT_EXPECT (heapstring_addSprintf (t, "%s", ", Martin") == PD_ERROR_NONE);
    TTT_EXPECT (heapstring_addSprintf (t, "%s", ", Martin") == PD_ERROR_NONE);
    TTT_EXPECT (strcmp (heapstring_getRaw (t), "Quand Martin, Martin, Martin") == 0);
    TTT_EXPECT (heapstring_addSprintf (t, "%s", ", Martin") == PD_ERROR_NONE);
    TTT_EXPECT (heapstring_addSprintf (t, "%s", ", Martin") == PD_ERROR_NONE);
    TTT_EXPECT (heapstring_addSprintf (t, "%s", ", Martin") == PD_ERROR_NONE);
    TTT_EXPECT (strcmp (heapstring_getRaw (t), "Quand Martin, Martin, Martin, Martin, Martin, Martin") == 0);
    
    heapstring_free (t);
    }
    
TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if 0
}
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
