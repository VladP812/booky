cmake_minimum_required(VERSION 3.20)

project(Booky 
        LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOMOC ON)
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/cpp")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

add_subdirectory("extern/pybind11")

find_library(MUPDF_LIB mupdf)
if(NOT MUPDF_LIB)
    message(FATAL_ERROR "Could not find mupdf library files")
endif()
link_libraries(mupdf)

find_library(MUPDFCPP_LIB mupdfcpp)
if(NOT MUPDF_LIB)
    message(FATAL_ERROR "Could not find mupdfcpp library files")
endif()
link_libraries(mupdfcpp)

# Find and link against Python
find_package(Python3 COMPONENTS Development REQUIRED)
include_directories(${_Python3_INCLUDE_DIR})
link_libraries(${PYTHON_LIBRARY})

message(STATUS "Python library found at: ${PYTHON_LIBRARY}")
message(STATUS "Python include dir found at:${_Python3_INCLUDE_DIR}")

# Find and link against OpenSSL
find_package(OpenSSL COMPONENTS Crypto REQUIRED)
link_libraries(OpenSSL::Crypto)

find_package(Qt6 COMPONENTS Widgets REQUIRED)
link_libraries(Qt6::Widgets)

file(GLOB_RECURSE SOURCES ${SOURCE_DIR}/*.cpp)

add_executable(booky ${SOURCES})
