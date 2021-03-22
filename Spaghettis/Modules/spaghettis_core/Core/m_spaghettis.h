
#ifndef M_SPAGHETTIS_H_
#define M_SPAGHETTIS_H_

#if 0   // Python script.

""" "

#endif  //

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( __linux__ )
    #define     PD_LINUX            1
#elif defined ( __APPLE__ )
    #define     PD_APPLE            1
#else
    #error "Unsupported platform!"
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( __clang__ )
    #define     PD_CLANG            1
#elif defined ( __GNUC__ )
    #define     PD_GCC              1
#else
  #error "Unsupported compiler!"
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( __LP64__ ) || defined ( _LP64 ) || defined ( __arm64__ )
    #define PD_64BIT                1

#else
    #define PD_32BIT                1
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( __arm__ ) || defined ( __arm64__ )
    #define PD_ARM                  1
#elif defined ( __i386__ ) || defined ( __x86_64__ )
    #define PD_INTEL                1
#else
    #error "Unsupported processor!"
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* < https://stackoverflow.com/a/27054190 > */

#if defined ( __BIG_ENDIAN__ )
    #define PD_BIG_ENDIAN           1
#elif defined ( __LITTLE_ENDIAN__ )
    #define PD_LITTLE_ENDIAN        1
#else

#include <endian.h>

#if defined ( __BYTE_ORDER ) && ( __BYTE_ORDER == __BIG_ENDIAN ) || \
    defined ( __BIG_ENDIAN__ ) || \
    defined ( __ARMEB__ ) || \
    defined ( __THUMBEB__ ) || \
    defined ( __AARCH64EB__ ) || \
    defined ( _MIBSEB ) || defined ( __MIBSEB ) || defined ( __MIBSEB__ )

    #define PD_BIG_ENDIAN           1
    
#elif defined ( __BYTE_ORDER ) && ( __BYTE_ORDER == __LITTLE_ENDIAN ) || \
    defined ( __LITTLE_ENDIAN__ ) || \
    defined ( __ARMEL__ ) || \
    defined ( __THUMBEL__ ) || \
    defined ( __AARCH64EL__ ) || \
    defined ( _MIPSEL ) || defined ( __MIPSEL ) || defined ( __MIPSEL__ )

    #define PD_LITTLE_ENDIAN        1
    
#endif
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#ifdef PD_LITTLE_ENDIAN
#else
#ifdef PD_BIG_ENDIAN
#else
    #error "Unknown endianness!"
#endif
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Check POSIX atomic and time availability. */

#if PD_APPLE

#include "Availability.h"

#if __MAC_OS_X_VERSION_MAX_ALLOWED >= 101200
#else
    #error "Unsupported platform!"
#endif

#endif // PD_APPLE

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if PD_GCC
    #define PD_GCC_VERSION          (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define PD_NAME                     "Spaghettis"
#define PD_NAME_LOWERCASE           "spaghettis"

#define PD_VERSION                  "0.9"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#define PD_VERSION_MAJOR            0
#define PD_VERSION_MINOR            9
#define PD_VERSION_PATCH            0

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define PD_PATCH                    ".pdpatch"
#define PD_HELP                     ".pdhelp"
#define PD_PLUGIN                   ".pdobject"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define PD_DLL                      __attribute__((visibility ("default")))

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( __cplusplus )

#define PD_STUB                     extern "C" __attribute__((visibility ("default")))

#else

#define PD_STUB                     __attribute__((visibility ("default")))

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_APPLICATION ) || defined ( PD_BUILDING_TERMINAL ) || defined ( PD_BUILDING_TESTS )

#define PD_BUILDING_PLUGIN          0
    
#else

#define PD_BUILDING_PLUGIN          1

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if PD_BUILDING_PLUGIN              /* Avoid namespace pollution. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include <stdint.h>
#include <stdlib.h>

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#else

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "m_includes.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef PD_WITH_DEBUG
#define PD_WITH_DEBUG               0                   /* Debug mode. */
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#ifndef PD_WITH_BELLE
#define PD_WITH_BELLE               0
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif                              /* Avoid namespace pollution. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define PD_STRING                   1024                /* Maximum size for a string. */
#define PD_ARGUMENTS                2                   /* Maximum number of typechecked arguments. */
                                                        /* Use A_GIMME when more are requiered. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define PD_ERROR                    1
#define PD_ERROR_NONE               0

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if defined ( PD_BUILDING_APPLICATION )

namespace spaghettis { namespace core {

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef intptr_t                    t_int;
typedef double                      t_float;
typedef float                       t_sample;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef int                         t_error;
typedef double                      t_systime;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef uint64_t                    t_rand48;
typedef uint64_t                    t_seed;
typedef uint64_t                    t_id;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Alignment may not work on stack (don't use local atomic variables). */

/* < http://gcc.gnu.org/bugzilla/show_bug.cgi?id=16660 > */
/* < http://stackoverflow.com/questions/841433/gcc-attribute-alignedx-explanation > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef int32_t     __attribute__ ((__aligned__ (4)))   t_int32Atomic;
typedef uint32_t    __attribute__ ((__aligned__ (4)))   t_uint32Atomic;
typedef uint64_t    __attribute__ ((__aligned__ (8)))   t_uint64Atomic;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if PD_64BIT
    typedef void*   __attribute__ ((__aligned__ (8)))   t_pointerAtomic;
#else
    typedef void*   __attribute__ ((__aligned__ (4)))   t_pointerAtomic;
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef double      __attribute__ ((__aligned__ (8)))   t_float64Atomic;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef int t_atomtype;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

enum {
    CLASS_DEFAULT       = 0,
    CLASS_ABSTRACT      = 1,
    CLASS_INVISIBLE     = 2,
    CLASS_BOX           = 3,
    CLASS_NOINLET       = 8,
    CLASS_SIGNAL        = 16
    };
    
enum {
    A_NULL              = 0,
    A_FLOAT             = 1,
    A_SYMBOL,
    A_SEMICOLON,
    A_COMMA,
    A_DEFFLOAT,
    A_DEFSYMBOL,
    A_DOLLAR,
    A_DOLLARSYMBOL,
    A_GIMME,
    A_VOID,
    A_CANT,
    };

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

struct _abstractions;
struct _clocks;
struct _dspcontext;
struct _dspthread;
struct _garray;
struct _gatom;
struct _outconnect;
struct _receiver;
struct _ringbuffer;
struct _sfthread;
struct _vinlet;
struct _voutlet;

struct _chain;
struct _class;
struct _clock;
struct _glist;
struct _inlet;
struct _outlet;
struct _space;

#define t_abstractions              struct _abstractions
#define t_clocks                    struct _clocks
#define t_dspcontext                struct _dspcontext
#define t_dspthread                 struct _dspthread
#define t_garray                    struct _garray
#define t_gatom                     struct _gatom
#define t_outconnect                struct _outconnect
#define t_receiver                  struct _receiver
#define t_ringbuffer                struct _ringbuffer
#define t_sfthread                  struct _sfthread
#define t_vinlet                    struct _vinlet
#define t_voutlet                   struct _voutlet

#define t_chain                     struct _chain
#define t_class                     struct _class
#define t_clock                     struct _clock
#define t_glist                     struct _glist
#define t_inlet                     struct _inlet
#define t_outlet                    struct _outlet
#define t_space                     struct _space

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef t_class *t_pd;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _symbol {
    const char      *s_name;
    t_pd            *s_thing;
    struct _symbol  *s_next;
    } t_symbol;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _atom {
    t_atomtype      a_type;
    union {
        t_float     a_float;
        int         a_index;
        t_symbol    *a_symbol;
        void        *a_void;
    } a_w;
    } t_atom;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _buffer {
    int             b_allocated;
    int             b_size;
    t_atom          *b_vector;
    } t_buffer;
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef enum {
    TYPE_COMMENT    = 0,
    TYPE_OBJECT     = 1,
    TYPE_MESSAGE,
    TYPE_ATOM
    } t_objecttype;

typedef struct _object {
    t_pd            g_pd;                           /* MUST be the first. */
    t_id            g_id[3];
    t_float64Atomic g_f;
    int             g_x;
    int             g_y;
    int             g_width;                        /* Zero for undefined. */
    int             g_selected;
    t_objecttype    g_type;
    t_buffer        *g_buffer;
    t_inlet         *g_inlets;
    t_outlet        *g_outlets;
    struct _object  *g_next;
    } t_object;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Use the form (*)(void) to avoid warnings with the Wcast-function-type GCC flag. */

typedef void        (*t_method)         (void);     /* Generic function. */
typedef void        (*t_newmethod)      (void);     /* Generic function that return a pointer. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef t_int       *(*t_perform)       (t_int *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef t_buffer    *(*t_datafn)        (t_object *, int);
typedef void        (*t_dismissfn)      (t_object *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef void        (*t_initializerfn)  (void *dest, void *src);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _signal {
    t_float             s_sampleRate;
    int                 s_vectorSize;
    int                 s_overlap;
    int                 s_hasBorrowed;
    t_sample            *s_vector;
    t_sample            *s_unused;
    struct _signal      *s_next;
    } t_signal;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _initializer {
    struct _initializer *s_next;
    void                *s_lhs;
    void                *s_rhs;
    t_initializerfn     s_fn;
    } t_initializer;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if defined ( __cplusplus )

extern "C" {

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Note that NONE of those functions are considered thread-safe. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_DLL t_symbol *gensym                         (const char *s);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Use the macros instead. */

PD_DLL void     *memory_getForExternal          (size_t n);
PD_DLL void     *memory_getResizeForExternal    (void *ptr, size_t oldSize, size_t newSize);

PD_DLL void     memory_freeForExternal          (void *ptr);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_DLL t_pd     *pd_new                         (t_class *c);

PD_DLL void     pd_free                         (t_pd *x);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PD_DLL void     pd_bind                         (t_pd *x, t_symbol *s);
PD_DLL void     pd_unbind                       (t_pd *x, t_symbol *s);
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PD_DLL void     pd_bang                         (t_pd *x);
PD_DLL void     pd_float                        (t_pd *x, t_float f);
PD_DLL void     pd_symbol                       (t_pd *x, t_symbol *s);
PD_DLL void     pd_list                         (t_pd *x, int argc, t_atom *argv);
PD_DLL void     pd_message                      (t_pd *x, t_symbol *s, int argc, t_atom *argv);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_DLL t_pd     *symbol_getThing                (t_symbol *s);
PD_DLL t_pd     *symbol_getThingByClass         (t_symbol *s, t_class *c);

PD_DLL const char *symbol_getName               (t_symbol *s);

PD_DLL int      symbol_hasThing                 (t_symbol *s);
PD_DLL int      symbol_hasThingQuiet            (t_symbol *s);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_DLL t_class  *class_new                      (t_symbol *name,
                                                    t_newmethod newMethod,
                                                    t_method freeMethod,
                                                    size_t size,
                                                    int flags,
                                                    t_atomtype type1, ...);

PD_DLL void     class_addCreator                (t_newmethod newMethod, t_symbol *s, t_atomtype type1, ...);

PD_DLL void     class_addMethod                 (t_class *c, t_method fn, t_symbol *s, t_atomtype type1, ...);
PD_DLL void     class_free                      (t_class *c);

PD_DLL void     class_addBang                   (t_class *c, t_method fn);
PD_DLL void     class_addFloat                  (t_class *c, t_method fn);
PD_DLL void     class_addSymbol                 (t_class *c, t_method fn);
PD_DLL void     class_addList                   (t_class *c, t_method fn);
PD_DLL void     class_addAnything               (t_class *c, t_method fn);

PD_DLL void     class_setHelpName               (t_class *c, t_symbol *s);
PD_DLL void     class_setHelpDirectory          (t_class *c, t_symbol *s);

PD_DLL void     class_setDataFunction           (t_class *c, t_datafn f);
PD_DLL void     class_setDismissFunction        (t_class *c, t_dismissfn f);

PD_DLL void     class_requirePending            (t_class *c);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_DLL int      object_isUndoOrEncaspulate      (t_object *x, int flags);
PD_DLL int      object_dspNeedInitializer       (t_object *x);

PD_DLL void     object_getSignalValues          (t_object *x, t_buffer *b);
PD_DLL void     object_copySignalValues         (t_object *x, t_object *old);

PD_DLL void     object_fetchAndCopySignalValuesIfRequired   (t_object *x);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_DLL t_inlet  *inlet_newFloat                 (t_object *owner, t_float *fp);
PD_DLL t_inlet  *inlet_newSymbol                (t_object *owner, t_symbol **sp);
PD_DLL t_inlet  *inlet_newSignal                (t_object *owner);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_DLL t_outlet *outlet_newBang                 (t_object *owner);
PD_DLL t_outlet *outlet_newFloat                (t_object *owner);
PD_DLL t_outlet *outlet_newSymbol               (t_object *owner);
PD_DLL t_outlet *outlet_newList                 (t_object *owner);
PD_DLL t_outlet *outlet_newAnything             (t_object *owner);
PD_DLL t_outlet *outlet_newMixed                (t_object *owner);
PD_DLL t_outlet *outlet_newSignal               (t_object *owner);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_DLL void     outlet_bang                     (t_outlet *x);
PD_DLL void     outlet_float                    (t_outlet *x, t_float f);
PD_DLL void     outlet_symbol                   (t_outlet *x, t_symbol *s);
PD_DLL void     outlet_list                     (t_outlet *x, int argc, t_atom *argv);
PD_DLL void     outlet_anything                 (t_outlet *x, t_symbol *s, int argc, t_atom *argv);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_DLL void     atom_setSymbol                  (t_atom *a, t_symbol *s);
PD_DLL void     atom_setFloat                   (t_atom *a, t_float f);

PD_DLL t_float  atom_getFloat                   (t_atom *a);
PD_DLL t_float  atom_getFloatAtIndex            (int n, int argc, t_atom *argv);

PD_DLL t_symbol *atom_getSymbol                 (t_atom *a);
PD_DLL t_symbol *atom_getSymbolAtIndex          (int n, int argc, t_atom *argv);

PD_DLL t_symbol *atom_getSymbolOrDollarSymbol           (t_atom *a);
PD_DLL t_symbol *atom_getSymbolOrDollarSymbolAtIndex    (int n, int argc, t_atom *argv);

PD_DLL char     *atom_atomsToString             (int argc, t_atom *argv);   /* Caller acquires ownership. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_DLL t_buffer *buffer_new                     (void);
PD_DLL t_buffer *buffer_newCopy                 (t_buffer *toCopy);
PD_DLL t_atom   *buffer_getAtoms                (t_buffer *x);
PD_DLL t_atom   *buffer_getAtomAtIndex          (t_buffer *x, int n);
PD_DLL t_atom   *buffer_getAtomAtIndexChecked   (t_buffer *x, int n);

PD_DLL int      buffer_getSize                  (t_buffer *x);
PD_DLL void     buffer_free                     (t_buffer *x);
PD_DLL void     buffer_clear                    (t_buffer *x);
PD_DLL void     buffer_shuffle                  (t_buffer *x);

PD_DLL void     buffer_reserve                  (t_buffer *x, int n);
PD_DLL void     buffer_resize                   (t_buffer *x, int n);
PD_DLL t_error  buffer_extend                   (t_buffer *x, int start, int end, int n);
PD_DLL t_error  buffer_replace                  (t_buffer *x, int start, int end, int argc, t_atom *argv);
PD_DLL t_error  buffer_getAtIndex               (t_buffer *x, int n, t_atom *a);
PD_DLL t_error  buffer_setAtIndex               (t_buffer *x, int n, t_atom *a);
PD_DLL t_error  buffer_insertAtIndex            (t_buffer *x, int n, t_atom *a);
PD_DLL t_error  buffer_pop                      (t_buffer *x, t_atom *a);

PD_DLL t_error  buffer_setFloatAtIndex          (t_buffer *x, int n, t_float f);
PD_DLL t_error  buffer_setSymbolAtIndex         (t_buffer *x, int n, t_symbol *s);

PD_DLL t_float  buffer_getFloatAtIndex          (t_buffer *x, int n);
PD_DLL t_symbol *buffer_getSymbolAtIndex        (t_buffer *x, int n);

PD_DLL void     buffer_prepend                  (t_buffer *x, int argc, t_atom *argv);
PD_DLL void     buffer_append                   (t_buffer *x, int argc, t_atom *argv);
PD_DLL void     buffer_appendAtom               (t_buffer *x, t_atom *a);
PD_DLL void     buffer_appendBuffer             (t_buffer *x, t_buffer *y);
PD_DLL void     buffer_appendFloat              (t_buffer *x, t_float f);
PD_DLL void     buffer_appendSymbol             (t_buffer *x, t_symbol *s);
PD_DLL void     buffer_appendDollar             (t_buffer *x, int n);
PD_DLL void     buffer_appendDollarSymbol       (t_buffer *x, t_symbol *s);
PD_DLL void     buffer_appendSemicolon          (t_buffer *x);
PD_DLL void     buffer_appendComma              (t_buffer *x);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Notice that unset and delay a clock MUST be done only in one (and always the same) thread. */
/* It can be inside the DSP perform. */
/* The new and free functions must be called NON-CONCURRENTLY in the main thread. */

PD_DLL t_clock  *clock_new                      (void *owner, t_method fn);

PD_DLL void     clock_free                      (t_clock *x);
PD_DLL void     clock_unset                     (t_clock *x);                   /* Usable in DSP. */
PD_DLL void     clock_delay                     (t_clock *x, double delay);     /* Usable in DSP. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_DLL void     post                            (const char *fmt, ...);
PD_DLL void     post_warning                    (const char *fmt, ...);
PD_DLL void     post_error                      (const char *fmt, ...);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_DLL t_glist  *instance_contextGetCurrent     (void);
PD_DLL t_chain  *instance_chainGetTemporary     (void);
PD_DLL t_object *instance_objectGetTemporary    (t_object *x);
PD_DLL t_space  *instance_objectGetNewSpace     (t_object *x);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_DLL t_sample *signal_getVector               (t_signal *s);

PD_DLL t_float  signal_getSampleRate            (t_signal *s);
PD_DLL int      signal_getVectorSize            (t_signal *s);
PD_DLL int      signal_getOverlap               (t_signal *s);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_DLL void     chain_append                    (t_chain *x, t_perform f, int n, ...);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_DLL t_initializer    *initializer_new        (t_initializerfn fn, void *lhs, void *rhs);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_DLL t_float  space_getFloat0                 (t_space *space);
PD_DLL t_float  space_getFloat1                 (t_space *space);
PD_DLL t_float  space_getFloat2                 (t_space *space);
PD_DLL t_float  space_getFloat3                 (t_space *space);
PD_DLL t_float  space_getFloat4                 (t_space *space);
PD_DLL t_float  space_getFloat5                 (t_space *space);
PD_DLL t_float  space_getFloat6                 (t_space *space);
PD_DLL t_float  space_getFloat7                 (t_space *space);

PD_DLL void     space_setFloat0                 (t_space *space, t_float f);
PD_DLL void     space_setFloat1                 (t_space *space, t_float f);
PD_DLL void     space_setFloat2                 (t_space *space, t_float f);
PD_DLL void     space_setFloat3                 (t_space *space, t_float f);
PD_DLL void     space_setFloat4                 (t_space *space, t_float f);
PD_DLL void     space_setFloat5                 (t_space *space, t_float f);
PD_DLL void     space_setFloat6                 (t_space *space, t_float f);
PD_DLL void     space_setFloat7                 (t_space *space, t_float f);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if defined ( __cplusplus )

}

#endif


// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if defined ( PD_BUILDING_APPLICATION )

} }

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_APPLICATION )

#if PD_WITH_DEBUG

#define PD_MEMORY_GET(n)                leak_getMemoryChecked ((n), __FUNCTION__, __LINE__)
#define PD_MEMORY_RESIZE(ptr, m, n)     leak_getMemoryResizeChecked ((ptr), (m), (n), __FUNCTION__, __LINE__)
#define PD_MEMORY_FREE(ptr)             leak_freeMemoryChecked ((ptr), __FUNCTION__, __LINE__);

#else

#define PD_MEMORY_GET(n)                memory_get ((n))
#define PD_MEMORY_RESIZE(ptr, m, n)     memory_getResize ((ptr), (m), (n))
#define PD_MEMORY_FREE(ptr)             memory_free ((ptr))

#endif

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_TERMINAL )

#if PD_WITH_DEBUG

#define PD_MEMORY_GET(n)                leak_getMemoryChecked ((n), __FUNCTION__, __LINE__)
#define PD_MEMORY_RESIZE(ptr, m, n)     leak_getMemoryResizeChecked ((ptr), (m), (n), __FUNCTION__, __LINE__)
#define PD_MEMORY_FREE(ptr)             leak_freeMemoryChecked ((ptr), __FUNCTION__, __LINE__);

#else

#define PD_MEMORY_GET(n)                memory_get ((n))
#define PD_MEMORY_RESIZE(ptr, m, n)     memory_getResize ((ptr), (m), (n))
#define PD_MEMORY_FREE(ptr)             memory_free ((ptr))

#endif

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_TESTS )

#define PD_MEMORY_GET(n)                memory_get ((n))
#define PD_MEMORY_RESIZE(ptr, m, n)     memory_getResize ((ptr), (m), (n))
#define PD_MEMORY_FREE(ptr)             memory_free ((ptr))

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if PD_BUILDING_PLUGIN

#define PD_MEMORY_GET(n)                memory_getForExternal ((n))
#define PD_MEMORY_RESIZE(ptr, m, n)     memory_getResizeForExternal ((ptr), (m), (n))
#define PD_MEMORY_FREE(ptr)             memory_freeForExternal ((ptr))

#endif

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

#if PD_BUILDING_PLUGIN

#define class_addDSP(c, m) class_addMethod ((c), (t_method)(m), gensym ("_dsp"), A_CANT, A_NULL)

#else

#define class_addDSP(c, m) class_addMethod ((c), (t_method)(m), sym__dsp, A_CANT, A_NULL)

#endif // PD_BUILDING_PLUGIN

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

// ====================================

#if 0

" """

import sys
import json
import os
import subprocess

assert sys.version_info >= (3, 4)

from pathlib import Path

manifest = { }
options  = { }

def manifestParse(path):
    global manifest
    manifest["source"]       = str(path)
    manifest["visibility"]   = "-fvisibility=hidden"
    manifest["product"]      = str(path.with_suffix(".pdobject"))

    if path.match('*.cpp'):
        manifest["compiler"] = "g++"
    else:
        manifest["compiler"] = "gcc"
    if sys.platform.startswith('linux'):
        manifest["plugin"]   = "-shared -fpic"
    elif sys.platform.startswith('darwin'):
        manifest["plugin"]   = "-bundle -undefined dynamic_lookup -bind_at_load"

def manifestParseJSON(path):
    if path.exists():
        with path.open() as f:
            global options
            try:
                options = json.load(f)
            except:
                print("Invalid JSON file: " + str(path))

def buildReport():
    global manifest
    object = Path(manifest["product"])
    ret = subprocess.run(["file", str(object)], stdout=subprocess.PIPE)
    if any(x in str(ret.stdout) for x in ['x86_64', '64-bit']):
        print ('64-bit' + ' - ' + str(object))
    elif any(x in str(ret.stdout) for x in ['i386', '32-bit']):
        print ('32-bit' + ' - ' + str(object))

def buildPlugin():
    global manifest
    command = []
    command.append(manifest["compiler"])
    command.append("-I" + manifest["spaghettis"])
    for v in options.values():
        command.append(v)
    command.append(manifest["plugin"])
    command.append(manifest["visibility"])
    command.append(manifest["source"])
    command.append("-o")
    command.append(manifest["product"])
    os.system(' '.join(command))

if __name__ == "__main__":
    header = Path(sys.argv.pop(0))
    manifest["spaghettis"] = str(header.resolve().parent)
    for file in sys.argv:
        options.clear()
        p = Path(file).resolve()
        if p.exists():
            manifestParse(p)
            o = p.with_suffix('.json')
            manifestParseJSON(o)
            buildPlugin()
            buildReport()
        else:
            print("No such file: " + file)

#endif  // Python script.

#endif  // M_SPAGHETTIS_H_
