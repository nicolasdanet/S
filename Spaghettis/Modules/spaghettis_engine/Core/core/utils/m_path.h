
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef M_PATH_H_
#define M_PATH_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int        path_isFileExist                    (const char *filepath);
int        path_isFileExistAsRegularFile       (const char *filepath);
int        path_isFileExistAsDirectory         (const char *filepath);
int        path_isValid                        (const char *filepath);
t_error    path_createDirectory                (const char *filepath);
t_error    path_createDirectoryIfNeeded        (const char *filepath);

t_error    path_withDirectoryAndName           (char *dest,
                                                            size_t size,
                                                            const char *directory,
                                                            const char *name);

t_error    path_toDirectoryAndName             (char *dest,
                                                            size_t size,
                                                            t_symbol **directory,
                                                            t_symbol **name);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // M_PATH_H_
