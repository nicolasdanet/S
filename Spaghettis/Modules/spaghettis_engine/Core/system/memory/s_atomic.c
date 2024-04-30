
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* TODO: Check required alignment? */

/* https://en.cppreference.com/w/cpp/atomic/atomic_ref/required_alignment */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( __cplusplus )

#include <atomic>

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wignored-attributes"

    static_assert (std::atomic<t_int32Atomic>::is_always_lock_free);
    static_assert (std::atomic<t_uint32Atomic>::is_always_lock_free);
    static_assert (std::atomic<t_uint64Atomic>::is_always_lock_free);
    static_assert (std::atomic<t_pointerAtomic>::is_always_lock_free);
    static_assert (std::atomic<t_float64Atomic>::is_always_lock_free);

    #pragma GCC diagnostic pop

#else

static_assert (__atomic_always_lock_free (sizeof (t_int32Atomic),   0), "");
static_assert (__atomic_always_lock_free (sizeof (t_uint32Atomic),  0), "");
static_assert (__atomic_always_lock_free (sizeof (t_uint64Atomic),  0), "");
static_assert (__atomic_always_lock_free (sizeof (t_pointerAtomic), 0), "");
static_assert (__atomic_always_lock_free (sizeof (t_float64Atomic), 0), "");

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < https://gcc.gnu.org/onlinedocs/gcc-4.8.2/gcc/_005f_005fatomic-Builtins.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int32_t atomic_int32Increment (t_int32Atomic *q)
{
    return  __atomic_add_fetch (q, 1, __ATOMIC_SEQ_CST);
}

int32_t atomic_int32Decrement (t_int32Atomic *q)
{
    return __atomic_sub_fetch (q, 1, __ATOMIC_SEQ_CST);
}

int32_t atomic_int32Read (t_int32Atomic *q)
{
    return __atomic_load_n (q, __ATOMIC_SEQ_CST);
}

void atomic_int32Write (t_int32Atomic *q, int32_t n)
{
    __atomic_store_n (q, n, __ATOMIC_SEQ_CST);
}

int32_t atomic_int32ReadRelaxed (t_int32Atomic *q)
{
    return __atomic_load_n (q, __ATOMIC_RELAXED);
}

void atomic_int32WriteRelaxed (t_int32Atomic *q, int32_t n)
{
    __atomic_store_n (q, n, __ATOMIC_RELAXED);
}

int atomic_int32CompareAndSwap (t_int32Atomic *q, int32_t *expected, int32_t n)
{
    return __atomic_compare_exchange_n (q, expected, n, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

uint32_t atomic_uInt32Set (t_uint32Atomic *q, uint32_t mask)
{
    return __atomic_or_fetch (q, mask, __ATOMIC_SEQ_CST);
}

uint32_t atomic_uInt32Unset (t_uint32Atomic *q, uint32_t mask)
{
    return __atomic_and_fetch (q, ~(mask), __ATOMIC_SEQ_CST);
}

int atomic_uInt32True (t_uint32Atomic *q, uint32_t mask)
{
    return ((mask) & atomic_uInt32Read (q));
}

int atomic_uInt32False (t_uint32Atomic *q, uint32_t mask)
{
    return !atomic_uInt32True (q, mask);
}

uint32_t atomic_uInt32Read (t_uint32Atomic *q)
{
    return __atomic_load_n (q, __ATOMIC_SEQ_CST);
}

void atomic_uInt32Write (t_uint32Atomic *q, uint32_t n)
{
    __atomic_store_n (q, n, __ATOMIC_SEQ_CST);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

uint64_t atomic_uInt64ReadRelaxed (t_uint64Atomic *q)
{
    return __atomic_load_n (q, __ATOMIC_RELAXED);
}

uint64_t atomic_uInt64ReadAcquire (t_uint64Atomic *q)
{
    return __atomic_load_n (q, __ATOMIC_ACQUIRE);
}

void atomic_uInt64WriteRelaxed (t_uint64Atomic *q, uint64_t n)
{
    __atomic_store_n (q, n, __ATOMIC_RELAXED);
}

void atomic_uInt64WriteRelease (t_uint64Atomic *q, uint64_t n)
{
    __atomic_store_n (q, n, __ATOMIC_RELEASE);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* < https://gcc.gnu.org/ml/gcc-help/2014-11/msg00029.html > */
/* < https://github.com/kmatheussen/jack_capture/blob/master/atomic.h#L198 > */

double atomic_float64Read (t_float64Atomic *q)
{
    double t = 0.0; __atomic_load (q, &t, __ATOMIC_SEQ_CST); return t;
}

void atomic_float64Write (t_float64Atomic *q, double f)
{
    double t = f; __atomic_store (q, &t, __ATOMIC_SEQ_CST);
}

double atomic_float64ReadRelaxed (t_float64Atomic *q)
{
    double t = 0.0; __atomic_load (q, &t, __ATOMIC_RELAXED); return t;
}

void atomic_float64WriteRelaxed (t_float64Atomic *q, double f)
{
    double t = f; __atomic_store (q, &t, __ATOMIC_RELAXED);
}

int atomic_float64CompareAndSwap (t_float64Atomic *q, double *expected, double f)
{
    double t = f; return __atomic_compare_exchange (q, expected, &t, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void *atomic_pointerRead (t_pointerAtomic *q)
{
    return __atomic_load_n (q, __ATOMIC_SEQ_CST);
}

void atomic_pointerWrite (t_pointerAtomic *q, void *p)
{
    __atomic_store_n (q, p, __ATOMIC_SEQ_CST);
}

int atomic_pointerCompareAndSwap (t_pointerAtomic *q, void **expected, void *p)
{
    return __atomic_compare_exchange_n (q, expected, p, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
}

void *atomic_pointerSwap (t_pointerAtomic *q, void *p)
{
    return __atomic_exchange_n (q, p, __ATOMIC_SEQ_CST);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
