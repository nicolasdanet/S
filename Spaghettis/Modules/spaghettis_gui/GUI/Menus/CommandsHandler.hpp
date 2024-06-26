
/* Copyright (c) 2022 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class CommandsHandler : public BaseCommands {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

public:
    CommandsHandler()  = default;
    ~CommandsHandler() = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void getInfo (juce::CommandID, juce::ApplicationCommandInfo&) override;
    void getCommands (juce::Array<juce::CommandID>&, bool application) override;
    bool performCommand (const juce::ApplicationCommandTarget::InvocationInfo&) override;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    static void addCloseWindowCommand (BaseComponent<CommandsHandler>*);

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CommandsHandler)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
