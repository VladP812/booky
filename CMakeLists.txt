cmake_minimum_required(VERSION 3.20)

project(Booky)

set(CMAKE_AUTOMOC ON)
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

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

find_package(Qt6 COMPONENTS Widgets REQUIRED)
link_libraries(Qt6::Widgets)

file(GLOB_RECURSE SOURCES ${SOURCE_DIR}/*.cpp)

add_executable(booky ${SOURCES})
