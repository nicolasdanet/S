
/* Copyright (c) 2022 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis::Keywords {

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
    DECLARE_STRING (tgl);
    
    DECLARE_STRING (Abstraction);
    DECLARE_STRING (ArrayBackground);
    DECLARE_STRING (ArrayValue);
    DECLARE_STRING (AskBeforeQuit);
    DECLARE_STRING (AtomBackground);
    DECLARE_STRING (AtomText);
    DECLARE_STRING (AtomClicked);
    DECLARE_STRING (Attributes);
    DECLARE_STRING (AudioDeviceIn0);
    DECLARE_STRING (BoxBackground);
    DECLARE_STRING (BoxPinsBackground);
    DECLARE_STRING (BoxSelected);
    DECLARE_STRING (BoxText);
    DECLARE_STRING (BoxWrong);
    DECLARE_STRING (BangBackground);
    DECLARE_STRING (BangFlashOff);
    DECLARE_STRING (BangFlashOn);
    DECLARE_STRING (Buttons);
    DECLARE_STRING (Class);
    DECLARE_STRING (Colors);
    DECLARE_STRING (CommentBackground);
    DECLARE_STRING (CommentText);
    DECLARE_STRING (Content);
    DECLARE_STRING (Copy);
    DECLARE_STRING (Count);
    DECLARE_STRING (Cut);
    DECLARE_STRING (DefaultIsRunView);
    DECLARE_STRING (Devices);
    DECLARE_STRING (DialBackground);
    DECLARE_STRING (DialForeground);
    DECLARE_STRING (DialNeedle);
    DECLARE_STRING (DialText);
    DECLARE_STRING (Digits);
    DECLARE_STRING (Dirty);
    DECLARE_STRING (Documentation);
    DECLARE_STRING (Duplicate);
    DECLARE_STRING (Editing);
    DECLARE_STRING (EditView);
    DECLARE_STRING (Embedded);
    DECLARE_STRING (Flashed);
    DECLARE_STRING (FlashTime);
    DECLARE_STRING (General);
    DECLARE_STRING (GridSize);
    DECLARE_STRING (Height);
    DECLARE_STRING (High);
    DECLARE_STRING (Included);
    DECLARE_STRING (Interval);
    DECLARE_STRING (Inlet);
    DECLARE_STRING (Inlets);
    DECLARE_STRING (Inspector);
    DECLARE_STRING (InspectorWidth);
    DECLARE_STRING (Label);
    DECLARE_STRING (Low);
    DECLARE_STRING (Line);
    DECLARE_STRING (LineSelected);
    DECLARE_STRING (LineSignal);
    DECLARE_STRING (Logarithmic);
    DECLARE_STRING (MessageBackground);
    DECLARE_STRING (MessageText);
    DECLARE_STRING (MessageClicked);
    DECLARE_STRING (MidiDeviceIn0);
    DECLARE_STRING (Multiple);
    DECLARE_STRING (Name);
    DECLARE_STRING (NonZero);
    DECLARE_STRING (OffsetX);
    DECLARE_STRING (OffsetY);
    DECLARE_STRING (Outlet);
    DECLARE_STRING (Outlets);
    DECLARE_STRING (Parameters);
    DECLARE_STRING (Paste);
    DECLARE_STRING (PatchBackground);
    DECLARE_STRING (PatchLasso);
    DECLARE_STRING (PatchLocator);
    DECLARE_STRING (Path);
    DECLARE_STRING (Path0);
    DECLARE_STRING (Path1);
    DECLARE_STRING (Path2);
    DECLARE_STRING (Path3);
    DECLARE_STRING (Paths);
    DECLARE_STRING (Peak);
    DECLARE_STRING (PinAnything);
    DECLARE_STRING (PinBang);
    DECLARE_STRING (PinFloat);
    DECLARE_STRING (PinList);
    DECLARE_STRING (PinSignal);
    DECLARE_STRING (PinSymbol);
    DECLARE_STRING (RadioBackground);
    DECLARE_STRING (RadioButton);
    DECLARE_STRING (Redo);
    DECLARE_STRING (Selected);
    DECLARE_STRING (Size);
    DECLARE_STRING (SliderBackground);
    DECLARE_STRING (SliderBar);
    DECLARE_STRING (SnapToGrid);
    DECLARE_STRING (State);
    DECLARE_STRING (Text);
    DECLARE_STRING (Title);
    DECLARE_STRING (ToggleBackground);
    DECLARE_STRING (Toggle);
    DECLARE_STRING (Undo);
    DECLARE_STRING (Value);
    DECLARE_STRING (Vertical);
    DECLARE_STRING (Visible);
    DECLARE_STRING (VuBackground);
    DECLARE_STRING (VuBarCold);
    DECLARE_STRING (VuBarHot);
    DECLARE_STRING (VuBarWarm);
    DECLARE_STRING (Width);
    DECLARE_STRING (X);
    DECLARE_STRING (Y);
    DECLARE_STRING (Zoom);
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#undef DECLARE_STRING

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace Tag = spaghettis::Keywords;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
