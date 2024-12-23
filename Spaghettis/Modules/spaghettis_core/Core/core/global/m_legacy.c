
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Ensure at most compatibility with Pure Data patches. */

// TODO: Convert short options (e.g. -k to -keep).

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static int legacy_convertArrayFetch (t_buffer *x,
    int *start,
    int *end,
    int *coordX,
    int *coordY,
    int *size,
    int *embed,
    int *width,
    int *height,
    t_symbol **name,
    t_buffer *data)
{
    int found = 0;
    
    t_iterator *iter = iterator_new (buffer_getSize (x), buffer_getAtoms (x));
    t_atom *atoms    = NULL;
    int count        = 0;

    while ((count = iterator_next (iter, &atoms))) {
    //
    if (count > 7 && atom_getSymbolAtIndex (0, count, atoms) == sym___hash__N) {
        *name  = NULL;
        *start = iterator_get (iter) - count;
    } else if (count > 5 && atom_getSymbolAtIndex (0, count, atoms) == sym___hash__X) {
        t_symbol *first = atom_getSymbolAtIndex (1, count, atoms);
        if (first == sym_restore) {
            if (atom_getSymbolAtIndex (4, count, atoms) == sym_graph) {
                if (*name) {
                    *coordX = atom_getFloatAtIndex (2, count, atoms);
                    *coordY = atom_getFloatAtIndex (3, count, atoms);
                    *end    = iterator_get (iter);
                    found   = 1;
                    break;
                }
            }
        } else if (first == sym_array) {
            int flags = atom_getFloatAtIndex (5, count, atoms);
            *name     = atom_getSymbolOrDollarSymbol (atoms + 2);
            *size     = atom_getFloatAtIndex (3, count, atoms);
            *embed    = ((flags & 1) != 0);
        } else if (first == sym_coords) {
            // *left  = atom_getFloatAtIndex (2, count, atoms);
            // *up    = atom_getFloatAtIndex (3, count, atoms);
            // *right = atom_getFloatAtIndex (4, count, atoms);
            // *down  = atom_getFloatAtIndex (5, count, atoms);
            *width    = atom_getFloatAtIndex (6, count, atoms);
            *height   = atom_getFloatAtIndex (7, count, atoms);
        } else {
            *name     = NULL;
        }
    } else if (atom_getSymbolAtIndex (0, count, atoms) == sym___hash__A) {
        if (*embed && count > 2) {
            buffer_clear (data);
            buffer_append (data, count - 1, atoms);     /* Could be a comma at end. */
            buffer_appendSemicolon (data);
        }
    }
    //
    }
    
    iterator_free (iter);
        
    return found;
}

static void legacy_convertOptionsReplace (int argc, t_atom *argv, t_symbol *o, t_symbol *n)
{
    int i;
    
    for (i = 0; i < argc; i++) {
    //
    t_atom *a = argv + i; if (atom_getSymbol (a) == o) { atom_setSymbol (a, n); }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static int legacy_convertArray (t_buffer *x)
{
    int start       = 0;
    int end         = 0;
    int coordX      = 0;
    int coordY      = 0;
    int size        = 0;
    int embed       = 0;
    int width       = 0;
    int height      = 0;
    t_symbol *name  = NULL;
    t_buffer *data  = buffer_new();
    
    int done = legacy_convertArrayFetch (x,
                    &start,
                    &end,
                    &coordX,
                    &coordY,
                    &size,
                    &embed,
                    &width,
                    &height,
                    &name,
                    data);

    if (done) {
    //
    PD_ASSERT (name && name != &s_);
    
    t_atom a[15];
    
    SET_SYMBOL (a + 0, sym___hash__X);
    SET_SYMBOL (a + 1, sym_obj);
    SET_FLOAT (a + 2,  coordX);
    SET_FLOAT (a + 3,  coordY);
    SET_SYMBOL (a + 4, sym_garray);
    SET_SYMBOL (a + 5, name);
    SET_FLOAT (a + 6,  size);
    SET_FLOAT (a + 7,  embed);
    SET_FLOAT (a + 8,  width);
    SET_FLOAT (a + 9,  height);
    SET_SEMICOLON (a + 10);
    
    buffer_prepend (data, 11, a);
    
    buffer_replace (x, start, end, buffer_getSize (data), buffer_getAtoms (data));
    //
    }
    
    buffer_free (data);
    
    return done;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static int legacy_convertOptions (t_buffer *x)
{
    t_iterator *iter = iterator_new (buffer_getSize (x), buffer_getAtoms (x));
    t_atom *atoms    = NULL;
    int count        = 0;
    
    while ((count = iterator_next (iter, &atoms))) {
    //
    if (count > 4) {
    if (atom_getSymbolAtIndex (0, count, atoms) == sym___hash__X) {
    if (atom_getSymbolAtIndex (1, count, atoms) == sym_obj) {
    //
    t_symbol *s  = atom_getSymbolAtIndex (4, count, atoms);
    int argc     = count - 4;
    t_atom *argv = atoms + 4;
    
    if (s == sym_netsend || s == sym_netreceive) {
        legacy_convertOptionsReplace (argc, argv, sym___dash__u, sym___arrobe__udp);
        legacy_convertOptionsReplace (argc, argv, sym___dash__b, sym___arrobe__binary);
    }
    if (s == sym_oscformat) {
        legacy_convertOptionsReplace (argc, argv, sym___dash__f, sym___arrobe__format);
    }
    if (s == sym_sigmund__tilde__) {
        legacy_convertOptionsReplace (argc, argv, sym___dash__growth,       sym___arrobe__growth);
        legacy_convertOptionsReplace (argc, argv, sym___dash__hop,          sym___arrobe__hop);
        legacy_convertOptionsReplace (argc, argv, sym___dash__maxfreq,      sym___arrobe__frequency);
        legacy_convertOptionsReplace (argc, argv, sym___dash__minpower,     sym___arrobe__power);
        legacy_convertOptionsReplace (argc, argv, sym___dash__npeak,        sym___arrobe__peaks);
        legacy_convertOptionsReplace (argc, argv, sym___dash__npts,         sym___arrobe__points);
        legacy_convertOptionsReplace (argc, argv, sym___dash__stabletime,   sym___arrobe__time);
        legacy_convertOptionsReplace (argc, argv, sym___dash__t,            sym___arrobe__table);
        legacy_convertOptionsReplace (argc, argv, sym___dash__vibrato,      sym___arrobe__vibrato);
    }
    if (s == sym_text) {
        legacy_convertOptionsReplace (argc, argv, sym___dash__g, sym___arrobe__global);
        legacy_convertOptionsReplace (argc, argv, sym___dash__t, sym___arrobe__unit);
        legacy_convertOptionsReplace (argc, argv, sym___dash__w, sym___arrobe__wait);
        legacy_convertOptionsReplace (argc, argv, sym___dash__k, sym___arrobe__keep);
    }
    if (s == sym_print) {
        legacy_convertOptionsReplace (argc, argv, sym___dash__n, sym___arrobe__none);
    }
    //
    }
    }
    }
    //
    }

    iterator_free (iter);
    
    return 0;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static int legacy_convertRename (t_buffer *x)
{
    t_iterator *iter = iterator_new (buffer_getSize (x), buffer_getAtoms (x));
    t_atom *atoms    = NULL;
    int count        = 0;
    
    while ((count = iterator_next (iter, &atoms))) {
    //
    if (count > 4) {
    if (atom_getSymbolAtIndex (0, count, atoms) == sym___hash__X) {
    if (atom_getSymbolAtIndex (1, count, atoms) == sym_obj) {
    //
    t_symbol *s = atom_getSymbolAtIndex (4, count, atoms);
    
    if (s == sym_vline__tilde__)            { SET_SYMBOL (atoms + 4, sym_line__tilde__);     }
    else if (s == sym_samplerate__tilde__)  { SET_SYMBOL (atoms + 4, sym_samplerate);        }
    else if (s == sym_delread4__tilde__)    { SET_SYMBOL (atoms + 4, sym_vd__tilde__);       }
    else if (s == sym_vsnapshot__tilde__)   { SET_SYMBOL (atoms + 4, sym_snapshot__tilde__); }
    else if (s == sym_cputime)              { SET_SYMBOL (atoms + 4, sym_realtime);          }
    else if (s == sym_fswap)                { SET_SYMBOL (atoms + 4, sym_swap);              }
    else if (s == sym_del)                  { SET_SYMBOL (atoms + 4, sym_delay);             }
    else if (s == sym_sel)                  { SET_SYMBOL (atoms + 4, sym_select);            }
    else if (s == sym_hsl)                  { SET_SYMBOL (atoms + 4, sym_hslider);           }
    else if (s == sym_vsl)                  { SET_SYMBOL (atoms + 4, sym_vslider);           }
    else if (s == sym_nbx)                  { SET_SYMBOL (atoms + 4, sym_dial);              }
    else if (s == sym_table)                { SET_SYMBOL (atoms + 4, sym_garray);            }
    //
    }
    }
    }
    //
    }
    
    iterator_free (iter);
    
    return 0;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Note that index of real objects are used to connect them. */
/* Only abstract ones can be removed. */

static int legacy_convertRemove (t_buffer *x)
{
    int found = 0;
    int start = 0;
    int end   = 0;
    
    t_iterator *iter = iterator_new (buffer_getSize (x), buffer_getAtoms (x));
    t_atom *atoms    = NULL;
    int count        = 0;
    
    while ((count = iterator_next (iter, &atoms))) {
    //
    t_symbol *s1 = atom_getSymbolAtIndex (0, count, atoms);
    t_symbol *s2 = atom_getSymbolAtIndex (1, count, atoms);
    
    found |= (s1 == sym___hash__X) && (s2 == sym_coords);
    found |= (s1 == sym___hash__X) && (s2 == sym_scalar);
    found |= (s1 == sym___hash__N) && (s2 == sym_struct);
    
    if (found) {
        start = iterator_get (iter) - count;
        end   = iterator_get (iter);
        break;
    }
    //
    }
    
    iterator_free (iter);
    
    if (found) { buffer_extend (x, start, end, 0); }
    
    return found;
}

/* Ensure that unsupported objects are instantiated (with an error message). */

static int legacy_convertReify (t_buffer *x)
{
    int found = 0;
    int start = 0;
    int end   = 0;

    t_iterator *iter = iterator_new (buffer_getSize (x), buffer_getAtoms (x));
    t_atom *atoms    = NULL;
    int count        = 0;
    
    while ((count = iterator_next (iter, &atoms))) {
    //
    if (count == 12 || count == 13) {
    //
    t_symbol *s1 = atom_getSymbolAtIndex (0, count, atoms);
    t_symbol *s2 = atom_getSymbolAtIndex (1, count, atoms);
    
    found |= (s1 == sym___hash__X) && (s2 == sym_symbolatom);
    found |= (s1 == sym___hash__X) && (s2 == sym_listbox);
    
    if (found) {
        SET_SYMBOL (atoms + 1, sym_obj);
        SET_SYMBOL (atoms + 4, s2);
        start = iterator_get (iter) - (count - 5);
        end   = iterator_get (iter) - 1;
        break;
    }
    //
    }
    //
    }
    
    iterator_free (iter);
    
    if (found) { buffer_extend (x, start, end, 0); }
    
    return found;
}

static int legacy_convertAtoms (t_buffer *x)
{
    int found = 0;
    int start = 0;
    int end   = 0;

    t_iterator *iter = iterator_new (buffer_getSize (x), buffer_getAtoms (x));
    t_atom *atoms    = NULL;
    int count        = 0;
    
    while ((count = iterator_next (iter, &atoms))) {
    //
    if (count == 12 || count == 13) {
    //
    t_symbol *s1 = atom_getSymbolAtIndex (0, count, atoms);
    t_symbol *s2 = atom_getSymbolAtIndex (1, count, atoms);
    
    found = (s1 == sym___hash__X) && (s2 == sym_floatatom);
    
    if (found) {
        start = iterator_get (iter) - (count - 7);
        end   = iterator_get (iter) - 1;
        break;
    }
    //
    }
    //
    }
    
    iterator_free (iter);
    
    if (found) { buffer_extend (x, start, end, 0); }
    
    return found;
}

static int legacy_convertGUI (t_buffer *x, t_symbol *key, int length, int m, int n)
{
    int found = 0;
    int start = 0;
    int end   = 0;

    t_iterator *iter = iterator_new (buffer_getSize (x), buffer_getAtoms (x));
    t_atom *atoms    = NULL;
    int count        = 0;
    
    while ((count = iterator_next (iter, &atoms))) {
    //
    if (count == length) {
    if (atom_getSymbolAtIndex (0, count, atoms) == sym___hash__X) {
    if (atom_getSymbolAtIndex (1, count, atoms) == sym_obj) {
    if (atom_getSymbolAtIndex (4, count, atoms) == key) {
    
        found = 1;
        start = iterator_get (iter) - 1 - m - n;
        end   = iterator_get (iter) - 1 - m;
        
        if (key == sym_vradio) { SET_FLOAT (atoms + 6, 0.0); atoms[7] = atoms[8]; }
        if (key == sym_hradio) { SET_FLOAT (atoms + 6, 0.0); atoms[7] = atoms[8]; }
        if (key == sym_dial)   {
            t_float k  = 1e+37 / 2.0;
            t_float f1 = atom_getFloat (atoms + 5);
            //t_float f2 = atom_getFloat (atoms + 6);
            t_float f3 = atom_getFloat (atoms + 7);
            t_float f4 = atom_getFloat (atoms + 8);
            SET_FLOAT (atoms + 5, 0);                       /* Force to use default size. */
            SET_FLOAT (atoms + 6, f1);
            if (PD_ABS (f3) > k || PD_ABS (f4) > k) {
                SET_FLOAT (atoms + 7, 0.0);
                SET_FLOAT (atoms + 8, 0.0);
            }
        }
        
        break;
    }
    }
    }
    }
    //
    }
    
    iterator_free (iter);
    
    if (found) { buffer_extend (x, start, end, 0); }
    
    return found;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static int legacy_convertDummy (t_buffer *x)
{
    #if 0
    
    t_iterator *iter = iterator_new (buffer_getSize (x), buffer_getAtoms (x));
    t_atom *atoms    = NULL;
    int count        = 0;
    
    while ((count = iterator_next (iter, &atoms))) {
    //
    int start = iterator_get (iter) - count;
    int end   = iterator_get (iter) - 1;
    char *t0  = atom_atomsToString (count, atoms);
    char *t1  = atom_atomsToString (1, buffer_getAtomAtIndex (x, start));
    char *t2  = atom_atomsToString (1, buffer_getAtomAtIndex (x, end));
    
    PD_DBG ("###");
    PD_DBG_NUMBER (count);
    PD_DBG (t0);
    PD_DBG (t1);    //  "#X"
    PD_DBG (t2);    //  ";"
    
    PD_MEMORY_FREE (t0);
    PD_MEMORY_FREE (t1);
    PD_MEMORY_FREE (t2);
    //
    }

    iterator_free (iter);
    
    #endif
    
    return 0;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void legacy_convertProceed (t_buffer *x)
{
    while (legacy_convertDummy (x))     { }
    while (legacy_convertArray (x))     { }
    while (legacy_convertAtoms (x))     { }
    
    while (legacy_convertRename (x))    { }
    while (legacy_convertRemove (x))    { }
    while (legacy_convertReify (x))     { }
    while (legacy_convertOptions (x))   { }
    
    while (legacy_convertGUI (x, sym_bng, 20, 0, 12))       { }
    while (legacy_convertGUI (x, sym_tgl, 20, 1, 12))       { }
    while (legacy_convertGUI (x, sym_vradio, 21, 0, 12))    { }
    while (legacy_convertGUI (x, sym_hradio, 21, 0, 12))    { }
    while (legacy_convertGUI (x, sym_vslider, 24, 0, 13))   { }
    while (legacy_convertGUI (x, sym_hslider, 24, 0, 13))   { }
    while (legacy_convertGUI (x, sym_dial, 24, 0, 13))      { }
    while (legacy_convertGUI (x, sym_vu, 18, 0, 10))        { }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void legacy_convert (t_buffer *x)
{
    if (buffer_getSize (x) > 5) {
    if (atom_getSymbol (buffer_getAtomAtIndex (x, 0)) == sym___hash__N)  {
    if (atom_getSymbol (buffer_getAtomAtIndex (x, 1)) == sym_spaghettis) {
        return;
    }
    }
    }
    
    legacy_convertProceed (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void legacy_version (t_buffer *x)
{
    buffer_appendSymbol (x, sym___hash__N);
    buffer_appendSymbol (x, sym_spaghettis);
    buffer_appendFloat (x,  PD_VERSION_MAJOR);
    buffer_appendFloat (x,  PD_VERSION_MINOR);
    buffer_appendFloat (x,  PD_VERSION_PATCH);
    buffer_appendSemicolon (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void legacy_start (void *dummy, t_symbol *s, int argc, t_atom *argv)
{

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
