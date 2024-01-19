
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "d_soundfile.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *soundinfo_class;        /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _soundinfo {
    t_object    x_obj;                  /* Must be the first. */
    t_outlet    *x_outletLeft;
    t_outlet    *x_outletRight;
    } t_soundinfo;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static int soundinfo_readHeader (t_soundinfo *x, int argc, t_atom *argv)
{
    t_audioproperties p; soundfile_propertiesInit (&p);
    
    t_error err = soundfile_readFileParse (NULL, sym_soundinfo, &argc, &argv, &p, cast_object (x));
    
    if (!err) {
    //
    int f = soundfile_readFileHeader (object_getOwner (cast_object (x)), &p);
    
    err = (f < 0);
    
    if (!err) { close (f); }    /* < http://stackoverflow.com/a/13691168 > */
    if (!err) {
    //
    int frames = p.ap_dataSizeInBytes / p.ap_bytesPerSample / p.ap_numberOfChannels;
    t_symbol *name = symbol_addSuffix (p.ap_fileName, p.ap_fileExtension);
    
    t_atom a;
    
    SET_SYMBOL (&a, name);                  outlet_anything (x->x_outletRight, sym_name,        1, &a);
    SET_FLOAT (&a, p.ap_sampleRate);        outlet_anything (x->x_outletRight, sym_samplerate,  1, &a);
    SET_FLOAT (&a, p.ap_numberOfChannels);  outlet_anything (x->x_outletRight, sym_channels,    1, &a);
    SET_FLOAT (&a, p.ap_bytesPerSample);    outlet_anything (x->x_outletRight, sym_bytes,       1, &a);
    SET_FLOAT (&a, p.ap_dataSizeInBytes);   outlet_anything (x->x_outletRight, sym_size,        1, &a);
    SET_FLOAT (&a, frames);                 outlet_anything (x->x_outletRight, sym_frames,      1, &a);

    if (p.ap_isBigEndian) { SET_SYMBOL (&a, sym_big); } else { SET_SYMBOL (&a, sym_little); }
    
    outlet_anything (x->x_outletRight, sym_endianness, 1, &a);
    
    if (p.ap_fileType == SOUNDFILE_WAVE)      { SET_SYMBOL (&a, sym_wave); }
    else if (p.ap_fileType == SOUNDFILE_AIFF) { SET_SYMBOL (&a, sym_aiff); }
    else {
        SET_SYMBOL (&a, sym_next);
    }
    
    outlet_anything (x->x_outletRight, sym_type, 1, &a);
    //
    }
    //
    }

    return (err == PD_ERROR_NONE);
}

static void soundinfo_read (t_soundinfo *x, t_symbol *s, int argc, t_atom *argv)
{
    outlet_float (x->x_outletLeft, (t_float)soundinfo_readHeader (x, argc, argv));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_soundinfo *soundinfo_new (void)
{
    t_soundinfo *x = (t_soundinfo *)pd_new (soundinfo_class);
    
    x->x_outletLeft  = outlet_newFloat (cast_object (x));
    x->x_outletRight = outlet_newAnything (cast_object (x));
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void soundinfo_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_soundinfo,
            (t_newmethod)soundinfo_new,
            NULL,
            sizeof (t_soundinfo),
            CLASS_DEFAULT,
            A_NULL);
            
    class_addMethod (c, (t_method)soundinfo_read, sym_read, A_GIMME, A_NULL);
    
    soundinfo_class = c;
}

void soundinfo_destroy (void)
{
    class_free (soundinfo_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
