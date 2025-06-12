#include "Hooks_Version.h"
#include "obse64_common/BranchTrampoline.h"
#include "obse64_common/Errors.h"
#include "obse64_common/Relocation.h"
#include "obse64_common/obse64_version.h"

const wchar_t* hHook_ShowVersion_FormatString =
	L"v%s (%s) CorrelationID:%s [OBSE64 "
	__PREPRO_TOKEN_STR__(OBSE_VERSION_INTEGER) "."
	__PREPRO_TOKEN_STR__(OBSE_VERSION_INTEGER_MINOR) "."
	__PREPRO_TOKEN_STR__(OBSE_VERSION_INTEGER_BETA) "]";

typedef void (*_FormatVersion)(void*, const wchar_t*, const wchar_t*, const wchar_t*, const wchar_t*);
RelocAddr<_FormatVersion> ShowVersion_Call(0x0491AE10 + 0x2E2);
RelocAddr<_FormatVersion> ShowVersion_Original(0x00DE1E60);

void ShowVersion_Hook(void* a_unk, const wchar_t*, const wchar_t* a_ver, const wchar_t* a_appID, const wchar_t* a_cID)
{
	return ShowVersion_Original(a_unk, hHook_ShowVersion_FormatString, a_ver, a_appID, a_cID);
}

void Hooks_Version_Apply()
{
	g_branchTrampoline.write5Call(ShowVersion_Call.getUIntPtr(), (uintptr_t)ShowVersion_Hook);
}
