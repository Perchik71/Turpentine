// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include <ms_rtti.h>

namespace Turpentine
{
	namespace Utils
	{
		[[nodiscard]] bool FileExists(const char* fname) noexcept(true);
		[[nodiscard]] bool FileExists(const wchar_t* fname) noexcept(true);
		[[nodiscard]] wstring AnsiToWide(const string& String) noexcept(true);
		[[nodiscard]] string WideToAnsi(const wstring& String) noexcept(true);
		[[nodiscard]] string AnsiToUtf8(const string& String) noexcept(true);
		[[nodiscard]] string Utf8ToAnsi(const string& String) noexcept(true);
		[[nodiscard]] string& Trim(string& String) noexcept(true);
		[[nodiscard]] string GetApplicationPath() noexcept(true);
		[[nodiscard]] string GetGamePluginPath() noexcept(true);
		[[nodiscard]] bool GetPESectionRange(const char* section, msrtti::section* data) noexcept(true);
		[[nodiscard]] uint32_t __stdcall MurmurHash32A(const void* Key, size_t Len, uint32_t Seed) noexcept(true);
		[[nodiscard]] uint64_t __stdcall MurmurHash64A(const void* Key, size_t Len, uint64_t Seed) noexcept(true);

		// thread-safe template versions of FastCall()

		template<typename TR>
		__forceinline TR FastCall(size_t reloff) {
			return ((TR(__fastcall*)())(reloff))();
		}

		template<typename TR, typename T1>
		__forceinline TR FastCall(size_t reloff, T1 a1) {
			return ((TR(__fastcall*)(T1))(reloff))(a1);
		}

		template<typename TR, typename T1, typename T2>
		__forceinline TR FastCall(size_t reloff, T1 a1, T2 a2) {
			return ((TR(__fastcall*)(T1, T2))(reloff))(a1, a2);
		}

		template<typename TR, typename T1, typename T2, typename T3>
		__forceinline TR FastCall(size_t reloff, T1 a1, T2 a2, T3 a3) {
			return ((TR(__fastcall*)(T1, T2, T3))(reloff))(a1, a2, a3);
		}

		template<typename TR, typename T1, typename T2, typename T3, typename T4>
		__forceinline TR FastCall(size_t reloff, T1 a1, T2 a2, T3 a3, T4 a4) {
			return ((TR(__fastcall*)(T1, T2, T3, T4))(reloff))(a1, a2, a3, a4);
		}

		template<typename TR, typename T1, typename T2, typename T3, typename T4, typename T5>
		__forceinline TR FastCall(size_t reloff, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5) {
			return ((TR(__fastcall*)(T1, T2, T3, T4, T5))(reloff))(a1, a2, a3, a4, a5);
		}

		template<typename TR, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
		__forceinline TR FastCall(size_t reloff, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6) {
			return ((TR(__fastcall*)(T1, T2, T3, T4, T5, T6))(reloff))(a1, a2, a3, a4, a5, a6);
		}

		template<typename TR, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
		__forceinline TR FastCall(size_t reloff, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7) {
			return ((TR(__fastcall*)(T1, T2, T3, T4, T5, T6, T7))(reloff))(a1, a2, a3, a4, a5, a6, a7);
		}

		template<typename TR, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
		__forceinline TR FastCall(size_t reloff, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8) {
			return ((TR(__fastcall*)(T1, T2, T3, T4, T5, T6, T7, T8))(reloff))(a1, a2, a3, a4, a5, a6, a7, a8);
		}

		// thread-safe template versions of ThisCall()

		template<typename TR>
		__forceinline TR ThisCall(size_t reloff, const void* ths) {
			return ((TR(__fastcall*)(const void*))(reloff))(ths);
		}

		template<typename TR, typename T1>
		__forceinline TR ThisCall(size_t reloff, const void* ths, T1 a1) {
			return ((TR(__fastcall*)(const void*, T1))(reloff))(ths, a1);
		}

		template<typename TR, typename T1, typename T2>
		__forceinline TR ThisCall(size_t reloff, const void* ths, T1 a1, T2 a2) {
			return ((TR(__fastcall*)(const void*, T1, T2))(reloff))(ths, a1, a2);
		}

		template<typename TR, typename T1, typename T2, typename T3>
		__forceinline TR ThisCall(size_t reloff, const void* ths, T1 a1, T2 a2, T3 a3) {
			return ((TR(__fastcall*)(const void*, T1, T2, T3))(reloff))(ths, a1, a2, a3);
		}

		template<typename TR, typename T1, typename T2, typename T3, typename T4>
		__forceinline TR ThisCall(size_t reloff, const void* ths, T1 a1, T2 a2, T3 a3, T4 a4) {
			return ((TR(__fastcall*)(const void*, T1, T2, T3, T4))(reloff))(ths, a1, a2, a3, a4);
		}
	}
}