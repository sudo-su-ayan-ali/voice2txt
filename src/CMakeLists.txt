# Common utilities library
add_library(voicetype_common STATIC
  common/log.h
  common/log.cpp
  common/json.h
  common/json.cpp
  common/string_utils.h
  common/string_utils.cpp
  common/filesystem.h
  common/filesystem.cpp
)

target_include_directories(voicetype_common PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/common>
  $<INSTALL_INTERFACE:include/voicetype>
)

target_link_libraries(voicetype_common PUBLIC Threads::Threads)

# Platform abstraction library
add_library(voicetype_platform STATIC
  platform/platform.h
  platform/platform.cpp
  platform/session_detect.h
  platform/session_detect.cpp
)

target_include_directories(voicetype_platform PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/platform>
)

target_link_libraries(voicetype_platform PUBLIC voicetype_common ${PLATFORM_LIBS})

# Audio capture library
add_library(voicetype_audio STATIC
  audio/capture.h
  audio/capture.cpp
  audio/ring_buffer.h
  audio/ring_buffer.cpp
)

target_include_directories(voicetype_audio PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/audio>
)

if(NOT USE_SYSTEM_MINIAUDIO)
  target_link_libraries(voicetype_audio PUBLIC miniaudio::miniaudio)
endif()
target_link_libraries(voicetype_audio PUBLIC voicetype_common ${PLATFORM_LIBS})

# Transcription library
add_library(voicetype_transcribe STATIC
  transcribe/whisper_wrapper.h
  transcribe/whisper_wrapper.cpp
  transcribe/live_transcriber.h
  transcribe/live_transcriber.cpp
)

target_include_directories(voicetype_transcribe PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/transcribe>
)

if(NOT USE_SYSTEM_WHISPER)
  target_link_libraries(voicetype_transcribe PUBLIC whisper)
else()
  target_link_libraries(voicetype_transcribe PUBLIC whisper::whisper)
endif()
target_link_libraries(voicetype_transcribe PUBLIC voicetype_common voicetype_audio)

# IPC library
add_library(voicetype_ipc STATIC
  ipc/protocol.h
  ipc/server.h
  ipc/server.cpp
  ipc/client.h
  ipc/client.cpp
  ipc/unix_socket_server.h
  ipc/unix_socket_server.cpp
  ipc/named_pipe_server.h
  ipc/named_pipe_server.cpp
)

target_include_directories(voicetype_ipc PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/ipc>
)

target_link_libraries(voicetype_ipc PUBLIC voicetype_common ${PLATFORM_LIBS})

# Injection library
add_library(voicetype_injection STATIC
  injection/injection.h
  injection/windows_injection.h
  injection/windows_injection.cpp
  injection/x11_injection.h
  injection/x11_injection.cpp
  injection/wtype_injection.h
  injection/wtype_injection.cpp
  injection/portal_injection.h
  injection/portal_injection.cpp
  injection/clipboard_paste_injection.h
  injection/clipboard_paste_injection.cpp
)

target_include_directories(voicetype_injection PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/injection>
)

target_link_libraries(voicetype_injection PUBLIC voicetype_common voicetype_platform ${PLATFORM_LIBS})

# Clipboard library
add_library(voicetype_clipboard STATIC
  clipboard/clipboard.h
  clipboard/clipboard.cpp
  clipboard/windows_clipboard.h
  clipboard/windows_clipboard.cpp
  clipboard/linux_clipboard.h
  clipboard/linux_clipboard.cpp
)

target_include_directories(voicetype_clipboard PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/clipboard>
)

target_link_libraries(voicetype_clipboard PUBLIC voicetype_common voicetype_platform ${PLATFORM_LIBS})

# Notification library
add_library(voicetype_notify STATIC
  notify/notify.h
  notify/notify.cpp
  notify/windows_toast.h
  notify/windows_toast.cpp
  notify/linux_notify.h
  notify/linux_notify.cpp
)

target_include_directories(voicetype_notify PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/notify>
)

target_link_libraries(voicetype_notify PUBLIC voicetype_common voicetype_platform ${PLATFORM_LIBS})

# Logging library
add_library(voicetype_logging STATIC
  logging/transcript_log.h
  logging/transcript_log.cpp
)

target_include_directories(voicetype_logging PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/logging>
)

target_link_libraries(voicetype_logging PUBLIC voicetype_common)

# Accuracy library
add_library(voicetype_accuracy STATIC
  accuracy/hallucination_filter.h
  accuracy/hallucination_filter.cpp
  accuracy/post_processor.h
  accuracy/post_processor.cpp
)

target_include_directories(voicetype_accuracy PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/accuracy>
)

target_link_libraries(voicetype_accuracy PUBLIC voicetype_common)

# Settings library
add_library(voicetype_settings STATIC
  settings/config.h
  settings/config.cpp
)

target_include_directories(voicetype_settings PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/settings>
)

target_link_libraries(voicetype_settings PUBLIC voicetype_common)

# Daemon executable
if(BUILD_DAEMON)
  add_executable(voicetyped
    daemon/daemon.h
    daemon/daemon.cpp
    daemon/state_machine.h
    daemon/state_machine.cpp
    daemon/main.cpp
  )

  target_include_directories(voicetyped PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/daemon
  )

  target_link_libraries(voicetyped PRIVATE
    voicetype_common
    voicetype_platform
    voicetype_audio
    voicetype_transcribe
    voicetype_ipc
    voicetype_injection
    voicetype_clipboard
    voicetype_notify
    voicetype_logging
    voicetype_accuracy
    voicetype_settings
    ${PLATFORM_LIBS}
  )

  if(HAVE_QT6)
    target_link_libraries(voicetyped PRIVATE Qt6::Core Qt6::Widgets Qt6::DBus)
    target_compile_definitions(voicetyped PRIVATE HAVE_QT6)
  endif()

  # Install daemon
  install(TARGETS voicetyped RUNTIME DESTINATION bin)
endif()

# CLI executable
if(BUILD_CLI)
  add_executable(voicetype
    cli/main.cpp
    cli/commands.h
    cli/commands.cpp
  )

  target_include_directories(voicetype PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/cli
  )

  target_link_libraries(voicetype PRIVATE
    voicetype_common
    voicetype_ipc
    voicetype_transcribe
    ${PLATFORM_LIBS}
  )

  # Install CLI
  install(TARGETS voicetype RUNTIME DESTINATION bin)
endif()