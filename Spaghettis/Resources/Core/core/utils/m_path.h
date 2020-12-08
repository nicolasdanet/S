
/* Copyright (c) 1997-2020 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef M_PATH_H_
#define M_PATH_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL int        path_isFileExist                    (const char *filepath);
PD_LOCAL int        path_isFileExistAsRegularFile       (const char *filepath);
PD_LOCAL int        path_isFileExistAsDirectory         (const char *filepath);
PD_LOCAL int        path_isValid                        (const char *filepath);
PD_LOCAL t_error    path_createDirectory                (const char *filepath);
PD_LOCAL t_error    path_createDirectoryIfNeeded        (const char *filepath);

PD_LOCAL t_error    path_withDirectoryAndName           (char *dest,
                                                            size_t size,
                                                            const char *directory,
                                                            const char *name);

PD_LOCAL t_error    path_toDirectoryAndName             (char *dest,
                                                            size_t size,
                                                            t_symbol **directory,
                                                            t_symbol **name);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // M_PATH_H_
