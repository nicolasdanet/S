
/* Copyright (c) 1997-2020 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef M_STRING_H_
#define M_STRING_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_error    string_addAtom                          (char *dest, size_t size, t_atom *a);

PD_LOCAL t_error    string_copy                             (char *dest, size_t size, const char *src);
PD_LOCAL t_error    string_add                              (char *dest, size_t size, const char *src);
PD_LOCAL t_error    string_append                           (char *dest, size_t size, const char *src, int n);
PD_LOCAL t_error    string_sprintf                          (char *dest, size_t size, const char *fmt, ...);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if ( PD_WITH_DEBUG ) || defined ( PD_BUILDING_TESTS )

PD_LOCAL t_error    string_addSprintf                       (char *dest, size_t size, const char *fmt, ...);

#endif

#if defined ( PD_BUILDING_TESTS )

PD_LOCAL void       string_clear                            (char *dest, size_t size);

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL uint64_t   string_hash                             (const char *s);

PD_LOCAL int    string_startWith                            (const char *s, const char *isStart);
PD_LOCAL int    string_endWith                              (const char *s, const char *isEnd);
PD_LOCAL int    string_contains                             (const char *s, const char *isContained);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL int    string_containsOccurrenceAtStart            (const char *s, const char *chars);
PD_LOCAL int    string_indexOfFirstOccurrenceFromEnd        (const char *s, const char *chars);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void   string_replaceCharacter                     (char *s, char toBeReplaced, char c);
PD_LOCAL void   string_removeCharacter                      (char *s, char toBeRemoved);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL int    string_startWithDollarNumber                (const char *s);
PD_LOCAL int    string_isDollarNumber                       (const char *s);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL int    char_isSemicolonOrComma                     (char c);
PD_LOCAL int    char_isBackslash                            (char c);
PD_LOCAL int    char_isWhitespaceOrNewlineOrTabulation      (char c);
PD_LOCAL int    char_isNumericOrDot                         (char c);
PD_LOCAL int    char_isAlphabeticLowercase                  (char c);
PD_LOCAL int    char_isAlphanumericOrUnderscore             (char c);
PD_LOCAL int    char_isAlphanumericLowercaseOrUnderscore    (char c);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // M_STRING_H_
