#pragma once

#include <string>
#include "obse64_common/Types.h"

bool CheckDLLSignature(const std::string & dllPath, u32 * trustError);
