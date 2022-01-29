
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

// ====================================

/*************************************************************************************************************

 BEGIN_JUCE_MODULE_DECLARATION

  ID:                 spaghettis_gui
  vendor:             Spaghettis
  version:            0.9
  name:               GUI
  description:        GUI of Spaghettis.
  website:            https://github.com/Spaghettis
  license:            BSD

  dependencies:       spaghettis_core juce_graphics juce_gui_basics
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

#include "spaghettis_core/spaghettis_core.hpp"

#include "juce_gui_basics/juce_gui_basics.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if JUCE_MAC
    
#define SPAGHETTIS_MENUBAR      0

#else

#define SPAGHETTIS_MENUBAR      1

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Application/Helpers/SafeRegister.hpp"
#include "GUI/Application/Helpers/AlertWindowRegister.hpp"
#include "GUI/Application/Helpers/CallOutBoxRegister.hpp"
#include "GUI/Application/Helpers/CallOutBoxTracker.hpp"
#include "GUI/Application/Helpers/ColourSelector.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Application/LookAndFeel/Colours.hpp"
#include "GUI/Application/LookAndFeel/LookAndFeel.hpp"
#include "GUI/Application/LookAndFeel/SliderLabel.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Application/Commands.hpp"
#include "GUI/Application/Icons.hpp"
#include "GUI/Application/MenuModel.hpp"
#include "GUI/Application/Preferences.hpp"
#include "GUI/Application/Instance.hpp"
#include "GUI/Application/Spaghettis.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Preferences/Parameters.hpp"
#include "GUI/Preferences/ParametersBoolean.hpp"
#include "GUI/Preferences/ParametersColour.hpp"
#include "GUI/Preferences/ParametersNumber.hpp"
#include "GUI/Preferences/ParametersText.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Application/Helpers/PropertyHeader.hpp"
#include "GUI/Application/Helpers/IconsButton.hpp"
#include "GUI/Application/Helpers/IconsFactory.hpp"
#include "GUI/Application/Helpers/IconsFactoryHelper.hpp"
#include "GUI/Application/Helpers/ListBoxFunctions.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Application/BaseComponent.hpp"
#include "GUI/Application/BaseWindow.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Console/ConsoleFactory.hpp"
#include "GUI/Console/ConsoleComponent.hpp"
#include "GUI/Console/ConsoleWindow.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Devices/DevicesComponent.hpp"
#include "GUI/Devices/DevicesWindow.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Patches/Patch.hpp"
#include "GUI/Patches/PatchHolder.hpp"
#include "GUI/Patches/PatchWindow.hpp"

#include "GUI/Patches/EditWindow/EditFactory.hpp"
#include "GUI/Patches/EditWindow/EditComponent.hpp"
#include "GUI/Patches/EditWindow/EditView.hpp"
#include "GUI/Patches/EditWindow/EditWindow.hpp"
#include "GUI/Patches/RunWindow/RunFactory.hpp"
#include "GUI/Patches/RunWindow/RunComponent.hpp"
#include "GUI/Patches/RunWindow/RunView.hpp"
#include "GUI/Patches/RunWindow/RunWindow.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Preferences/PreferencesComponent.hpp"
#include "GUI/Preferences/PreferencesWindow.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/SearchPaths/SearchPathsFactory.hpp"
#include "GUI/SearchPaths/SearchPathsComponent.hpp"
#include "GUI/SearchPaths/SearchPathsWindow.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
