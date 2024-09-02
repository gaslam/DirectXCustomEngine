set_source_files_properties(pch.cpp
    PROPERTIES
    COMPILE_FLAGS "/Yupch.h"
)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin") 

set(CMAKE_MAP_IMPORTED_CONFIG_RELWITHDEBINFO Release)

if(DEFINED VCPKG_TARGET_ARCHITECTURE)
    set(DIRECTX_ARCH ${VCPKG_TARGET_ARCHITECTURE})
elseif(CMAKE_GENERATOR_PLATFORM MATCHES "^[Ww][Ii][Nn]32$")
    set(DIRECTX_ARCH x86)
elseif(CMAKE_GENERATOR_PLATFORM MATCHES "^[Xx]64$")
    set(DIRECTX_ARCH x64)
elseif(CMAKE_GENERATOR_PLATFORM MATCHES "^[Aa][Rr][Mm]$")
    set(DIRECTX_ARCH arm)
elseif(CMAKE_GENERATOR_PLATFORM MATCHES "^[Aa][Rr][Mm]64$")
    set(DIRECTX_ARCH arm64)
elseif(CMAKE_VS_PLATFORM_NAME_DEFAULT MATCHES "^[Ww][Ii][Nn]32$")
    set(DIRECTX_ARCH x86)
elseif(CMAKE_VS_PLATFORM_NAME_DEFAULT MATCHES "^[Xx]64$")
    set(DIRECTX_ARCH x64)
elseif(CMAKE_VS_PLATFORM_NAME_DEFAULT MATCHES "^[Aa][Rr][Mm]$")
    set(DIRECTX_ARCH arm)
elseif(CMAKE_VS_PLATFORM_NAME_DEFAULT MATCHES "^[Aa][Rr][Mm]64$")
    set(DIRECTX_ARCH arm64)
endif()

file(GLOB_RECURSE COMMAND_HEADERS "Command/*.h" "Command/*.cpp")
file(GLOB_RECURSE CORE_HEADERS "Core/*.h" "Core/*.cpp")
file(GLOB_RECURSE COMP_HEADERS "Components/*.h" "Components/*.cpp")
file(GLOB_RECURSE ECS_HEADERS "ECS/*.h" "ECS/*.cpp")
file(GLOB_RECURSE MAIN_HEADERS "Main/*.h" "Main/*.cpp")
file(GLOB_RECURSE MANAGER_HEADERS "Managers/*.h" "Managers/*.cpp")
file(GLOB_RECURSE PREFAB_HEADERS "Prefabs/*.h" "Prefabs/*.cpp")
file(GLOB_RECURSE SCENECLASS_HEADERS "SceneClasses/*.h" "SceneClasses/*.cpp")
file(GLOB_RECURSE STRUCTS_HEADERS "Structs/*.h" "Structs/*.cpp")
file(GLOB_RECURSE UTILS_HEADERS "Utils/*.h" "Utils/*.cpp")


if(CMAKE_BUILD_TYPE MATCHES Debug)
add_library(GameEngine STATIC
${COMMAND_HEADERS}
${CORE_HEADERS}
${COMP_HEADERS}
${ECS_HEADERS}
${MAIN_HEADERS}
${MANAGER_HEADERS}
${PREFAB_HEADERS}
${SCENECLASS_HEADERS}
${STRUCTS_HEADERS}
${UTILS_HEADERS}
pch.h
StepTimer.h
d3dx12.h
${IMGUI_SOURCES}
 "ECS/ECS.h" "Managers/TextureManager.h" "Managers/TextureManager.cpp")
else()
add_library(GameEngine STATIC
${COMMAND_HEADERS}
${CORE_HEADERS}
${COMP_HEADERS}
${ECS_HEADERS}
${MAIN_HEADERS}
${MANAGER_HEADERS}
${PREFAB_HEADERS}
${SCENECLASS_HEADERS}
${STRUCTS_HEADERS}
${UTILS_HEADERS}
d3dx12.h
StepTimer.h
pch.h
)
endif()

target_include_directories(GameEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

set_property(TARGET GameEngine PROPERTY CXX_STANDARD 20)

add_subdirectory(${CMAKE_SOURCE_DIR}/ThirdParty/DirectXTK12 ${CMAKE_BINARY_DIR}/bin/DirectXTK12)
target_link_libraries(GameEngine PUBLIC
    d3d12.lib dxgi.lib dxguid.lib uuid.lib
    kernel32.lib user32.lib
    comdlg32.lib advapi32.lib shell32.lib
    ole32.lib oleaut32.lib
    runtimeobject.lib
    DirectXTK12
)

if(MSVC)
    target_compile_options(GameEngine PRIVATE /Wall /EHsc /GR "$<$<NOT:$<CONFIG:DEBUG>>:/guard:cf>")
    target_link_options(GameEngine PRIVATE "$<$<NOT:$<CONFIG:DEBUG>>:/guard:cf>" /DYNAMICBASE /NXCOMPAT)

    if((CMAKE_SIZEOF_VOID_P EQUAL 4) AND (NOT ${DIRECTX_ARCH} MATCHES "^arm"))
        target_link_options(GameEngine PRIVATE /SAFESEH)
    endif()

    if((MSVC_VERSION GREATER_EQUAL 1928)
       AND (CMAKE_SIZEOF_VOID_P EQUAL 8)
       AND ((NOT (CMAKE_CXX_COMPILER_ID MATCHES "Clang|IntelLLVM")) OR (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 13.0)))
        target_compile_options(GameEngine PRIVATE "$<$<NOT:$<CONFIG:DEBUG>>:/guard:ehcont>")
        target_link_options(GameEngine PRIVATE "$<$<NOT:$<CONFIG:DEBUG>>:/guard:ehcont>")
    endif()
else()
    target_compile_definitions(GameEngine PRIVATE $<IF:$<CONFIG:DEBUG>,_DEBUG,NDEBUG>)
endif()

if(NOT ${DIRECTX_ARCH} MATCHES "^arm")
    if(CMAKE_SIZEOF_VOID_P EQUAL 4)
        set(ARCH_SSE2 $<$<CXX_COMPILER_ID:MSVC,Intel>:/arch:SSE2> $<$<NOT:$<CXX_COMPILER_ID:MSVC,Intel>>:-msse2>)
    else()
        set(ARCH_SSE2 $<$<NOT:$<CXX_COMPILER_ID:MSVC,Intel>>:-msse2>)
    endif()

    target_compile_options(GameEngine PRIVATE ${ARCH_SSE2})
endif()

if(MINGW)
    find_package(directxmath CONFIG REQUIRED)
    find_package(directx-headers CONFIG REQUIRED)
else()
    find_package(directxmath CONFIG QUIET)
    find_package(directx-headers CONFIG QUIET)
endif()

if(directxmath_FOUND)
    message(STATUS "Using DirectXMath package")
    target_link_libraries(GameEngine PUBLIC Microsoft::DirectXMath)
endif()

if(directx-headers_FOUND)
    message(STATUS "Using DirectX-Headers package")
    target_link_libraries(GameEngine PUBLIC Microsoft::DirectX-Headers Microsoft::DirectX-Guids)
    target_compile_definitions(GameEngine PRIVATE USING_DIRECTX_HEADERS)
endif()

# Uncomment if using the DirectX Tool Kit
# find_package(directxtk12 CONFIG REQUIRED)
# target_link_libraries(GameEngine PRIVATE Microsoft::DirectXTK12)

# Uncomment if using the DXC compiler
# find_package(directx-dxc CONFIG REQUIRED)
# DIRECTX_DXC_TOOL is the compiler itself
# target_link_libraries(GameEngine PRIVATE Microsoft::DirectXShaderCompiler)

if(MINGW)
    target_link_options(GameEngine PRIVATE -municode)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|IntelLLVM")
    target_compile_options(GameEngine PRIVATE
        -Wpedantic -Wextra
        "-Wno-c++98-compat" "-Wno-c++98-compat-pedantic"
        "-Wno-exit-time-destructors" "-Wno-global-constructors" "-Wno-language-extension-token"
        "-Wno-missing-prototypes" "-Wno-missing-variable-declarations" "-Wno-reserved-id-macro"
        "-Wno-float-equal" "-Wno-switch-enum" "-Wno-tautological-type-limit-compare")

    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 16.0)
        target_compile_options(GameEngine PRIVATE /ZH:SHA_256 "-Wno-unsafe-buffer-usage")
    endif()

    if(BUILD_TEST_TEMPLATE)
        target_compile_options(GameEngine PRIVATE "-Wno-unused-value")
    endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(GameEngine PRIVATE -Wno-ignored-attributes)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Intel")
    target_compile_options(GameEngine PRIVATE /Zc:__cplusplus /Zc:inline /fp:fast /Qdiag-disable:161)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(GameEngine PRIVATE
         /sdl /permissive- /Zc:__cplusplus /Zc:inline /fp:fast
         "/wd4061" "/wd4365" "/wd4514" "/wd4571" "/wd4668" "/wd4710" "/wd4820" "/wd5039" "/wd5045"
         "/wd4265" "/wd4625" "/wd4626" "/wd4986" "/wd5204" "/wd5220")

    if(ENABLE_CODE_ANALYSIS)
        target_compile_options(GameEngine PRIVATE /analyze)
    endif()

    if(CMAKE_INTERPROCEDURAL_OPTIMIZATION)
        target_compile_options(GameEngine PRIVATE /Gy /Gw)
    endif()

    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.24)
        target_compile_options(GameEngine PRIVATE /ZH:SHA_256)
    endif()

    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.26)
        target_compile_options(GameEngine PRIVATE /Zc:preprocessor /wd5105)
    endif()

    if((CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.27) AND (NOT (${DIRECTX_ARCH} MATCHES "^arm")))
        target_link_options(GameEngine PRIVATE /CETCOMPAT)
    endif()

    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.28)
        target_compile_options(GameEngine PRIVATE /Zc:lambda)
    endif()

    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.34)
        target_compile_options(GameEngine PRIVATE /wd5262 /wd5264)
    endif()

    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.35)
        target_compile_options(GameEngine PRIVATE /Zc:checkGwOdr $<$<VERSION_GREATER_EQUAL:${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION},10.0.22000>:/Zc:templateScope>)
    endif()

    if(BUILD_TEST_TEMPLATE)
        target_compile_options(GameEngine PRIVATE "/wd4555")
    endif()
endif()

if(WIN32)
    target_compile_definitions(GameEngine PRIVATE _UNICODE UNICODE _WIN32_WINNT=0x0A00)
endif()