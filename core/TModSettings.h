// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "TSettings.h"

namespace Turpentine
{
	// All threads have priority sets normal, process sets highest, for adequate latency, prohibition on changing processor cores
	extern std::shared_ptr<Setting> CVarThreads;
	// Replaced OS function's memory manager (+12.5% memory consumption)
	extern std::shared_ptr<Setting> CVarMemory;
	// Replaced function's memory manager AK::MemoryMgr related to the sound system (+12.5% memory consumption)
	extern std::shared_ptr<Setting> CVarAudioMemory;
	// Replaces the maximum stdio handles.Default 512, max 8192 https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/setmaxstdio?view=msvc-170
	extern std::shared_ptr<Setting> CVarMaxStdio;
	// Fixed shuttering. If the waiting time exceeds one second, it forcibly resets the lock from thread
	extern std::shared_ptr<Setting> CVarSafeExit;
}