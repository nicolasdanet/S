
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

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

static int legacy_replaceObjects (t_buffer *x)
{
    t_iterator *iter = iterator_new (buffer_getSize (x), buffer_getAtoms (x));
    t_atom *atoms    = NULL;
    int count;
    
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
    else if (s == sym_cnv)                  { SET_SYMBOL (atoms + 4, sym_panel);             }
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

static int legacy_removeUnnecessary (t_buffer *x)
{
    int found = 0;
    int start = 0;
    int end   = 0;
    
    t_iterator *iter = iterator_new (buffer_getSize (x), buffer_getAtoms (x));
    t_atom *atoms    = NULL;
    int count;
    
    while ((count = iterator_next (iter, &atoms))) {
    //
    t_symbol *s1 = atom_getSymbolAtIndex (0, count, atoms);
    t_symbol *s2 = atom_getSymbolAtIndex (1, count, atoms);
    t_symbol *s3 = atom_getSymbolAtIndex (2, count, atoms);
    
    found |= (s1 == sym___hash__X) && (s2 == sym_coords);
    found |= (s1 == sym___hash__X) && (s2 == sym_scalar);
    found |= (s1 == sym___hash__N) && (s2 == sym_struct);
    found |= (s1 == sym___hash__A) && (s2 == sym__restore) && (s3 == sym__SLOT_);
    
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

static int legacy_convertGraphicAtoms (t_buffer *x)
{
    int found = 0;
    int start = 0;
    int end   = 0;

    t_iterator *iter = iterator_new (buffer_getSize (x), buffer_getAtoms (x));
    t_atom *atoms    = NULL;
    int count;
    
    while ((count = iterator_next (iter, &atoms))) {
    //
    if (count == 12) {
    
    t_symbol *s1 = atom_getSymbolAtIndex (0, count, atoms);
    t_symbol *s2 = atom_getSymbolAtIndex (1, count, atoms);
    
    found |= (s1 == sym___hash__X) && (s2 == sym_floatatom);
    found |= (s1 == sym___hash__X) && (s2 == sym_symbolatom);
    
    /* For now replace unsupported symbolatom by a floatatom. */
    /* Could be another GUI object later. */
    
    if (found && s2 == sym_symbolatom) { SET_SYMBOL (atoms + 1, sym_floatatom); }
    
    if (found) {
        start = iterator_get (iter) - 5;
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
    int count;
    
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
        if (key == sym_panel)  { atoms[5] = atoms[6]; atoms[6] = atoms[7]; }
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

static void legacy_convertProceed (t_buffer *x)
{
    while (legacy_convertArray (x))        { }
    while (legacy_replaceObjects (x))      { }
    while (legacy_removeUnnecessary (x))   { }
    while (legacy_convertGraphicAtoms (x)) { }
    
    while (legacy_convertGUI (x, sym_bng, 20, 0, 12))       { }
    while (legacy_convertGUI (x, sym_tgl, 20, 1, 12))       { }
    while (legacy_convertGUI (x, sym_vradio, 21, 0, 12))    { }
    while (legacy_convertGUI (x, sym_hradio, 21, 0, 12))    { }
    while (legacy_convertGUI (x, sym_vslider, 24, 0, 13))   { }
    while (legacy_convertGUI (x, sym_hslider, 24, 0, 13))   { }
    while (legacy_convertGUI (x, sym_dial, 24, 0, 13))      { }
    while (legacy_convertGUI (x, sym_vu, 18, 0, 10))        { }
    while (legacy_convertGUI (x, sym_panel, 18, 0, 10))     { }
    while (legacy_convertGUI (x, sym_panel, 19, 0, 11))     { }
    while (legacy_convertGUI (x, sym_menubutton, 17, 0, 9)) { }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void legacy_convert (t_buffer *x)
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

PD_LOCAL void legacy_version (t_buffer *x)
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

PD_LOCAL void legacy_start (void *dummy, t_symbol *s, int argc, t_atom *argv)
{
    instance_viewReset();
}

PD_LOCAL void legacy_view (void *dummy, t_symbol *s, int argc, t_atom *argv)
{
    t_rectangle r; rectangle_setNothing (&r);
    
    if (argc && argv) {
    //
    int a = atom_getFloatAtIndex (0, argc, argv);
    int b = atom_getFloatAtIndex (1, argc, argv);
    int w = atom_getFloatAtIndex (2, argc, argv);
    int h = atom_getFloatAtIndex (3, argc, argv);
    
    rectangle_set (&r, a, b, a + PD_ABS (w), b + PD_ABS (h));
    
    instance_viewSet (&r);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
