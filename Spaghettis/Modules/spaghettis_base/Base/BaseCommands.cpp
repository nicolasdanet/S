
/* Copyright (c) 2023 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void BaseCommands::set (MenuCommand m)
{
    jassert (!has (m.command_)); enabled_.push_back (m);
}

bool BaseCommands::perform (const juce::ApplicationCommandTarget::InvocationInfo& info)
{
    if (invoke (info) == false) { return performCommand (info); }
    
    return true;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool BaseCommands::has (juce::CommandID command)
{
    for (const auto& m : enabled_) {
        if (m.command_ == command) { return m.check_(); }
    }
    
    return false;
}

bool BaseCommands::invoke (const juce::ApplicationCommandTarget::InvocationInfo& info)
{
    const juce::CommandID command (info.commandID);
    
    for (const auto& m : enabled_) {
        if (m.command_ == command) {
            if (m.check_()) { m.invoke_ (info); return true; }
            else {
                return false;
            }
        }
    }
    
    return false;
}

juce::String BaseCommands::name (juce::CommandID command, juce::String fallback)
{
    for (const auto& m : enabled_) {
        if (m.command_ == command) {
            juce::String s (m.name_()); return (s.isNotEmpty() ? s : fallback);
        }
    }
    
    return fallback;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
