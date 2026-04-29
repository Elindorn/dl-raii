#pragma once

#include <filesystem>

namespace dl::backend
{
	/**
	 * @brief Concept describing the requirements for a dynamic library backend.
	 *
	 * A type `B` satisfies `BackendConcept` if it provides:
	 * - A nested type `B::Handle` (e.g., `void*` or `HMODULE`)
	 * - A nested convertible to string_view field `B::prefix`
	 * - A nested convertible to string_view field `B::postfix`
	 * - A nested convertible to string_view field `B::extension`
	 * - `static Handle open(const std::filesystem::path& path)`
	 * - `static void close(Handle handle) noexcept`
	 * - `static void* getSymbol(Handle handle, const std::string& name)`
	 *
	 * @see dl::Library, dl::backend::DefaultBackend
	 */
	template<typename Backend>
	concept BackendConcept = requires
	{
		typename Backend::Handle;

		{ Backend::prefix } -> std::convertible_to<std::string_view>;
		{ Backend::postfix } -> std::convertible_to<std::string_view>;
		{ Backend::extension } -> std::convertible_to<std::string_view>;
	} &&
	requires(const std::filesystem::path& path, typename Backend::Handle, const std::string& symbol)
	{
		{ Backend::loadLibrary(path) } -> std::same_as<typename Backend::Handle>;
		{ Backend::getSymbol(symbol) } -> std::same_as<void*>;
		{ Backend::unloadLibrary(path) } noexcept -> std::same_as<void>;
	};
}
