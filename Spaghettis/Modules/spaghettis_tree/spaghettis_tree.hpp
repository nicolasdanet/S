
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

// ====================================

/*************************************************************************************************************

 BEGIN_JUCE_MODULE_DECLARATION

  ID:                 spaghettis_tree
  vendor:             Spaghettis
  version:            0.9
  name:               Tree
  description:        The data tree of Spaghettis.
  website:            https://github.com/Spaghettis
  license:            BSD

  dependencies:       juce_core juce_data_structures juce_events juce_graphics juce_gui_basics
  OSXFrameworks:
  OSXLibs:
  linuxLibs:

 END_JUCE_MODULE_DECLARATION

*************************************************************************************************************/

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#pragma once

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "juce_core/juce_core.h"
#include "juce_data_structures/juce_data_structures.h"
#include "juce_events/juce_events.h"
#include "juce_gui_basics/juce_gui_basics.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "Identifiers/Identifiers.hpp"
#include "Identifiers/Keywords.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "Data/Cast.hpp"
#include "Data/Iterator.hpp"
#include "Data/Invariant.hpp"
#include "Data/Delegate.hpp"
#include "Data/Filter.hpp"
#include "Data/Parameter.hpp"
#include "Data/Group.hpp"
#include "Data/Observer.hpp"
#include "Data/Data.hpp"
#include "Data/Cached.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "Helpers/Strings.hpp"
#include "Helpers/Colours.hpp"
#include "Helpers/WeakPointer.hpp"
#include "Helpers/SafeRegister.hpp"
#include "Helpers/AlertWindowRegister.hpp"
#include "Helpers/CallOutBoxRegister.hpp"
#include "Helpers/CallOutBoxTracker.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
