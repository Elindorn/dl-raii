cmake_minimum_required(VERSION 4.2)

project(dl_raii LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

add_library(dl_raii INTERFACE
        include/dl_raii/library.hpp

)

add_library(dl_raii::dl_raii ALIAS dl_raii)

target_include_directories(dl_raii INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
