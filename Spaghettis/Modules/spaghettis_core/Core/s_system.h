
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef S_SYSTEM_H_
#define S_SYSTEM_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "system/memory/s_memory.h"
#include "system/memory/s_atomic.h"
#include "system/memory/s_trylock.h"
#include "system/memory/s_fifo.h"
#include "system/memory/s_fifo8.h"
#include "system/memory/s_fifo32.h"
#include "system/memory/s_ring.h"
#include "system/scheduler/s_monitor.h"
#include "system/scheduler/s_clock.h"
#include "system/scheduler/s_time.h"
#include "system/scheduler/s_scheduler.h"
#include "system/log/s_debug.h"
#include "system/log/s_logger.h"
#include "system/files/s_file.h"
#include "system/files/s_searchpath.h"
#include "system/policy/s_denormal.h"
#include "system/policy/s_privilege.h"
#include "system/audio/s_audio.h"
#include "system/MIDI/s_midi.h"
#include "system/devices/s_devices.h"
#include "system/devices/s_audiodevices.h"
#include "system/devices/s_mididevices.h"
#include "system/backends/s_API.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if PD_WITH_DEBUG

int sys_isControlThread (void);

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // S_SYSTEM_H_
