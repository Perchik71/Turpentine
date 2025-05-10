#pragma once

#include "obse64_common/Relocation.h"
#include "obse64_common/Utilities.h"

inline RelocPtr <std::uintptr_t> _Console_Print(0x06593FA0);

template <class... Args>
void Console_Print(const char* fmt, Args... args)
{
	using func_t = void(*)(const char*, ...);
	auto func = reinterpret_cast<func_t>(_Console_Print.getPtr());
	return func(fmt, args...);
}
