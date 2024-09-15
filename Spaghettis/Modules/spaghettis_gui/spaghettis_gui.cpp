
/* Copyright (c) 2021 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "spaghettis_gui.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Menus/MenuModel.cpp"
#include "GUI/Menus/CommandsHandler.cpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Instance/Instance.cpp"
#include "GUI/Instance/Preferences.cpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Console/ConsoleFactory.cpp"
#include "GUI/Console/ConsoleComponent.cpp"
#include "GUI/Console/ConsoleConverter.cpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/SearchPaths/SearchPathsFactory.cpp"
#include "GUI/SearchPaths/SearchPathsComponent.cpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Maker/Maker.cpp"
#include "GUI/Maker/MakerEntry.cpp"
#include "GUI/Maker/MakerList.cpp"
#include "GUI/Maker/MakerEmpty.cpp"
#include "GUI/Maker/MakerDocumentation.cpp"
#include "GUI/Maker/MakerComponent.cpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Patches/Presets/PresetsAddresses.cpp"
#include "GUI/Patches/Presets/AbsoluteToLocal.cpp"
#include "GUI/Patches/Presets/LocalToAbsolute.cpp"
#include "GUI/Patches/Presets/PresetsResolver.cpp"
#include "GUI/Patches/Presets/PresetsManager.cpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Patches/PatchRoot.cpp"
#include "GUI/Patches/PatchRootReport.cpp"
#include "GUI/Patches/PatchesHolder.cpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Patches/Graphics/PainterStrategy.cpp"
#include "GUI/Patches/Graphics/PainterHelpers.cpp"
#include "GUI/Patches/Graphics/Painter.cpp"

#include "GUI/Patches/Graphics/Painters/ArrayPainter.cpp"
#include "GUI/Patches/Graphics/Painters/AtomPainter.cpp"
#include "GUI/Patches/Graphics/Painters/BangPainter.cpp"
#include "GUI/Patches/Graphics/Painters/BoxPainter.cpp"
#include "GUI/Patches/Graphics/Painters/CommentPainter.cpp"
#include "GUI/Patches/Graphics/Painters/DialPainter.cpp"
#include "GUI/Patches/Graphics/Painters/InletPainter.cpp"
#include "GUI/Patches/Graphics/Painters/MessagePainter.cpp"
#include "GUI/Patches/Graphics/Painters/SliderPainter.cpp"
#include "GUI/Patches/Graphics/Painters/RadioPainter.cpp"
#include "GUI/Patches/Graphics/Painters/TogglePainter.cpp"
#include "GUI/Patches/Graphics/Painters/VuPainter.cpp"

#include "GUI/Patches/Graphics/Components/MouseHandler.cpp"
#include "GUI/Patches/Graphics/Components/PinComponent.cpp"
#include "GUI/Patches/Graphics/Components/ObjectComponent.cpp"
#include "GUI/Patches/Graphics/Components/ObjectComponentUpdate.cpp"
#include "GUI/Patches/Graphics/Components/LineComponent.cpp"
#include "GUI/Patches/Graphics/Components/LineComponentUpdate.cpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Patches/Windows/EditWindow/Helpers/ContextMenu.cpp"
#include "GUI/Patches/Windows/EditWindow/Helpers/Synchronizer.cpp"
#include "GUI/Patches/Windows/EditWindow/Helpers/Locator.cpp"
#include "GUI/Patches/Windows/EditWindow/Helpers/InspectorView.cpp"

#include "GUI/Patches/Windows/EditWindow/Helpers/Actions/ActionConnectPinTracker.cpp"
#include "GUI/Patches/Windows/EditWindow/Helpers/Actions/ActionConnectComponent.cpp"
#include "GUI/Patches/Windows/EditWindow/Helpers/Actions/ActionConnect.cpp"
#include "GUI/Patches/Windows/EditWindow/Helpers/Actions/ActionLassoComponent.cpp"
#include "GUI/Patches/Windows/EditWindow/Helpers/Actions/ActionLasso.cpp"
#include "GUI/Patches/Windows/EditWindow/Helpers/Actions/ActionHand.cpp"
#include "GUI/Patches/Windows/EditWindow/Helpers/Actions/ActionMove.cpp"
#include "GUI/Patches/Windows/EditWindow/Helpers/Actions/ActionResize.cpp"

#include "GUI/Patches/Windows/EditWindow/EditFactory.cpp"
#include "GUI/Patches/Windows/EditWindow/EditView.cpp"
#include "GUI/Patches/Windows/EditWindow/EditPort.cpp"
#include "GUI/Patches/Windows/EditWindow/EditInspector.cpp"
#include "GUI/Patches/Windows/EditWindow/EditZoom.cpp"
#include "GUI/Patches/Windows/EditWindow/EditComponent.cpp"
#include "GUI/Patches/Windows/EditWindow/EditWindow.cpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Patches/Windows/RunWindow/RunFactory.cpp"
#include "GUI/Patches/Windows/RunWindow/RunLayout.cpp"
#include "GUI/Patches/Windows/RunWindow/RunView.cpp"
#include "GUI/Patches/Windows/RunWindow/RunPresets.cpp"
#include "GUI/Patches/Windows/RunWindow/RunComponent.cpp"
#include "GUI/Patches/Windows/RunWindow/RunWindow.cpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "Wrap/Outputs.cpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
