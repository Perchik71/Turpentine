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
	// Fixes crashes related to exiting the game that can be caused erroneously by OBSE64 plugin hooks and also fast quit
	extern std::shared_ptr<Setting> CVarSafeExit;
	// Fixes crashes in the ffxQuery function no check Next pointer in struct ffxApiHeader 
	extern std::shared_ptr<Setting> CVarFFXQueryCrash;

	extern std::shared_ptr<Setting> CVarLoadScreen;
	// Enemies don't attack (they don't see player)
	extern std::shared_ptr<Setting> CVarFriendship;
	// Removes the "MessageBox" function from scripts (without buttons), fixes an issue with annoying messages in gameplay
	extern std::shared_ptr<Setting> CVarNoScriptMessageBox;
	// Removes the [NL] prefix from all strings
	extern std::shared_ptr<Setting> CVarWithoutPrefixNL;
	// Removes messages about the use of poison on weapons or bows
	extern std::shared_ptr<Setting> CVarNoPoisonConfirmMessage;
	// Removes the message when the spell is purchased
	extern std::shared_ptr<Setting> CVarNoTradeSpellConfirmMessage;
	// Removes the message when buying or selling items, but retains the window when selecting a number of items
	extern std::shared_ptr<Setting> CVarNoTradeItemConfirmMessage;
	// Removes the message when recharging a weapon or all weapons
	extern std::shared_ptr<Setting> CVarNoRechargeItemConfirmMessage;
	// Removes the message when all equipment is repaired or one
	extern std::shared_ptr<Setting> CVarNoRepairItemConfirmMessage;
}