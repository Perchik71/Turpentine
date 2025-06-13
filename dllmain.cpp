#include <obse64/PluginAPI.h>
#include <detours/Detours.h>
#include <Voltek.MemoryManager.h>
#include <TRelocation.h>

// Core
#include "TUtils.h"
#include "TSettings.h"
#include "TModSettings.h"
#include "TModVersion.h"

// Hooks
#include "THookDataHandler.h"

// Patches
#include "TMemory.h"
#include "TThreads.h"
#include "TMaxStdio.h"
#include "TLoadScreen.h"

// Messages
#include "TNoScriptMessageBox.h"
#include "TNoPoisonConfirmMessage.h"
#include "TNoTradeSpellConfirmMessage.h"
#include "TNoTradeItemConfirmMessage.h"
#include "TNoRechargeConfirmMessage.h"
#include "TNoRepairConfirmMessage.h"
#include "TNoLoadGameConfirmMessage.h"

// Fixes
#include "TSafeExit.h"
#include "TFFXQueryCrash.h"
#include "TWithoutPrefixNL.h"

// Jokes
#include "TJokeFriendship.h"

#define USE_RTTI_EXPORT 0
#define USE_CMDTABLE_DUMP 0

bool APIENTRY Start(const OBSEInterface* obse);

std::string GlobalAppPath;
Turpentine::TOMLCollectionSettings GlobalModSettings;
uintptr_t GlobalBase = 0;
uintptr_t GlobalModuleBase = 0;
msrtti::section GlobalSection[3];
std::unordered_map<std::string, int> GlobalActivePlugins;

extern "C"
{
	// for obse64 0.1 alpha and newer
	__declspec(dllexport) OBSEPluginVersionData OBSEPlugin_Version =
	{
		OBSEPluginVersionData::kVersion,
		MOD_VERSION,
		MOD_NAME,
		MOD_AUTHOR,
		OBSEPluginVersionData::kAddressIndependence_AddressLibrary,
		OBSEPluginVersionData::kStructureIndependence_NoStructs,
		{ RUNTIME_VERSION_1_511_102, 0 },
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

static bool APIENTRY OpenAddressLibrary(const OBSEInterface* obse)
{
	auto fileName = std::make_unique<wchar_t[]>(500);
	const wchar_t* buildType = L"";

	swprintf_s(fileName.get(), 500, L"OBSE\\Plugins\\versionlib-%d-%d-%d-%d%s.bin",
		GET_EXE_VERSION_MAJOR(obse->runtimeVersion),
		GET_EXE_VERSION_MINOR(obse->runtimeVersion),
		GET_EXE_VERSION_BUILD(obse->runtimeVersion),
		GET_EXE_VERSION_SUB(obse->runtimeVersion),
		buildType);

	return Turpentine::REL::IDDB::Create(fileName.get(), obse->runtimeVersion) != nullptr;
}

bool APIENTRY Start(const OBSEInterface* obse)
{
	InitializeOBSE64LogSystem();
	
	_MESSAGE("OBSE64 version check. obse64Version: 0x%x, runtimeVersion: 0x%x", obse->obse64Version, obse->runtimeVersion);
	_MESSAGE("Plugin \"" MOD_NAME "\" version check. Version: 0x%x, Author: %s", MOD_VERSION, MOD_AUTHOR);

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

	OpenAddressLibrary(obse);

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
	auto rtti_fname = Utils::GetGamePluginPath() + "rtti-" MOD_NAME ".txt";
	FILE* f = _fsopen(rtti_fname.c_str(), "wb", _SH_DENYWR);
	if (!f)
		_MESSAGE("Failed to create a \"%s\" file for RTTI output.", rtti_fname.c_str());
	else
	{
		msrtti::dump(f);
		fclose(f);
	}
#endif // USE_RTTI_EXPORT

	// Added control settings
	GlobalModSettings.Add(Turpentine::CVarThreads);
	GlobalModSettings.Add(Turpentine::CVarMemory);
	GlobalModSettings.Add(Turpentine::CVarAudioMemory);
	GlobalModSettings.Add(Turpentine::CVarMaxStdio);
	GlobalModSettings.Add(Turpentine::CVarSafeExit);
	//GlobalModSettings.Add(Turpentine::CVarLoadScreen);
	GlobalModSettings.Add(Turpentine::CVarFriendship);
	GlobalModSettings.Add(Turpentine::CVarNoScriptMessageBox);
	GlobalModSettings.Add(Turpentine::CVarWithoutPrefixNL);
	GlobalModSettings.Add(Turpentine::CVarNoPoisonConfirmMessage);
	GlobalModSettings.Add(Turpentine::CVarNoTradeSpellConfirmMessage);
	GlobalModSettings.Add(Turpentine::CVarNoTradeItemConfirmMessage);
	GlobalModSettings.Add(Turpentine::CVarNoRechargeItemConfirmMessage);
	GlobalModSettings.Add(Turpentine::CVarNoRepairItemConfirmMessage);
	GlobalModSettings.Add(Turpentine::CVarNoLoadGameConfirmMessage);

	// Load settings
	GlobalModSettings.LoadFromFile((Turpentine::Utils::GetGamePluginPath() + MOD_NAME ".toml").c_str());

	// Install Hooks
	//
	Turpentine::Hooks::DataHandler();

#if USE_CMDTABLE_DUMP
	Turpentine::RE::DumpCommandTable("commands_dump.txt");
#endif // USE_CMDTABLE_DUMP

	// Install patches
	//
	if (Turpentine::CVarThreads->GetBool())
		Turpentine::Patches::Threads();

	Turpentine::Patches::Memory(
		Turpentine::CVarMemory->GetBool(), 
		Turpentine::CVarAudioMemory->GetBool());

	Turpentine::Patches::MaxStdio(Turpentine::CVarMaxStdio->GetSignedInt());

	//if (Turpentine::CVarLoadScreen->GetBool())
	//	Turpentine::Patches::LoadScreen();

	// Install gameplay
	//
	if (Turpentine::CVarNoScriptMessageBox->GetBool())
		Turpentine::Patches::NoScriptMessageBox();

	if (Turpentine::CVarNoPoisonConfirmMessage->GetBool())
		Turpentine::Patches::NoPoisonConfirmMessage();
	
	if (Turpentine::CVarNoTradeSpellConfirmMessage->GetBool())
		Turpentine::Patches::NoTradeSpellConfirmMessage();

	if (Turpentine::CVarNoTradeItemConfirmMessage->GetBool())
		Turpentine::Patches::NoTradeItemConfirmMessage();

	if (Turpentine::CVarNoRechargeItemConfirmMessage->GetBool())
		Turpentine::Patches::NoRechargeConfirmMessage();

	if (Turpentine::CVarNoRepairItemConfirmMessage->GetBool())
		Turpentine::Patches::NoRepairConfirmMessage();

	if (Turpentine::CVarNoLoadGameConfirmMessage->GetBool())
		Turpentine::Patches::NoLoadGameConfirmMessage();

	// Install fixes
	//
	if (Turpentine::CVarSafeExit->GetBool())
		Turpentine::Fixes::SafeExit();

	if (Turpentine::CVarFFXQueryCrash->GetBool())
		Turpentine::Fixes::FFXQueryCrash();

	if (Turpentine::CVarWithoutPrefixNL->GetBool())
		Turpentine::Fixes::WithoutPrefixNL();

	// Install jokes
	//
	if (Turpentine::CVarFriendship->GetBool())
		Turpentine::Jokes::Friendship();

	// Release address library
	Turpentine::REL::IDDB::Release();

	return true;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
		GlobalModuleBase = (uintptr_t)hModule;

    return TRUE;
}

