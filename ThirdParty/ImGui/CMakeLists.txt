set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/)

file(GLOB_RECURSE BACKEND_HEADERS "backends/*.h" "backends/*.cpp")

add_library(IMGUI
                STATIC
                    ${IMGUI_DIR}/imgui_demo.cpp
                    ${IMGUI_DIR}/imgui_draw.cpp
                    ${IMGUI_DIR}/imgui_tables.cpp
                    ${IMGUI_DIR}/imgui_widgets.cpp
                    ${IMGUI_DIR}/imgui.cpp
                    ${BACKEND_HEADERS}
)
set_property(TARGET IMGUI PROPERTY CXX_STANDARD 17)
target_include_directories( IMGUI
                            PUBLIC ${IMGUI_DIR}
                            PUBLIC ${IMGUI_DIR}/backends
)