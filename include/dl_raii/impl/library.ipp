#pragma once

namespace dl
{
	template <typename Backend>
	Library<Backend>::Library(const std::filesystem::path& path) :
		_handle(nullptr)
	{
		if (!std::filesystem::is_regular_file(path))
			throw std::invalid_argument("path is not a regular file");

		_handle = Backend::loadLibrary(path);
	}

	template <typename Backend>
	Library<Backend>::Library(const typename Backend::Handle& handle) :
		_handle(handle) {}

	template <typename Backend>
	Library<Backend>::~Library() noexcept
	{
		if (_handle != Backend::nullHandle)
			Backend::unloadLibrary(_handle);
	}

	template <typename Backend>
	Library<Backend>::Library(Library&& other) noexcept
	{
		_handle = other._handle;
		other._handle = Backend::nullHandle;
	}

	template <typename Backend>
	Library<Backend>& Library<Backend>::operator=(Library&& other) noexcept
	{
		if (this != &other)
		{
			if (_handle != Backend::nullHandle)
				Backend::unloadLibrary(_handle);

			_handle = other._handle;
			other._handle = Backend::nullHandle;
		}

		return *this;
	}

	template <typename Backend>
	void* Library<Backend>::getSymbol(const std::string& path) const
	{
		if (_handle == Backend::nullHandle)
			throw std::runtime_error("Handle is null when trying to get symbol");

		return Backend::getSymbol(_handle, path);
	}
}
