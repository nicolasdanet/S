
/* Copyright (c) 2021 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis::core {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

extern int  main_argc;

extern char **main_argv;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

Wrapper *main_wrapper;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void main_threadLoopFakeCommandLine (const juce::StringArray& cmd)
{
    int n = cmd.size();
    
    if (n) {
    //
    main_argc = n;
    main_argv = static_cast<char**> (malloc (sizeof (char*) * n));
    
    for (int i = 0; i < n; ++i) {
        const juce::String& s = cmd[i];
        const size_t size = s.getNumBytesAsUTF8() + 1;
        main_argv[i] = static_cast<char*> (malloc (size));
        s.copyToUTF8 (main_argv[i], size);
    }
    //
    } else { jassertfalse; }
}

void main_threadLoopFakeCommandLineRelease()
{
    for (int i = 0; i < main_argc; ++i) { free (main_argv[i]); }
    
    free (main_argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool main_threadLoop (Wrapper *owner)
{
    main_wrapper = owner;

    main_threadLoopFakeCommandLine (main_wrapper->getCommandLine());
    
    PD_ASSERT (sys_isControlThread());
    
    int error = main_start();
    
    main_threadLoopFakeCommandLineRelease();
    
    return (error != 0);
}

void main_threadExit()
{
    main_exit();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------