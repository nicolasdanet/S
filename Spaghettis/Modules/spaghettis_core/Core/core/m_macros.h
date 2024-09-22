
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef M_MACROS_H_
#define M_MACROS_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define class_addDsp(c, m)  class_addMethod ((c), (t_method)(m), sym__dsp, A_CANT, A_NULL)

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define dsp_add1(fn, a)                         chain_append (instance_chainGetTemporary(), \
                                                                (t_perform)(fn), 1, \
                                                                (t_int)(a))

#define dsp_add2(fn, a, b)                      chain_append (instance_chainGetTemporary(), \
                                                                (t_perform)(fn), 2, \
                                                                (t_int)(a), \
                                                                (t_int)(b))

#define dsp_add3(fn, a, b, c)                   chain_append (instance_chainGetTemporary(), \
                                                                (t_perform)(fn), 3, \
                                                                (t_int)(a), \
                                                                (t_int)(b), \
                                                                (t_int)(c))

#define dsp_add4(fn, a, b, c, d)                chain_append (instance_chainGetTemporary(), \
                                                                (t_perform)(fn), 4, \
                                                                (t_int)(a), \
                                                                (t_int)(b), \
                                                                (t_int)(c), \
                                                                (t_int)(d))

#define dsp_add5(fn, a, b, c, d, e)             chain_append (instance_chainGetTemporary(), \
                                                                (t_perform)(fn), 5, \
                                                                (t_int)(a), \
                                                                (t_int)(b), \
                                                                (t_int)(c), \
                                                                (t_int)(d), \
                                                                (t_int)(e))

#define dsp_add6(fn, a, b, c, d, e, f)          chain_append (instance_chainGetTemporary(), \
                                                                (t_perform)(fn), 6, \
                                                                (t_int)(a), \
                                                                (t_int)(b), \
                                                                (t_int)(c), \
                                                                (t_int)(d), \
                                                                (t_int)(e), \
                                                                (t_int)(f))

#define dsp_add7(fn, a, b, c, d, e, f, g)       chain_append (instance_chainGetTemporary(), \
                                                                (t_perform)(fn), 7, \
                                                                (t_int)(a), \
                                                                (t_int)(b), \
                                                                (t_int)(c), \
                                                                (t_int)(d), \
                                                                (t_int)(e), \
                                                                (t_int)(f), \
                                                                (t_int)(g))

#define dsp_add8(fn, a, b, c, d, e, f, g, h)    chain_append (instance_chainGetTemporary(), \
                                                                (t_perform)(fn), 8, \
                                                                (t_int)(a), \
                                                                (t_int)(b), \
                                                                (t_int)(c), \
                                                                (t_int)(d), \
                                                                (t_int)(e), \
                                                                (t_int)(f), \
                                                                (t_int)(g), \
                                                                (t_int)(h))
                                                                
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if PD_WITH_DEBUG

#define PD_MEMORY_GET(n)                leak_getMemoryChecked ((n), __FUNCTION__, __LINE__)
#define PD_MEMORY_RESIZE(ptr, m, n)     leak_getMemoryResizeChecked ((ptr), (m), (n), __FUNCTION__, __LINE__)
#define PD_MEMORY_FREE(ptr)             leak_freeMemoryChecked ((ptr), __FUNCTION__, __LINE__);

#else

#define PD_MEMORY_GET(n)                memory_get ((n))
#define PD_MEMORY_RESIZE(ptr, m, n)     memory_getResize ((ptr), (m), (n))
#define PD_MEMORY_FREE(ptr)             memory_free ((ptr))

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define PD_SHORT_FILE                   (strrchr (__FILE__, '/') ? strrchr (__FILE__, '/') + 1 : __FILE__)

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void post_log (const char *, ...);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if PD_WITH_DEBUG

    #define PD_BUG          PD_ASSERT (0)
    #define PD_ASSERT(x)    if (!(x)) { post_log ("*** Assert / %s / line %d", PD_SHORT_FILE, __LINE__); }

#else
    
    #define PD_BUG
    #define PD_ASSERT(x)

#endif // PD_WITH_DEBUG

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_APPLICATION )
    
    #define PD_TRY          try {
    #define PD_CATCH        } catch (...) { core::scheduler_needToExitWithError(); throw; }
    
#else
    
    #define PD_TRY
    #define PD_CATCH
    
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* < https://locklessinc.com/articles/vectorize/ > */
/* < https://www.gnu.org/software/libc/manual/html_node/Aligned-Memory-Blocks.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if PD_64BIT
    #define PD_MALLOC_ALIGNED       1           /* Assume malloc aligned to 16-bytes. */
#else
    #define PD_MALLOC_ALIGNED       0
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if PD_MALLOC_ALIGNED
#if PD_CLANG
    #if __has_builtin(__builtin_assume_aligned)
        #define PD_ASSUME_ALIGNED   1
    #else
        #define PD_ASSUME_ALIGNED   0
    #endif
#elif PD_GCC && PD_GCC_VERSION >= 40700
    #define PD_ASSUME_ALIGNED       1
#else
    #define PD_ASSUME_ALIGNED       0
#endif
#else
    #define PD_ASSUME_ALIGNED       0
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if PD_ASSUME_ALIGNED
    #define PD_RESTRICTED   t_sample* __restrict__
    #define PD_ALIGNED(x)   __builtin_assume_aligned((x), 16)
#else
    #define PD_RESTRICTED   t_sample*
    #define PD_ALIGNED(x)   (x)
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define PD_LIKELY(x)        __builtin_expect ((x), 1)
#define PD_UNLIKELY(x)      __builtin_expect ((x), 0)

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define PD_ATOMS_ALLOCA(x, n)  \
    (x) = (t_atom *)((n) < 64 ? alloca ((n) * sizeof (t_atom)) : PD_MEMORY_GET ((n) * sizeof (t_atom)))
        
#define PD_ATOMS_FREEA(x, n)   \
    if (n >= 64) { PD_MEMORY_FREE ((x)); }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define PD_UNUSED(x)        (void)(x)

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#define PD_ABORT(x)         if (PD_UNLIKELY (x)) { abort(); }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#define PD_TRANSLATE(s)     (s)

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define PD_SECONDS_TO_MILLISECONDS(n)       ((double)(n) * 1000.0)
#define PD_MILLISECONDS_TO_SECONDS(n)       ((double)(n) * 1e-3)
#define PD_SECONDS_TO_MICROSECONDS(n)       ((double)(n) * 1000000.0)
#define PD_MICROSECONDS_TO_SECONDS(n)       ((double)(n) * 1e-6)
#define PD_MILLISECONDS_TO_MICROSECONDS(n)  ((double)(n) * 1000.0)
#define PD_MICROSECONDS_TO_MILLISECONDS(n)  ((double)(n) * 1e-3)
#define PD_SECONDS_TO_NANOSECONDS(n)        ((double)(n) * 1000000000.0)
#define PD_NANOSECONDS_TO_SECONDS(n)        ((double)(n) * 1e-9)
#define PD_MILLISECONDS_TO_NANOSECONDS(n)   ((double)(n) * 1000000.0)
#define PD_NANOSECONDS_TO_MILLISECONDS(n)   ((double)(n) * 1e-6)

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define PD_MAX(a,b)                         ((a)>(b)?(a):(b))
#define PD_MIN(a,b)                         ((a)<(b)?(a):(b))

#define PD_ABS(a)                           ((a)<0?-(a):(a))
#define PD_CLAMP(u,a,b)                     ((u)<(a)?(a):(u)>(b)?(b):(u))

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < http://www.math-solutions.org/graphplotter.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define PD_HALF_PI                          1.5707963267948966192313216916398
#define PD_PI                               3.1415926535897932384626433832795
#define PD_TWO_PI                           6.283185307179586476925286766559
#define PD_LOG_TWO                          0.69314718055994530941723212145818
#define PD_LOG_TEN                          2.3025850929940456840179914546844
#define PD_E                                2.7182818284590452353602874713527

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_seed time_makeRandomSeed (void);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* < http://en.wikipedia.org/wiki/Linear_congruential_generator > */

#define PD_RAND48_SEED                      ((t_rand48)time_makeRandomSeed() & 0xffffffffffffULL)
#define PD_RAND48_INIT(s)                   ((s) = PD_RAND48_SEED)
#define PD_RAND48_NEXT(s)                   ((s) = (((s) * 0x5deece66dULL + 0xbULL) & 0xffffffffffffULL))
#define PD_RAND48_UINT32(s)                 (PD_RAND48_NEXT (s) >> 16)
#define PD_RAND48_DOUBLE(s)                 (PD_RAND48_UINT32 (s) * (1.0 / 4294967296.0))

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define pd_class(x)                         (*((t_pd *)(x)))

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define cast_pd(x)                          ((t_pd *)(x))
#define cast_glist(x)                       ((t_glist *)(x))
#define cast_object(x)                      ((t_object *)(x))

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define IS_NULL(atom)                       ((atom)->a_type == A_NULL)
#define IS_SEMICOLON(atom)                  ((atom)->a_type == A_SEMICOLON)
#define IS_COMMA(atom)                      ((atom)->a_type == A_COMMA)
#define IS_FLOAT(atom)                      ((atom)->a_type == A_FLOAT)
#define IS_SYMBOL(atom)                     ((atom)->a_type == A_SYMBOL)
#define IS_DOLLAR(atom)                     ((atom)->a_type == A_DOLLAR)
#define IS_DOLLARSYMBOL(atom)               ((atom)->a_type == A_DOLLARSYMBOL)
#define IS_VOID(atom)                       ((atom)->a_type == A_VOID)

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define IS_SEMICOLON_OR_COMMA(atom)         ((IS_SEMICOLON(atom)) || (IS_COMMA(atom)))

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define SET_NULL(atom)                      ((atom)->a_type = A_NULL)
#define SET_SEMICOLON(atom)                 ((atom)->a_type = A_SEMICOLON, (atom)->a_w.a_index = 0)
#define SET_COMMA(atom)                     ((atom)->a_type = A_COMMA, (atom)->a_w.a_index = 0)
#define SET_FLOAT(atom, f)                  ((atom)->a_type = A_FLOAT, (atom)->a_w.a_float = (f))
#define SET_SYMBOL(atom, s)                 ((atom)->a_type = A_SYMBOL, (atom)->a_w.a_symbol = (s))
#define SET_DOLLAR(atom, n)                 ((atom)->a_type = A_DOLLAR, (atom)->a_w.a_index = (n))
#define SET_DOLLARSYMBOL(atom, s)           ((atom)->a_type = A_DOLLARSYMBOL, (atom)->a_w.a_symbol = (s))
#define SET_VOID(atom, p)                   ((atom)->a_type = A_VOID, (atom)->a_w.a_void = (p))
#define SET_OBJECT(atom, p)                 SET_VOID (atom, (void *)(p))
#define SET_BUFFER(atom, p)                 SET_VOID (atom, (void *)(p))
#define SET_CLOCK(atom, p)                  SET_VOID (atom, (void *)(p))

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define GET_FLOAT(atom)                     ((atom)->a_w.a_float)
#define GET_SYMBOL(atom)                    ((atom)->a_w.a_symbol)
#define GET_DOLLAR(atom)                    ((atom)->a_w.a_index)
#define GET_VOID(atom)                      ((atom)->a_w.a_void)
#define GET_OBJECT(atom)                    (t_object *)GET_VOID (atom)
#define GET_BUFFER(atom)                    (t_buffer *)GET_VOID (atom)
#define GET_CLOCK(atom)                     (t_clock *)GET_VOID (atom)

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define ADDRESS_FLOAT(atom)                 &((atom)->a_w.a_float)
#define ADDRESS_SYMBOL(atom)                &((atom)->a_w.a_symbol)

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define class_addLoadbang(c, m)             class_addMethod ((c), (t_method)(m), sym__loadbang, A_NULL)
#define class_addClosebang(c, m)            class_addMethod ((c), (t_method)(m), sym__closebang, A_NULL)
#define class_addPolling(c, m)              class_addMethod ((c), (t_method)(m), sym__polling, A_NULL)
#define class_addAutorelease(c, m)          class_addMethod ((c), (t_method)(m), sym__autorelease, A_NULL)

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define inlet_new2(x, type)                 inlet_new (cast_object ((x)), cast_pd ((x)), (type), sym__inlet2)
#define inlet_new3(x, type)                 inlet_new (cast_object ((x)), cast_pd ((x)), (type), sym__inlet3)

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define PD_INT_MAX                          0x7fffffff
#define PD_FLT_MAX                          DBL_MAX
#define PD_EPSILON                          1E-9

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define PD_IS_POWER_2(v)                    (!((v) & ((v) - 1)))
#define PD_NEXT_POWER_2(v)                  sys_nextPowerOfTwo ((uint64_t)(v))
#define PD_TO_RADIANS(degrees)              ((PD_PI * (degrees)) / 180.0)
#define PD_IS_ALIGNED_16(p)                 (((unsigned long)(p) & 0xfUL) == 0)

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Notice that it returns zero with an argument of zero. */

static inline uint64_t sys_nextPowerOfTwo (uint64_t v)
{
    v--;
    v |= (v >> 1);
    v |= (v >> 2);
    v |= (v >> 4);
    v |= (v >> 8);
    v |= (v >> 16);
    v |= (v >> 32);
    v++;
    
    return v;
}

static inline int sys_ilog2 (uint32_t v)
{
    int k = -1; while (v) { v >>= 1; k++; } return k;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // M_MACROS_H_
