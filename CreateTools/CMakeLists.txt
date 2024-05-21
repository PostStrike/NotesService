cmake_minimum_required(VERSION 3.22)

set(project "create_tools")
project(${project})

set(CMAKE_CXX_STANDARD 17)

set(${project}_HEADERS
        Grid.h
	Objects.h)

set(${project}_SOURCES
        Grid.cpp)

set(${project}_SOURCE_LIST
        ${${project}_SOURCES}
        ${${project}_HEADERS})

add_library(${project}
        INTERFACE)

target_sources(${project}
        INTERFACE
        ${${project}_SOURCE_LIST})

target_include_directories(${project}
        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR})
        
# GTK
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

include_directories(${GTK3_INCLUDE_DIRS})

add_definitions(${GTK3_CFLAGS_OTHER})

target_link_libraries(${project}
        INTERFACE
        ${GTK3_LIBRARIES})

# libsigc++-2.0-dev
find_package(PkgConfig REQUIRED)
pkg_check_modules(SIGC REQUIRED sigc++-2.0)

include_directories(${SIGC_INCLUDE_DIRS})

add_definitions(${SIGC_CFLAGS_OTHER})

target_link_libraries(${project}
        INTERFACE
        ${SIGC_LIBRARIES})