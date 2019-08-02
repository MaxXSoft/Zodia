cmake_minimum_required(VERSION 3.5)
project(Zodia VERSION "0.0.1")

# set CMake module path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# # set compiler path
# set(CMAKE_C_COMPILER "/usr/local/opt/gcc/bin/gcc-8")
# set(CMAKE_CXX_COMPILER "/usr/local/opt/gcc/bin/g++-8")

# C++17 standard support
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# enable all warnings and treat them as errors
if(MSVC)
  add_compile_options(/W3 /WX)
else()
  add_compile_options(-Wall -Werror)
endif()

# some definitions
add_compile_definitions(APP_NAME="${PROJECT_NAME}")
add_compile_definitions(APP_VERSION="${PROJECT_VERSION}")
add_compile_definitions(APP_VERSION_MAJOR=${PROJECT_VERSION_MAJOR})
add_compile_definitions(APP_VERSION_MINOR=${PROJECT_VERSION_MINOR})
add_compile_definitions(APP_VERSION_PATCH=${PROJECT_VERSION_PATCH})

# SDL2 & SDL2 image
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

# Ionia
include(ExternalProject)
ExternalProject_Add(Ionia
    PREFIX ionia
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/ionia"
    CMAKE_ARGS -DCMAKE_BUILD_TYPE=Release
    BINARY_DIR ionia/bin
    INSTALL_COMMAND "")
include_directories(3rdparty/ionia/src)
link_directories(${PROJECT_BINARY_DIR}/ionia/bin)

# project include directories
include_directories(src)
include_directories(3rdparty)

# all of C++ source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# executable
add_executable(zodia ${SOURCES})
add_dependencies(zodia Ionia)
target_link_libraries(zodia ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ionia)
