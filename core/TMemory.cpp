// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "TMemory.h"
#include <detours/Detours.h>
#include <Voltek.MemoryManager.h>

extern uintptr_t GlobalBase;

namespace Turpentine
{
	namespace Patches
	{
		namespace Impl
		{
			MemoryHeap GlobalMemoryHeap;

			MemoryHeap::MemoryHeap()
			{
				voltek::scalable_memory_manager_initialize();
			}

			void* MemoryHeap::alloc(size_t size, size_t alignment, bool aligned = false, bool zeroed = true) noexcept(true)
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

			void MemoryHeap::dealloc(void* block) noexcept(true)
			{
				if (!voltek::scalable_msize(block))
					_aligned_free(block);
				else
					voltek::scalable_free(block);
			}

			size_t MemoryHeap::msize(void* block) noexcept(true)
			{
				return voltek::scalable_msize(block);
			}

			void* MemoryHeapIntf::calloc(size_t count, size_t size) noexcept(true)
			{
				return MemoryHeap::alloc(count * size, 0);
			}

			void* MemoryHeapIntf::malloc(size_t size) noexcept(true)
			{
				return MemoryHeap::alloc(size, 0);
			}

			void* MemoryHeapIntf::realloc(void* memory, size_t size) noexcept(true)
			{
				void* newMemory = nullptr;

				if (size > 0)
				{
					newMemory = MemoryHeap::alloc(size, 0, false);

					if (memory)
						memcpy(newMemory, memory, min(size, voltek::scalable_msize(memory)));
				}

				MemoryHeap::dealloc(memory);
				return newMemory;
			}

			void MemoryHeapIntf::free(void* block) noexcept(true)
			{
				MemoryHeap::dealloc(block);
			}

			void* MemoryHeapIntf::aligned_malloc(size_t size, size_t alignment) noexcept(true)
			{
				return MemoryHeap::alloc(size, alignment, true);
			}

			void* MemoryHeapIntf::aligned_realloc(void* memory, size_t size, size_t alignment) noexcept(true)
			{
				void* newMemory = nullptr;

				if (size > 0)
				{
					newMemory = MemoryHeap::alloc(size, alignment, true);

					if (memory)
						memcpy(newMemory, memory, min(size, voltek::scalable_msize(memory)));
				}

				MemoryHeap::dealloc(memory);
				return newMemory;
			}

			void MemoryHeapIntf::aligned_free(void* block) noexcept(true)
			{
				MemoryHeap::dealloc(block);
			}
		}

		void APIENTRY PatchMemory() noexcept(true)
		{
			// Replacement of all functions of the standard allocator.

			Detours::IATHook(GlobalBase, "API-MS-WIN-CRT-HEAP-L1-1-0.DLL", "_aligned_free", (uintptr_t)&Impl::MemoryHeapIntf::aligned_free);
			Detours::IATHook(GlobalBase, "API-MS-WIN-CRT-HEAP-L1-1-0.DLL", "_aligned_malloc", (uintptr_t)&Impl::MemoryHeapIntf::aligned_malloc);
			Detours::IATHook(GlobalBase, "API-MS-WIN-CRT-HEAP-L1-1-0.DLL", "_aligned_realloc", (uintptr_t)&Impl::MemoryHeapIntf::aligned_realloc);
			Detours::IATHook(GlobalBase, "API-MS-WIN-CRT-HEAP-L1-1-0.DLL", "calloc", (uintptr_t)&Impl::MemoryHeapIntf::calloc);
			Detours::IATHook(GlobalBase, "API-MS-WIN-CRT-HEAP-L1-1-0.DLL", "free", (uintptr_t)&Impl::MemoryHeapIntf::free);
			Detours::IATHook(GlobalBase, "API-MS-WIN-CRT-HEAP-L1-1-0.DLL", "malloc", (uintptr_t)&Impl::MemoryHeapIntf::malloc);
			Detours::IATHook(GlobalBase, "API-MS-WIN-CRT-HEAP-L1-1-0.DLL", "realloc", (uintptr_t)&Impl::MemoryHeapIntf::realloc);
		}
	}
}
