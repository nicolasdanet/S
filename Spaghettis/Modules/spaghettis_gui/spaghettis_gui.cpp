
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "spaghettis_gui.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Base/BaseComponent.cpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Instance/Instance.cpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Menus/MenuModel.cpp"
#include "GUI/Menus/CommandsManager.cpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Console/ConsoleFactory.cpp"
#include "GUI/Console/ConsoleComponent.cpp"
#include "GUI/Console/ConsoleConverter.cpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Patches/WindowBounds.cpp"
#include "GUI/Patches/PatchRoot.cpp"
#include "GUI/Patches/PatchesHolder.cpp"
#include "GUI/Patches/View.cpp"

#include "GUI/Patches/Graphics/Dragable.cpp"
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

#include "GUI/Patches/Maker/Maker.cpp"
#include "GUI/Patches/Maker/MakerEntry.cpp"
#include "GUI/Patches/Maker/MakerList.cpp"
#include "GUI/Patches/Maker/MakerEmpty.cpp"
#include "GUI/Patches/Maker/MakerDocumentation.cpp"
#include "GUI/Patches/Maker/MakerComponent.cpp"

#include "GUI/Patches/EditWindow/EditFactory.cpp"
#include "GUI/Patches/EditWindow/Actions/PinTracker.cpp"
#include "GUI/Patches/EditWindow/Actions/ActionConnectComponent.cpp"
#include "GUI/Patches/EditWindow/Actions/ActionConnect.cpp"
#include "GUI/Patches/EditWindow/Actions/ActionLassoComponent.cpp"
#include "GUI/Patches/EditWindow/Actions/ActionLasso.cpp"
#include "GUI/Patches/EditWindow/Actions/ActionHand.cpp"
#include "GUI/Patches/EditWindow/Actions/ActionMove.cpp"
#include "GUI/Patches/EditWindow/Actions/ActionResize.cpp"
#include "GUI/Patches/EditWindow/ContextMenu.cpp"
#include "GUI/Patches/EditWindow/Sync.cpp"
#include "GUI/Patches/EditWindow/EditView.cpp"
#include "GUI/Patches/EditWindow/Locator.cpp"
#include "GUI/Patches/EditWindow/EditPort.cpp"
#include "GUI/Patches/EditWindow/InspectorView.cpp"
#include "GUI/Patches/EditWindow/EditInspector.cpp"
#include "GUI/Patches/EditWindow/EditZoom.cpp"
#include "GUI/Patches/EditWindow/EditComponent.cpp"
#include "GUI/Patches/EditWindow/EditWindow.cpp"

#include "GUI/Patches/RunWindow/RunFactory.cpp"
#include "GUI/Patches/RunWindow/RunLayout.cpp"
#include "GUI/Patches/RunWindow/RunView.cpp"
#include "GUI/Patches/RunWindow/RunPresets.cpp"
#include "GUI/Patches/RunWindow/RunComponent.cpp"
#include "GUI/Patches/RunWindow/RunWindow.cpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Preferences/Preferences.cpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/SearchPaths/SearchPathsFactory.cpp"
#include "GUI/SearchPaths/SearchPathsComponent.cpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "Wrap/Outputs.cpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
