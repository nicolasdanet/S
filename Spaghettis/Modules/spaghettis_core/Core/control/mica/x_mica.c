
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "x_mica.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if PD_WITH_BELLE

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

t_class *concept_class;             /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _concept {
    t_pd        x_pd;               /* Must be the first. */
    t_symbol    *x_name;
    mica::UUID  x_uuid;
    } t_concept;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_concept *concept_new (t_symbol *s, mica::UUID uuid)
{
    t_concept *x = (t_concept *)pd_new (concept_class);
    
    x->x_name = s;
    x->x_uuid = uuid;
    
    pd_bind (cast_pd (x), x->x_name);
    
    return x;
}

static void concept_free (t_concept *x)
{
    pd_unbind (cast_pd (x), x->x_name);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_symbol *concept_tag (mica::Concept c)
{
    std::string t (sym___arrobe__->s_name); t += mica::Concept::asHex (c);
    
    t_symbol *s = gensym (t.c_str());
    
    if (!symbol_getThingByClass (s, concept_class)) { concept_new (s, mica::Concept::asUUID (c)); }
    
    return s;
}

PD_LOCAL t_symbol *concept_tagParsed (int argc, t_atom *argv)
{
    mica::Concept c;
    
    if (argc) {
    //
    if (argc == 1 && IS_FLOAT (argv)) {
        c = mica::Concept ((prim::int64)GET_FLOAT (argv));
    } else if (argc == 2 && IS_FLOAT (argv) && IS_FLOAT (argv + 1)) {
        c = mica::Concept (prim::Ratio ((prim::int64)GET_FLOAT (argv), (prim::int64)GET_FLOAT (argv + 1)));
    } else {
        char *t = atom_atomsToString (argc, argv);
        c = mica::Concept (std::string (t));
        PD_MEMORY_FREE (t);
    }
    //
    }

    return concept_tag (c);
}

PD_LOCAL mica::Concept concept_fetch (t_symbol *s)
{
    if (s != &s_) {
    //
    t_concept *concept = (t_concept *)symbol_getThingByClass (s, concept_class);

    if (concept) { return mica::Concept (concept->x_uuid); }
    else {
    //
    /* It is allowed to store tag as symbol in a patch. */
    /* The tag is unique and deterministic for each concept. */
    
    if (string_startWith (s->s_name, sym___arrobe__->s_name)) {
        mica::Concept t (mica::UUID::withHex (std::string (s->s_name + 1)));
        if (t.isValid()) { concept_tag (t); return t; }
    }
    //
    }
    //
    }
    
    return mica::Concept();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void concept_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_concept,
            NULL,
            (t_method)concept_free,
            sizeof (t_concept),
            CLASS_INVISIBLE,
            A_NULL);
    
    concept_class = c;
}

PD_LOCAL void concept_destroy (void)
{
    class_free (concept_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *mica_makeObject (t_symbol *s, int argc, t_atom *argv)
{
    t_pd *newest = NULL;
    
    instance_objectSetNewest (NULL);
    
    if (!argc) { newest = (t_pd *)micaset_new (s, 0, NULL); }
    else {
    //
    t_symbol *t = atom_getSymbolAtIndex (0, argc, argv);
    
    if (t == sym_set)               { newest = (t_pd *)micaset_new (s,      argc - 1, argv + 1); }
    else if (t == sym_get)          { newest = (t_pd *)micaget_new (s,      argc - 1, argv + 1); }
    else if (t == sym_info)         { newest = (t_pd *)micainfo_new (s,     argc - 1, argv + 1); }
    else if (t == sym_sequence)     { newest = (t_pd *)micasequence_new (s, argc - 1, argv + 1); }
    else if (t == sym_map)          { newest = (t_pd *)micamap_new (s,      argc - 1, argv + 1); }
    else if (t == sym_index)        { newest = (t_pd *)micaindex_new (s,    argc - 1, argv + 1); }
    else if (t == sym_item)         { newest = (t_pd *)micaitem_new (s,     argc - 1, argv + 1); }
    else if (t == sym_interval)     { newest = (t_pd *)micainterval_new (s, argc - 1, argv + 1); }
    else if (t == sym_spell)        { newest = (t_pd *)micaspell_new (s,    argc - 1, argv + 1); }
    else {
        newest = (t_pd *)micaset_new (s, argc, argv);
    }
    //
    }
    
    instance_objectSetNewest (newest);
    
    return newest;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void mica_setup (void)
{
    class_addCreator ((t_newmethod)mica_makeObject, sym_mica, A_GIMME, A_NULL);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#else

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void concept_setup (void)      { }
PD_LOCAL void concept_destroy (void)    { }
PD_LOCAL void mica_setup (void)         { }

#endif // PD_WITH_BELLE

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
