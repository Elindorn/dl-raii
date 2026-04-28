#pragma once

#include <string>
#include <filesystem>

#include <dl_raii/impl/defaultBackend.hpp>

namespace dl
{
	template <typename Backend = backend::defaultBackend>
	class Library
	{
	public:
		explicit Library(std::filesystem::path& path);
		~Library() noexcept;

		Library(const Library&) = delete;
		Library& operator=(const Library&) = delete;

		Library(Library&&) noexcept;
		Library& operator=(Library&&) noexcept;

		[[nodiscard]]
		void* getSymbol(const std::string& path) const;

	private:
		Backend::Handle _handle;
	};
}

#include <dl_raii/impl/library.ipp>
