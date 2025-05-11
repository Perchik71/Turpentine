// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

namespace Turpentine
{
	namespace Patches
	{
		namespace Impl
		{
			class MemoryHeap
			{
				MemoryHeap(const MemoryHeap&) = delete;
				MemoryHeap& operator=(const MemoryHeap&) = delete;
			public:
				MemoryHeap();

				[[nodiscard]] static void* alloc(size_t size, size_t alignment, bool aligned = false, bool zeroed = true) noexcept(true);
				inline static void dealloc(void* block)  noexcept(true);
				[[nodiscard]] inline static size_t msize(void* block) noexcept(true);
			};

			class MemoryHeapIntf
			{
			public:
				[[nodiscard]] static void* calloc(size_t count, size_t size) noexcept(true);
				[[nodiscard]] static void* malloc(size_t size) noexcept(true);
				[[nodiscard]] static void* realloc(void* memory, size_t size) noexcept(true);
				static void free(void* block) noexcept(true);
				[[nodiscard]] static void* aligned_malloc(size_t size, size_t alignment) noexcept(true);
				[[nodiscard]] static void* aligned_realloc(void* memory, size_t size, size_t alignment) noexcept(true);
				static void aligned_free(void* block) noexcept(true);
			};

			class AkMemSettings
			{};

			namespace AK
			{
				class MemoryMgr
				{
				public:
					struct CategoryStats
					{};

					struct GlobalStats
					{};

					MemoryMgr() = default;

					static void DumpToFile(const wchar_t* filename) noexcept(true);				// no used
					static void Free(int unk, void* pointer) noexcept(true);
					static void GetCategoryStats(int unk, CategoryStats& stats) noexcept(true);	// no used
					static void GetDefaultSettings(AkMemSettings& setting) noexcept(true);
					static void GetGlobalStats(GlobalStats& stats) noexcept(true);
					[[nodiscard]] static bool Init(AkMemSettings& setting) noexcept(true);
					[[nodiscard]] static bool InitForThread() noexcept(true);
					[[nodiscard]] static bool IsInitialized() noexcept(true);
					[[nodiscard]] static void* Malign(int unk, size_t size, uint32_t alignment) noexcept(true);
					[[nodiscard]] static void* Malloc(int unk, size_t size) noexcept(true);
					[[nodiscard]] static void* Realloc(int unk, void* pointer, size_t size) noexcept(true);
					[[nodiscard]] static void* ReallocAligned(int unk, void* pointer, size_t size, uint32_t alignment) noexcept(true);
					static void StartProfileThreadUsage() noexcept(true);						// no used
					static void StopProfileThreadUsage() noexcept(true);						// no used
					static void Term() noexcept(true);
					static void TermForThread() noexcept(true);
					static void TrimForThread() noexcept(true);
				};
			}
		}

		void APIENTRY PatchMemory(bool crtmem, bool audiomem) noexcept(true);
	}
}