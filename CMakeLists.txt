cmake_minimum_required(VERSION 3.20)
project(VoiceType VERSION 0.1.0 LANGUAGES CXX)

# C++20 required
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Default build type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Platform-specific settings
if(WIN32)
  # Prefer static runtime to avoid DLL hell
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  add_compile_definitions(_CRT_SECURE_NO_WARNINGS NOMINMAX WIN32_LEAN_AND_MEAN)
else()
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
  add_compile_options(-Wall -Wextra -Wpedantic -Werror=return-type)
endif()

# Build options
option(BUILD_DAEMON "Build voicetyped daemon" ON)
option(BUILD_CLI "Build voicetype CLI" ON)
option(BUILD_TESTS "Build tests" ON)
option(USE_SYSTEM_WHISPER "Use system-installed whisper.cpp" OFF)
option(USE_SYSTEM_MINIAUDIO "Use system-installed miniaudio" OFF)
option(USE_SYSTEM_QT "Use system Qt6" ON)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Include custom CMake modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Find dependencies
find_package(Threads REQUIRED)

# Audio: miniaudio (header-only)
if(USE_SYSTEM_MINIAUDIO)
  find_path(MINIAUDIO_INCLUDE_DIR miniaudio.h)
  if(NOT MINIAUDIO_INCLUDE_DIR)
    message(FATAL_ERROR "miniaudio.h not found. Install miniaudio or set USE_SYSTEM_MINIAUDIO=OFF")
  endif()
else()
  # Will use third_party/miniaudio via FetchContent
endif()

# Transcription: whisper.cpp
if(USE_SYSTEM_WHISPER)
  find_package(whisper REQUIRED)
else()
  # Will use third_party/whisper.cpp via FetchContent
endif()

# Qt6 for tray app (Phase 6+)
if(BUILD_DAEMON)  # Tray is part of daemon build for now
  find_package(Qt6 QUIET COMPONENTS Core Widgets DBus)
  if(Qt6_FOUND)
    set(HAVE_QT6 TRUE)
  else()
    message(WARNING "Qt6 not found - tray app will be disabled. Install Qt6 or set BUILD_DAEMON=OFF for headless build.")
    set(HAVE_QT6 FALSE)
  endif()
endif()

# Platform-specific libraries
if(WIN32)
  find_library(OLE32_LIB ole32)
  find_library(USER32_LIB user32)
  find_library(SHELL32_LIB shell32)
  find_library(OLEAUT32_LIB oleaut32)
  find_library(UUID_LIB uuid)
  set(PLATFORM_LIBS ${OLE32_LIB} ${USER32_LIB} ${SHELL32_LIB} ${OLEAUT32_LIB} ${UUID_LIB})
elseif(UNIX)
  find_library(RT_LIB rt)
  find_library(DL_LIB dl)
  find_library(PULSE_LIB pulse-simple pulse)
  find_library(ASOUND_LIB asound)
  find_library(X11_LIB X11)
  find_library(XTEST_LIB Xtst)
  find_library(WAYLAND_CLIENT_LIB wayland-client)
  set(PLATFORM_LIBS ${RT_LIB} ${DL_LIB} ${PULSE_LIB} ${ASOUND_LIB} ${X11_LIB} ${XTEST_LIB} ${WAYLAND_CLIENT_LIB})
endif()

# Subdirectories
add_subdirectory(third_party)
add_subdirectory(src)
if(BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

# Install targets (for packaging)
install(DIRECTORY share/ DESTINATION ${CMAKE_INSTALL_DATADIR})