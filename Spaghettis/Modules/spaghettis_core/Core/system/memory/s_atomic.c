
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < https://gcc.gnu.org/onlinedocs/gcc-4.8.2/gcc/_005f_005fatomic-Builtins.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL int32_t atomic_int32Increment (t_int32Atomic *q)
{
    return  __atomic_add_fetch (q, 1, __ATOMIC_SEQ_CST);
}

PD_LOCAL int32_t atomic_int32Decrement (t_int32Atomic *q)
{
    return __atomic_sub_fetch (q, 1, __ATOMIC_SEQ_CST);
}

PD_LOCAL int32_t atomic_int32Read (t_int32Atomic *q)
{
    return __atomic_load_n (q, __ATOMIC_SEQ_CST);
}

PD_LOCAL void atomic_int32Write (int32_t n, t_int32Atomic *q)
{
    __atomic_store_n (q, n, __ATOMIC_SEQ_CST);
}

PD_LOCAL int32_t atomic_int32ReadRelaxed (t_int32Atomic *q)
{
    return __atomic_load_n (q, __ATOMIC_RELAXED);
}

PD_LOCAL void atomic_int32WriteRelaxed (int32_t n, t_int32Atomic *q)
{
    __atomic_store_n (q, n, __ATOMIC_RELAXED);
}

PD_LOCAL int atomic_int32CompareAndSwap (int32_t *expected, int32_t n, t_int32Atomic *q)
{
    return __atomic_compare_exchange_n (q, expected, n, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL uint32_t atomic_uInt32Set (uint32_t mask, t_uint32Atomic *q)
{
    return __atomic_or_fetch (q, mask, __ATOMIC_SEQ_CST);
}

PD_LOCAL uint32_t atomic_uInt32Unset (uint32_t mask, t_uint32Atomic *q)
{
    return __atomic_and_fetch (q, ~(mask), __ATOMIC_SEQ_CST);
}

PD_LOCAL int atomic_uInt32True (uint32_t mask, t_uint32Atomic *q)
{
    return ((mask) & atomic_uInt32Read (q));
}

PD_LOCAL int atomic_uInt32False (uint32_t mask, t_uint32Atomic *q)
{
    return !atomic_uInt32True (mask, q);
}

PD_LOCAL uint32_t atomic_uInt32Read (t_uint32Atomic *q)
{
    return __atomic_load_n (q, __ATOMIC_SEQ_CST);
}

PD_LOCAL void atomic_uInt32Write (uint32_t n, t_uint32Atomic *q)
{
    __atomic_store_n (q, n, __ATOMIC_SEQ_CST);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_TESTS )

PD_LOCAL uint64_t atomic_uInt64Read (t_uint64Atomic *q)
{
    return __atomic_load_n (q, __ATOMIC_SEQ_CST);
}

PD_LOCAL void atomic_uInt64Write (uint64_t n, t_uint64Atomic *q)
{
    __atomic_store_n (q, n, __ATOMIC_SEQ_CST);
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* < https://gcc.gnu.org/ml/gcc-help/2014-11/msg00029.html > */
/* < https://github.com/kmatheussen/jack_capture/blob/master/atomic.h#L198 > */

PD_LOCAL double atomic_float64Read (t_float64Atomic *q)
{
    double t = 0.0; __atomic_load (q, &t, __ATOMIC_SEQ_CST); return t;
}

PD_LOCAL void atomic_float64Write (double f, t_float64Atomic *q)
{
    double t = f; __atomic_store (q, &t, __ATOMIC_SEQ_CST);
}

PD_LOCAL double atomic_float64ReadRelaxed (t_float64Atomic *q)
{
    double t = 0.0; __atomic_load (q, &t, __ATOMIC_RELAXED); return t;
}

PD_LOCAL void atomic_float64WriteRelaxed (double f, t_float64Atomic *q)
{
    double t = f; __atomic_store (q, &t, __ATOMIC_RELAXED);
}

PD_LOCAL int atomic_float64CompareAndSwap (double *expected, double f, t_float64Atomic *q)
{
    double t = f; return __atomic_compare_exchange (q, expected, &t, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void *atomic_pointerRead (t_pointerAtomic *q)
{
    return __atomic_load_n (q, __ATOMIC_SEQ_CST);
}

PD_LOCAL void atomic_pointerWrite (void *p, t_pointerAtomic *q)
{
    __atomic_store_n (q, p, __ATOMIC_SEQ_CST);
}

PD_LOCAL int atomic_pointerCompareAndSwap (void **expected, void *p, t_pointerAtomic *q)
{
    return __atomic_compare_exchange_n (q, expected, p, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
}

PD_LOCAL void *atomic_pointerSwap (void *p, t_pointerAtomic *q)
{
    return __atomic_exchange_n (q, p, __ATOMIC_SEQ_CST);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
