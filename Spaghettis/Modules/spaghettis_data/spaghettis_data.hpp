
/* Copyright (c) 2023 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

// ====================================

/*************************************************************************************************************

 BEGIN_JUCE_MODULE_DECLARATION

  ID:                 spaghettis_data
  vendor:             Spaghettis
  version:            0.9
  name:               Data
  description:        Data
  website:            https://github.com/Spaghettis
  license:            AGPLv3

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

#include "Data/Getters/Update/Update.hpp"
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
#include "Data/Getters/Update/Painted.hpp"
#include "Data/Getters/Update/Resized.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "View/Parameters/ColourSlider.hpp"
#include "View/Parameters/ColourSpace.hpp"
#include "View/Parameters/HueSelector.hpp"
#include "View/Parameters/ColourSelector.hpp"
#include "View/Parameters/ColourEditor.hpp"
#include "View/Parameters/FolderEditor.hpp"
#include "View/Parameters/ParameterBoolean.hpp"
#include "View/Parameters/ParameterColour.hpp"
#include "View/Parameters/ParameterNumber.hpp"
#include "View/Parameters/ParameterSlider.hpp"
#include "View/Parameters/ParameterText.hpp"
#include "View/Parameters/ParameterFolder.hpp"
#include "View/Parameters/PresetElement.hpp"
#include "View/Parameters/ViewCommon.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "View/ParametersViewHeader.hpp"
#include "View/ParametersView.hpp"
#include "View/PresetsViewHeader.hpp"
#include "View/PresetsView.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
