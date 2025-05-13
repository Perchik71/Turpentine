// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include <TUtils.h>

extern uintptr_t GlobalBase;

namespace Turpentine
{
	namespace Utils
	{
		bool FileExists(const char* fname) noexcept(true)
		{
			auto fflags = GetFileAttributesA(fname);
			return !((fflags == INVALID_FILE_ATTRIBUTES) || ((fflags & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY));
		}

		bool FileExists(const wchar_t* fname) noexcept(true)
		{
			auto fflags = GetFileAttributesW(fname);
			return !((fflags == INVALID_FILE_ATTRIBUTES) || ((fflags & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY));
		}

		wstring AnsiToWide(const string& String) noexcept(true)
		{
			auto l = MultiByteToWideChar(CP_ACP, 0, String.c_str(), (int32_t)String.length(), nullptr, 0);
			if (l <= 0) return L"";
			wstring temp;
			temp.resize(l);
			MultiByteToWideChar(CP_ACP, 0, String.c_str(), (int32_t)String.length(), temp.data(), (int32_t)temp.length());
			return temp;
		}

		string WideToAnsi(const wstring& String) noexcept(true)
		{
			auto l = WideCharToMultiByte(CP_ACP, 0, String.c_str(), (int32_t)String.length(), nullptr, 0, nullptr, nullptr);
			if (l <= 0) return "";
			string temp;
			temp.resize(l);
			WideCharToMultiByte(CP_ACP, 0, String.c_str(), (int32_t)String.length(), temp.data(), (int32_t)temp.length(), nullptr, nullptr);
			return temp;
		}

		string AnsiToUtf8(const string& String) noexcept(true)
		{
			auto Source = AnsiToWide(String);
			auto l = WideCharToMultiByte(CP_UTF8, 0, Source.c_str(), (int32_t)String.length(), nullptr, 0, nullptr, nullptr);
			if (l <= 0) return "";
			string temp;
			temp.resize(l);
			WideCharToMultiByte(CP_ACP, 0, Source.c_str(), (int32_t)Source.length(), temp.data(), (int32_t)temp.length(), nullptr, nullptr);
			return temp;
		}

		string Utf8ToAnsi(const string& String) noexcept(true)
		{
			auto l = MultiByteToWideChar(CP_UTF8, 0, String.c_str(), (int32_t)String.length(), nullptr, 0);
			if (l <= 0) return "";
			wstring temp;
			temp.resize(l);
			MultiByteToWideChar(CP_UTF8, 0, String.c_str(), (int32_t)String.length(), temp.data(), (int32_t)temp.length());
			return WideToAnsi(temp);
		}

		string& Trim(string& String) noexcept(true)
		{
			constexpr static char whitespaceDelimiters[] = " \t\n\r\f\v";

			if (!String.empty())
			{
				String.erase(String.find_last_not_of(whitespaceDelimiters) + 1);
				String.erase(0, String.find_first_not_of(whitespaceDelimiters));
			}

			return String;
		}

		string GetApplicationPath() noexcept(true)
		{
			string _app_path;
			const uint32_t BufferSize = 1024;

			auto Buffer = make_unique<char[]>((size_t)BufferSize + 1);
			if (Buffer && GetModuleFileNameA(GetModuleHandleA(nullptr), Buffer.get(), BufferSize))
			{
				PathRemoveFileSpecA(Buffer.get());
				_app_path = Buffer.get();
				_app_path += "\\";
			}

			return _app_path;
		}

		string GetGamePluginPath() noexcept(true)
		{
			return GetApplicationPath() + "OBSE\\Plugins\\";
		}

		bool GetPESectionRange(const char* section, msrtti::section* data) noexcept(true)
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

		uint32_t __stdcall MurmurHash32A(const void* Key, size_t Len, uint32_t Seed) noexcept(true)
		{
			/* -----------------------------------------------------------------------------
			// https://github.com/abrandoned/murmur2/blob/master/MurmurHash2.c#L65
			// MurmurHash2, by Austin Appleby
			//
			// Note - This code makes a few assumptions about how your machine behaves -
			// 1. We can read a 4-byte value from any address without crashing
			// 2. sizeof(int) == 4
			//
			// And it has a few limitations -
			//
			// 1. It will not work incrementally.
			// 2. It will not produce the same results on little-endian and big-endian
			//    machines.    */

			/* 'm' and 'r' are mixing constants generated offline.
			 They're not really 'magic', they just happen to work well.  */

			const uint32_t m = 0x5bd1e995;
			const int r = 24;

			/* Initialize the hash to a 'random' value */

			uint32_t h = (uint32_t)((size_t)Seed ^ Len);

			/* Mix 4 bytes at a time into the hash */

			const unsigned char* data = (const unsigned char*)Key;

			while (Len >= 4)
			{
				uint32_t k = *(uint32_t*)data;

				k *= m;
				k ^= k >> r;
				k *= m;

				h *= m;
				h ^= k;

				data += 4;
				Len -= 4;
			}

			/* Handle the last few bytes of the input array  */

			switch (Len)
			{
			case 3: h ^= data[2] << 16;
			case 2: h ^= data[1] << 8;
			case 1: h ^= data[0];
				h *= m;
			};

			/* Do a few final mixes of the hash to ensure the last few
			// bytes are well-incorporated.  */

			h ^= h >> 13;
			h *= m;
			h ^= h >> 15;

			return h;
		}

		uint64_t __stdcall MurmurHash64A(const void* Key, size_t Len, uint64_t Seed) noexcept(true)
		{
			/*-----------------------------------------------------------------------------
			// https://github.com/abrandoned/murmur2/blob/master/MurmurHash2.c#L65
			// MurmurHash2, 64-bit versions, by Austin Appleby
			//
			// The same caveats as 32-bit MurmurHash2 apply here - beware of alignment
			// and endian-ness issues if used across multiple platforms.
			//
			// 64-bit hash for 64-bit platforms
			*/
			const uint64_t m = 0xc6a4a7935bd1e995ull;
			const int r = 47;

			uint64_t h = Seed ^ (Len * m);

			const uint64_t* data = (const uint64_t*)Key;
			const uint64_t* end = data + (Len / 8);

			while (data != end)
			{
				uint64_t k = *data++;

				k *= m;
				k ^= k >> r;
				k *= m;

				h ^= k;
				h *= m;
			}

			const unsigned char* data2 = (const unsigned char*)data;

			switch (Len & 7)
			{
			case 7: h ^= ((uint64_t)data2[6]) << 48;
			case 6: h ^= ((uint64_t)data2[5]) << 40;
			case 5: h ^= ((uint64_t)data2[4]) << 32;
			case 4: h ^= ((uint64_t)data2[3]) << 24;
			case 3: h ^= ((uint64_t)data2[2]) << 16;
			case 2: h ^= ((uint64_t)data2[1]) << 8;
			case 1: h ^= ((uint64_t)data2[0]);
				h *= m;
			}

			h ^= h >> r;
			h *= m;
			h ^= h >> r;

			return h;
		}
	}
}
