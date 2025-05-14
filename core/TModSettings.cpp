// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "TSettings.h"

namespace Turpentine
{
	std::shared_ptr<Setting> CVarThreads = std::make_shared<Setting>("bThreads:Patches", true);
	std::shared_ptr<Setting> CVarMemory = std::make_shared<Setting>("bMemory:Patches", true);
	std::shared_ptr<Setting> CVarAudioMemory = std::make_shared<Setting>("bAudioMemory:Patches", true);
	std::shared_ptr<Setting> CVarMaxStdio = std::make_shared<Setting>("iMaxStdio:Patches", 2048);	
	std::shared_ptr<Setting> CVarThreadTaskDelay = std::make_shared<Setting>("bThreadTaskDelay:Patches", true);
}