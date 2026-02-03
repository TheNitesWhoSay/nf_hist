# Compile the nf_hist example project
cmake_minimum_required (VERSION 3.20)

if ( WIN32 AND NOT "${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU" )
  cmake_policy(SET CMP0091 NEW)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

set(This nf_hist)

project(${This} C CXX)

set(CMAKE_BUILD_PARALLEL_LEVEL)
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 20)

if ( "${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU" )
  add_compile_options(
    -pedantic-errors -Wall -Wextra -Wconversion
  )
  # The following definitions were required for MinGW, they may not be appropriate on every system
  add_definitions(-DEOVERFLOW=132 -D_GLIBCXX_HAVE_EOVERFLOW=1)
elseif ( "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" )
  add_compile_options(
    -pedantic-errors -Wall -Wextra -Wconversion
  )
endif ()

if ( MSVC )
  add_compile_options(/permissive-)
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif ( MSVC )

include(FetchContent)
FetchContent_Declare(
  rarecpp
  GIT_REPOSITORY https://github.com/TheNitesWhoSay/RareCpp.git
  GIT_TAG d5b58a6 # release-2.6.0
)
FetchContent_MakeAvailable(rarecpp)
include_directories(${rarecpp_SOURCE_DIR}/include)

set(Headers
  
)
set(Sources
  examples/01_read_example.cpp
  examples/02_write_example.cpp
  examples/03_action_example.cpp
  examples/04_default_init.cpp
  examples/05_untracked_init.cpp
  examples/06_tracked_init.cpp
  examples/07_undo_redo.cpp
  examples/08_redo_elision.cpp
  examples/09_edit_sub_elements.cpp
  examples/10_using_selections.cpp
  examples/11_change_notification_paths.cpp
  examples/12_all_change_notifications.cpp
  examples/13_attach_data.cpp
  examples/14_user_defined_action_data.cpp
  examples/15_render_actions.cpp
  examples/16_create_action_labels.cpp
  examples/17_optimize_history_size.cpp
  examples/18_cap_history_size.cpp
  examples/main.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${This} ${Sources} ${Headers})
