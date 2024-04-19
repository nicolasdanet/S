
/* Copyright (c) 2021 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

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
        
        post (core::UniquePath::invalid(), NEEDS_TRANS (s), LoggerType::error);
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

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
