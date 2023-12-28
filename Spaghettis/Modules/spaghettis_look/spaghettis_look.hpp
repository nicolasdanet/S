
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

// ====================================

/*************************************************************************************************************

 BEGIN_JUCE_MODULE_DECLARATION

  ID:                 spaghettis_look
  vendor:             Spaghettis
  version:            0.9
  name:               Look
  description:        Look
  website:            https://github.com/Spaghettis
  license:            BSD

  dependencies:       juce_core juce_data_structures juce_events juce_graphics juce_gui_basics
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

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if JUCE_MAC
    
#define SPAGHETTIS_MENUBAR      0

#else

#define SPAGHETTIS_MENUBAR      1

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "Helpers/Colour.hpp"
#include "Helpers/Normalized.hpp"
#include "Helpers/ParameterBase.hpp"
#include "Helpers/Strings.hpp"
#include "Helpers/WeakPointer.hpp"
#include "Helpers/SafeRegister.hpp"
#include "Helpers/AlertWindowRegister.hpp"
#include "Helpers/CallOutBoxRegister.hpp"
#include "Helpers/CallOutBoxTracker.hpp"
#include "Helpers/EditDistance.hpp"
#include "Helpers/Autocomplete.hpp"
#include "Helpers/WindowsProperties.hpp"
#include "Helpers/Mouse.hpp"
#include "Helpers/Resizer.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "LookAndFeel/Fonts.hpp"
#include "LookAndFeel/Palette.hpp"
#include "LookAndFeel/Colours.hpp"
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
