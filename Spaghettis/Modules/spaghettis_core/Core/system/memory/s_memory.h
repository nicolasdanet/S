
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef S_MEMORY_H_
#define S_MEMORY_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void   *memory_get                     (size_t n);
void   *memory_getResize               (void *ptr, size_t oldSize, size_t newSize);

void   memory_free                     (void *ptr);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if PD_WITH_DEBUG

void   *leak_getMemoryChecked          (size_t n, const char *f, int line);
void   *leak_getMemoryResizeChecked    (void *ptr,
                                                    size_t oldSize,
                                                    size_t newSize,
                                                    const char *f,
                                                    int line);

void   leak_freeMemoryChecked          (void *ptr, const char *f, int line);


void   leak_initialize                 (void);
void   leak_release                    (void);

#endif // PD_WITH_DEBUG

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // S_MEMORY_H_
