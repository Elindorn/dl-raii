#pragma once

#include <string>
#include <filesystem>

#include <dl_raii/impl/defaultBackend.hpp>
#include <dl_raii/impl/backendConcept.hpp>

namespace dl
{
	/**
	 * @brief RAII wrapper for a dynamically loaded library.
	 *
	 * Manages the lifetime of a dynamic library (`.dll`, `.so`, `.dylib`).
	 * The library is unloaded automatically when the object is destroyed.
	 * Instances are move-only; copying is not allowed.
	 *
	 * @tparam Backend A type satisfying the `dl::backend::BackendConcept`, responsible
	 *                 for the low-level platform operations (load, unload, symbol lookup).
	 *                 The default is `dl::backend::DefaultBackend`.
	 *
	 * Example usage:
	 * @code
	 *   auto lib = dl::Library<>::load("myplugin");
	 *   auto greet = lib.getFunction<void()>("greet");
	 *   greet();
	 * @endcode
	 *
	 * @see dl::backend::DefaultBackend, dl::backend::BackendConcept
	 */
	template <backend::BackendConcept Backend = backend::DefaultBackend>
	class Library
	{
	public:
		explicit Library(const std::filesystem::path& path);
		explicit Library(const Backend::Handle& handle);

		/**
		 * @brief Load a dynamic library from a filesystem path.
		 *
		 * @param path Path to the library file.
		 * @return A Library object that owns the loaded library handle.
		 *
		 * @throws std::runtime_error if the library cannot be loaded.
		 *
		 * @note The path is passed as `const std::filesystem::path&` for modern C++ usage,
		 *       but the backend may convert it to a platform-specific string internally.
		 */
		static Library load(const std::filesystem::path& path);

		~Library() noexcept;

		Library(const Library&) = delete;
		Library& operator=(const Library&) = delete;

		Library(Library&&) noexcept;
		Library& operator=(Library&&) noexcept;

		/**
		 * @brief Retrieve a raw symbol pointer by name.
		 *
		 * @param name The mangled or unmangled name of the symbol.
		 * @return void* Pointer to the symbol, or nullptr if not found.
		 *
		 * @warning The returned pointer is not type-safe. It is recommended to use
		 *          getFunction() with a typed signature when calling functions.
		 */
		[[nodiscard]] void* getSymbol(const std::string& name) const;

		/**
		 * @brief Retrieve a type-safe function pointer from the library.
		 *
		 * The template parameter `F` must be a function type (e.g., `void()` or `int(double)`).
		 * The concept `std::is_function_v<F>` is enforced.
		 *
		 * @tparam F Function type to retrieve.
		 * @param name The name of the symbol.
		 * @return A function pointer of type `F*`.
		 *
		 * @throws std::runtime_error if the symbol is not found or is not a valid function.
		 *
		 * @note This method automatically casts the raw symbol to the requested
		 *       function pointer type. Only use it with symbols that are known to
		 *       be C-linkage functions (or be careful with mangling).
		 */
		template <typename F> requires std::is_function_v<F>
		[[nodiscard]] F* getFunction(const std::string& name) const;

	private:
		Backend::Handle _handle;
	};
}

#include <dl_raii/impl/library.ipp>
