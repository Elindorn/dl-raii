#pragma once

#include <string>

namespace dl::util
{
	/**
	 * @brief Demangle a C++ symbol name into a human-readable form.
	 *
	 * Wraps platform-specific demangling functions (`abi::__cxa_demangle` on POSIX,
	 * `UnDecorateSymbolName` on Windows). Falls back to returning the original
	 * string if demangling is not available or fails.
	 *
	 * @param mangledName The mangled symbol name.
	 * @return A std::string containing the demangled name, or the original
	 *         name on failure.
	 *
	 * @note This is a quality-of-life utility; it is not required for the core
	 *       library functionality.
	 */
	std::string demangle(const std::string& mangledName);
}

#include <dl_raii/impl/util.ipp>
