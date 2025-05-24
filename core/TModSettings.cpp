// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "TSettings.h"

namespace Turpentine
{
	std::shared_ptr<Setting> CVarThreads = std::make_shared<Setting>("bThreads:Patches", true);
	std::shared_ptr<Setting> CVarMemory = std::make_shared<Setting>("bMemoryManager:Patches", true);
	std::shared_ptr<Setting> CVarAudioMemory = std::make_shared<Setting>("bAudioAllocator:Patches", true);
	std::shared_ptr<Setting> CVarMaxStdio = std::make_shared<Setting>("iMaxStdio:Patches", 2048);	
	std::shared_ptr<Setting> CVarSafeExit = std::make_shared<Setting>("bSafeExit:Fixes", true);
	std::shared_ptr<Setting> CVarFFXQueryCrash = std::make_shared<Setting>("bFFXQueryCrash:Fixes", true);
	std::shared_ptr<Setting> CVarLoadScreen = std::make_shared<Setting>("bLoadScreen:Patches", false);
	std::shared_ptr<Setting> CVarFriendship = std::make_shared<Setting>("bFriendship:Jokes", false);
	std::shared_ptr<Setting> CVarNoScriptMessageBox = std::make_shared<Setting>("bNoScriptMessageBox:Gameplay", false);
	std::shared_ptr<Setting> CVarWithoutPrefixNL = std::make_shared<Setting>("bWithoutPrefixNL:Fixes", true);
	std::shared_ptr<Setting> CVarNoPoisonConfirmMessage = std::make_shared<Setting>("bNoPoisonConfirmMessage:Gameplay", false);
	std::shared_ptr<Setting> CVarNoTradeSpellConfirmMessage = std::make_shared<Setting>("bNoTradeSpellConfirmMessage:Gameplay", false);
	std::shared_ptr<Setting> CVarNoTradeItemConfirmMessage = std::make_shared<Setting>("bNoTradeItemConfirmMessage:Gameplay", false);
	std::shared_ptr<Setting> CVarNoRechargeItemConfirmMessage = std::make_shared<Setting>("bNoRechargeItemConfirmMessage:Gameplay", false);
	std::shared_ptr<Setting> CVarNoRepairItemConfirmMessage = std::make_shared<Setting>("bNoRepairItemConfirmMessage:Gameplay", false);
}