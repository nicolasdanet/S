
/* Copyright (c) 2021 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

using Perform = std::function<void()>;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class Queues : private juce::AsyncUpdater {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit Queues()
    {

    }
    
    ~Queues() override
    {
        jassert (inputs_.empty());
        jassert (outputs_.empty());
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    bool isAllInputsAlreadyHandled()
    {
        return empty (inputs_, lockInputs_);
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void addInput (const Perform& f)
    {
        add (inputs_, lockInputs_, f);
    }

    void addOutput (const Perform& f)
    {
        add (outputs_, lockOutputs_, f); triggerAsyncUpdate();
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void clear()
    {
        cancelPendingUpdate();
        
        poll (inputs_,  lockInputs_,  true);
        poll (outputs_, lockOutputs_, true);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void pollInputs()
    {
        poll (inputs_, lockInputs_, false);
    }

private:
    void handleAsyncUpdate() override
    {
        poll (outputs_, lockOutputs_, false);
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void poll (std::vector<Perform>& c, std::mutex& lock, bool fake)
    {
        std::vector<Perform> scoped;
        
        {
            const std::lock_guard<std::mutex> l (lock); scoped.swap (c);
        }
        
        if (!fake) { for (auto f : scoped) { f(); } }
    }
    
    void add (std::vector<Perform>& c, std::mutex& lock, Perform f)
    {
        const std::lock_guard<std::mutex> l (lock); c.push_back (std::move (f));
    }
    
    bool empty (std::vector<Perform>& c, std::mutex& lock)
    {
        const std::lock_guard<std::mutex> l (lock); return c.empty();
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    std::vector<Perform> inputs_;
    std::mutex lockInputs_;

private:
    std::vector<Perform> outputs_;
    std::mutex lockOutputs_;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Queues)
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------