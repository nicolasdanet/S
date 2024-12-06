
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define TIME_NSEC_PER_SEC           1000000000ULL
#define TIME_USEC_PER_SEC           1000000ULL

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#define TIME_FROM_1900_TO_1970      2208988800ULL

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* < http://graphics.stanford.edu/~seander/bithacks.html > */
/* < http://aggregate.org/MAGIC/ > */

static uint8_t time_makeSeed16Reversed8 (uint8_t v)
{
    return (v * 0x0202020202ULL & 0x010884422010ULL) % 1023;
}

static uint16_t time_makeSeed16Reversed (uint16_t v)
{
    uint8_t hi = (uint8_t)(v >> 8);
    uint8_t lo = (uint8_t)(v & 0xff);
    
    return (((uint16_t)(time_makeSeed16Reversed8 (lo))) << 8) | (uint16_t)(time_makeSeed16Reversed8 (hi));
}

static uint64_t time_makeSeed16 (void)
{
    static uint16_t base = 0;           /* Static. */
    
    t_time t1, t2;
    uint16_t v1, v2;
    
    time_set (&t1);
    time_set (&t2);
    
    v1 = (uint16_t)t1;
    v2 = time_makeSeed16Reversed ((uint16_t)t2);
    base ^= (v1 ^ v2);
    
    return (uint64_t)base;
}

static uint64_t time_makeSeed (void)
{
    uint64_t seed, lo, hi;

    seed = time_makeSeed16();
    seed |= (time_makeSeed16() << 16);
    seed |= (time_makeSeed16() << 32);
    
    lo = PD_RAND48_UINT32 (seed);       /* To spread the bits. */
    hi = PD_RAND48_UINT32 (seed);       /* To spread the bits. */
    
    return ((hi << 32) | lo);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Do NOT fit for cryptography purpose. */

t_seed time_makeRandomSeed (void)
{
    static int once = 0;
    static uint64_t base = 0;   /* Static. */
    static uint64_t seed = 0;   /* Static. */
    
    if (!once) { once = 1; base = getpid(); }
    
    seed ^= base;
    
    PD_RAND48_NEXT (seed);
    PD_RAND48_NEXT (base);
    
    seed ^= time_makeSeed();
    
    return (t_seed)seed;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void time_set (t_time *t)
{
    struct timespec time;
    
    clock_gettime (CLOCK_MONOTONIC, &time);
    
    uint64_t seconds     = (uint64_t)time.tv_sec;
    uint64_t nanoseconds = (uint64_t)time.tv_nsec;
    
    (*t) = (seconds * TIME_NSEC_PER_SEC) + nanoseconds;
}

void time_addNanoseconds (t_time *t, t_nano ns)
{
    (*t) += ns;
}

t_error time_elapsedNanoseconds (const t_time *t0, const t_time *t1, t_nano *r)
{
    (*r) = 0ULL;
    
    if ((*t1) > (*t0)) { (*r) = (*t1) - (*t0); return PD_ERROR_NONE; }

    return PD_ERROR;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if PD_LINUX

void time_wait (t_time *t)
{
    struct timespec deadline;

    deadline.tv_sec  = (time_t)((*t) / TIME_NSEC_PER_SEC);
    deadline.tv_nsec = (long)((*t) % TIME_NSEC_PER_SEC);

    clock_nanosleep (CLOCK_MONOTONIC, TIMER_ABSTIME, &deadline, NULL);
}

#endif // PD_LINUX

#if PD_APPLE

void time_wait (t_time *t)
{
    t_nano ns; t_time now; time_set (&now);
    
    if (time_elapsedNanoseconds (&now, t, &ns) == PD_ERROR_NONE) { nano_sleep (ns); }
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Assumed thread-safe. */

/* < https://www.gnu.org/software/libc/manual/html_node/Sleeping.html > */

void nano_sleep (t_nano ns)
{
    if (ns != 0ULL) {
    //
    struct timespec t0, t1;
    struct timespec *ptrA = &t0;
    struct timespec *ptrB = &t1;
    struct timespec *temp = NULL;

    t0.tv_sec  = (time_t)(ns / TIME_NSEC_PER_SEC);
    t0.tv_nsec = (long)(ns % TIME_NSEC_PER_SEC);

    while ((nanosleep (ptrA, ptrB) == -1) && (errno == EINTR)) {
        temp = ptrA;
        ptrA = ptrB;
        ptrB = temp;
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void stamp_set (t_stamp *stamp)
{
    uint64_t s, f;
    struct timeval tv;
    
    gettimeofday (&tv, NULL);
    s = (uint64_t)tv.tv_sec + TIME_FROM_1900_TO_1970;
    f = ((uint64_t)tv.tv_usec << 32) / TIME_USEC_PER_SEC;
    // f = ((tv.tv_usec << 12) + (tv.tv_usec << 8) - ((tv.tv_usec * 1825) >> 5));

    (*stamp) = (s << 32) | f; 
}

void stamp_addNanoseconds (t_stamp *stamp, t_nano ns)
{
    uint64_t hi = ((*stamp) >> 32);
    uint64_t lo = ((*stamp) & 0xffffffffULL);
    uint64_t s = hi;
    uint64_t n = ((lo * TIME_NSEC_PER_SEC) >> 32) + ns;
        
    s += (uint64_t)(n / TIME_NSEC_PER_SEC);
    n %= TIME_NSEC_PER_SEC;

    (*stamp) = (s << 32) | ((uint64_t)((n << 32) / TIME_NSEC_PER_SEC));
}

t_error stamp_elapsedNanoseconds (const t_stamp *t0, const t_stamp *t1, t_nano *r)
{
    t_error err = PD_ERROR_NONE;

    uint64_t s0 = (*t0) >> 32;
    uint64_t n0 = (((*t0) & 0xffffffffULL) * TIME_NSEC_PER_SEC) >> 32;
    uint64_t s1 = (*t1) >> 32;
    uint64_t n1 = (((*t1) & 0xffffffffULL) * TIME_NSEC_PER_SEC) >> 32;
    
    (*r) = 0ULL;
    
    err |= (s1 < s0);                               /* < https://en.wikipedia.org/wiki/Year_2038_problem > */
    err |= (s1 == s0) && (n1 < n0);
    err |= ((*t0) == (*t1));
    
    if (!err) {
        (*r) = ((s1 * TIME_NSEC_PER_SEC) + n1) - ((s0 * TIME_NSEC_PER_SEC) + n0);
    }
    
    return err;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int stamp_isTagElement (t_symbol *s)
{
    return 0;
}

t_error stamp_setAsTags (int argc, t_atom *argv, t_stamp *stamp)
{
    return PD_ERROR;
}

t_error stamp_getWithTags (int argc, t_atom *argv, t_stamp *stamp)
{
    return PD_ERROR;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
