
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (StringHeap, "String - Heap")

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
