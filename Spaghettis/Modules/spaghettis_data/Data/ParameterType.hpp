
/* Copyright (c) 2024 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis::data {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

template <class T> struct ParameterType { };

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

template<> struct ParameterType<bool>
{
    static const char* get() { return "boolean"; }
};

template<> struct ParameterType<juce::Colour>
{
    static const char* get() { return "color"; }
};

template<> struct ParameterType<juce::Rectangle<int>>
{
    static const char* get() { return "rectangle"; }
};

template<> struct ParameterType<int>
{
    static const char* get() { return "integer"; }
};

template<> struct ParameterType<double>
{
    static const char* get() { return "float"; }
};

template<> struct ParameterType<juce::String>
{
    static const char* get() { return "string"; }
};

template<> struct ParameterType<CodeBlock>
{
    static const char* get() { return "code"; }
};

template<> struct ParameterType<Directory>
{
    static const char* get() { return "directory"; }
};

template<> struct ParameterType<Device>
{
    static const char* get() { return "device"; }
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
