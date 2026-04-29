#pragma once

#include <filesystem>

namespace dl::backend
{
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
