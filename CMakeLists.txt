cmake_minimum_required(VERSION 3.28)

if (MSVC)
    # For precompiled header.
    # Set 
    # "Precompiled Header" to "Use (/Yu)"
    # "Precompiled Header File" to "pch.h"
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Yupch.h /FIpch.h")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
  # Force to always compile with W4
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # Update if necessary
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
endif()

project("GameEngineCMAKE")

add_subdirectory("GameEngine")
add_subdirectory("Game")
add_subdirectory("Benchmarks")

add_subdirectory(${CMAKE_SOURCE_DIR}/ThirdParty/ImGui ${CMAKE_BINARY_DIR}/bin/IMGUI)
target_link_libraries(GameExe PRIVATE IMGUI)
target_link_libraries(GameEngine PUBLIC IMGUI)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
add_subdirectory(${CMAKE_SOURCE_DIR}/ThirdParty/vld)
add_custom_target(prebuild ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different ${CMAKE_SOURCE_DIR}/ThirdParty/vld/bin/Win64/ $<TARGET_FILE_DIR:GameExe>/)
target_include_directories(GameExe PRIVATE "${VLD_H}")
add_dependencies(GameExe prebuild)
target_link_libraries(GameExe PRIVATE "${VLD}")
target_include_directories(GameEngine PRIVATE "${VLD_H}")
target_link_libraries(GameEngine PRIVATE "${VLD}")
endif()

target_link_libraries(GameExe PRIVATE GameEngine)

