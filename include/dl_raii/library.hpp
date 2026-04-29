#pragma once

#include <string>
#include <filesystem>

#include <dl_raii/impl/defaultBackend.hpp>
#include <dl_raii/impl/backendConcept.hpp>

namespace dl
{
	template <backend::BackendConcept Backend = backend::DefaultBackend>
	class Library
	{
	public:
		explicit Library(const std::filesystem::path& path);
		explicit Library(const Backend::Handle& handle);

		static Library load(const std::filesystem::path& path);

		~Library() noexcept;

		Library(const Library&) = delete;
		Library& operator=(const Library&) = delete;

		Library(Library&&) noexcept;
		Library& operator=(Library&&) noexcept;

		[[nodiscard]] void* getSymbol(const std::string& name) const;

		template <typename F> requires std::is_function_v<F>
		[[nodiscard]] F* getFunction(const std::string& name) const;

	private:
		Backend::Handle _handle;
	};
}

#include <dl_raii/impl/library.ipp>
