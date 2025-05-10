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
		}

		void APIENTRY PatchMemory() noexcept(true);
	}
}