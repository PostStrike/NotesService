cmake_minimum_required(VERSION 3.22)

set(project "notes")
project(${project})

set(CMAKE_CXX_STANDARD 17)

set(${project}_SOURCES
        main.cpp)

add_subdirectory(MainWindow)  
add_subdirectory(CreateWindow)   
add_subdirectory(CreateTools)  

set(${project}_SOURCE_LIST
        ${${project}_SOURCES})

add_executable(${project}
        ${${project}_SOURCE_LIST})

target_link_libraries(${project} MainWindow CreateWindow)

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