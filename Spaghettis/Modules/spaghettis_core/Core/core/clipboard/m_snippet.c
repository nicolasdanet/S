
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void snippet_renameArrays (t_buffer *x, t_glist *glist)
{
    t_iterator *iter = iterator_new (buffer_getSize (x), buffer_getAtoms (x));
    t_atom *atoms = NULL;
    int count;
    
    while ((count = iterator_next (iter, &atoms))) {
    //
    if (count >= 8) {
    if (atom_getSymbolAtIndex (0, count, atoms) == sym___hash__X) {
    if (atom_getSymbolAtIndex (1, count, atoms) == sym_obj) {
    if (atom_getSymbolAtIndex (4, count, atoms) == sym_garray) {

        t_atom *a   = atoms + 5;
        t_symbol *s = atom_getSymbolOrDollarSymbol (a);
        
        PD_ASSERT (s != &s_); s = symbol_appendCopySuffix (s); SET_SYMBOL (a, s);
    }
    }
    }
    }
    //
    }
    
    iterator_free (iter);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void snippet_addOffsetToLines (t_buffer *x, int i)
{
    t_iterator *iter = iterator_new (buffer_getSize (x), buffer_getAtoms (x));
    t_atom *atoms = NULL;
    int count;
    
    int k = 0;
    
    while ((count = iterator_next (iter, &atoms))) {
    //
    if (count >= 2) {
    //
    t_symbol *s = atom_getSymbolAtIndex (1, count, atoms);
    
    if (s == sym_canvas)  { k++; }      /* Connections in subpatches must not be changed. */
    if (s == sym_restore) { k--; }
    if (s == sym_connect) {
    //
    if (!k && count == 7) {
    //
    t_float m = atom_getFloat (atoms + 2);
    t_float n = atom_getFloat (atoms + 4);
    SET_FLOAT (atoms + 2, m + i);
    SET_FLOAT (atoms + 4, n + i);
    //
    }
    //
    }
    //
    }
    //
    }
    
    iterator_free (iter);
}

void snippet_substractOffsetToLines (t_buffer *x, int i)
{
    snippet_addOffsetToLines (x, -i);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static int snippet_isObject (t_symbol *s)
{
    if (s == sym_obj)               { return 1; }
    else if (s == sym_msg)          { return 1; }
    else if (s == sym_text)         { return 1; }
    else if (s == sym_floatatom)    { return 1; }
    else if (s == sym_restore)      { return 1; }
    
    return 0;
}

static t_rectangle snippet_getRawBoundingBoxOfObjects (t_buffer *x)
{
    t_iterator *iter = iterator_new (buffer_getSize (x), buffer_getAtoms (x));
    t_atom *atoms = NULL;
    int count;
    
    int k = 0;
    
    t_rectangle r; rectangle_setNothing (&r);
    
    while ((count = iterator_next (iter, &atoms))) {
    //
    if (count >= 2) {
    //
    t_symbol *s = atom_getSymbolAtIndex (1, count, atoms);
    
    if (s == sym_canvas)  { k++; }      /* Objects in subpatches must not be changed. */
    if (s == sym_restore) { k--; }
    
    if (!k && snippet_isObject (s)) {
        int a      = atom_getFloatAtIndex (2, count, atoms);
        int b      = atom_getFloatAtIndex (3, count, atoms);
        t_point pt = point_make (a, b);
        
        rectangle_addPoint (&r, &pt);
    }
    //
    }
    //
    }
    
    iterator_free (iter);
    
    return r;
}

static void snippet_displaceObjects (t_buffer *x, int deltaX, int deltaY)
{
    t_iterator *iter = iterator_new (buffer_getSize (x), buffer_getAtoms (x));
    t_atom *atoms = NULL;
    int count;
    
    int k = 0;
    
    while ((count = iterator_next (iter, &atoms))) {
    //
    if (count >= 2) {
    //
    t_symbol *s = atom_getSymbolAtIndex (1, count, atoms);
    
    if (s == sym_canvas)  { k++; }      /* Objects in subpatches must not be changed. */
    if (s == sym_restore) { k--; }
    
    if (!k && snippet_isObject (s)) {
        t_float a = atom_getFloatAtIndex (2, count, atoms);
        t_float b = atom_getFloatAtIndex (3, count, atoms);
        SET_FLOAT (atoms + 2, a + deltaX);
        SET_FLOAT (atoms + 3, b + deltaY);
    }
    //
    }
    //
    }
    
    iterator_free (iter);
}

void snippet_disposeObjects (t_buffer *x, int offset)
{
    t_rectangle r = snippet_getRawBoundingBoxOfObjects (x);
    
    if (!rectangle_isNothing (&r)) {
    //
    int deltaX = offset - rectangle_getTopLeftX (&r);
    int deltaY = offset - rectangle_getTopLeftY (&r);
    
    snippet_displaceObjects (x, deltaX, deltaY);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
