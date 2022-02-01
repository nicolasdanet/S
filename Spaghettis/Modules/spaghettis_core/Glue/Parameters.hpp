
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace core {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

struct Parameter {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

friend struct Tree;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    static juce::String getItem (const juce::ValueTree& parameter)
    {
        return parameter.getProperty (Ids::item).toString();
    }

    static juce::var getValue (const juce::ValueTree& parameter)
    {
        return parameter.getProperty (Ids::value);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    static bool isValid (const juce::ValueTree& parameter)
    {
        return (parameter.hasType (Ids::PARAMETER)
                    && parameter.getProperty (Ids::item).isString()
                    && parameter.getProperty (Ids::text).isString()
                    && parameter.getProperty (Ids::info).isString()
                    && parameter.getProperty (Ids::type).isString()
                    && parameter.hasProperty (Ids::value));
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

struct Group {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

friend struct Tree;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    static juce::ValueTree getParameter (const juce::ValueTree& group, const juce::String& item)
    {
        for (const auto& parameter : group) {
            if (parameter.getProperty (Ids::item).equalsWithSameType (item)) { return parameter; }
        }
        
        return juce::ValueTree();
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    static void addParameter (juce::ValueTree& group,
        const juce::String& item,
        const juce::String& text,
        const juce::String& info,
        bool b)
    {
        return addParameter (group, item, text, info, "boolean", b);
    }

    static void addParameter (juce::ValueTree& group,
        const juce::String& item,
        const juce::String& text,
        const juce::String& info,
        juce::Colour c)
    {
        return addParameter (group, item, text, info, "color", Colours::getColourAsString (c));
    }

    static void addParameter (juce::ValueTree& group,
        const juce::String& item,
        const juce::String& text,
        const juce::String& info,
        int n)
    {
        return addParameter (group, item, text, info, "integer", n);
    }

    static void addParameter (juce::ValueTree& group,
        const juce::String& item,
        const juce::String& text,
        const juce::String& info,
        double f)
    {
        return addParameter (group, item, text, info, "float", f);
    }

    static void addParameter (juce::ValueTree& group,
        const juce::String& item,
        const juce::String& text,
        const juce::String& info,
        const juce::String& s)
    {
        return addParameter (group, item, text, info, "text", s);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

private:
    static void addParameter (juce::ValueTree& group,
        const juce::String& item,
        const juce::String& text,
        const juce::String& info,
        const juce::String& type,
        juce::var v)
    {
        juce::ValueTree parameter (Ids::PARAMETER);
        
        parameter.setProperty (Ids::item,  item, nullptr);
        parameter.setProperty (Ids::text,  text, nullptr);
        parameter.setProperty (Ids::info,  info, nullptr);
        parameter.setProperty (Ids::type,  type, nullptr);
        parameter.setProperty (Ids::value, v, nullptr);
        
        group.appendChild (parameter, nullptr);
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    static bool isValid (const juce::ValueTree& group)
    {
        return (group.hasType (Ids::GROUP) && group.getProperty (Ids::name).isString());
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

struct Tree {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    static juce::ValueTree getGroup (const juce::ValueTree& tree, const juce::String& name)
    {
        for (const auto& group : tree) {
            if (group.getProperty (Ids::name).equalsWithSameType (name)) { return group; }
        }
        
        return juce::ValueTree();
    }
    
    static juce::ValueTree addGroup (juce::ValueTree& tree, const juce::String& name)
    {
        juce::ValueTree group (Ids::GROUP); group.setProperty (Ids::name, name, nullptr);
        
        tree.appendChild (group, nullptr);
        
        return group;
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    static bool isValid (const juce::ValueTree& tree, const juce::Identifier& identifier)
    {
        if (tree.isValid() && tree.hasType (identifier)) {
        //
        for (const auto& group : tree) {
        //
        if (Group::isValid (group)) {
            for (const auto& parameter : group)  {
                if (!Parameter::isValid (parameter)) { return false; }
            }
        } else { return false; }
        //
        }
        //
        } else { return false; }
        
        return true;
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace core

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
