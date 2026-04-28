# CMake tests

include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.14.0
)
FetchContent_MakeAvailable(Catch2)


add_library(testLib SHARED testLibrary.cpp)
set_target_properties(testLib PROPERTIES
        RUNTIME_OUTPUT_DIRECTORIES "${CMAKE_CURRENT_BINARY_DIR}"
        LIBRARY_OUTPUT_DIRECTORIES "${CMAKE_CURRENT_BINARY_DIR}"
)


add_executable(unitTests main.cpp)
target_link_libraries(unitTests PRIVATE
        Catch2::Catch2WithMain
        dl_raii
)

target_compile_definitions(unitTests PRIVATE
        TEST_LIBRARY_PATH="$<TARGET_FILE:testLib>"
)


include(Catch)
catch_discover_tests(unitTests)
