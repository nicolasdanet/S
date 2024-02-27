
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

#include "GUI/Maker/Maker.hpp"
#include "GUI/Maker/MakerEntry.hpp"
#include "GUI/Maker/MakerList.hpp"
#include "GUI/Maker/MakerEmpty.hpp"
#include "GUI/Maker/MakerDocumentation.hpp"
#include "GUI/Maker/MakerComponent.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Patches/PatchReport.hpp"
#include "GUI/Patches/PatchRoot.hpp"
#include "GUI/Patches/PatchesHolder.hpp"
#include "GUI/Patches/PatchBase.hpp"
#include "GUI/Patches/PatchWindow.hpp"
#include "GUI/Patches/PatchView.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Patches/Graphics/PainterStrategy.hpp"
#include "GUI/Patches/Graphics/Painter.hpp"
#include "GUI/Patches/Graphics/PainterHelpers.hpp"

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

#include "GUI/Patches/Graphics/Components/MouseHandler.hpp"
#include "GUI/Patches/Graphics/Components/Scalable.hpp"
#include "GUI/Patches/Graphics/Components/PinComponent.hpp"
#include "GUI/Patches/Graphics/Components/ObjectComponent.hpp"
#include "GUI/Patches/Graphics/Components/LineComponent.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Patches/Windows/Table.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Patches/Windows/EditWindow/Helpers/Broadcast.hpp"
#include "GUI/Patches/Windows/EditWindow/Helpers/ContextMenu.hpp"
#include "GUI/Patches/Windows/EditWindow/Helpers/Synchronizer.hpp"
#include "GUI/Patches/Windows/EditWindow/Helpers/Locator.hpp"
#include "GUI/Patches/Windows/EditWindow/Helpers/InspectorView.hpp"
#include "GUI/Patches/Windows/EditWindow/Helpers/ZoomSteps.hpp"

#include "GUI/Patches/Windows/EditWindow/Helpers/DragStrategy.hpp"
#include "GUI/Patches/Windows/EditWindow/Helpers/Actions/ActionConnectPinTracker.hpp"
#include "GUI/Patches/Windows/EditWindow/Helpers/Actions/ActionConnectComponent.hpp"
#include "GUI/Patches/Windows/EditWindow/Helpers/Actions/ActionConnect.hpp"
#include "GUI/Patches/Windows/EditWindow/Helpers/Actions/ActionLassoComponent.hpp"
#include "GUI/Patches/Windows/EditWindow/Helpers/Actions/ActionLasso.hpp"
#include "GUI/Patches/Windows/EditWindow/Helpers/Actions/ActionHand.hpp"
#include "GUI/Patches/Windows/EditWindow/Helpers/Actions/ActionMove.hpp"
#include "GUI/Patches/Windows/EditWindow/Helpers/Actions/ActionResize.hpp"

#include "GUI/Patches/Windows/EditWindow/EditFactory.hpp"
#include "GUI/Patches/Windows/EditWindow/EditView.hpp"
#include "GUI/Patches/Windows/EditWindow/EditPort.hpp"
#include "GUI/Patches/Windows/EditWindow/EditInspector.hpp"
#include "GUI/Patches/Windows/EditWindow/EditZoom.hpp"
#include "GUI/Patches/Windows/EditWindow/EditComponent.hpp"
#include "GUI/Patches/Windows/EditWindow/EditWindow.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Patches/Windows/RunWindow/RunFactory.hpp"
#include "GUI/Patches/Windows/RunWindow/RunLayoutElement.hpp"
#include "GUI/Patches/Windows/RunWindow/RunLayout.hpp"
#include "GUI/Patches/Windows/RunWindow/RunView.hpp"
#include "GUI/Patches/Windows/RunWindow/RunPresets.hpp"
#include "GUI/Patches/Windows/RunWindow/RunComponent.hpp"
#include "GUI/Patches/Windows/RunWindow/RunWindow.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
