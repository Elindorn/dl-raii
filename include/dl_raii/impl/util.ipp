#pragma once

#if defined(__GNUC__) || defined(__clang__)

#include <cstdlib>
#include <cxxabi.h>

inline std::string dl::util::demangle(const std::string& mangledName)
{
#if __has_include(<cxxabi.h>)
	int status = 0;
	char* demangled = abi::__cxa_demangle(mangledName.c_str(), nullptr, nullptr, &status);

	if (status == 0 && demangled != nullptr)
	{
		std::string result = demangled;
		std::free(demangled);
		return result;
	}
#endif

	// Return as is, if failed to parse
	return mangledName;
}

#elif defined(_MSC_VER)

#include <windows.h>
#include <dbghelp.h>

#pragma comment(lib, "dbghelp.lib")

inline std::string dl::util::demangle(const std::string& mangledName)
{
	char buffer[1024];

	if (UnDecorateSymbolName(mangledName.c_str(), buffer, sizeof(buffer), UNDNAME_COMPLETE))
		return std::string(buffer);

	return mangledName;
}

#endif
