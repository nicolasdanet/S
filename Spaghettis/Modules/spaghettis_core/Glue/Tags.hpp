
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis::core::Keywords {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define DECLARE_STRING(name)    constexpr char name[] = #name

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

    DECLARE_STRING (Colors);
    DECLARE_STRING (Editing);
    DECLARE_STRING (General);
        
    DECLARE_STRING (AskBeforeQuit);
    DECLARE_STRING (BoxBackground);
    DECLARE_STRING (BoxText);
    DECLARE_STRING (BangBackground);
    DECLARE_STRING (BangFlashOff);
    DECLARE_STRING (BangFlashOn);
    DECLARE_STRING (DefaultIsRunView);
    DECLARE_STRING (GridSize);
    DECLARE_STRING (PatchBackground);
    DECLARE_STRING (SnapToGrid);
    
    DECLARE_STRING (Attributes);
    DECLARE_STRING (Parameters);
    
    DECLARE_STRING (Buffer);
    DECLARE_STRING (Class);
    DECLARE_STRING (EditView);
    DECLARE_STRING (Flashed);
    DECLARE_STRING (FlashTime);
    DECLARE_STRING (Inlets);
    DECLARE_STRING (Outlets);
    DECLARE_STRING (Path);
    DECLARE_STRING (RunView);
    DECLARE_STRING (Selected);
    DECLARE_STRING (Title);
    DECLARE_STRING (Visible);
    DECLARE_STRING (Width);
    DECLARE_STRING (X);
    DECLARE_STRING (Y);
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#undef DECLARE_STRING

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace Tags = spaghettis::core::Keywords;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
