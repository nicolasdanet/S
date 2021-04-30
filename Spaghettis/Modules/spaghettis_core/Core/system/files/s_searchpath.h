
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef S_SEARCHPATH_H_
#define S_SEARCHPATH_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_FORCE t_pathlist     *searchpath_getRoots            (void);
PD_LOCAL t_pathlist     *searchpath_getExtended         (void);

PD_FORCE void       searchpath_appendRoot               (const char *filepath);

PD_LOCAL int        searchpath_isExternalAvailable      (t_symbol *s);
PD_LOCAL int        searchpath_isAbstractionAvailable   (t_symbol *s);

PD_LOCAL void       searchpath_rescan                   (int logged);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // S_SEARCHPATH_H_
