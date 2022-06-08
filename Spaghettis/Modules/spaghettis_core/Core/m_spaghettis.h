
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

#ifndef PD_WITH_DEBUG
#define PD_WITH_DEBUG               0                   /* Debug mode. */
#endif

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

#if defined ( PD_BUILDING_TESTS )
#if PD_WITH_DEBUG
    #error
#endif
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_APPLICATION ) || defined ( PD_BUILDING_TERMINAL ) || defined ( PD_BUILDING_TESTS )

#else

#define PD_BUILDING_PLUGIN          1

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if defined ( PD_BUILDING_PLUGIN )

#include <stdint.h>
#include <stdlib.h>

#else

#include "m_includes.h"

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if defined ( PD_BUILDING_APPLICATION )

namespace spaghettis::core {

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
    int             g_selected;
    t_objecttype    g_type;
    t_symbol        *g_label;
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
// MARK: -

/* Notice that unset and delay a clock MUST be done only in one (and always the same) thread. */
/* It can be inside the DSP perform. */
/* The new and free functions must be called NON-CONCURRENTLY in the main thread. */

PD_DLL t_clock      *spaghettis_clockNew                (void *x, t_method fn);

PD_DLL void         spaghettis_clockFree                (t_clock *x);
PD_DLL void         spaghettis_clockUnset               (t_clock *x);                   /* Usable in DSP. */
PD_DLL void         spaghettis_clockDelay               (t_clock *x, double delay);     /* Usable in DSP. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Note that NONE of those functions are considered thread-safe. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PD_DLL t_class      *spaghettis_classNew                (t_symbol *name,
                                                            t_newmethod fnNew,
                                                            t_method fnFree,
                                                            size_t size,
                                                            int flags);

PD_DLL t_class      *spaghettis_classNewWithArguments   (t_symbol *name,
                                                            t_newmethod fnNew,
                                                            t_method fnFree,
                                                            size_t size,
                                                            int flags);
                                                            
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_DLL void         spaghettis_classFree                (t_class *c);

PD_DLL void         spaghettis_classAddMethod               (t_class *c, t_method fn, t_symbol *s);
PD_DLL void         spaghettis_classAddMethodWithArguments  (t_class *c, t_method fn, t_symbol *s);

PD_DLL void         spaghettis_classAddBang             (t_class *c, t_method fn);
PD_DLL void         spaghettis_classAddFloat            (t_class *c, t_method fn);
PD_DLL void         spaghettis_classAddSymbol           (t_class *c, t_method fn);
PD_DLL void         spaghettis_classAddList             (t_class *c, t_method fn);
PD_DLL void         spaghettis_classAddAnything         (t_class *c, t_method fn);
PD_DLL void         spaghettis_classAddDsp              (t_class *c, t_method fn);

PD_DLL void         spaghettis_classSetHelpDirectory    (t_class *c, t_symbol *s);

PD_DLL void         spaghettis_classSetDataFunction     (t_class *c, t_datafn f);
PD_DLL void         spaghettis_classSetDismissFunction  (t_class *c, t_dismissfn f);

PD_DLL void         spaghettis_classRequirePending      (t_class *c);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_DLL void         spaghettis_post                     (t_object *x, const char *s);
PD_DLL void         spaghettis_postWarning              (t_object *x, const char *s);
PD_DLL void         spaghettis_postError                (t_object *x, const char *s);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_DLL void         spaghettis_bind                     (t_pd *x, t_symbol *s);
PD_DLL void         spaghettis_unbind                   (t_pd *x, t_symbol *s);
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_DLL void         spaghettis_handleBang               (t_pd *x);
PD_DLL void         spaghettis_handleFloat              (t_pd *x, t_float f);
PD_DLL void         spaghettis_handleSymbol             (t_pd *x, t_symbol *s);
PD_DLL void         spaghettis_handleList               (t_pd *x, int argc, t_atom *argv);
PD_DLL void         spaghettis_handleMessage            (t_pd *x, t_symbol *s, int argc, t_atom *argv);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_DLL t_symbol     *spaghettis_symbol                  (const char *s);

PD_DLL const char   *spaghettis_symbolGetName           (t_symbol *s);
PD_DLL t_pd         *spaghettis_symbolGetThing          (t_symbol *s);

PD_DLL int          spaghettis_symbolHasThing           (t_symbol *s);
PD_DLL int          spaghettis_symbolHasThingQuiet      (t_symbol *s);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_DLL t_symbol     *spaghettis_getRestoreSymbol        (void);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_DLL void         *spaghettis_memoryGet               (size_t n);
PD_DLL void         *spaghettis_memoryResize            (void *m, size_t oldSize, size_t newSize);

PD_DLL void         spaghettis_memoryFree               (void *m);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_DLL t_object     *spaghettis_objectNew               (t_class *c);

PD_DLL void         spaghettis_objectFree               (t_object *x);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_DLL t_object     *spaghettis_objectGetTemporary      (t_object *x);
PD_DLL t_space      *spaghettis_objectGetNewSpace       (t_object *x);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PD_DLL int          spaghettis_objectDspNeedInitializer                     (t_object *x);
PD_DLL void         spaghettis_objectGetSignalValues                        (t_object *x, t_buffer *b);
PD_DLL void         spaghettis_objectCopySignalValues                       (t_object *x, t_object *old);
PD_DLL void         spaghettis_objectFetchAndCopySignalValuesIfRequired     (t_object *x);
PD_DLL int          spaghettis_objectFlagIsUndoOrEncaspulate                (t_object *x, int flags);


// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_DLL t_inlet      *spaghettis_objectInletNewFloat     (t_object *x, t_float *fp);
PD_DLL t_inlet      *spaghettis_objectInletNewSymbol    (t_object *x, t_symbol **sp);
PD_DLL t_inlet      *spaghettis_objectInletNewSignal    (t_object *x);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_DLL t_outlet     *spaghettis_objectOutletNewBang     (t_object *x);
PD_DLL t_outlet     *spaghettis_objectOutletNewFloat    (t_object *x);
PD_DLL t_outlet     *spaghettis_objectOutletNewSymbol   (t_object *x);
PD_DLL t_outlet     *spaghettis_objectOutletNewList     (t_object *x);
PD_DLL t_outlet     *spaghettis_objectOutletNewAnything (t_object *x);
PD_DLL t_outlet     *spaghettis_objectOutletNewMixed    (t_object *x);
PD_DLL t_outlet     *spaghettis_objectOutletNewSignal   (t_object *x);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_DLL void         spaghettis_outletBang               (t_outlet *x);
PD_DLL void         spaghettis_outletFloat              (t_outlet *x, t_float f);
PD_DLL void         spaghettis_outletSymbol             (t_outlet *x, t_symbol *s);
PD_DLL void         spaghettis_outletList               (t_outlet *x, int argc, t_atom *argv);
PD_DLL void         spaghettis_outletAnything           (t_outlet *x, t_symbol *s, int argc, t_atom *argv);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_DLL void spaghettis_dspAdd1  (t_perform fn, t_int a);
PD_DLL void spaghettis_dspAdd2  (t_perform fn, t_int a, t_int b);
PD_DLL void spaghettis_dspAdd3  (t_perform fn, t_int a, t_int b, t_int c);
PD_DLL void spaghettis_dspAdd4  (t_perform fn, t_int a, t_int b, t_int c, t_int d);
PD_DLL void spaghettis_dspAdd5  (t_perform fn, t_int a, t_int b, t_int c, t_int d, t_int e);
PD_DLL void spaghettis_dspAdd6  (t_perform fn, t_int a, t_int b, t_int c, t_int d, t_int e, t_int f);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_DLL t_sample     *spaghettis_signalGetVector         (t_signal *s);

PD_DLL t_float      spaghettis_signalGetSampleRate      (t_signal *s);
PD_DLL int          spaghettis_signalGetVectorSize      (t_signal *s);
PD_DLL int          spaghettis_signalGetOverlap         (t_signal *s);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_DLL t_buffer     *spaghettis_bufferNew               (void);
PD_DLL t_atom       *spaghettis_bufferGetAtoms          (t_buffer *x);

PD_DLL void         spaghettis_bufferFree               (t_buffer *x);
PD_DLL int          spaghettis_bufferGetSize            (t_buffer *x);
PD_DLL void         spaghettis_bufferClear              (t_buffer *x);

PD_DLL void         spaghettis_bufferAppend             (t_buffer *x, int argc, t_atom *argv);
PD_DLL void         spaghettis_bufferAppendFloat        (t_buffer *x, t_float f);
PD_DLL void         spaghettis_bufferAppendSymbol       (t_buffer *x, t_symbol *s);
PD_DLL void         spaghettis_bufferAppendSemicolon    (t_buffer *x);
PD_DLL void         spaghettis_bufferAppendComma        (t_buffer *x);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_DLL char         *spaghettis_atomsToString   (int argc, t_atom *argv);   /* Caller acquires ownership. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_DLL void         spaghettis_atomSetSymbol            (t_atom *a, t_symbol *s);
PD_DLL void         spaghettis_atomSetFloat             (t_atom *a, t_float f);

PD_DLL t_float      spaghettis_atomGetFloat             (t_atom *a);
PD_DLL t_float      spaghettis_atomGetFloatAtIndex      (int n, int argc, t_atom *argv);

PD_DLL t_symbol     *spaghettis_atomGetSymbol           (t_atom *a);
PD_DLL t_symbol     *spaghettis_atomGetSymbolAtIndex    (int n, int argc, t_atom *argv);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_DLL t_initializer    *spaghettis_initializerNew      (t_initializerfn fn, void *lhs, void *rhs);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_DLL t_float      spaghettis_spaceGetFloat0           (t_space *space);
PD_DLL t_float      spaghettis_spaceGetFloat1           (t_space *space);
PD_DLL t_float      spaghettis_spaceGetFloat2           (t_space *space);
PD_DLL t_float      spaghettis_spaceGetFloat3           (t_space *space);
PD_DLL t_float      spaghettis_spaceGetFloat4           (t_space *space);
PD_DLL t_float      spaghettis_spaceGetFloat5           (t_space *space);
PD_DLL t_float      spaghettis_spaceGetFloat6           (t_space *space);
PD_DLL t_float      spaghettis_spaceGetFloat7           (t_space *space);

PD_DLL void         spaghettis_spaceSetFloat0           (t_space *space, t_float f);
PD_DLL void         spaghettis_spaceSetFloat1           (t_space *space, t_float f);
PD_DLL void         spaghettis_spaceSetFloat2           (t_space *space, t_float f);
PD_DLL void         spaghettis_spaceSetFloat3           (t_space *space, t_float f);
PD_DLL void         spaghettis_spaceSetFloat4           (t_space *space, t_float f);
PD_DLL void         spaghettis_spaceSetFloat5           (t_space *space, t_float f);
PD_DLL void         spaghettis_spaceSetFloat6           (t_space *space, t_float f);
PD_DLL void         spaghettis_spaceSetFloat7           (t_space *space, t_float f);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if defined ( __cplusplus )

}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if defined ( PD_BUILDING_APPLICATION )

}

#endif

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
