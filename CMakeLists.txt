cmake_minimum_required(VERSION 3.25)

# The DLL uses the static release CRT in both supported configurations.
set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreaded)
if(CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_CONFIGURATION_TYPES "Release;RelWithDebInfo" CACHE STRING "Build configurations" FORCE)
elseif(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build configuration" FORCE)
endif()
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Release RelWithDebInfo)

project(OrionLoader VERSION 0.1.0 LANGUAGES CXX)

if(CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_CONFIGURATION_TYPES "Release;RelWithDebInfo" CACHE STRING "Build configurations" FORCE)
endif()

if(NOT WIN32 OR NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "Orion.Loader requires a Windows x64 target. On Linux use the clang-cl-x64 toolchain.")
endif()
if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR NOT CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
    message(FATAL_ERROR "Build with clang-cl (MSVC-compatible Clang).")
endif()
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE MATCHES "^(Release|RelWithDebInfo)$")
    message(FATAL_ERROR "Supported configurations: Release and RelWithDebInfo.")
endif()

include("${CMAKE_CURRENT_SOURCE_DIR}/toolchains/runtimeimporter/toolchain.cmake")

set(ORION_NATIVE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/external/Orion.Native" CACHE PATH "Orion.Native source directory")
if(NOT EXISTS "${ORION_NATIVE_ROOT}/CMakeLists.txt")
    message(FATAL_ERROR "Orion.Native submodule not found. Run git submodule update --init --recursive, or set ORION_NATIVE_ROOT to its source directory.")
endif()
if(NOT TARGET orion::NativeHeaders)
    add_subdirectory("${ORION_NATIVE_ROOT}" "${CMAKE_CURRENT_BINARY_DIR}/orion-native" EXCLUDE_FROM_ALL)
endif()

add_library(OrionLoader SHARED src/Main.cpp src/Exports.def)
add_library(orion::Loader ALIAS OrionLoader)
target_compile_features(OrionLoader PUBLIC cxx_std_20)
target_link_libraries(OrionLoader PRIVATE orion::NativeHeaders)
target_include_directories(OrionLoader PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_compile_definitions(OrionLoader PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)
target_compile_options(OrionLoader PRIVATE /W4 /permissive- /utf-8)
target_link_options(OrionLoader PRIVATE /INCREMENTAL:NO /OPT:REF /OPT:ICF)
set_target_properties(OrionLoader PROPERTIES
    OUTPUT_NAME "Orion.Loader"
    PREFIX ""
    CXX_EXTENSIONS OFF
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    PDB_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    COMPILE_PDB_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/obj")
