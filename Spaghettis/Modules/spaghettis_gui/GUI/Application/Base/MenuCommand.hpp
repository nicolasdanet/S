
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class MenuCommand {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

friend class BaseCommands;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit MenuCommand (juce::CommandID c,
        std::function<void()> f         = MenuCommand::defaultExecute,
        std::function<bool()> g         = MenuCommand::defaultCheck,
        std::function<juce::String()> h = MenuCommand::defaultName) :
            command_ (c),
            execute_ (f),
            check_ (g),
            name_ (h)
    {
    }
    
public:
    ~MenuCommand() = default;

public:
    MenuCommand (const MenuCommand&) = default;
    MenuCommand (MenuCommand&&) = default;
    MenuCommand& operator = (const MenuCommand&) = default;
    MenuCommand& operator = (MenuCommand&&) = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    static void defaultExecute()        { }
    static bool defaultCheck()          { return true; }
    static juce::String defaultName()   { return juce::String (""); }
    
private:
    juce::CommandID command_;
    std::function<void()> execute_;
    std::function<bool()> check_;
    std::function<juce::String()> name_;
    
private:
    JUCE_LEAK_DETECTOR (MenuCommand)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------