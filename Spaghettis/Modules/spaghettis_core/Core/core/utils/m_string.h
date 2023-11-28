
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef M_STRING_H_
#define M_STRING_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error    string_addAtom                          (char *dest, size_t size, t_atom *a);

t_error    string_copy                             (char *dest, size_t size, const char *src);
t_error    string_add                              (char *dest, size_t size, const char *src);
t_error    string_append                           (char *dest, size_t size, const char *src, int n);
t_error    string_sprintf                          (char *dest, size_t size, const char *fmt, ...);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

uint64_t   string_hash                             (const char *s);

int    string_startWith                            (const char *s, const char *isStart);
int    string_endWith                              (const char *s, const char *isEnd);
int    string_contains                             (const char *s, const char *isContained);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int    string_containsOccurrenceAtStart            (const char *s, const char *chars);
int    string_indexOfFirstOccurrenceFromEnd        (const char *s, const char *chars);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void   string_replaceCharacter                     (char *s, char toBeReplaced, char c);
void   string_removeCharacter                      (char *s, char toBeRemoved);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int    string_startWithDollarNumber                (const char *s);
int    string_isDollarNumber                       (const char *s);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int    char_isSemicolonOrComma                     (char c);
int    char_isBackslash                            (char c);
int    char_isWhitespaceOrNewlineOrTabulation      (char c);
int    char_isNumericOrDot                         (char c);
int    char_isAlphabeticLowercase                  (char c);
int    char_isAlphanumericOrUnderscore             (char c);
int    char_isAlphanumericLowercaseOrUnderscore    (char c);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // M_STRING_H_
