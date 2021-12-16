
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void Wrapper::run()
{
    JUCE_TRY
    {
        const bool error = core::main_threadLoop (this);

        if (error) {
        //
        juce::String s = juce::String (PD_NAME_LOWERCASE) + juce::String (": core error");
        
        post (NEEDS_TRANS (s), Logger::Type::error, core::Unique::invalid());
        //
        }
    }
    JUCE_CATCH_EXCEPTION
}

void Wrapper::setCommandLine (const juce::StringArray& commandLine)
{
    commandLine_ = commandLine;
    
    commandLine_.removeString ("-NSDocumentRevisionsDebugMode");
    commandLine_.removeString ("YES");
    commandLine_.insert (0, PD_NAME);
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
