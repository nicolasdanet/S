
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (StringBase, "String - Base")

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
