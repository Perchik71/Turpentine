#include <obse64/PluginAPI.h>
#include <detours/Detours.h>
#include <Voltek.MemoryManager.h>
#include <ms_rtti.h>

#define TURPENTINE_VERSION MAKE_EXE_VERSION(0, 1, 0)
#define TURPENTINE_NAME "Turpentine"
#define TURPENTINE_AUTHOR "perchik71"
#define USE_RTTI_EXPORT 0

bool APIENTRY Start(const OBSEInterface* obse);
void APIENTRY PatchMemory();

std::string GlobalAppPath;
uintptr_t GlobalBase = 0;
uintptr_t GlobalModuleBase = 0;
msrtti::section GlobalSection[3];

extern "C"
{
#if (FO4_VER < FO4_984_VERSION)
	// for f4se 6.23 and older
	__declspec(dllexport) bool F4SEPlugin_Query(const F4SEInterface* f4se, PluginInfo* info)
	{
		if (f4se->runtimeVersion != RUNTIME_VERSION_1_10_163)
			return false;

		info->infoVersion = PluginInfo::kInfoVersion;
		info->version = XCell::ModVersion;
		info->name = XCell::ModName;

		return true;
	}
#else
	// for f4se 0.1 alpha and newer
	__declspec(dllexport) OBSEPluginVersionData OBSEPlugin_Version =
	{
		OBSEPluginVersionData::kVersion,
		TURPENTINE_VERSION,
		TURPENTINE_NAME,
		TURPENTINE_AUTHOR,
		0,
		0,
		{ RUNTIME_VERSION, 0 },
		0,	// works with any version of the script extender. you probably do not need to put anything here
	};
#endif // !FO4_V984

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

bool APIENTRY Start(const OBSEInterface* obse)
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

		//_MESSAGE("[DEBUG] FileName:\"%s\" Path:\"%s\"",  FileName, Path.get());
	}

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

	PatchMemory();

	return true;
}

constexpr auto MEM_GB = 1073741824;

namespace impl
{
	class memory_manager
	{
	public:
		memory_manager()
		{
			voltek::scalable_memory_manager_initialize();
		}

		memory_manager(const memory_manager&) = default;
		memory_manager& operator=(const memory_manager&) = default;

		static void* alloc(size_t size, size_t alignment, bool aligned = false, bool zeroed = true)
		{
			if (!aligned)
				alignment = 16;

			if (!size)
				return voltek::scalable_alloc(0);

			void* ptr = nullptr;

			if (alignment <= 16)
			{
				if ((alignment & (alignment - 1)) != 0)
				{
					alignment--;
					alignment |= alignment >> 1;
					alignment |= alignment >> 2;
					alignment |= alignment >> 4;
					alignment |= alignment >> 8;
					alignment |= alignment >> 16;
					alignment++;
				}

				if ((size % alignment) != 0)
					size = ((size + alignment - 1) / alignment) * alignment;

				ptr = voltek::scalable_alloc(size);
			}
			else
				ptr = _aligned_malloc(size, alignment);

			if (ptr && zeroed) memset(ptr, 0, size);
			return ptr;
		}

		inline static void dealloc(void* block)
		{
			if (!voltek::scalable_msize(block))
				_aligned_free(block);
			else
				voltek::scalable_free(block);
		}

		inline static size_t msize(void* block)
		{
			return voltek::scalable_msize(block);
		}
	};

	memory_manager g_memory_mgr;

	namespace detail
	{
		class BGSMemoryManager
		{
		public:
			static void* alloc(BGSMemoryManager* self, size_t size, uint32_t alignment, bool aligned)
			{
				return memory_manager::alloc(size, alignment, aligned, true);
			}

			static void dealloc(BGSMemoryManager* self, void* block, bool aligned)
			{
				memory_manager::dealloc(block);
			}

			static void* realloc(BGSMemoryManager* self, void* old_block, size_t size, uint32_t alignment, bool aligned)
			{
				auto new_ptr = memory_manager::alloc(size, alignment, aligned, true);
				if (!new_ptr) return nullptr;

				if (old_block)
				{
					auto old_size = memory_manager::msize(old_block);
					memcpy(new_ptr, old_block, min(old_size, size));
					memory_manager::dealloc(old_block);
				}

				return new_ptr;
			}

			static size_t msize(BGSMemoryManager* self, void* memory)
			{
				return memory_manager::msize(memory);
			}
		};
	}

	static void* calloc(size_t count, size_t size)
	{
		return memory_manager::alloc(count * size, 0);
	}

	static void* malloc(size_t size)
	{
		return memory_manager::alloc(size, 0);
	}

	static void* realloc(void* memory, size_t size)
	{
		void* newMemory = nullptr;

		if (size > 0)
		{
			newMemory = memory_manager::alloc(size, 0, false);

			if (memory)
				memcpy(newMemory, memory, min(size, voltek::scalable_msize(memory)));
		}

		memory_manager::dealloc(memory);
		return newMemory;
	}

	static void free(void* block)
	{
		memory_manager::dealloc(block);
	}

	static void* aligned_malloc(size_t size, size_t alignment)
	{
		return memory_manager::alloc(size, alignment, true);
	}

	static void* aligned_realloc(void* memory, size_t size, size_t alignment)
	{
		void* newMemory = nullptr;

		if (size > 0)
		{
			newMemory = memory_manager::alloc(size, alignment, true);

			if (memory)
				memcpy(newMemory, memory, min(size, voltek::scalable_msize(memory)));
		}

		memory_manager::dealloc(memory);
		return newMemory;
	}	

	static void aligned_free(void* block)
	{
		memory_manager::dealloc(block);
	}
}

void APIENTRY PatchMemory()
{
	// Replacement of all functions of the standard allocator.

	Detours::IATHook(GlobalBase, "API-MS-WIN-CRT-HEAP-L1-1-0.DLL", "_aligned_free", (uintptr_t)&impl::aligned_free);
	Detours::IATHook(GlobalBase, "API-MS-WIN-CRT-HEAP-L1-1-0.DLL", "_aligned_malloc", (uintptr_t)&impl::aligned_malloc);
	Detours::IATHook(GlobalBase, "API-MS-WIN-CRT-HEAP-L1-1-0.DLL", "_aligned_realloc", (uintptr_t)&impl::aligned_realloc);
	Detours::IATHook(GlobalBase, "API-MS-WIN-CRT-HEAP-L1-1-0.DLL", "calloc", (uintptr_t)&impl::calloc);
	Detours::IATHook(GlobalBase, "API-MS-WIN-CRT-HEAP-L1-1-0.DLL", "free", (uintptr_t)&impl::free);
	Detours::IATHook(GlobalBase, "API-MS-WIN-CRT-HEAP-L1-1-0.DLL", "malloc", (uintptr_t)&impl::malloc);
	Detours::IATHook(GlobalBase, "API-MS-WIN-CRT-HEAP-L1-1-0.DLL", "realloc", (uintptr_t)&impl::realloc);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		GlobalModuleBase = (uintptr_t)hModule;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

