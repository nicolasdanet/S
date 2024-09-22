
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "d_soundfile.h"
#include "d_sfthread.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

struct _sfthread {
    t_pd                sft_pd;                /* Must be the first. */
    t_audioproperties   sft_properties;
    t_id                sft_owner;
    int                 sft_type;
    int                 sft_fileDescriptor;
    int                 sft_remainsToRead;
    int                 sft_alreadyWritten;
    int                 sft_maximumToWrite;
    t_error             sft_error;
    pthread_t           sft_thread;
    t_int32Atomic       sft_flag;
    t_int32Atomic       sft_corrupted;
    t_ring              *sft_ring;
    };

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define SFTHREAD_QUIT           1
#define SFTHREAD_CORRUPTED      1

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define SFTHREAD_CHUNK          4096
#define SFTHREAD_SLEEP          PD_MILLISECONDS_TO_NANOSECONDS (37.0)

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_class *sfthread_class;     /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static void *sfthread_readerThread (void *z)
{
    t_sfthread *x = (t_sfthread *)z;
    
    PD_TRY
    
    denormal_setPolicy();   /* If inheritance is broken. */
    
    while (!atomic_int32Read (&x->sft_flag)) {
    //
    while (ring_getAvailableWrite (x->sft_ring) > SFTHREAD_CHUNK) {
        if (!atomic_int32Read (&x->sft_flag)) {
            char t[SFTHREAD_CHUNK] = { 0 };
            size_t required = PD_MIN (SFTHREAD_CHUNK, x->sft_remainsToRead);
            int bytes = (int)read (x->sft_fileDescriptor, t, required);
            if (bytes > 0) {
                ring_write (x->sft_ring, t, bytes);
                x->sft_remainsToRead -= bytes;
            } else {
                atomic_int32Write (&x->sft_flag, SFTHREAD_QUIT);
            }
        } else {
            break;
        }
    }
    
    if (!atomic_int32Read (&x->sft_flag)) {
        nano_sleep (SFTHREAD_SLEEP);
    }
    //
    }
    
    close (x->sft_fileDescriptor);
    
    PD_CATCH
    
    return (NULL);
}

static void *sfthread_writerThread (void *z)
{
    t_sfthread *x = (t_sfthread *)z;
    
    PD_TRY
    
    denormal_setPolicy();   /* If inheritance is broken. */
    
    while (1) {
    //
    while (ring_getAvailableRead (x->sft_ring) > 0) {
        char t[SFTHREAD_CHUNK] = { 0 };
        int required = PD_MIN (SFTHREAD_CHUNK, x->sft_maximumToWrite - x->sft_alreadyWritten);
        int loaded   = ring_read (x->sft_ring, t, required);
        int written  = (int)write (x->sft_fileDescriptor, t, (size_t)loaded);
        
        x->sft_alreadyWritten += written;
        
        if (required == 0) { atomic_int32Write (&x->sft_flag, SFTHREAD_QUIT); break; }
        if (written != loaded) {
            atomic_int32Write (&x->sft_corrupted, SFTHREAD_CORRUPTED);
            atomic_int32Write (&x->sft_flag, SFTHREAD_QUIT);
            break;
        }
    }
    
    if (!atomic_int32Read (&x->sft_flag)) {
        nano_sleep (SFTHREAD_SLEEP);
    } else {
        break;
    }
    //
    }
    
    {
    //
    int size = x->sft_properties.ap_numberOfChannels * x->sft_properties.ap_bytesPerSample;
    int framesWritten = x->sft_alreadyWritten / size;
    
    if (soundfile_writeFileClose (x->sft_fileDescriptor, framesWritten, &x->sft_properties)) {
        atomic_int32Write (&x->sft_corrupted, SFTHREAD_CORRUPTED);
    }
    //
    }
    
    close (x->sft_fileDescriptor);
    
    PD_CATCH
    
    return (NULL);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_ring *sfthread_getRingBuffer (t_sfthread *x)
{
    return x->sft_ring;
}

int sfthread_getNumberOfChannels (t_sfthread *x)
{
    return x->sft_properties.ap_numberOfChannels;
}

int sfthread_getBytesPerSample (t_sfthread *x)
{
    return x->sft_properties.ap_bytesPerSample;
}

int sfthread_isBigEndian (t_sfthread *x)
{
    return x->sft_properties.ap_isBigEndian;
}

int sfthread_isEnd (t_sfthread *x)
{
    return (atomic_int32Read (&x->sft_flag) != 0);
}

void sfthread_setCorrupted (t_sfthread *x)
{
    if (!sfthread_isEnd (x)) { atomic_int32Write (&x->sft_corrupted, SFTHREAD_CORRUPTED); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_sfthread *sfthread_new (t_object *owner, int type, int bufferSize, int fd, t_audioproperties *p)
{
    t_sfthread *x = (t_sfthread *)pd_new (sfthread_class);
    
    bufferSize = PD_MAX (SFTHREAD_CHUNK * 2, bufferSize);
    
    soundfile_propertiesCopy (&x->sft_properties, p);
    
    x->sft_owner          = object_getUnique (owner);
    x->sft_type           = type;
    x->sft_fileDescriptor = fd;
    x->sft_ring           = ring_new (bufferSize, 1);
    
    if (x->sft_type == SFTHREAD_READER) {
    //
    int dataSize = x->sft_properties.ap_dataSizeInBytes;
    int frames   = x->sft_properties.ap_numberOfFrames;

    if (frames  != SOUNDFILE_UNKNOWN) {
        int size = x->sft_properties.ap_numberOfChannels * x->sft_properties.ap_bytesPerSample;
        dataSize = PD_MIN (dataSize, frames * size);
    }
        
    x->sft_remainsToRead = PD_MAX (0, dataSize);
    x->sft_error = (pthread_create (&x->sft_thread, NULL, sfthread_readerThread, (void *)x) != 0);
    //
    } else {
    //
    int dataSize = PD_INT_MAX;
    int frames   = x->sft_properties.ap_numberOfFrames;

    if (frames  != SOUNDFILE_UNKNOWN) {
        int size = x->sft_properties.ap_numberOfChannels * x->sft_properties.ap_bytesPerSample;
        dataSize = PD_MIN (dataSize, frames * size);
    }
    
    x->sft_maximumToWrite = PD_MAX (0, dataSize);
    x->sft_error = (pthread_create (&x->sft_thread, NULL, sfthread_writerThread, (void *)x) != 0);
    //
    }
    
    if (x->sft_error) { close (x->sft_fileDescriptor); pd_free (cast_pd (x)); x = NULL; PD_BUG; }
    
    return x;
}

static void sfthread_free (t_sfthread *x)
{
    if (!x->sft_error) {
    //
    pthread_join (x->sft_thread, NULL);
    
    /* Notice that it is reported here since logging is NOT thread-safe. */
    /* However it should never happens. */
    
    if (atomic_int32Read (&x->sft_corrupted) != 0) {
    //
    t_symbol *filename = symbol_addSuffix (x->sft_properties.ap_fileName, x->sft_properties.ap_fileExtension);

    warning_fileIsCorrupted (instance_registerGetObject (x->sft_owner), filename);
    //
    }
    //
    }
    
    ring_free (x->sft_ring);
}

void sfthread_release (t_sfthread *x)
{
    if (x) {
    //
    atomic_int32Write (&x->sft_flag, SFTHREAD_QUIT);
    
    instance_autoreleaseRegister (cast_pd (x));
    //
    }
}

static void sfthread_autorelease (t_sfthread *x)
{
    instance_autoreleaseProceed (cast_pd (x));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void sfthread_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_sfthread,
            NULL,
            (t_method)sfthread_free,
            sizeof (t_sfthread),
            CLASS_INVISIBLE,
            A_NULL);
    
    class_addAutorelease (c, (t_method)sfthread_autorelease);
    
    sfthread_class = c;
}

void sfthread_destroy (void)
{
    class_free (sfthread_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
