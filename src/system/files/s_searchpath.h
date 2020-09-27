
/* Copyright (c) 1997-2020 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef S_SEARCHPATH_H_
#define S_SEARCHPATH_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_pathlist     *searchpath_getRoots            (void);
PD_LOCAL t_pathlist     *searchpath_getExtended         (void);

PD_LOCAL void       searchpath_appendRoot               (const char *filepath);
PD_LOCAL t_error    searchpath_scan                     (void);
PD_LOCAL void       searchpath_report                   (void);
PD_LOCAL int        searchpath_isExternalAvailable      (t_symbol *s);
PD_LOCAL int        searchpath_isAbstractionAvailable   (t_symbol *s);
PD_LOCAL int        searchpath_hasDuplicates            (void);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // S_SEARCHPATH_H_
