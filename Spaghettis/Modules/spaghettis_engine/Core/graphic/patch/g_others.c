
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Files are searching in the directory of the patch first. */
/* Without success it tries to find it using the search path. */

int glist_fileExist (t_glist *glist, const char *name, const char *extension, t_fileproperties *p)
{
    int f = glist_fileOpen (glist, name, extension, p);
    
    if (f >= 0) { close (f); return 1; }
    
    return 0;
}

/* Caller is responsible to close the file. */

int glist_fileOpen (t_glist *glist, const char *name, const char *extension, t_fileproperties *p)
{
    const char *directory = glist ? environment_getDirectoryAsString (glist_getEnvironment (glist)) : ".";
    
    int f = file_openReadConsideringSearchPath (directory, name, extension, p);
        
    return f;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void glist_setUniqueAndSourceOfLast (t_glist *glist, int argc, t_atom *argv)
{
    t_object *o = glist_graphicsGetLast (glist);
    
    if (o) {
    //
    t_id u; t_error err = utils_uniqueWithAtoms (argc, argv, &u);
    
    PD_ASSERT (!err); PD_UNUSED (err);

    object_changeUnique (o, u);
    object_changeSource (o, u);
    //
    }
}

void glist_setSourceOfLast (t_glist *glist, int argc, t_atom *argv)
{
    t_object *o = glist_graphicsGetLast (glist);
    
    if (o) {
    //
    t_id u; t_error err = utils_uniqueWithAtoms (argc, argv, &u);
    
    PD_ASSERT (!err); PD_UNUSED (err);
    
    object_changeSource (o, u);
    //
    }
}

void glist_setInclusionOfLast (t_glist *glist, int argc, t_atom *argv)
{
    t_object *o = glist_graphicsGetLast (glist);
    
    if (o) {
    //
    object_setLabelUpdate (o, atom_getSymbolAtIndex (0, argc, argv));
    object_setIncludedUpdate (o, 1);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_rectangle glist_getBoundingBoxProceed (t_glist *glist, int onlySelected)
{
    int i, n = glist_graphicsGetSize (glist);
        
    t_rectangle r;
    
    rectangle_setNothing (&r);
    
    for (i = 0; i < n; i++) {
    //
    t_object *y = glist_graphicsGetObjectAt (glist, i);
    t_point t   = object_getPoint (y);
    
    if (!onlySelected || glist_objectIsSelected (glist, y)) { rectangle_addPoint (&r, &t); }
    //
    }

    return r;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_rectangle glist_getBoundingBox (t_glist *glist)
{
    return glist_getBoundingBoxProceed (glist, 0);
}

t_rectangle glist_getBoundingBoxOfSelected (t_glist *glist)
{
    return glist_getBoundingBoxProceed (glist, 1);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
