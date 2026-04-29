# DL-RAII - Dynamic Library RAII Wrapper

[![C++20](https://img.shields.io/badge/C%2B%2B-20-blue)]()
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey)]()

A cross-platform C++20 header-only RAII wrapper for dynamic library loading.


## Overview

`dl-raii` provides a safe and convenient C++ interface for loading dynamic libraries (`.dll`, `.so`, `.dylib`) and retrieving symbols from them. By leveraging **RAII**, it guarantees that loaded libraries are automatically unloaded when the wrapper object goes out of scope, preventing resource leaks and making code both cleaner and more exception-safe.

The library is designed to be minimal and unobtrusive:
- **Cross-platform**: Works on Windows, Linux, and macOS.
- **C++20**: Uses concepts to constrain function pointer retrieval.
- **Header-only**: Easy to integrate into any CMake project.
- **Extensible back-end**: The default back-end can be replaced to support custom loaders or platforms.


## Features

- `dl::Library<Backend>` — a move-only RAII handle that loads and unloads a dynamic library.
- `getSymbol()` — retrieves a raw symbol pointer.
- `getFunction<F>()` — retrieves a type-safe function pointer; constrained with `std::is_function_v`.
- `dl::util::demangle()` — a cross-platform utility to demangle C++ symbol names.
- Header-only library with a single top-level include: `#include <dl_raii/library.hpp>`.


## Quick Example

```cpp
#include <dl_raii/library.hpp>
#include <iostream>

int main() {
    try {
        // Load the library
        dl::Library lib = dl::Library::load("mylib");

        // Retrieve a type-safe function pointer
        auto greet = lib.getFunction<void()>("greet");
        
        // Call the function
        greet();

        // Library is automatically unloaded when `lib` goes out of scope.
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
    return 0;
}
```


## Platform Support

The library automatically selects the appropriate backend based on the platform.
A built-in backend supports Windows and POSIX APIs (`dl::backend::DefaultBackend`).
You can also provide your own backend by implementing the required concept
(see `impl/backendConcept.hpp`).


## Requirements

- **C++20** (concepts, `std::filesystem`, `constexpr` variables)
- **CMake >= 3.20** (for FetchContent, or just copy headers)


## Integration

`dl-raii` is a header-only library. You can integrate it into your CMake project in several ways:

### Option 1: FetchContent (Recommended)

```cmake
include(FetchContent)
FetchContent_Declare(
  dl_raii
  GIT_REPOSITORY https://github.com/Elindorn/dl-raii.git
  GIT_TAG main
)
FetchContent_MakeAvailable(dl_raii)

target_link_libraries(my_app PRIVATE dl_raii::dl_raii)
```

### Option 2: Copy headers

Simply copy the include/dl_raii/ directory into your project and add:

```cmake
target_include_directories(my_app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
```

## Structure

```
.
├── include/dl_raii/
│   ├── library.hpp          # Main RAII class template
│   ├── util.hpp             # Demangle utility
│   └── impl/
│       ├── backendConcept.hpp   # Concept constraints for backends
│       ├── defaultBackend.hpp   # Platform-specific loading implementations
│       ├── library.ipp          # Template implementations
│       └── util.ipp             # Demangle implementations
├── tests/
│   ├── main.cpp
│   ├── testLibrary.cpp
│   └── CMakeLists.txt       # Catch2-based unit tests
├── CMakeLists.txt
├── LICENSE.md               # MIT
└── README.md
```


## Building and Running Tests

Tests use Catch2 and are automatically fetched during CMake configuration.
```bash
git clone https://github.com/Elindorn/dl-raii.git
cd dl-raii
cmake -B build -DDL_RAII_BUILD_TESTS=ON
cmake --build build
cd build && ctest
```


## Why Another Library?

While excellent projects like [`martin-olivier/dylib`](https://github.com/martin-olivier/dylib)
already exist, `dl-raii` was built with the following priorities:
- **Modern C++20**: Uses concepts and `std::filesystem` instead of raw strings.
- **Educational**: The codebase is intentionally small and readable, making it easy to study or extend.
- **Header-only**: Much easier to use or debug in projects.


## License

This project is licensed under the MIT License — see the LICENSE file for details.


## Contributing

Pull requests are welcome! This is a personal portfolio project, so there are no
strict contribution guidelines — just open an issue or PR, and we'll figure it out together.
