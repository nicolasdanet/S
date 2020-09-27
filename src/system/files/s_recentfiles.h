
/* Copyright (c) 1997-2020 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef S_RECENTFILES_H_
#define S_RECENTFILES_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_pathlist     *recentfiles_get    (void);

PD_LOCAL void   recentfiles_add             (t_symbol *name, t_symbol *directory, int check);
PD_LOCAL void   recentfiles_append          (const char *filepath);
PD_FORCE void   recentfiles_clear           (void);
PD_LOCAL void   recentfiles_update          (void);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // S_RECENTFILES_H_
