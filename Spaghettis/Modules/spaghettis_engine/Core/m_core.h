
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef M_CORE_H_
#define M_CORE_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef t_float64Atomic t_word;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Order of inclusion matters. */

#include "core/m_externals.h"
#include "core/m_macros.h"
#include "core/m_punning.h"
#include "core/m_float.h"
#include "core/m_symbols.h"
#include "core/helpers/m_point.h"
#include "core/helpers/m_rectangle.h"
#include "core/helpers/m_fileproperties.h"
#include "core/helpers/m_iterator.h"
#include "core/helpers/m_pathlist.h"
#include "core/helpers/m_heapstring.h"
#include "core/helpers/m_items.h"
#include "core/helpers/m_MT.h"
#include "core/helpers/m_ringbuffer.h"
#include "core/clipboard/m_clipboard.h"
#include "core/clipboard/m_encapsulate.h"
#include "core/instance/m_pool.h"
#include "core/instance/m_environment.h"
#include "core/instance/m_register.h"
#include "core/instance/m_clocks.h"
#include "core/instance/m_abstractions.h"
#include "core/instance/m_instance.h"
#include "core/class/m_class.h"
#include "core/class/m_object.h"
#include "core/class/m_inlet.h"
#include "core/class/m_outlet.h"
#include "core/buffer/m_buffer.h"
#include "core/utils/m_utils.h"
#include "core/utils/m_symbol.h"
#include "core/utils/m_math.h"
#include "core/utils/m_string.h"
#include "core/utils/m_path.h"
#include "core/utils/m_error.h"


// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "undo/m_undo.h"
#include "undo/m_undoactions.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Avoid typing. */

extern t_class *bindlist_class;
extern t_class *block_class;
extern t_class *canvas_class;
extern t_class *catch_tilde_class;
extern t_class *delwrite_tilde_class;
extern t_class *floatinlet_class;
extern t_class *garray_class;
extern t_class *inlet_class;
extern t_class *send_tilde_class;
extern t_class *symbolinlet_class;
extern t_class *comment_class;
extern t_class *textdefine_class;
extern t_class *vinlet_class;
extern t_class *voutlet_class;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "s_system.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static inline void w_setFloat (t_word *w, t_float f)
{
    atomic_float64WriteRelaxed (w, f);
}

static inline t_float w_getFloat (t_word *w)
{
    return atomic_float64ReadRelaxed (w);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "g_graphic.h"
#include "d_dsp.h"
#include "w_wrap.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // M_CORE_H_
