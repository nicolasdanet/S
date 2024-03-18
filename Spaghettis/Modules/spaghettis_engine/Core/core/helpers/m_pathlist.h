
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef M_PATHLIST_H_
#define M_PATHLIST_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _pathregister {
    int                     pr_allocated;
    int                     pr_size;
    uint64_t                *pr_hashes;
    } t_pathregister;

typedef struct _pathlist {
    struct _pathlist        *pl_next;
    struct _pathregister    *pl_register;
    char                    *pl_string;
    uint64_t                pl_hash;
    } t_pathlist;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_pathlist     *pathlist_newAppend     (t_pathlist *x, t_pathlist **d, const char *s);
char           *pathlist_getPath       (t_pathlist *x);
t_pathlist     *pathlist_getNext       (t_pathlist *x);

t_pathlist     *pathlist_removeFirst   (t_pathlist *x);
t_pathlist     *pathlist_moveFront     (t_pathlist *x, int n);

int            pathlist_contains       (t_pathlist *x, const char *s);
int            pathlist_getSize        (t_pathlist *x);
void           pathlist_free           (t_pathlist *x);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if PD_WITH_DEBUG

t_error    pathlist_check              (t_pathlist *x);

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // M_PATHLIST_H_
