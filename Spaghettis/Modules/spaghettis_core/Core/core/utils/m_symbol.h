
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef M_SYMBOL_H_
#define M_SYMBOL_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_symbol    *symbol_withAtoms               (int argc, t_atom *argv);

int         symbol_containsWhitespace       (t_symbol *s);
int         symbol_hasExtension             (t_symbol *s, const char *);

t_symbol    *symbol_replaceDoubleDollar     (t_symbol *s);
t_symbol    *symbol_removeExtension         (t_symbol *s);
t_symbol    *symbol_appendExtensionPatch    (t_symbol *s);
t_symbol    *symbol_appendExtensionHelp     (t_symbol *s);

t_symbol    *symbol_addPrefix               (t_symbol *s, t_symbol *prefix);
t_symbol    *symbol_addSuffix               (t_symbol *s, t_symbol *suffix);
t_symbol    *symbol_removeCopySuffix        (t_symbol *s);
t_symbol    *symbol_appendCopySuffix        (t_symbol *s);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // M_SYMBOL_H_
