#pragma once

#include <windows.h>
#include <shlobj.h>
#include <shellapi.h>
#include <shlwapi.h>
#include <objbase.h>
#include <commdlg.h>

#include <memory>
#include <string>
#include <algorithm>
#include <vector>
#include <map>

#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <comdef.h>

using namespace std;

#include <obse64_common/obse64_version.h>
#include <obse64_common/Utilities.h>
#include <f4se_common/IDebugLog.h>				// I don't like obse's interface, it's bad and doesn't call flush
#include <f4se_common/IErrors.h>

#define RUNTIME_VERSION	CURRENT_RELEASE_RUNTIME

#define Property(rfunc, wfunc)	__declspec(property(get = rfunc, put = wfunc))
#define PropertyReadOnly(rfunc)	__declspec(property(get = rfunc))