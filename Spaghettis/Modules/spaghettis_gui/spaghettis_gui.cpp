
/* Copyright (c) 2021 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "spaghettis_gui.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Instance/Menus/MenuModel.cpp"
#include "GUI/Instance/Menus/CommandsHandler.cpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Instance/Instance.cpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Instance/Preferences/Preferences.cpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Instance/Console/ConsoleFactory.cpp"
#include "GUI/Instance/Console/ConsoleComponent.cpp"
#include "GUI/Instance/Console/ConsoleConverter.cpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Tools/SearchPaths/SearchPathsFactory.cpp"
#include "GUI/Tools/SearchPaths/SearchPathsComponent.cpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Patches/Maker/Maker.cpp"
#include "GUI/Patches/Maker/MakerEntry.cpp"
#include "GUI/Patches/Maker/MakerList.cpp"
#include "GUI/Patches/Maker/MakerEmpty.cpp"
#include "GUI/Patches/Maker/MakerDocumentation.cpp"
#include "GUI/Patches/Maker/MakerComponent.cpp"

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

#include "GUI/Patches/Views/EditWindow/Helpers/Synchronizer.cpp"
#include "GUI/Patches/Views/EditWindow/Helpers/Locator.cpp"
#include "GUI/Patches/Views/EditWindow/Helpers/InspectorView.cpp"

#include "GUI/Patches/Views/EditWindow/Helpers/Actions/ActionConnectPinTracker.cpp"
#include "GUI/Patches/Views/EditWindow/Helpers/Actions/ActionConnectComponent.cpp"
#include "GUI/Patches/Views/EditWindow/Helpers/Actions/ActionConnect.cpp"
#include "GUI/Patches/Views/EditWindow/Helpers/Actions/ActionLassoComponent.cpp"
#include "GUI/Patches/Views/EditWindow/Helpers/Actions/ActionLasso.cpp"
#include "GUI/Patches/Views/EditWindow/Helpers/Actions/ActionHand.cpp"
#include "GUI/Patches/Views/EditWindow/Helpers/Actions/ActionMove.cpp"
#include "GUI/Patches/Views/EditWindow/Helpers/Actions/ActionResize.cpp"

#include "GUI/Patches/Views/EditWindow/EditFactory.cpp"
#include "GUI/Patches/Views/EditWindow/EditView.cpp"
#include "GUI/Patches/Views/EditWindow/EditPort.cpp"
#include "GUI/Patches/Views/EditWindow/EditInspector.cpp"
#include "GUI/Patches/Views/EditWindow/EditZoom.cpp"
#include "GUI/Patches/Views/EditWindow/EditComponent.cpp"
#include "GUI/Patches/Views/EditWindow/EditWindow.cpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Patches/Views/RunWindow/RunFactory.cpp"
#include "GUI/Patches/Views/RunWindow/RunLayout.cpp"
#include "GUI/Patches/Views/RunWindow/RunView.cpp"
#include "GUI/Patches/Views/RunWindow/RunPresets.cpp"
#include "GUI/Patches/Views/RunWindow/RunComponent.cpp"
#include "GUI/Patches/Views/RunWindow/RunWindow.cpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "Wrap/Outputs.cpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
