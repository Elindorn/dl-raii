#pragma once

#include <stdexcept>
#include <filesystem>
#include <string>


#if defined(_WIN32)


#include <windows.h>

namespace dl::backend
{
	class DefaultBackend
	{
	public:
		using Handle = HMODULE;

		static constexpr Handle nullHandle = nullptr;

		static constexpr auto prefix = "";
		static constexpr auto postfix = "";
		static constexpr auto extension = "dll";

		// the path is guaranteed to exist
		static Handle loadLibrary(const std::filesystem::path& path)
		{
			HMODULE handle = LoadLibraryW(path.wstring().c_str());

			if (handle == nullHandle)
				throwException(GetLastError());

			return handle;
		}

		// handle is not nullHandle
		static void* getSymbol(const Handle handle, const char* symbol)
		{
			FARPROC proc =  GetProcAddress(handle, symbol);

			if (proc == nullptr)
				throwException(GetLastError());

			return reinterpret_cast<void*>(proc);
		}

		// handle is not nullHandle
		static void unloadLibrary(const Handle handle) noexcept
		{
			FreeLibrary(handle);
		}

	private:
		static void throwException(const DWORD errorCode)
		{
			LPSTR messageBuffer = nullptr;
			std::string message;

			const size_t size = FormatMessageA(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				nullptr,
				errorCode,
				MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
				messageBuffer,
				0,
				nullptr
			);

			if (size == 0)
				message = "Unknown error";
			else
				message = messageBuffer;

			LocalFree(messageBuffer);

			throw std::runtime_error(message);
		}
	};
}


#elif defined(__linux__) || defined(__APPLE__)


#include <dlfcn.h>

namespace dl::backend
{
	class DefaultBackend
	{
	public:
		using Handle = void*;

		static constexpr Handle nullHandle = nullptr;

#if defined(__linux__)
		static constexpr auto prefix = "lib";
		static constexpr auto postfix = "";
		static constexpr auto extension = "so";
#else
		static constexpr auto prefix = "lib";
		static constexpr auto postfix = "";
		static constexpr auto extension = "dylib";
#endif


		static Handle loadLibrary(const std::filesystem::path& path)
		{
			Handle handle = dlopen(path.wstring().c_str(), RTLD_LAZY);

			if (handle == nullHandle)
				throw std::runtime_error(dlerror());

			return handle;
		}

		static void* getSymbol(const Handle handle, const char* symbol)
		{
			void* proc = dlsym(handle, symbol);

			if (proc == nullptr)
				throw std::runtime_error(dlerror());

			return proc;
		}

		static void unloadLibrary(const Handle handle) noexcept
		{
			dlclose(handle);
		}
	};
}


#else
#error "Unsupported platform"
#endif
