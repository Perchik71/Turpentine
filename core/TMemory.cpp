// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include <TMemory.h>
#include <TRelocation.h>
#include <Voltek.MemoryManager.h>
#include <detours/Detours.h>

#include "..\depends\vmm\source\vmmmain.h"

extern uintptr_t GlobalBase;

namespace Turpentine
{
	namespace Patches
	{
		constexpr auto MEM_GB = 1073741824;

		namespace Impl
		{
			MemoryHeap GlobalMemoryHeap;

			MemoryHeap::MemoryHeap()
			{
				voltek::scalable_memory_manager_initialize();
			}

			void* MemoryHeap::alloc(size_t size, size_t alignment, bool aligned, bool zeroed) noexcept(true)
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

					if (memory && newMemory)
						memcpy(newMemory, memory, min(size, voltek::scalable_msize(memory)));
				}

				MemoryHeap::dealloc(memory);
				return newMemory;
			}

			void MemoryHeapIntf::aligned_free(void* block) noexcept(true)
			{
				MemoryHeap::dealloc(block);
			}
	
			namespace AK
			{
				voltek::memory_manager::memory_manager* audiomgr = nullptr;

				void MemoryMgr::DumpToFile(const wchar_t* filename) noexcept(true)
				{}
				
				void MemoryMgr::Free(int unk, void* pointer) noexcept(true)
				{
					if (!audiomgr->msize(pointer))
						_aligned_free(pointer);
					else
						audiomgr->free(pointer);
				}
				
				void MemoryMgr::GetCategoryStats(int unk, AK::MemoryMgr::CategoryStats& stats) noexcept(true)
				{}
				
				void MemoryMgr::GetDefaultSettings(AkMemSettings& setting) noexcept(true)
				{}
				
				void MemoryMgr::GetGlobalStats(GlobalStats& stats) noexcept(true)
				{}
				
				bool MemoryMgr::Init(AkMemSettings& setting) noexcept(true)
				{
					return true;
				}
				
				bool MemoryMgr::InitForThread() noexcept(true)
				{
					return true;
				}
				
				bool MemoryMgr::IsInitialized() noexcept(true)
				{
					return true;
				}

				void* MemoryMgr::Malign(int unk, size_t size, uint32_t alignment) noexcept(true)
				{
					if (!alignment)
						alignment = 16;

					if (!size)
						return audiomgr->alloc(0);

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

						ptr = audiomgr->alloc(size);
					}
					else
						ptr = _aligned_malloc(size, alignment);

					return ptr;
				}

				void* MemoryMgr::Malloc(int unk, size_t size) noexcept(true)
				{
					return Malign(unk, size, 0);
				}

				void* MemoryMgr::Realloc(int unk, void* pointer, size_t size) noexcept(true)
				{
					return ReallocAligned(unk, pointer, size, 0);
				}

				void* MemoryMgr::ReallocAligned(int unk, void* pointer, size_t size, uint32_t alignment) noexcept(true)
				{
					void* newMemory = nullptr;

					if (size > 0)
					{
						newMemory = Malign(unk, size, alignment);

						if (pointer && newMemory)
							memcpy(newMemory, pointer, min(size, audiomgr->msize(pointer)));
					}

					MemoryMgr::Free(unk, pointer);
					return newMemory;
				}

				void MemoryMgr::StartProfileThreadUsage() noexcept(true)
				{}	

				void MemoryMgr::StopProfileThreadUsage() noexcept(true)
				{}	

				void MemoryMgr::Term() noexcept(true)
				{}

				void MemoryMgr::TermForThread() noexcept(true)
				{}

				void MemoryMgr::TrimForThread() noexcept(true)
				{}
			}
		}

		void APIENTRY Memory(bool crtmem, bool audiomem) noexcept(true)
		{
			if (crtmem || audiomem)
			{
				MEMORYSTATUSEX statex = { 0 };
				statex.dwLength = sizeof(MEMORYSTATUSEX);
				if (!GlobalMemoryStatusEx(&statex))
					return;

				_MESSAGE("Memory (Total: %.2f Gb, Available: %.2f Gb)",
					((double)statex.ullTotalPageFile / MEM_GB), ((double)statex.ullAvailPageFile / MEM_GB));
			}

			if (crtmem)
			{
				// Replacement of all functions of the standard allocator.

				REL::DetourIAT(GlobalBase, "API-MS-WIN-CRT-HEAP-L1-1-0.DLL", "_aligned_free", (uintptr_t)&Impl::MemoryHeapIntf::aligned_free);
				REL::DetourIAT(GlobalBase, "API-MS-WIN-CRT-HEAP-L1-1-0.DLL", "_aligned_malloc", (uintptr_t)&Impl::MemoryHeapIntf::aligned_malloc);
				REL::DetourIAT(GlobalBase, "API-MS-WIN-CRT-HEAP-L1-1-0.DLL", "_aligned_realloc", (uintptr_t)&Impl::MemoryHeapIntf::aligned_realloc);
				REL::DetourIAT(GlobalBase, "API-MS-WIN-CRT-HEAP-L1-1-0.DLL", "calloc", (uintptr_t)&Impl::MemoryHeapIntf::calloc);
				REL::DetourIAT(GlobalBase, "API-MS-WIN-CRT-HEAP-L1-1-0.DLL", "free", (uintptr_t)&Impl::MemoryHeapIntf::free);
				REL::DetourIAT(GlobalBase, "API-MS-WIN-CRT-HEAP-L1-1-0.DLL", "malloc", (uintptr_t)&Impl::MemoryHeapIntf::malloc);
				REL::DetourIAT(GlobalBase, "API-MS-WIN-CRT-HEAP-L1-1-0.DLL", "realloc", (uintptr_t)&Impl::MemoryHeapIntf::realloc);
			
				_MESSAGE("Install MemoryManager patch");
			}

			// AK::MemoryMgr for 0.411.140.0
			// AK::MemoryMgr::DumpToFile(wchar_t const*)	0000000140D54A90	95
			// AK::MemoryMgr::Free(int, void*)	00000001463CA220	102
			// AK::MemoryMgr::GetCategoryStats(int, AK::MemoryMgr::CategoryStats&)	0000000140D54A90	116
			// AK::MemoryMgr::GetDefaultSettings(AkMemSettings&)	00000001463CA950	126
			// AK::MemoryMgr::GetGlobalStats(AK::MemoryMgr::GlobalStats&)	00000001463CA230	141
			// AK::MemoryMgr::Init(AkMemSettings*)	00000001463CAA10	186
			// AK::MemoryMgr::InitForThread(void)	00000001463CAD00	190
			// AK::MemoryMgr::IsInitialized(void)	00000001463CA250	192
			// AK::MemoryMgr::Malign(int, unsigned __int64, uint)	00000001463CA260	210
			// AK::MemoryMgr::Malloc(int, unsigned __int64)	00000001463CA270	211
			// AK::MemoryMgr::Realloc(int, void*, unsigned __int64)	00000001463CA280	287
			// AK::MemoryMgr::ReallocAligned(int, void*, unsigned __int64, uint)	00000001463CA290	288
			// AK::MemoryMgr::StartProfileThreadUsage(void)	0000000140D54A90	420
			// AK::MemoryMgr::StopProfileThreadUsage(void)	0000000140D73BC0	427
			// AK::MemoryMgr::Term(void)	00000001463CAD10	431
			// AK::MemoryMgr::TermForThread(void)	00000001463CAD50	434
			// AK::MemoryMgr::TrimForThread(void)	00000001463CAD60	435

			if (audiomem)
			{
				Impl::AK::audiomgr = new voltek::memory_manager::memory_manager();

				REL::DetourJump(REL::Offset(0x63CA220), (uintptr_t)&Impl::AK::MemoryMgr::Free);
				REL::DetourJump(REL::Offset(0x63CA260), (uintptr_t)&Impl::AK::MemoryMgr::Malign);
				REL::DetourJump(REL::Offset(0x63CA270), (uintptr_t)&Impl::AK::MemoryMgr::Malloc);
				REL::DetourJump(REL::Offset(0x63CA280), (uintptr_t)&Impl::AK::MemoryMgr::Realloc);
				REL::DetourJump(REL::Offset(0x63CA290), (uintptr_t)&Impl::AK::MemoryMgr::ReallocAligned);

				_MESSAGE("Install AudioAllocator patch");
			}
		}
	}
}