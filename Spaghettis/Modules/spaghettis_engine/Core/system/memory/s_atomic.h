
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef S_ATOMIC_H_
#define S_ATOMIC_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define     PD_MEMORY_BARRIER               __sync_synchronize()

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int32_t     atomic_int32Increment           (t_int32Atomic *q);
int32_t     atomic_int32Decrement           (t_int32Atomic *q);

int32_t     atomic_int32Read                (t_int32Atomic *q);
void        atomic_int32Write               (t_int32Atomic *q, int32_t n);

int32_t     atomic_int32ReadRelaxed         (t_int32Atomic *q);
void        atomic_int32WriteRelaxed        (t_int32Atomic *q, int32_t n);

int         atomic_int32CompareAndSwap      (t_int32Atomic *q, int32_t *expected, int32_t n);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

uint32_t    atomic_uInt32Set                (t_uint32Atomic *q, uint32_t mask);
uint32_t    atomic_uInt32Unset              (t_uint32Atomic *q, uint32_t mask);

int         atomic_uInt32True               (t_uint32Atomic *q, uint32_t mask);
int         atomic_uInt32False              (t_uint32Atomic *q, uint32_t mask);

uint32_t    atomic_uInt32Read               (t_uint32Atomic *q);
void        atomic_uInt32Write              (t_uint32Atomic *q, uint32_t n);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

uint64_t    atomic_uInt64ReadRelaxed        (t_uint64Atomic *q);
uint64_t    atomic_uInt64ReadAcquire        (t_uint64Atomic *q);

void        atomic_uInt64WriteRelaxed       (t_uint64Atomic *q, uint64_t n);
void        atomic_uInt64WriteRelease       (t_uint64Atomic *q, uint64_t n);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

double      atomic_float64Read              (t_float64Atomic *q);
void        atomic_float64Write             (t_float64Atomic *q, double f);

double      atomic_float64ReadRelaxed       (t_float64Atomic *q);
void        atomic_float64WriteRelaxed      (t_float64Atomic *q, double f);

int         atomic_float64CompareAndSwap    (t_float64Atomic *q, double *expected, double f);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void        *atomic_pointerRead             (t_pointerAtomic *q);
void        atomic_pointerWrite             (t_pointerAtomic *q, void *p);

void        *atomic_pointerSwap             (t_pointerAtomic *q, void *p);
int         atomic_pointerCompareAndSwap    (t_pointerAtomic *q, void **expected, void *p);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // S_ATOMIC_H_
