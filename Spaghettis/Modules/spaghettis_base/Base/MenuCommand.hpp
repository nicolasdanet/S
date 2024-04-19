
/* Copyright (c) 2022 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

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
    explicit MenuCommand (juce::CommandID c) : command_ (c),
        invoke_ (MenuCommand::defaultInvoke),
        check_  (MenuCommand::defaultCheck),
        name_   (MenuCommand::defaultName)
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

public:
    MenuCommand& setInvoke (std::function<void (const juce::ApplicationCommandTarget::InvocationInfo&)> f)
    {
        invoke_ = f; return *this;
    }
    
    MenuCommand& setCheck (std::function<bool()> f)
    {
        check_ = f;  return *this;
    }
    
    MenuCommand& setName (std::function<juce::String()> f)
    {
        name_ = f;   return *this;
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    static void defaultInvoke (const juce::ApplicationCommandTarget::InvocationInfo&) { }
    static bool defaultCheck() { return true; }
    static juce::String defaultName() { return juce::String(); }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    static bool isFromMenu (const juce::ApplicationCommandTarget::InvocationInfo& info)
    {
        return (info.invocationMethod == juce::ApplicationCommandTarget::InvocationInfo::fromMenu);
    }
    
private:
    juce::CommandID command_;
    std::function<void (const juce::ApplicationCommandTarget::InvocationInfo&)> invoke_;
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
