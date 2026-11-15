cmake_minimum_required(VERSION 3.24)
project(waddles LANGUAGES C CXX)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.c src/*.cpp)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)  # for Zed autocomplete

include(FetchContent)

# Configure SDL3 options (Static Only)
set(SDL_STATIC  ON  CACHE BOOL "" FORCE)
set(SDL_SHARED  OFF CACHE BOOL "" FORCE)
set(SDL_TESTS   OFF CACHE BOOL "" FORCE)
set(SDL_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG        release-3.2.0
    GIT_SHALLOW    TRUE
    FIND_PACKAGE_ARGS NAMES SDL3
)
FetchContent_MakeAvailable(SDL3)

# Configure SDL3_image to build statically as well
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_INSTALL  OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_TESTS    OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    SDL3_image
    GIT_REPOSITORY         https://github.com/libsdl-org/SDL_image.git
    GIT_TAG                release-3.2.0
    GIT_SHALLOW            TRUE
    GIT_SUBMODULES_RECURSE FALSE  # Prevents path-length errors on Windows
    FIND_PACKAGE_ARGS NAMES SDL3_image
)
FetchContent_MakeAvailable(SDL3_image)

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE src)

# Link both libraries statically
target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image
)
