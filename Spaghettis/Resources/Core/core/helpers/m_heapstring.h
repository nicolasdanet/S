
/* Copyright (c) 1997-2020 Miller Puckette and others. */

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

PD_LOCAL t_heapstring   *heapstring_new                         (int size);
PD_LOCAL char           *heapstring_getRaw                      (t_heapstring *x);

PD_LOCAL void       heapstring_free                             (t_heapstring *x);
PD_LOCAL int        heapstring_getSize                          (t_heapstring *x);

PD_LOCAL t_error    heapstring_add                              (t_heapstring *x, const char *src);
PD_LOCAL t_error    heapstring_append                           (t_heapstring *x, const char *src, int n);
PD_LOCAL void       heapstring_clear                            (t_heapstring *x);

#if defined ( PD_BUILDING_TESTS )

PD_LOCAL t_error    heapstring_addSprintf                       (t_heapstring *x, const char *format, ...);

#endif

PD_LOCAL int        heapstring_containsCharacterAtEnd           (t_heapstring *x, char c);
PD_LOCAL void       heapstring_removeCharacterIfContainsAtEnd   (t_heapstring *x, char c);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // M_HEAPSTRING_H_
