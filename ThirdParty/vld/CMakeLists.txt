find_library(VLD vld HINTS "${CMAKE_CURRENT_SOURCE_DIR}/lib/Win64")
find_path(VLD_H vld.h HINTS "${CMAKE_CURRENT_SOURCE_DIR}/include")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

if(VLD)
message("lib found")
endif()

if(VLD_H)
message("header found")
endif()