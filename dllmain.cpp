#include <obse64/PluginAPI.h>
#include <detours/Detours.h>
#include <Voltek.MemoryManager.h>

// Core
#include "TUtils.h"
#include "TSettings.h"
#include "TModSettings.h"

// Patches
#include "TMemory.h"
#include "TThreads.h"
#include "TMaxStdio.h"
#include "TGarbageCollector.h"

#define TURPENTINE_VERSION MAKE_EXE_VERSION(0, 3, 2)
#define TURPENTINE_NAME "Turpentine"
#define TURPENTINE_AUTHOR "perchik71"
#define USE_RTTI_EXPORT 0

bool APIENTRY Start(const OBSEInterface* obse);

std::string GlobalAppPath;
Turpentine::TOMLCollectionSettings GlobalModSettings;
uintptr_t GlobalBase = 0;
uintptr_t GlobalModuleBase = 0;
msrtti::section GlobalSection[3];

extern "C"
{
	// for f4se 0.1 alpha and newer
	__declspec(dllexport) OBSEPluginVersionData OBSEPlugin_Version =
	{
		OBSEPluginVersionData::kVersion,
		TURPENTINE_VERSION,
		TURPENTINE_NAME,
		TURPENTINE_AUTHOR,
		0,
		0,
		{ RUNTIME_VERSION_0_411_140, 0 },
		0,	// works with any version of the script extender. you probably do not need to put anything here
	};

	__declspec(dllexport) bool OBSEPlugin_Load(const OBSEInterface* obse)
	{
		if (obse->isEditor)
			return false;

		return Start(obse);
	}
};

static void APIENTRY InitializeOBSE64LogSystem()
{
	const uint32_t BufferSize = 1024;
	auto Buffer = make_unique<char[]>((size_t)BufferSize + 1);
	if (!Buffer)
	{
	UnknownDllName:
		gLog.OpenRelative(CSIDL_MYDOCUMENTS, "\\My Games\\Oblivion Remastered\\OBSE\\Logs\\UnknownPluginName.log");
	}
	else
	{
		if (!GetModuleFileNameA((HMODULE)GlobalModuleBase, Buffer.get(), BufferSize))
			goto UnknownDllName;

		const auto FormattedString = "\\My Games\\Oblivion Remastered\\OBSE\\Logs\\%s.log";

		PathStripPathA(Buffer.get());
		auto FileName = Buffer.get();
		auto ExtPosition = strchr(FileName, '.');
		if (ExtPosition)
			*ExtPosition = 0;

		auto NeedLen = _scprintf(FormattedString, FileName);
		if (NeedLen <= 0)
			goto UnknownDllName;

		auto Path = make_unique<char[]>((size_t)NeedLen + 1);
		if (!Path)
			goto UnknownDllName;

		sprintf(Path.get(), FormattedString, Buffer.get());
		gLog.OpenRelative(CSIDL_MYDOCUMENTS, Path.get());
	}
}

bool APIENTRY Start(const OBSEInterface* obse)
{
	InitializeOBSE64LogSystem();
	
	_MESSAGE("OBSE64 version check. obse64Version: 0x%x, runtimeVersion: 0x%x", obse->obse64Version, obse->runtimeVersion);
	_MESSAGE("Plugin \"" TURPENTINE_NAME "\" version check. Version: 0x%x, Author: %s", TURPENTINE_VERSION, TURPENTINE_AUTHOR);

	GlobalAppPath = Turpentine::Utils::GetApplicationPath();
	GlobalBase = (uintptr_t)GetModuleHandleA(nullptr);

	_MESSAGE("AppPath: %s", GlobalAppPath.c_str());

	if (!Turpentine::Utils::GetPESectionRange(".text", &GlobalSection[0]))
	{
		_ERROR("There is no information about \".text\" in the module");
		return false;
	}

	if (!Turpentine::Utils::GetPESectionRange(".rdata", &GlobalSection[1]))
	{
		_ERROR("There is no information about \".rdata\" in the module");
		return false;
	}

	if (!Turpentine::Utils::GetPESectionRange(".data", &GlobalSection[2]))
	{
		_ERROR("There is no information about \".data\" in the module");
		return false;
	}

	msrtti::section temp;
	if (Turpentine::Utils::GetPESectionRange(".textbss", &temp))
	{
		GlobalSection[0].base = std::min(GlobalSection[0].base, temp.base);
		GlobalSection[0].end = std::max(GlobalSection[0].end, temp.end);
	}

	if (Turpentine::Utils::GetPESectionRange(".interpr", &temp))
	{
		GlobalSection[0].base = std::min(GlobalSection[0].base, temp.base);
		GlobalSection[0].end = std::max(GlobalSection[0].end, temp.end);
	}

	msrtti::init(GlobalBase, GlobalSection[0], GlobalSection[2], GlobalSection[1]);

	_MESSAGE("Base: %llX", GlobalBase);
	_MESSAGE("The \".text\" section: (base: %llX end: %llX)", GlobalSection[0].base, GlobalSection[0].end);
	_MESSAGE("The \".rdata\" section: (base: %llX end: %llX)", GlobalSection[1].base, GlobalSection[1].end);
	_MESSAGE("The \".data\" section: (base: %llX end: %llX)", GlobalSection[2].base, GlobalSection[2].end);

#if USE_RTTI_EXPORT
	auto rtti_fname = Utils::GetGamePluginPath() + "rtti-" TURPENTINE_NAME ".txt";
	FILE* f = _fsopen(rtti_fname.c_str(), "wb", _SH_DENYWR);
	if (!f)
		_MESSAGE("Failed to create a \"%s\" file for RTTI output.", rtti_fname.c_str());
	else
	{
		msrtti::dump(f);
		fclose(f);
	}
#endif

	// Patches
	GlobalModSettings.Add(Turpentine::CVarThreads);
	GlobalModSettings.Add(Turpentine::CVarMemory);
	GlobalModSettings.Add(Turpentine::CVarAudioMemory);
	GlobalModSettings.Add(Turpentine::CVarMaxStdio);
	GlobalModSettings.Add(Turpentine::CVarThreadTaskDelay);

	// Load settings
	GlobalModSettings.LoadFromFile((Turpentine::Utils::GetGamePluginPath() + TURPENTINE_NAME ".toml").c_str());

	// Install patches
	//
	if (Turpentine::CVarThreads->GetBool())
		Turpentine::Patches::PatchThreads();

	Turpentine::Patches::PatchMemory(
		Turpentine::CVarMemory->GetBool(), 
		Turpentine::CVarAudioMemory->GetBool());

	Turpentine::Patches::PatchMaxStdio(Turpentine::CVarMaxStdio->GetSignedInt());

	if (Turpentine::CVarThreadTaskDelay->GetBool())
		Turpentine::Patches::PatchGarbageCollector();

	return true;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
		GlobalModuleBase = (uintptr_t)hModule;

    return TRUE;
}

