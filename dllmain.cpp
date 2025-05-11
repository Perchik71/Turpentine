#include <obse64/PluginAPI.h>
#include <detours/Detours.h>
#include <Voltek.MemoryManager.h>
#include <ms_rtti.h>
#include <toml.hpp>

// Patches
#include "TMemory.h"
#include "TThreads.h"
#include "TMaxStdio.h"

#define TURPENTINE_VERSION MAKE_EXE_VERSION(0, 2, 0)
#define TURPENTINE_NAME "Turpentine"
#define TURPENTINE_AUTHOR "perchik71"
#define USE_RTTI_EXPORT 0

bool APIENTRY Start(const OBSEInterface* obse);

std::string GlobalAppPath;
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
		{ 0 },
		0,	// works with any version of the script extender. you probably do not need to put anything here
	};

	__declspec(dllexport) bool OBSEPlugin_Load(const OBSEInterface* obse)
	{
		if (obse->isEditor)
			return false;

		return Start(obse);
	}
};

namespace Utils
{
	static std::wstring __stdcall AnsiToWide(const std::string& String) noexcept(true)
	{
		auto l = MultiByteToWideChar(CP_ACP, 0, String.c_str(), (int32_t)String.length(), nullptr, 0);
		if (l <= 0) return L"";
		std::wstring temp;
		temp.resize(l);
		MultiByteToWideChar(CP_ACP, 0, String.c_str(), (int32_t)String.length(), temp.data(), (int32_t)temp.length());
		return temp;
	}

	static std::string __stdcall WideToAnsi(const std::wstring& String) noexcept(true)
	{
		auto l = WideCharToMultiByte(CP_ACP, 0, String.c_str(), (int32_t)String.length(), nullptr, 0, nullptr, nullptr);
		if (l <= 0) return "";
		std::string temp;
		temp.resize(l);
		WideCharToMultiByte(CP_ACP, 0, String.c_str(), (int32_t)String.length(), temp.data(), (int32_t)temp.length(), nullptr, nullptr);
		return temp;
	}

	static std::string __stdcall AnsiToUtf8(const std::string& String) noexcept(true)
	{
		auto Source = AnsiToWide(String);
		auto l = WideCharToMultiByte(CP_UTF8, 0, Source.c_str(), (int32_t)String.length(), nullptr, 0, nullptr, nullptr);
		if (l <= 0) return "";
		std::string temp;
		temp.resize(l);
		WideCharToMultiByte(CP_ACP, 0, Source.c_str(), (int32_t)Source.length(), temp.data(), (int32_t)temp.length(), nullptr, nullptr);
		return temp;
	}

	static std::string __stdcall Utf8ToAnsi(const std::string& String) noexcept(true)
	{
		auto l = MultiByteToWideChar(CP_UTF8, 0, String.c_str(), (int32_t)String.length(), nullptr, 0);
		if (l <= 0) return "";
		std::wstring temp;
		temp.resize(l);
		MultiByteToWideChar(CP_UTF8, 0, String.c_str(), (int32_t)String.length(), temp.data(), (int32_t)temp.length());
		return WideToAnsi(temp);
	}

	static std::string& __stdcall Trim(std::string& String) noexcept(true)
	{
		constexpr static char whitespaceDelimiters[] = " \t\n\r\f\v";

		if (!String.empty())
		{
			String.erase(String.find_last_not_of(whitespaceDelimiters) + 1);
			String.erase(0, String.find_first_not_of(whitespaceDelimiters));
		}

		return String;
	}

	static std::string __stdcall GetApplicationPath() noexcept(true)
	{
		std::string _app_path;
		const uint32_t BufferSize = 1024;

		auto Buffer = std::make_unique<char[]>((size_t)BufferSize + 1);
		if (Buffer && GetModuleFileNameA(GetModuleHandleA(nullptr), Buffer.get(), BufferSize))
		{
			PathRemoveFileSpecA(Buffer.get());
			_app_path = Buffer.get();
			_app_path += "\\";
		}

		return _app_path;
	}

	static std::string __stdcall GetGamePluginPath() noexcept(true)
	{
		return GetApplicationPath() + "OBSE\\Plugins\\";
	}

	static bool __stdcall GetPESectionRange(const char* section, msrtti::section* data) noexcept(true)
	{
		if (!data)
			return false;

		PIMAGE_NT_HEADERS ntHeaders = (PIMAGE_NT_HEADERS)(GlobalBase + ((PIMAGE_DOS_HEADER)GlobalBase)->e_lfanew);
		PIMAGE_SECTION_HEADER cur_section = IMAGE_FIRST_SECTION(ntHeaders);

		if (!section || (strlen(section) <= 0))
		{
			data->base = GlobalBase;
			data->end = data->base + ntHeaders->OptionalHeader.SizeOfHeaders;

			return true;
		}

		for (uint32_t i = 0; i < ntHeaders->FileHeader.NumberOfSections; i++, cur_section++)
		{
			char sectionName[sizeof(IMAGE_SECTION_HEADER::Name) + 1] = { };
			memcpy(sectionName, cur_section->Name, sizeof(IMAGE_SECTION_HEADER::Name));

			if (!strcmp(sectionName, section))
			{
				data->base = GlobalBase + cur_section->VirtualAddress;
				data->end = data->base + cur_section->Misc.VirtualSize;

				return true;
			}
		}

		return false;
	}
}

static void APIENTRY InitializeOBSE64LogSystem()
{
	const uint32_t BufferSize = 1024;
	auto Buffer = make_unique<char[]>((size_t)BufferSize + 1);
	if (!Buffer)
	{
	UnknownDllName:
		DebugLog::openRelative(CSIDL_MYDOCUMENTS, "\\My Games\\Oblivion Remastered\\OBSE\\Logs\\UnknownPluginName.log");
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
		DebugLog::openRelative(CSIDL_MYDOCUMENTS, Path.get());
	}
}

bool APIENTRY Start(const OBSEInterface* obse)
{
	InitializeOBSE64LogSystem();
	
	_MESSAGE("OBSE64 version check. obse64Version: 0x%x, runtimeVersion: 0x%x", obse->obse64Version, obse->runtimeVersion);
	_MESSAGE("Plugin \"" TURPENTINE_NAME "\" version check. Version: 0x%x, Author: %s", TURPENTINE_VERSION, TURPENTINE_AUTHOR);

	GlobalAppPath = Utils::GetApplicationPath();
	GlobalBase = (uintptr_t)GetModuleHandleA(nullptr);

	_MESSAGE("AppPath: %s", GlobalAppPath.c_str());

	if (!Utils::GetPESectionRange(".text", &GlobalSection[0]))
	{
		_ERROR("There is no information about \".text\" in the module");
		return false;
	}

	if (!Utils::GetPESectionRange(".rdata", &GlobalSection[1]))
	{
		_ERROR("There is no information about \".rdata\" in the module");
		return false;
	}

	if (!Utils::GetPESectionRange(".data", &GlobalSection[2]))
	{
		_ERROR("There is no information about \".data\" in the module");
		return false;
	}

	msrtti::section temp;
	if (Utils::GetPESectionRange(".textbss", &temp))
	{
		GlobalSection[0].base = std::min(GlobalSection[0].base, temp.base);
		GlobalSection[0].end = std::max(GlobalSection[0].end, temp.end);
	}

	if (Utils::GetPESectionRange(".interpr", &temp))
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

	DebugLog::flush();

	auto TOMLResult = toml::try_parse(TURPENTINE_NAME ".toml", toml::spec::v(1, 1, 0));
	if (!TOMLResult.is_ok())
	{
		_ERROR("Error reading the settings file " TURPENTINE_NAME ".toml");
		return false;
	}

	auto& TOMLData = TOMLResult.unwrap();
	if (TOMLData.contains("Patches"))
	{
		auto& TOMLPatches = TOMLData.at("Patches");
		if (TOMLPatches.contains("bMemory") && TOMLPatches.at("bMemory").is_boolean() && TOMLPatches.at("bMemory").as_boolean())
			Turpentine::Patches::PatchMemory();
		if (TOMLPatches.contains("bThreads") && TOMLPatches.at("bThreads").is_boolean() && TOMLPatches.at("bThreads").as_boolean())
			Turpentine::Patches::PatchThreads();
		if (TOMLPatches.contains("iMaxStdio") && TOMLPatches.at("iMaxStdio").is_integer())
			Turpentine::Patches::PatchMaxStdio(TOMLPatches.at("iMaxStdio").as_integer());
	}

	return true;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
		GlobalModuleBase = (uintptr_t)hModule;

    return TRUE;
}

