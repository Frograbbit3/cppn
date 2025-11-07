cmake_minimum_required(VERSION 3.16)

project(CPPN LANGUAGES C CXX)

option(OPTIMIZATIONS "Enables optimizations using /O2" ON)

include(FetchContent)
FetchContent_Declare(
  SDL2
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG release-2.30.6
)
FetchContent_MakeAvailable(SDL2)

add_executable(CPPN main.cpp)
target_link_libraries(CPPN PRIVATE SDL2::SDL2 SDL2::SDL2main)
target_include_directories(CPPN PRIVATE include)

if(MSVC)
    if(OPTIMIZATIONS)
      target_compile_options(CPPN PRIVATE /O2)
    endif()
else()
    if(OPTIMIZATIONS)
      target_compile_options(CPPN PRIVATE -O2)
    endif()
endif()