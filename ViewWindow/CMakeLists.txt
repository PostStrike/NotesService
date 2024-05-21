cmake_minimum_required(VERSION 3.22)

set(project "ViewWindow")
project(${project})

set(CMAKE_CXX_STANDARD 17)

set(${project}_SOURCES
        ViewWindow.cpp)

set(${project}_HEADERS
        ViewWindow.h)

set(${project}_SOURCE_LIST
        ${${project}_SOURCES}
        ${${project}_HEADERS})

add_library(${project}
        STATIC
        ${${project}_SOURCE_LIST})

# Линкуем с библиотекой create_tools
target_link_libraries(${project} create_tools)

# GTK
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

include_directories(${GTK3_INCLUDE_DIRS})

add_definitions(${GTK3_CFLAGS_OTHER})

target_link_libraries(${project} ${GTK3_LIBRARIES})

# libsigc++-2.0-dev
find_package(PkgConfig REQUIRED)
pkg_check_modules(SIGC REQUIRED sigc++-2.0)

include_directories(${SIGC_INCLUDE_DIRS})

add_definitions(${SIGC_CFLAGS_OTHER})

target_link_libraries(${project} ${SIGC_LIBRARIES})