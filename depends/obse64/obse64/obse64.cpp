#include <Windows.h>
#include <ShlObj.h>
#include <corecrt_startup.h>
#include "obse64_common/Log.h"
#include "obse64_common/obse64_version.h"
#include "obse64_common/Utilities.h"
#include "obse64_common/SafeWrite.h"
#include "obse64_common/BranchTrampoline.h"
#include "obse64_common/CoreInfo.h"
#include "PluginManager.h"

#include "Hooks_Script.h"

HINSTANCE g_moduleHandle = nullptr;

void OBSE64_Preinit();
void OBSE64_Initialize();

// api-ms-win-crt-runtime-l1-1-0.dll
typedef int (*__initterm_e)(_PIFV *, _PIFV *);
__initterm_e _initterm_e_Original = nullptr;

typedef char * (*__get_narrow_winmain_command_line)();
__get_narrow_winmain_command_line _get_narrow_winmain_command_line_Original = NULL;

// runs before global initializers
int __initterm_e_Hook(_PIFV * a, _PIFV * b)
{
	// could be used for plugin optional preload

	OBSE64_Preinit();

	return _initterm_e_Original(a, b);
}

// runs after global initializers
char * __get_narrow_winmain_command_line_Hook()
{
	// the usual load time

	OBSE64_Initialize();

	return _get_narrow_winmain_command_line_Original();
}

void installBaseHooks(void)
{
	DebugLog::openRelative(CSIDL_MYDOCUMENTS, "\\My Games\\" SAVE_FOLDER_NAME "\\OBSE\\Logs\\obse64.txt");

	HANDLE exe = GetModuleHandle(nullptr);

	// fetch functions to hook
	auto * initterm = (__initterm_e *)getIATAddr(exe, "api-ms-win-crt-runtime-l1-1-0.dll", "_initterm_e");
	auto * cmdline = (__get_narrow_winmain_command_line *)getIATAddr(exe, "api-ms-win-crt-runtime-l1-1-0.dll", "_get_narrow_winmain_command_line");

	// hook them
	if(initterm)
	{
		_initterm_e_Original = *initterm;
		safeWrite64(uintptr_t(initterm), u64(__initterm_e_Hook));
	}
	else
	{
		_ERROR("couldn't find _initterm_e");
	}

	if(cmdline)
	{
		_get_narrow_winmain_command_line_Original = *cmdline;
		safeWrite64(uintptr_t(cmdline), u64(__get_narrow_winmain_command_line_Hook));
	}
	else
	{
		_ERROR("couldn't find _get_narrow_winmain_command_line");
	}
}

void WaitForDebugger(void)
{
	while(!IsDebuggerPresent())
	{
		Sleep(10);
	}

	Sleep(1000 * 2);
}

void OBSE64_Preinit()
{
	static bool runOnce = false;
	if(runOnce) return;
	runOnce = true;

	SYSTEMTIME now;
	GetSystemTime(&now);

	_MESSAGE("OBSE64 runtime: initialize (version = %d.%d.%d %08X %04d-%02d-%02d %02d:%02d:%02d, os = %s)",
		OBSE_VERSION_INTEGER, OBSE_VERSION_INTEGER_MINOR, OBSE_VERSION_INTEGER_BETA, RUNTIME_VERSION,
		now.wYear, now.wMonth, now.wDay, now.wHour, now.wMinute, now.wSecond,
		getOSInfoStr().c_str());

	_MESSAGE("imagebase = %016I64X", g_moduleHandle);
	_MESSAGE("reloc mgr imagebase = %016I64X", RelocationManager::s_baseAddr);

#ifdef _DEBUG
	SetPriorityClass(GetCurrentProcess(), IDLE_PRIORITY_CLASS);

	// WaitForDebugger();
#endif

	if(!g_branchTrampoline.create(1024 * 64))
	{
		_ERROR("couldn't create branch trampoline. this is fatal. skipping remainder of init process.");
		return;
	}

	if(!g_localTrampoline.create(1024 * 64, g_moduleHandle))
	{
		_ERROR("couldn't create codegen buffer. this is fatal. skipping remainder of init process.");
		return;
	}

	// scan plugin folder
	g_pluginManager.init();

	// preload plugins
	g_pluginManager.installPlugins(PluginManager::kPhase_Preload);

	_MESSAGE("preinit complete");
}

void OBSE64_Initialize()
{
	static bool runOnce = false;
	if(runOnce) return;
	runOnce = true;

	// load plugins
	g_pluginManager.installPlugins(PluginManager::kPhase_Load);
	g_pluginManager.loadComplete();

	Hooks_Script_Apply();

	FlushInstructionCache(GetCurrentProcess(), NULL, 0);

	_MESSAGE("init complete");

	DebugLog::flush();
}

extern "C" {
	void StartOBSE64()
	{
		installBaseHooks();
	}
	
	BOOL WINAPI DllMain(HANDLE hDllHandle, DWORD dwReason, LPVOID lpreserved)
	{
		switch(dwReason)
		{
		case DLL_PROCESS_ATTACH:
			g_moduleHandle = (HINSTANCE)hDllHandle;
			break;

		case DLL_PROCESS_DETACH:
			break;
		};

		return TRUE;
	}

	__declspec(dllexport) OBSECoreVersionData OBSECore_Version =
	{
		OBSECoreVersionData::kVersion,

		RUNTIME_VERSION,
	};
}
