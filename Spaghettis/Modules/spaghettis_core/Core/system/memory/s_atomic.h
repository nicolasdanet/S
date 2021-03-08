
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

#define PD_MEMORY_BARRIER                       __sync_synchronize()

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define PD_ATOMIC_INT32_INCREMENT(q)            atomic_int32Increment((q))
#define PD_ATOMIC_INT32_DECREMENT(q)            atomic_int32Decrement((q))

#define PD_ATOMIC_INT32_READ(q)                 atomic_int32Read ((q))
#define PD_ATOMIC_INT32_WRITE(value, q)         atomic_int32Write ((value), (q))

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define PD_ATOMIC_UINT32_SET(mask, q)           atomic_uInt32Set ((mask), (q))
#define PD_ATOMIC_UINT32_UNSET(mask, q)         atomic_uInt32Unset ((mask), (q))

#define PD_ATOMIC_UINT32_READ(q)                atomic_uInt32Read ((q))
#define PD_ATOMIC_UINT32_WRITE(value, q)        atomic_uInt32Write ((value), (q))

#define PD_ATOMIC_UINT32_TRUE(mask, q)          atomic_uInt32True ((mask), (q))
#define PD_ATOMIC_UINT32_FALSE(mask, q)         atomic_uInt32False ((mask), (q))

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define PD_ATOMIC_UINT64_READ(q)                atomic_uInt64Read ((q))
#define PD_ATOMIC_UINT64_WRITE(value, q)        atomic_uInt64Write ((value), (q))

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define PD_ATOMIC_FLOAT64_READ(q)               atomic_float64Read ((q))
#define PD_ATOMIC_FLOAT64_WRITE(value, q)       atomic_float64Write ((value), (q))

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define PD_ATOMIC_POINTER_READ(q)               atomic_pointerRead ((q))
#define PD_ATOMIC_POINTER_WRITE(value, q)       atomic_pointerWrite (((void *)value), (q))

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL int32_t    atomic_int32Increment       (t_int32Atomic *q);
PD_LOCAL int32_t    atomic_int32Decrement       (t_int32Atomic *q);

PD_LOCAL int32_t    atomic_int32Read            (t_int32Atomic *q);
PD_LOCAL void       atomic_int32Write           (int32_t n, t_int32Atomic *q);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL uint32_t   atomic_uInt32Set            (uint32_t mask, t_uint32Atomic *q);
PD_LOCAL uint32_t   atomic_uInt32Unset          (uint32_t mask, t_uint32Atomic *q);

PD_LOCAL int        atomic_uInt32True           (uint32_t mask, t_uint32Atomic *q);
PD_LOCAL int        atomic_uInt32False          (uint32_t mask, t_uint32Atomic *q);

PD_LOCAL uint32_t   atomic_uInt32Read           (t_uint32Atomic *q);
PD_LOCAL void       atomic_uInt32Write          (uint32_t n, t_uint32Atomic *q);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_TESTS )

PD_LOCAL uint64_t   atomic_uInt64Read           (t_uint64Atomic *q);
PD_LOCAL void       atomic_uInt64Write          (uint64_t n, t_uint64Atomic *q);

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL double     atomic_float64Read          (t_float64Atomic *q);
PD_LOCAL void       atomic_float64Write         (double f, t_float64Atomic *q);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void       *atomic_pointerRead         (t_pointerAtomic *q);
PD_LOCAL void       atomic_pointerWrite         (void *p, t_pointerAtomic *q);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // S_ATOMIC_H_
