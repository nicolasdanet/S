
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef M_HEAPSTRING_H_
#define M_HEAPSTRING_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _heapstring {
    size_t  hs_allocated;
    size_t  hs_size;
    char    *hs_raw;
    } t_heapstring;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_heapstring   *heapstring_new                         (int size);
char           *heapstring_getRaw                      (t_heapstring *x);

void       heapstring_free                             (t_heapstring *x);
int        heapstring_getSize                          (t_heapstring *x);

t_error    heapstring_add                              (t_heapstring *x, const char *src);
t_error    heapstring_append                           (t_heapstring *x, const char *src, int n);
void       heapstring_clear                            (t_heapstring *x);

int        heapstring_containsCharacterAtEnd           (t_heapstring *x, char c);
void       heapstring_removeCharacterIfContainsAtEnd   (t_heapstring *x, char c);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // M_HEAPSTRING_H_
