
/* Copyright (c) 2021 Nicolas Danet. */

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
  description:        GUI of Spaghettis.
  website:            https://github.com/Spaghettis
  license:            BSD

  dependencies:       spaghettis_core juce_gui_basics
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

#include "GUI/Application/Colours.hpp"
#include "GUI/Application/Commands.hpp"
#include "GUI/Application/Identifiers.hpp"
#include "GUI/Application/LookAndFeel.hpp"
#include "GUI/Application/MenuModel.hpp"
#include "GUI/Application/SpaghettisInstance.hpp"
#include "GUI/Application/Spaghettis.hpp"
#include "GUI/Application/IconsShared.hpp"
#include "GUI/Application/IconsFactory.hpp"
#include "GUI/Application/ApplicationComponent.hpp"
#include "GUI/Application/ApplicationWindow.hpp"

#include "GUI/Console/ConsoleFactory.hpp"
#include "GUI/Console/ConsoleComponent.hpp"
#include "GUI/Console/Console.hpp"

#include "GUI/SearchPaths/SearchPathsFactory.hpp"
#include "GUI/SearchPaths/SearchPathsComponent.hpp"
#include "GUI/SearchPaths/SearchPaths.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
