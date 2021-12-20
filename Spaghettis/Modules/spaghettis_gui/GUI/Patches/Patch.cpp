
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Patch::requestCloseDialog()
{
    const juce::String title = NEEDS_TRANS ("Save patch before closing?");
        
    juce::MessageBoxOptions options (juce::MessageBoxOptions().withTitle (title)
        .withMessage (file_.getFileName())
        .withButton (NEEDS_TRANS ("Yes"))
        .withButton (NEEDS_TRANS ("No")));
    
    auto f = [u = u_](int result) { DBG (result); Spaghettis()->getPatches().removeRequest (u); };
    
    juce::NativeMessageBox::showAsync (options, f);
}

// Metal API Validation Enabled
// MTLIOAccelDevice bad MetalPluginClassName property (null)
// +[MTLIOAccelDevice registerDevices]: Zero Metal services found

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
