
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Files are searching in the directory of the patch first. */
/* Without success it tries to find it using the search path. */

PD_LOCAL int glist_fileExist (t_glist *glist, const char *name, const char *extension, t_fileproperties *p)
{
    int f = glist_fileOpen (glist, name, extension, p);
    
    if (f >= 0) { close (f); return 1; }
    
    return 0;
}

/* Caller is responsible to close the file. */

PD_LOCAL int glist_fileOpen (t_glist *glist, const char *name, const char *extension, t_fileproperties *p)
{
    const char *directory = glist ? environment_getDirectoryAsString (glist_getEnvironment (glist)) : ".";
    
    int f = file_openReadConsideringSearchPath (directory, name, extension, p);
        
    return f;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void glist_setUniqueAndSourceOfLast (t_glist *glist, int argc, t_atom *argv)
{
    if (glist->gl_graphics) {
    //
    t_object *g1 = NULL;
    t_object *g2 = NULL;
    
    t_id u; t_error err = utils_uniqueWithAtoms (argc, argv, &u);
    
    PD_ASSERT (!err); PD_UNUSED (err);
    
    for ((g1 = glist->gl_graphics); (g2 = g1->g_next); (g1 = g2)) { }
    
    object_changeUnique (g1, u);
    object_changeSource (g1, u);
    //
    }
}

PD_LOCAL void glist_setSourceOfLast (t_glist *glist, int argc, t_atom *argv)
{
    if (glist->gl_graphics) {
    //
    t_object *g1 = NULL;
    t_object *g2 = NULL;
    
    t_id u; t_error err = utils_uniqueWithAtoms (argc, argv, &u);
    
    PD_ASSERT (!err); PD_UNUSED (err);
    
    for ((g1 = glist->gl_graphics); (g2 = g1->g_next); (g1 = g2)) { }
    
    object_changeSource (g1, u);
    //
    }
}

PD_LOCAL void glist_setWidthOfLast (t_glist *glist, int w)
{
    if (glist->gl_graphics) {
    //
    t_object *g1 = NULL;
    t_object *g2 = NULL;
    
    for ((g1 = glist->gl_graphics); (g2 = g1->g_next); (g1 = g2)) { }
    
    object_setWidth (g1, PD_MAX (1, w));
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_rectangle glist_getBoundingBoxOfAll (t_glist *glist)
{
    t_rectangle r; rectangle_setNothing (&r);
    
    t_object *y = NULL;
    
    for (y = glist->gl_graphics; y; y = y->g_next) {
    //
    rectangle_add (&r, object_getX (y), object_getY (y));
    //
    }

    return r;
}

PD_LOCAL t_rectangle glist_getBoundingBoxOfSelected (t_glist *glist)
{
    t_rectangle r; rectangle_setNothing (&r);
    
    t_object *y = NULL;
    
    for (y = glist->gl_graphics; y; y = y->g_next) {
    //
    if (glist_objectIsSelected (glist, y)) { rectangle_add (&r, object_getX (y), object_getY (y)); }
    //
    }

    return r;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
