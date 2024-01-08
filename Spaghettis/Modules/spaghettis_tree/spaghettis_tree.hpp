
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
  description:        Tree
  website:            https://github.com/Spaghettis
  license:            BSD

  dependencies:       spaghettis_base
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

#include "spaghettis_base/spaghettis_base.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "Data/Getters/Cast.hpp"
#include "Data/Getters/Iterator.hpp"
#include "Data/Getters/Filter.hpp"
#include "Data/Delegate/Invariant.hpp"
#include "Data/Delegate/Delegate.hpp"
#include "Data/Delegate/DelegateManager.hpp"
#include "Data/Delegate/DelegateCache.hpp"
#include "Data/ParameterType.hpp"
#include "Data/Parameter.hpp"
#include "Data/Group.hpp"
#include "Data/Getters/Observer.hpp"
#include "Data/Data.hpp"
#include "Data/Getters/Cached.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "View/ColourSlider.hpp"
#include "View/ColourSpace.hpp"
#include "View/HueSelector.hpp"
#include "View/ColourSelector.hpp"
#include "View/ParametersHeader.hpp"
#include "View/ParametersView.hpp"
#include "View/ParametersBoolean.hpp"
#include "View/ParametersColour.hpp"
#include "View/ParametersNumber.hpp"
#include "View/ParametersText.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
