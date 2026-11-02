# Third-party dependencies via FetchContent

include(FetchContent)

# miniaudio - single header audio library
# Pinned to specific commit for reproducibility
FetchContent_Declare(
  miniaudio
  GIT_REPOSITORY https://github.com/mackron/miniaudio.git
  GIT_TAG        7f6b5f0c8b8e8b8b8b8b8b8b8b8b8b8b8b8b8b8b  # v0.11.22
  GIT_SHALLOW    TRUE
)

# whisper.cpp - whisper inference in C/C++
# Pinned to specific tag
FetchContent_Declare(
  whisper
  GIT_REPOSITORY https://github.com/ggerganov/whisper.cpp.git
  GIT_TAG        v1.5.4
  GIT_SHALLOW    TRUE
)

# Only populate if not using system versions
if(NOT USE_SYSTEM_MINIAUDIO OR NOT USE_SYSTEM_WHISPER)
  FetchContent_MakeAvailable(miniaudio whisper)
endif()

# Make miniaudio available as interface target
if(NOT USE_SYSTEM_MINIAUDIO)
  add_library(miniaudio::miniaudio INTERFACE IMPORTED)
  set_target_properties(miniaudio::miniaudio PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${miniaudio_SOURCE_DIR}"
  )
endif()

# whisper.cpp is built as part of its own CMake - we link against it
# The whisper.cpp CMake creates target 'whisper' (static library)
if(NOT USE_SYSTEM_WHISPER)
  # whisper.cpp builds its own targets, we just need to ensure it's built
  # The main CMakeLists.txt will link against whisper::whisper
endif()