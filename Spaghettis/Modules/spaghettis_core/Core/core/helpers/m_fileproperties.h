
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef M_FILEPROPERTIES_H_
#define M_FILEPROPERTIES_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define FILEPROPERTIES_EXTERNAL     1
#define FILEPROPERTIES_ABSTRACTION  2

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _fileproperties {
    char        f_directory[PD_STRING]; char *f_name;
    char        f_flag;
    t_symbol    *f_key;
    } t_fileproperties;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error fileproperties_set (t_fileproperties *p, char *filepath);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static inline void fileproperties_init (t_fileproperties *p)
{
    p->f_directory[0] = 0; p->f_name = p->f_directory; p->f_flag = 0; p->f_key = NULL;
}

static inline void fileproperties_clear (t_fileproperties *p)
{
    p->f_directory[0] = 0; p->f_name = p->f_directory;
}

static inline char *fileproperties_getDirectory (t_fileproperties *p)
{
    return p->f_directory;
}

static inline char *fileproperties_getName (t_fileproperties *p)
{
    return p->f_name;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static inline void fileproperties_initExternal (t_fileproperties *p, t_symbol *name)
{
    fileproperties_init (p); p->f_flag = FILEPROPERTIES_EXTERNAL; p->f_key = name;
}

static inline void fileproperties_initAbstraction (t_fileproperties *p, t_symbol *name)
{
    fileproperties_init (p); p->f_flag = FILEPROPERTIES_ABSTRACTION; p->f_key = name;
}

static inline int fileproperties_isExternal (t_fileproperties *p)
{
    return (p->f_flag == FILEPROPERTIES_EXTERNAL);
}

static inline int fileproperties_isAbstraction (t_fileproperties *p)
{
    return (p->f_flag == FILEPROPERTIES_ABSTRACTION);
}

static inline t_symbol *fileproperties_getKey (t_fileproperties *p)
{
    PD_ASSERT (p->f_key); return p->f_key;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // M_FILEPROPERTIES_H_
