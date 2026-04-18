# 指定CMake最低版本要求
cmake_minimum_required(VERSION 3.20)

project(abin_string_view LANGUAGES C CXX VERSION 0.1.0)

include(GNUInstallDirs)

# ---------------------------------------------------------------------------------------
# Set default build to Release for single-config generators (Makefile/Ninja)
# ---------------------------------------------------------------------------------------
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  if((DEFINED PROJECT_IS_TOP_LEVEL AND PROJECT_IS_TOP_LEVEL)
    OR (NOT DEFINED PROJECT_IS_TOP_LEVEL AND CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR))
      set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose Release or Debug" FORCE)
  endif()
endif()

add_library(abin_string_view INTERFACE)
add_library(abin::string_view ALIAS abin_string_view)
target_include_directories(abin_string_view
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_compile_features(abin_string_view INTERFACE cxx_std_11)

# Compiler-specific warning/options
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options(abin_string_view INTERFACE
  "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
  "$<${msvc_cxx}:$<BUILD_INTERFACE:/W3;/utf-8>>"
)

# ----------------------------------------------------------
# Detect master (top-level) project
# ----------------------------------------------------------
if(NOT DEFINED ABIN_STRING_VIEW_MASTER_PROJECT)
  if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(ABIN_STRING_VIEW_MASTER_PROJECT ON)
  else()
    set(ABIN_STRING_VIEW_MASTER_PROJECT OFF)
  endif()
endif()

if(ABIN_STRING_VIEW_MASTER_PROJECT)
  message(STATUS "[abin_string_view] Master project: ${ABIN_STRING_VIEW_MASTER_PROJECT}")
  # Generate compile_commands.json for clangd/clang-tidy
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
  # Enable PIC for all targets (safest for static libs)
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

# ----------------------------------------
# Options for examples/tests/install
# ----------------------------------------
option(ABIN_STRING_VIEW_BUILD_EXAMPLES  "Build examples" ${ABIN_STRING_VIEW_MASTER_PROJECT})
option(ABIN_STRING_VIEW_BUILD_TESTS "Build tests" ${ABIN_STRING_VIEW_MASTER_PROJECT})

if(ABIN_STRING_VIEW_BUILD_EXAMPLES)
  message(STATUS "[abin_string_view] Building examples...")
  add_subdirectory(example)
endif()

if(ABIN_STRING_VIEW_BUILD_TESTS)
  message(STATUS "[abin_string_view] Building tests...")
  include(CTest)  # enable add_test
  if(NOT TARGET Catch2::Catch2)
    add_subdirectory(3rd/Catch2)
  endif()
  add_subdirectory(test)
endif()

