#! /usr/bin/env bash

# ------------------------------------------------------------------------------------------------------------
# ------------------------------------------------------------------------------------------------------------

# A series of dummy plugins for Spaghettis.

#   hello.c             Hello world!
#   helloRoot.c         Locate the dependencies.
#   helloRelease.c      Clean up your room.
#   helloBad.c          Manage failure at creation time.
#   helloCPP.cpp        C++ spoken.
#   helloSimple.c       A simple object.
#   helloInlet.c        Additional inlets.
#   helloNew.c          The arguments at instantiation.
#   helloBind.c         Bind to symbol.
#   helloClock.c        Schedule a task in time.
#   helloData.c         Save data into the patch (and more).
    
#   hello~.c            A no-op DSP object.
#   helloSpace~.c       Add closures to the DSP chain.
#   helloData~.c        Encapsulation safety for DSP object.
#   helloBiquad~.c      Biquad anatomized.
    
# ------------------------------------------------------------------------------------------------------------
# ------------------------------------------------------------------------------------------------------------

# Requires Python (>= 3.4) installed.

python3 ../Spaghettis/Modules/spaghettis_core/Core/m_spaghettis.h Examples/hello.c
python3 ../Spaghettis/Modules/spaghettis_core/Core/m_spaghettis.h Examples/helloRoot.c
python3 ../Spaghettis/Modules/spaghettis_core/Core/m_spaghettis.h Examples/helloRelease.c
python3 ../Spaghettis/Modules/spaghettis_core/Core/m_spaghettis.h Examples/helloBad.c
python3 ../Spaghettis/Modules/spaghettis_core/Core/m_spaghettis.h Examples/helloCPP.cpp
python3 ../Spaghettis/Modules/spaghettis_core/Core/m_spaghettis.h Examples/helloSimple.c
python3 ../Spaghettis/Modules/spaghettis_core/Core/m_spaghettis.h Examples/helloInlet.c
python3 ../Spaghettis/Modules/spaghettis_core/Core/m_spaghettis.h Examples/helloNew.c
python3 ../Spaghettis/Modules/spaghettis_core/Core/m_spaghettis.h Examples/helloBind.c
python3 ../Spaghettis/Modules/spaghettis_core/Core/m_spaghettis.h Examples/helloClock.c
python3 ../Spaghettis/Modules/spaghettis_core/Core/m_spaghettis.h Examples/helloData.c
python3 ../Spaghettis/Modules/spaghettis_core/Core/m_spaghettis.h Examples/hello~.c
python3 ../Spaghettis/Modules/spaghettis_core/Core/m_spaghettis.h Examples/helloSpace~.c
python3 ../Spaghettis/Modules/spaghettis_core/Core/m_spaghettis.h Examples/helloData~.c

# python3 ../Spaghettis/Modules/spaghettis_core/Core/m_spaghettis.h Examples/*.c Examples/*.cpp

# ------------------------------------------------------------------------------------------------------------
# ------------------------------------------------------------------------------------------------------------

mv Examples/*.pdobject Help/ 2> /dev/null

# ------------------------------------------------------------------------------------------------------------
# ------------------------------------------------------------------------------------------------------------
