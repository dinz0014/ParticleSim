cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# Set the CMake tool chain for vcpkg integration
if(NOT "$ENV{VCPKG_ROOT}" STREQUAL "")
    set(
        CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file"
    )

    message(STATUS "CMAKE_TOOLCHAIN_FILE set appropriately")
else()
    message(FATAL_ERROR "No VCPKG_ROOT variable set. Please set this environment variable to the root of your vcpkg install")
endif()

# Enable manifest mode for vcpkg
set(VCPKG_MANIFEST_MODE on)

project(
    ParticleSim
    VERSION 1.0
    LANGUAGES CXX
)

message(STATUS "C++ Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
if(MSVC)
    # Adjust debug flags for MSVC
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Zi")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /DEBUG")
else()
    # Adjust debug flags for gcc/clang
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")
endif()

find_package(SFML 2.6.1 COMPONENTS system window graphics CONFIG REQUIRED)

add_subdirectory(src/physics)
add_subdirectory(src/app)
add_subdirectory(src/render)