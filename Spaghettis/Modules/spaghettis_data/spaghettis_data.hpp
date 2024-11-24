
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

#include "View/Parameters/Editors/ColourSlider.hpp"
#include "View/Parameters/Editors/ColourSpace.hpp"
#include "View/Parameters/Editors/HueSelector.hpp"
#include "View/Parameters/Editors/ColourSelector.hpp"
#include "View/Parameters/Editors/ColourEditor.hpp"
#include "View/Parameters/Editors/ChoicesSelector.hpp"
#include "View/Parameters/Editors/ChoicesList.hpp"
#include "View/Parameters/Editors/DirectoryEditor.hpp"
#include "View/Parameters/Editors/DirectorySelector.hpp"
#include "View/Parameters/Editors/CodeEditor.hpp"
#include "View/Parameters/Editors/DocumentationEditor.hpp"

#include "View/Parameters/ParameterBoolean.hpp"
#include "View/Parameters/ParameterColour.hpp"
#include "View/Parameters/ParameterNumber.hpp"
#include "View/Parameters/ParameterSlider.hpp"
#include "View/Parameters/ParameterString.hpp"
#include "View/Parameters/ParameterCode.hpp"
#include "View/Parameters/ParameterDirectory.hpp"
#include "View/Parameters/ParameterChoices.hpp"
#include "View/Parameters/ParameterDocumentation.hpp"
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
