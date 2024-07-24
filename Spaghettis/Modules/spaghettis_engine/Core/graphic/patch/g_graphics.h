
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef G_GRAPHICS_H_
#define G_GRAPHICS_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int         glist_graphicsGetSize                   (t_glist *g);
int         glist_graphicsGetIndexOf                (t_glist *g, t_object *y);
int         glist_graphicsGetIndexAmongSelected     (t_glist *g, t_object *y);
int         glist_graphicsGetCountByClass           (t_glist *g, t_class  *c);

void        glist_graphicsAppend                    (t_glist *g, t_object *y);
void        glist_graphicsRemove                    (t_glist *g, t_object *y);

t_object    *glist_graphicsGetObjectAt              (t_glist *g, int i);
t_object    *glist_graphicsGetFirst                 (t_glist *g);
t_object    *glist_graphicsGetLast                  (t_glist *g);
t_object    *glist_graphicsGetNext                  (t_glist *g, t_object *y);

void        glist_graphicsMoveAtFirst               (t_glist *g, t_object *y);
void        glist_graphicsMoveAtLast                (t_glist *g, t_object *y);
void        glist_graphicsMoveAt                    (t_glist *g, t_object *y, int n);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // G_GRAPHICS_H_
