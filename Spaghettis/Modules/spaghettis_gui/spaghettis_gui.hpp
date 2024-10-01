
/* Copyright (c) 2021 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

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
  license:            AGPLv3

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

#include "GUI/Instance/Menus/Commands.hpp"
#include "GUI/Instance/Menus/MenuModel.hpp"
#include "GUI/Instance/Menus/CommandsHandler.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Instance/Preferences/Preferences.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Instance/Instance.hpp"
#include "GUI/Instance/Spaghettis.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Instance/Console/ConsoleFactory.hpp"
#include "GUI/Instance/Console/ConsoleComponent.hpp"
#include "GUI/Instance/Console/ConsoleWindow.hpp"
#include "GUI/Instance/Console/ConsoleConverter.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Tools/Devices/DevicesFactory.hpp"
#include "GUI/Tools/Devices/DevicesComponent.hpp"
#include "GUI/Tools/Devices/DevicesWindow.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Instance/Preferences/PreferencesFactory.hpp"
#include "GUI/Instance/Preferences/PreferencesComponent.hpp"
#include "GUI/Instance/Preferences/PreferencesWindow.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Tools/SearchPaths/SearchPathsFactory.hpp"
#include "GUI/Tools/SearchPaths/SearchPathsComponent.hpp"
#include "GUI/Tools/SearchPaths/SearchPathsWindow.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Patches/Maker/Maker.hpp"
#include "GUI/Patches/Maker/MakerEntry.hpp"
#include "GUI/Patches/Maker/MakerList.hpp"
#include "GUI/Patches/Maker/MakerEmpty.hpp"
#include "GUI/Patches/Maker/MakerDocumentation.hpp"
#include "GUI/Patches/Maker/MakerComponent.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Patches/Broadcast.hpp"

#include "GUI/Patches/Presets/PresetsAddresses.hpp"
#include "GUI/Patches/Presets/AbsoluteToLocal.hpp"
#include "GUI/Patches/Presets/LocalToAbsolute.hpp"
#include "GUI/Patches/Presets/PresetsResolver.hpp"
#include "GUI/Patches/Presets/PresetsManager.hpp"

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

#include "GUI/Patches/Views/Table.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Patches/Views/EditWindow/Helpers/Synchronizer.hpp"
#include "GUI/Patches/Views/EditWindow/Helpers/Locator.hpp"
#include "GUI/Patches/Views/EditWindow/Helpers/InspectorView.hpp"
#include "GUI/Patches/Views/EditWindow/Helpers/ZoomSteps.hpp"

#include "GUI/Patches/Views/EditWindow/Helpers/DragStrategy.hpp"
#include "GUI/Patches/Views/EditWindow/Helpers/Actions/ActionConnectPinTracker.hpp"
#include "GUI/Patches/Views/EditWindow/Helpers/Actions/ActionConnectComponent.hpp"
#include "GUI/Patches/Views/EditWindow/Helpers/Actions/ActionConnect.hpp"
#include "GUI/Patches/Views/EditWindow/Helpers/Actions/ActionLassoComponent.hpp"
#include "GUI/Patches/Views/EditWindow/Helpers/Actions/ActionLasso.hpp"
#include "GUI/Patches/Views/EditWindow/Helpers/Actions/ActionHand.hpp"
#include "GUI/Patches/Views/EditWindow/Helpers/Actions/ActionMove.hpp"
#include "GUI/Patches/Views/EditWindow/Helpers/Actions/ActionResize.hpp"

#include "GUI/Patches/Views/EditWindow/EditFactory.hpp"
#include "GUI/Patches/Views/EditWindow/EditView.hpp"
#include "GUI/Patches/Views/EditWindow/EditPort.hpp"
#include "GUI/Patches/Views/EditWindow/EditInspector.hpp"
#include "GUI/Patches/Views/EditWindow/EditZoom.hpp"
#include "GUI/Patches/Views/EditWindow/EditComponent.hpp"
#include "GUI/Patches/Views/EditWindow/EditWindow.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Patches/Views/RunWindow/RunFactory.hpp"
#include "GUI/Patches/Views/RunWindow/RunLayoutElement.hpp"
#include "GUI/Patches/Views/RunWindow/RunLayout.hpp"
#include "GUI/Patches/Views/RunWindow/RunView.hpp"
#include "GUI/Patches/Views/RunWindow/RunPresets.hpp"
#include "GUI/Patches/Views/RunWindow/RunComponent.hpp"
#include "GUI/Patches/Views/RunWindow/RunWindow.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
