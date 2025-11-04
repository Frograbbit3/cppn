cmake_minimum_required(VERSION 3.16)
project(CPPN LANGUAGES CXX)

# ------------------------------------------
# 🔧 Choose build target
# ------------------------------------------
set(target "" CACHE STRING "Build target: native, web, or windows")

if (target STREQUAL "web")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        SUFFIX ".html"
        LINK_FLAGS "--bind -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS=['png'] -s USE_ZLIB=1 -s WASM=1 -s ALLOW_MEMORY_GROWTH=1"
    )
endif()

if (target STREQUAL "web")
    message(STATUS "Target: WebAssembly (Emscripten)")
    if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
        set(CMAKE_TOOLCHAIN_FILE "$ENV{EMSDK}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake" CACHE FILEPATH "" FORCE)
    endif()
elseif (target STREQUAL "windows")
    message(STATUS "Target: Windows (MinGW)")
    if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
        set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/toolchains/mingw-w64-x86_64.cmake" CACHE FILEPATH "" FORCE)
    endif()
else()
    message(STATUS "Target: Native (default)")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ------------------------------------------
# 🧱 Source files
# ------------------------------------------
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})

include_directories(${PROJECT_SOURCE_DIR}/include)

# ------------------------------------------
# 🧩 Platform-specific configuration
# ------------------------------------------
if (target STREQUAL "native")
    # ---- Linux / macOS ----
    find_package(SDL2 REQUIRED)
    find_package(ZLIB REQUIRED)
    find_path(SDL2_IMAGE_INCLUDE_DIR SDL_image.h PATH_SUFFIXES SDL2)
    find_library(SDL2_IMAGE_LIBRARY SDL2_image)

    include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARY} ${ZLIB_LIBRARIES})

elseif (target STREQUAL "windows")
    # ---- Windows (MinGW cross-compile) ----
    message(STATUS "Using manual SDL2 setup for MinGW")
    set(SDL2_SDK_ROOT "${CMAKE_SOURCE_DIR}/SDKs/mingw-w64-x86_64-SDL2-2.32.10-1-any/mingw64")

    # Some SDL2 MinGW packages use just "include", not "include/SDL2"
    include_directories(
        "${SDL2_SDK_ROOT}/include"
        "${SDL2_SDK_ROOT}/include/SDL2"
        "${PROJECT_SOURCE_DIR}/include"
    )

    link_directories("${SDL2_SDK_ROOT}/lib")


    target_link_libraries(${PROJECT_NAME}
        mingw32
        SDL2main
        SDL2
        -mwindows
    )

elseif (target STREQUAL "web")
    # ---- WebAssembly (Emscripten) ----
    message(STATUS "Using built-in Emscripten SDL2")
    # Emscripten ships SDL2, so you don’t link system libs.
    # You can add compile flags if you want canvas support:
    set_target_properties(${PROJECT_NAME} PROPERTIES
        LINK_FLAGS "--bind -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS=['png','jpg'] -s USE_ZLIB=1"
    )
endif()
