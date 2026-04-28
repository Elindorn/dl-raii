#pragma once

#include <string>
#include <filesystem>

#include <dl_raii\defaultBackend.hpp>

namespace dl
{
	class Library
	{
	public:
		explicit Library(std::filesystem::path path);
		~Library() noexcept;

		Library(const Library&) = delete;
		Library& operator=(const Library&) = delete;

		Library(Library&&) noexcept;
		Library& operator=(Library&&) noexcept;

		[[nodiscard]]
		void* getSymbol(const std::string& path);

	private:
		void* _handle;
	};
}
