set_source_files_properties(pch.cpp
    PROPERTIES
    COMPILE_FLAGS "/Yupch.h"
)

option(BUILD_TEST_TEMPLATE "Ignore warnings related to TODOs" OFF)

option(ENABLE_CODE_ANALYSIS "Use Static Code Analysis on build" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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

if(CMAKE_BUILD_TYPE MATCHES Debug)
add_executable(GameExe WIN32
Main.cpp
Utils/SceneUtils.h
Scene/TestScene.h
${IMGUI_SOURCES} "Commands/ChangeMouseCommand.h"  "Scene/TestScene.cpp")
else()
add_executable(GameExe WIN32
Main.cpp
Utils/SceneUtils.h
Scene/TestScene.cpp
 "Commands/QuitCommand.h" )
endif()

target_include_directories(GameExe PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Scene)

if(MSVC)
    target_compile_options(GameExe PRIVATE /Wall /EHsc /GR "$<$<NOT:$<CONFIG:DEBUG>>:/guard:cf>")
    target_link_options(GameExe PRIVATE "$<$<NOT:$<CONFIG:DEBUG>>:/guard:cf>" /DYNAMICBASE /NXCOMPAT)

    if((CMAKE_SIZEOF_VOID_P EQUAL 4) AND (NOT ${DIRECTX_ARCH} MATCHES "^arm"))
        target_link_options(GameExe PRIVATE /SAFESEH)
    endif()

    if((MSVC_VERSION GREATER_EQUAL 1928)
       AND (CMAKE_SIZEOF_VOID_P EQUAL 8)
       AND ((NOT (CMAKE_CXX_COMPILER_ID MATCHES "Clang|IntelLLVM")) OR (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 13.0)))
        target_compile_options(GameExe PRIVATE "$<$<NOT:$<CONFIG:DEBUG>>:/guard:ehcont>")
        target_link_options(GameExe PRIVATE "$<$<NOT:$<CONFIG:DEBUG>>:/guard:ehcont>")
    endif()
else()
    target_compile_definitions(GameExe PRIVATE $<IF:$<CONFIG:DEBUG>,_DEBUG,NDEBUG>)
endif()

if(NOT ${DIRECTX_ARCH} MATCHES "^arm")
    if(CMAKE_SIZEOF_VOID_P EQUAL 4)
        set(ARCH_SSE2 $<$<CXX_COMPILER_ID:MSVC,Intel>:/arch:SSE2> $<$<NOT:$<CXX_COMPILER_ID:MSVC,Intel>>:-msse2>)
    else()
        set(ARCH_SSE2 $<$<NOT:$<CXX_COMPILER_ID:MSVC,Intel>>:-msse2>)
    endif()

    target_compile_options(GameExe PRIVATE ${ARCH_SSE2})
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
    target_link_libraries(GameExe PUBLIC Microsoft::DirectXMath)
endif()

if(directx-headers_FOUND)
    message(STATUS "Using DirectX-Headers package")
    target_link_libraries(GameExe PUBLIC Microsoft::DirectX-Headers Microsoft::DirectX-Guids)
    target_compile_definitions(GameExe PRIVATE USING_DIRECTX_HEADERS)
endif()

# Uncomment if using the DirectX Tool Kit
# find_package(directxtk12 CONFIG REQUIRED)
# target_link_libraries(GameExe PRIVATE Microsoft::DirectXTK12)

# Uncomment if using the DXC compiler
# find_package(directx-dxc CONFIG REQUIRED)
# DIRECTX_DXC_TOOL is the compiler itself
# target_link_libraries(GameExe PRIVATE Microsoft::DirectXShaderCompiler)

if(MINGW)
    target_link_options(GameExe PRIVATE -municode)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|IntelLLVM")
    target_compile_options(GameExe PRIVATE
        -Wpedantic -Wextra
        "-Wno-c++98-compat" "-Wno-c++98-compat-pedantic"
        "-Wno-exit-time-destructors" "-Wno-global-constructors" "-Wno-language-extension-token"
        "-Wno-missing-prototypes" "-Wno-missing-variable-declarations" "-Wno-reserved-id-macro"
        "-Wno-float-equal" "-Wno-switch-enum" "-Wno-tautological-type-limit-compare")

    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 16.0)
        target_compile_options(GameExe PRIVATE /ZH:SHA_256 "-Wno-unsafe-buffer-usage")
    endif()

    if(BUILD_TEST_TEMPLATE)
        target_compile_options(GameExe PRIVATE "-Wno-unused-value")
    endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(GameExe PRIVATE -Wno-ignored-attributes)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Intel")
    target_compile_options(GameExe PRIVATE /Zc:__cplusplus /Zc:inline /fp:fast /Qdiag-disable:161)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(GameExe PRIVATE
         /sdl /permissive- /Zc:__cplusplus /Zc:inline /fp:fast
         "/wd4061" "/wd4365" "/wd4514" "/wd4571" "/wd4668" "/wd4710" "/wd4820" "/wd5039" "/wd5045"
         "/wd4265" "/wd4625" "/wd4626" "/wd4986" "/wd5204" "/wd5220")

    if(ENABLE_CODE_ANALYSIS)
        target_compile_options(GameExe PRIVATE /analyze)
    endif()

    if(CMAKE_INTERPROCEDURAL_OPTIMIZATION)
        target_compile_options(GameExe PRIVATE /Gy /Gw)
    endif()

    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.24)
        target_compile_options(GameExe PRIVATE /ZH:SHA_256)
    endif()

    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.26)
        target_compile_options(GameExe PRIVATE /Zc:preprocessor /wd5105)
    endif()

    if((CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.27) AND (NOT (${DIRECTX_ARCH} MATCHES "^arm")))
        target_link_options(GameExe PRIVATE /CETCOMPAT)
    endif()

    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.28)
        target_compile_options(GameExe PRIVATE /Zc:lambda)
    endif()

    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.34)
        target_compile_options(GameExe PRIVATE /wd5262 /wd5264)
    endif()

    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.35)
        target_compile_options(GameExe PRIVATE /Zc:checkGwOdr $<$<VERSION_GREATER_EQUAL:${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION},10.0.22000>:/Zc:templateScope>)
    endif()

    if(BUILD_TEST_TEMPLATE)
        target_compile_options(GameExe PRIVATE "/wd4555")
    endif()
endif()

if(WIN32)
    target_compile_definitions(GameExe PRIVATE _UNICODE UNICODE _WIN32_WINNT=0x0A00)
endif()

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT GameExe)

add_custom_command(TARGET GameExe POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
                       ${CMAKE_CURRENT_SOURCE_DIR}/Resources/ $<TARGET_FILE_DIR:GameExe>/Resources/)

