
/* Copyright (c) 1997-2020 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef S_MEMORY_H_
#define S_MEMORY_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void   *memory_get                     (size_t n);
PD_LOCAL void   *memory_getResize               (void *ptr, size_t oldSize, size_t newSize);

PD_LOCAL void   memory_free                     (void *ptr);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if PD_WITH_DEBUG

PD_LOCAL void   *leak_getMemoryChecked          (size_t n, const char *f, int line);
PD_LOCAL void   *leak_getMemoryResizeChecked    (void *ptr,
                                                    size_t oldSize,
                                                    size_t newSize,
                                                    const char *f,
                                                    int line);

PD_LOCAL void   leak_freeMemoryChecked          (void *ptr, const char *f, int line);


PD_LOCAL void   leak_initialize                 (void);
PD_LOCAL void   leak_release                    (void);

#endif // PD_WITH_DEBUG

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // S_MEMORY_H_
