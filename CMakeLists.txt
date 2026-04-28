cmake_minimum_required(VERSION 4.2)

project(dl_raii LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

add_library(dl_raii INTERFACE
        include/dl_raii/library.hpp
        include/dl_raii/impl/defaultBackend.hpp
        include/dl_raii/impl/library.ipp
)

add_library(dl_raii::dl_raii ALIAS dl_raii)

target_include_directories(dl_raii INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

option(DL_RAII_BUILD_TESTS "Build tests for dl_raii library" ON)

if(DL_RAII_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
