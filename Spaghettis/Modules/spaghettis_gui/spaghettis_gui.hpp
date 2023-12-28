
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
  description:        GUI
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

#include "GUI/Menus/Commands.hpp"
#include "GUI/Menus/MenuModel.hpp"
#include "GUI/Menus/CommandsHandler.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Instance/Preferences.hpp"
#include "GUI/Instance/Instance.hpp"
#include "GUI/Instance/Spaghettis.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Console/ConsoleFactory.hpp"
#include "GUI/Console/ConsoleComponent.hpp"
#include "GUI/Console/ConsoleWindow.hpp"
#include "GUI/Console/ConsoleConverter.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Devices/DevicesFactory.hpp"
#include "GUI/Devices/DevicesComponent.hpp"
#include "GUI/Devices/DevicesWindow.hpp"

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

#include "GUI/Patches/WindowBounds.hpp"
#include "GUI/Patches/PatchRoot.hpp"
#include "GUI/Patches/PatchesHolder.hpp"
#include "GUI/Patches/PatchWindow.hpp"
#include "GUI/Patches/Resizer.hpp"
#include "GUI/Patches/View.hpp"

#include "GUI/Patches/Graphics/PainterPolicy.hpp"
#include "GUI/Patches/Graphics/Dragable.hpp"
#include "GUI/Patches/Graphics/Scalable.hpp"
#include "GUI/Patches/Graphics/Table.hpp"
#include "GUI/Patches/Graphics/Painters/ArrayPainter.hpp"
#include "GUI/Patches/Graphics/Painters/AtomPainter.hpp"
#include "GUI/Patches/Graphics/Painters/BangPainter.hpp"
#include "GUI/Patches/Graphics/Painters/BoxPainter.hpp"
#include "GUI/Patches/Graphics/Painters/CommentPainter.hpp"
#include "GUI/Patches/Graphics/Painters/DialPainter.hpp"
#include "GUI/Patches/Graphics/Painters/InletPainter.hpp"
#include "GUI/Patches/Graphics/Painters/MessagePainter.hpp"
#include "GUI/Patches/Graphics/Painters/RadioPainter.hpp"
#include "GUI/Patches/Graphics/Painters/SliderPainter.hpp"
#include "GUI/Patches/Graphics/Painters/TogglePainter.hpp"
#include "GUI/Patches/Graphics/Painters/VuPainter.hpp"
#include "GUI/Patches/Graphics/PinComponent.hpp"
#include "GUI/Patches/Graphics/ObjectComponent.hpp"
#include "GUI/Patches/Graphics/LineComponent.hpp"

#include "GUI/Patches/Maker/Maker.hpp"
#include "GUI/Patches/Maker/MakerEntry.hpp"
#include "GUI/Patches/Maker/MakerList.hpp"
#include "GUI/Patches/Maker/MakerEmpty.hpp"
#include "GUI/Patches/Maker/MakerDocumentation.hpp"
#include "GUI/Patches/Maker/MakerComponent.hpp"

#include "GUI/Patches/EditWindow/DragAction.hpp"
#include "GUI/Patches/EditWindow/EditCommands.hpp"
#include "GUI/Patches/EditWindow/EditFactory.hpp"
#include "GUI/Patches/EditWindow/Actions/PinTracker.hpp"
#include "GUI/Patches/EditWindow/Actions/ActionConnectComponent.hpp"
#include "GUI/Patches/EditWindow/Actions/ActionConnect.hpp"
#include "GUI/Patches/EditWindow/Actions/ActionLassoComponent.hpp"
#include "GUI/Patches/EditWindow/Actions/ActionLasso.hpp"
#include "GUI/Patches/EditWindow/Actions/ActionHand.hpp"
#include "GUI/Patches/EditWindow/Actions/ActionMove.hpp"
#include "GUI/Patches/EditWindow/Actions/ActionResize.hpp"
#include "GUI/Patches/EditWindow/ContextMenu.hpp"
#include "GUI/Patches/EditWindow/Sync.hpp"
#include "GUI/Patches/EditWindow/EditView.hpp"
#include "GUI/Patches/EditWindow/Locator.hpp"
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
