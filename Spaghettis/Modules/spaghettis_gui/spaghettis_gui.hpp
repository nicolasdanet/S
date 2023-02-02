
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

  dependencies:       spaghettis_core
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

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if JUCE_MAC
    
#define SPAGHETTIS_MENUBAR      0

#else

#define SPAGHETTIS_MENUBAR      1

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Application/Helpers/Helpers.hpp"
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

#include "GUI/Application/Parameters/ParametersHeader.hpp"
#include "GUI/Application/Parameters/ParametersView.hpp"
#include "GUI/Application/Parameters/ParametersWidth.hpp"
#include "GUI/Application/Parameters/ParametersBoolean.hpp"
#include "GUI/Application/Parameters/ParametersColour.hpp"
#include "GUI/Application/Parameters/ParametersNumber.hpp"
#include "GUI/Application/Parameters/ParametersText.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Application/Autocomplete.hpp"
#include "GUI/Application/Commands.hpp"
#include "GUI/Application/Documentation.hpp"
#include "GUI/Application/Icons.hpp"
#include "GUI/Application/MenuModel.hpp"
#include "GUI/Application/Preferences.hpp"
#include "GUI/Application/Instance.hpp"
#include "GUI/Application/Spaghettis.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Application/Helpers/IconsButton.hpp"
#include "GUI/Application/Helpers/IconsFactory.hpp"
#include "GUI/Application/Helpers/IconsFactoryHelper.hpp"
#include "GUI/Application/Helpers/ListBoxFunctions.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Application/Base/MenuCommand.hpp"
#include "GUI/Application/Base/BaseCommands.hpp"
#include "GUI/Application/Base/BaseComponent.hpp"
#include "GUI/Application/Base/BaseWindow.hpp"
 
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Console/ConsoleFactory.hpp"
#include "GUI/Console/ConsoleComponent.hpp"
#include "GUI/Console/ConsoleWindow.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Devices/DevicesFactory.hpp"
#include "GUI/Devices/DevicesComponent.hpp"
#include "GUI/Devices/DevicesWindow.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Patches/Mouse.hpp"
#include "GUI/Patches/PatchRoot.hpp"
#include "GUI/Patches/PatchesHolder.hpp"
#include "GUI/Patches/PatchWindow.hpp"
#include "GUI/Patches/Resizer.hpp"
#include "GUI/Patches/View.hpp"

#include "GUI/Patches/Graphics/Scalable.hpp"
#include "GUI/Patches/Graphics/Table.hpp"
#include "GUI/Patches/Graphics/PainterPolicy.hpp"
#include "GUI/Patches/Graphics/Painters/BangPainter.hpp"
#include "GUI/Patches/Graphics/Painters/BoxPainter.hpp"
#include "GUI/Patches/Graphics/PinComponent.hpp"
#include "GUI/Patches/Graphics/ObjectComponent.hpp"
#include "GUI/Patches/Graphics/LineComponent.hpp"

#include "GUI/Patches/Maker/Maker.hpp"
#include "GUI/Patches/Maker/MakerEntry.hpp"
#include "GUI/Patches/Maker/MakerDocumentation.hpp"
#include "GUI/Patches/Maker/MakerEditor.hpp"

#include "GUI/Patches/EditWindow/DragAction.hpp"
#include "GUI/Patches/EditWindow/EditCommands.hpp"
#include "GUI/Patches/EditWindow/EditFactory.hpp"
#include "GUI/Patches/EditWindow/ActionLassoComponent.hpp"
#include "GUI/Patches/EditWindow/ActionLasso.hpp"
#include "GUI/Patches/EditWindow/ActionHand.hpp"
#include "GUI/Patches/EditWindow/ActionMove.hpp"
#include "GUI/Patches/EditWindow/Sync.hpp"
#include "GUI/Patches/EditWindow/EditView.hpp"
#include "GUI/Patches/EditWindow/EditPort.hpp"
#include "GUI/Patches/EditWindow/InspectorView.hpp"
#include "GUI/Patches/EditWindow/EditInspector.hpp"
#include "GUI/Patches/EditWindow/EditZoom.hpp"
#include "GUI/Patches/EditWindow/EditComponent.hpp"
#include "GUI/Patches/EditWindow/EditWindow.hpp"

#include "GUI/Patches/RunWindow/RunFactory.hpp"
#include "GUI/Patches/RunWindow/RunLayout.hpp"
#include "GUI/Patches/RunWindow/RunView.hpp"
#include "GUI/Patches/RunWindow/RunPresets.hpp"
#include "GUI/Patches/RunWindow/RunComponent.hpp"
#include "GUI/Patches/RunWindow/RunWindow.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Preferences/PreferencesFactory.hpp"
#include "GUI/Preferences/PreferencesComponent.hpp"
#include "GUI/Preferences/PreferencesWindow.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/SearchPaths/SearchPathsFactory.hpp"
#include "GUI/SearchPaths/SearchPathsComponent.hpp"
#include "GUI/SearchPaths/SearchPathsWindow.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
