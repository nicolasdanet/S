
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

PD_LOCAL t_symbol   *symbol_withAtoms               (int argc, t_atom *argv);

PD_LOCAL int        symbol_containsWhitespace       (t_symbol *s);

PD_LOCAL t_symbol   *symbol_replaceDoubleDollar     (t_symbol *s);
PD_LOCAL t_symbol   *symbol_removeExtension         (t_symbol *s);
PD_LOCAL t_symbol   *symbol_appendExtensionPatch    (t_symbol *s);
PD_LOCAL t_symbol   *symbol_appendExtensionHelp     (t_symbol *s);

PD_LOCAL t_symbol   *symbol_addPrefix               (t_symbol *s, t_symbol *prefix);
PD_LOCAL t_symbol   *symbol_addSuffix               (t_symbol *s, t_symbol *suffix);
PD_FORCE t_symbol   *symbol_appendCopySuffix        (t_symbol *s);
PD_LOCAL t_symbol   *symbol_removeCopySuffix        (t_symbol *s);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // M_SYMBOL_H_
