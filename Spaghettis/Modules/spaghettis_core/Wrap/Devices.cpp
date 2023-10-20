
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

template <class T> juce::StringArray Devices<T>::getNames (const std::vector<T>& devices)
{
    juce::StringArray a; for (const auto& d : devices) { a.add (d.getName()); }
    
    a.sortNatural();
    
    return a;
}

template <class T> juce::String Devices<T>::getNameAt (const std::vector<T>& devices, int i)
{
    jassert (i >= 0); const typename std::vector<T>::size_type n = i;
    
    if (n < devices.size()) { return devices[n].getName(); }
    
    return juce::String();
}

template <class T> int Devices<T>::getChannelsFor (const std::vector<T>& devices, const juce::String& name)
{
    auto f = [&name] (const T& d)
    {
        return d.getName() == name;
    };
    
    auto r = std::find_if (devices.cbegin(), devices.cend(), f);
    
    return r != devices.cend() ? r->getChannels() : 0;
}

template <class T> void Devices<T>::removeDeviceAt (std::vector<T>& devices, int i)
{
    const typename std::vector<T>::size_type n = i;
    
    if (n < devices.size()) { devices.erase (devices.cbegin() + n); }
}

template <class T> void Devices<T>::changeDeviceAt (std::vector<T>& devices,
    int i,
    const juce::String& name,
    int channels)
{
    const typename std::vector<T>::size_type n = i;
    
    if (n < devices.size()) { devices[n] = T (name, channels); }
    else {
        devices.emplace_back (name, channels);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

template <class T> std::vector<T> Devices<T>::getDevicesInChangedAt (const juce::String& name, int n) const
{
    std::vector<T> t (currentDevicesIn_);
    
    if (name.isNotEmpty()) { changeDeviceAt (t, n, name, getChannelsFor (availableDevicesIn_, name)); }
    else {
        removeDeviceAt (t, n);
    }
    
    return t;
}

template <class T> std::vector<T> Devices<T>::getDevicesOutChangedAt (const juce::String& name, int n) const
{
    std::vector<T> t (currentDevicesOut_);
    
    if (name.isNotEmpty()) { changeDeviceAt (t, n, name, getChannelsFor (availableDevicesOut_, name)); }
    else {
        removeDeviceAt (t, n);
    }
    
    return t;
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

template class Devices<AudioDevice>;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
