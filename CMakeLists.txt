cmake_minimum_required(VERSION 2.8.10)

if(${CMAKE_MAJOR_VERSION} GREATER 2)
    cmake_policy(SET CMP0043 OLD)
    cmake_policy(SET CMP0054 OLD)
    cmake_policy(SET CMP0048 NEW)

    project(sandboxer VERSION 1.0.0)
else()
    project(sandboxer)
    set(PROJECT_VERSION "1.0.0")
endif()
cmake_policy(SET CMP0020 OLD)


add_definitions(-DPROJECT_VERSION="${PROJECT_VERSION}")

# API version
set(${PROJECT_NAME}_API_VERSION "1.0.0")
add_definitions(-DQMUSIC_API_VERSION="${${PROJECT_NAME}_API_VERSION}")

# Unit tests flag
set(CMAKE_BUILD_TESTS ON CACHE BOOL "Build unit tests")

# Target directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/target/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/target/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

# Installation destination
set(INSTALL_BIN_PATH ${CMAKE_INSTALL_PREFIX}/bin)
set(INSTALL_LIB_PATH ${CMAKE_INSTALL_PREFIX}/lib)

# Path to look for custom CMake scripts
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# Building for x86_64 required -fPIC flag
if(UNIX AND NOT WIN32)
    find_program(CMAKE_UNAME uname /bin /usr/bin /usr/local/bin)
    if(CMAKE_UNAME)
        exec_program(uname ARGS -m OUTPUT_VARIABLE CMAKE_SYSTEM_PROCESSOR)
        set(CMAKE_SYSTEM_PROCESSOR ${CMAKE_SYSTEM_PROCESSOR} CACHE INTERNAL "processor type (i386 and x86_64)")
        if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
            add_definitions(-fPIC)
        endif(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
    endif(CMAKE_UNAME)
endif(UNIX AND NOT WIN32)

if(WIN32)
    # Qt has some problems with Windows-defined min/max
    add_definitions(-DNOMINMAX)

    set(QT_DIR $ENV{QT5_PATH})
    set(WINSDK_LIB_DIR $ENV{WINSDK_PATH})
    set(CMAKE_LIBRARY_PATH "${QT_DIR};${WINSDK_LIB_DIR}")
endif()

# Qt-related flags
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

# Activate unit-testing
enable_testing()

# Process external libraries
#add_subdirectory(externals)

# Process source code
add_subdirectory(source)

include(install_qt)
include(make_package)
