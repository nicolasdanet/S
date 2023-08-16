
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "spaghettis_gui.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Application/Base/BaseCommands.cpp"
#include "GUI/Application/Base/BaseComponent.cpp"
#include "GUI/Application/Base/BaseWindow.cpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Application/Autocomplete.cpp"
#include "GUI/Application/Icons.cpp"
#include "GUI/Application/Documentation.cpp"
#include "GUI/Application/MenuModel.cpp"
#include "GUI/Application/Preferences.cpp"
#include "GUI/Application/Instance.cpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Application/LookAndFeel/LookAndFeel.cpp"
#include "GUI/Application/LookAndFeel/LookAndFeelAlertWindow.cpp"
#include "GUI/Application/LookAndFeel/LookAndFeelCallOutBox.cpp"
#include "GUI/Application/LookAndFeel/LookAndFeelComboBox.cpp"
#include "GUI/Application/LookAndFeel/LookAndFeelMaker.cpp"
#include "GUI/Application/LookAndFeel/LookAndFeelMenubar.cpp"
#include "GUI/Application/LookAndFeel/LookAndFeelPopup.cpp"
#include "GUI/Application/LookAndFeel/LookAndFeelPreferences.cpp"
#include "GUI/Application/LookAndFeel/LookAndFeelResizer.cpp"
#include "GUI/Application/LookAndFeel/LookAndFeelToolbar.cpp"
#include "GUI/Application/LookAndFeel/LookAndFeelTooltip.cpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Application/Parameters/ParametersHeader.cpp"
#include "GUI/Application/Parameters/ParametersView.cpp"
#include "GUI/Application/Parameters/ParametersColour.cpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Application/Helpers/CallOutBoxTracker.cpp"
#include "GUI/Application/Helpers/ListBoxFunctions.cpp"
#include "GUI/Application/Helpers/ColourSelector/ColourSpace.cpp"
#include "GUI/Application/Helpers/ColourSelector/HueSelector.cpp"
#include "GUI/Application/Helpers/ColourSelector/ColourSelector.cpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "GUI/Console/ConsoleFactory.cpp"
#include "GUI/Console/ConsoleComponent.cpp"

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
#include "GUI/Patches/Graphics/Painters/InletPainter.cpp"
#include "GUI/Patches/Graphics/Painters/MessagePainter.cpp"
#include "GUI/Patches/Graphics/Painters/TogglePainter.cpp"
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

#include "GUI/SearchPaths/SearchPathsFactory.cpp"
#include "GUI/SearchPaths/SearchPathsComponent.cpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "Wrap/Outputs.cpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
