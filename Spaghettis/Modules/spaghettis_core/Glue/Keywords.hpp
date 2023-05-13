
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis::core::Keywords {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Note that keywords must be thread-safe. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define DECLARE_STRING(name)    constexpr char name[] = #name

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -
 
    DECLARE_STRING (comment);
    DECLARE_STRING (floatatom);
    DECLARE_STRING (gatom);
    DECLARE_STRING (message);
    DECLARE_STRING (msg);
    
    DECLARE_STRING (Abstraction);
    DECLARE_STRING (AskBeforeQuit);
    DECLARE_STRING (Attributes);
    DECLARE_STRING (BoxBackground);
    DECLARE_STRING (BoxPinsBackground);
    DECLARE_STRING (BoxSelected);
    DECLARE_STRING (BoxText);
    DECLARE_STRING (BoxWrong);
    DECLARE_STRING (BangBackground);
    DECLARE_STRING (BangFlashOff);
    DECLARE_STRING (BangFlashOn);
    DECLARE_STRING (Class);
    DECLARE_STRING (Colors);
    DECLARE_STRING (CommentBackground);
    DECLARE_STRING (CommentText);
    DECLARE_STRING (Content);
    DECLARE_STRING (Copy);
    DECLARE_STRING (Cut);
    DECLARE_STRING (DefaultIsRunView);
    DECLARE_STRING (Documentation);
    DECLARE_STRING (Duplicate);
    DECLARE_STRING (Editing);
    DECLARE_STRING (EditView);
    DECLARE_STRING (Flashed);
    DECLARE_STRING (FlashTime);
    DECLARE_STRING (General);
    DECLARE_STRING (GridSize);
    DECLARE_STRING (Height);
    DECLARE_STRING (Inlet);
    DECLARE_STRING (Inlets);
    DECLARE_STRING (Label);
    DECLARE_STRING (LabelBackground);
    DECLARE_STRING (LabelText);
    DECLARE_STRING (Lasso);
    DECLARE_STRING (Locator);
    DECLARE_STRING (Line);
    DECLARE_STRING (LineSelected);
    DECLARE_STRING (LineSignal);
    DECLARE_STRING (MessageBackground);
    DECLARE_STRING (MessageText);
    DECLARE_STRING (MessageTextClicked);
    DECLARE_STRING (Outlet);
    DECLARE_STRING (Outlets);
    DECLARE_STRING (Parameters);
    DECLARE_STRING (Paste);
    DECLARE_STRING (PatchBackground);
    DECLARE_STRING (PatchText);
    DECLARE_STRING (Path);
    DECLARE_STRING (PinAnything);
    DECLARE_STRING (PinBang);
    DECLARE_STRING (PinFloat);
    DECLARE_STRING (PinList);
    DECLARE_STRING (PinOver);
    DECLARE_STRING (PinSignal);
    DECLARE_STRING (PinSymbol);
    DECLARE_STRING (Redo);
    DECLARE_STRING (RunView);
    DECLARE_STRING (Selected);
    DECLARE_STRING (SnapToGrid);
    DECLARE_STRING (Text);
    DECLARE_STRING (Title);
    DECLARE_STRING (Undo);
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

namespace Tag = spaghettis::core::Keywords;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
