
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

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

#include "GUI/Patches/PatchBounds.cpp"
#include "GUI/Patches/PatchRoot.cpp"
#include "GUI/Patches/PatchesHolder.cpp"
#include "GUI/Patches/PatchView.cpp"

#include "GUI/Patches/Graphics/DragHandler.cpp"
#include "GUI/Patches/Graphics/PainterPolicy.cpp"
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
#include "GUI/Patches/Graphics/PinComponent.cpp"
#include "GUI/Patches/Graphics/ObjectComponent.cpp"
#include "GUI/Patches/Graphics/LineComponent.cpp"

#include "GUI/Patches/Windows/EditWindow/EditFactory.cpp"
#include "GUI/Patches/Windows/EditWindow/Actions/PinTracker.cpp"
#include "GUI/Patches/Windows/EditWindow/Actions/ActionConnectComponent.cpp"
#include "GUI/Patches/Windows/EditWindow/Actions/ActionConnect.cpp"
#include "GUI/Patches/Windows/EditWindow/Actions/ActionLassoComponent.cpp"
#include "GUI/Patches/Windows/EditWindow/Actions/ActionLasso.cpp"
#include "GUI/Patches/Windows/EditWindow/Actions/ActionHand.cpp"
#include "GUI/Patches/Windows/EditWindow/Actions/ActionMove.cpp"
#include "GUI/Patches/Windows/EditWindow/Actions/ActionResize.cpp"
#include "GUI/Patches/Windows/EditWindow/ContextMenu.cpp"
#include "GUI/Patches/Windows/EditWindow/Sync.cpp"
#include "GUI/Patches/Windows/EditWindow/EditView.cpp"
#include "GUI/Patches/Windows/EditWindow/Locator.cpp"
#include "GUI/Patches/Windows/EditWindow/EditPort.cpp"
#include "GUI/Patches/Windows/EditWindow/InspectorView.cpp"
#include "GUI/Patches/Windows/EditWindow/EditInspector.cpp"
#include "GUI/Patches/Windows/EditWindow/EditZoom.cpp"
#include "GUI/Patches/Windows/EditWindow/EditComponent.cpp"
#include "GUI/Patches/Windows/EditWindow/EditWindow.cpp"

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
