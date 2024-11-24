
/* Copyright (c) 2023 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

// ====================================

/*************************************************************************************************************

 BEGIN_JUCE_MODULE_DECLARATION

  ID:                 spaghettis_base
  vendor:             Spaghettis
  version:            0.9
  name:               Base
  description:        Look&Feel
  website:            https://github.com/Spaghettis
  license:            AGPLv3

  dependencies:       juce_core juce_data_structures juce_events juce_graphics juce_gui_basics juce_gui_extra
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

#include "juce_core/juce_core.h"
#include "juce_data_structures/juce_data_structures.h"
#include "juce_events/juce_events.h"
#include "juce_gui_basics/juce_gui_basics.h"
#include "juce_gui_extra/juce_gui_extra.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if JUCE_MAC
    
#define SPAGHETTIS_MENUBAR      0

#else

#define SPAGHETTIS_MENUBAR      1

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "Helpers/RadioButton.hpp"
#include "Helpers/ChoicesSource.hpp"
#include "Helpers/AvailableDevices.hpp"
#include "Helpers/Colour.hpp"
#include "Helpers/Normalized.hpp"
#include "Helpers/PropertyLookAndFeel.hpp"
#include "Helpers/Strings.hpp"
#include "Helpers/WeakPointer.hpp"
#include "Helpers/SafeRegister.hpp"
#include "Helpers/AlertWindowRegister.hpp"
#include "Helpers/CallOutBoxRegister.hpp"
#include "Helpers/CallOutBoxTracker.hpp"
#include "Helpers/EditDistance.hpp"
#include "Helpers/Device.hpp"
#include "Helpers/Directory.hpp"
#include "Helpers/CodeBlock.hpp"
#include "Helpers/InfoBlock.hpp"
#include "Helpers/Autocomplete.hpp"
#include "Helpers/WindowsProperties.hpp"
#include "Helpers/Mouse.hpp"
#include "Helpers/Resizer.hpp"
#include "Helpers/ToolbarComponent.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "LookAndFeel/FontsElement.hpp"
#include "LookAndFeel/Fonts.hpp"
#include "LookAndFeel/Palette.hpp"
#include "LookAndFeel/Colours.hpp"
#include "LookAndFeel/Directories.hpp"
#include "LookAndFeel/LookAndFeel.hpp"
#include "LookAndFeel/SliderLabel.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "Helpers/VariantConverter.hpp"
#include "Helpers/ListBoxFunctions.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "Identifiers/Identifiers.hpp"
#include "Identifiers/Keywords.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "Icons/IconsElement.hpp"
#include "Icons/Icons.hpp"
#include "Icons/IconsButton.hpp"
#include "Icons/IconsFactory.hpp"
#include "Icons/IconsFactoryHelper.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "Base/MenuCommand.hpp"
#include "Base/BaseComponentFocus.hpp"
#include "Base/BaseComponentToolbar.hpp"
#include "Base/BaseWindow.hpp"
#include "Base/BaseCommands.hpp"
#include "Base/BaseComponent.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
