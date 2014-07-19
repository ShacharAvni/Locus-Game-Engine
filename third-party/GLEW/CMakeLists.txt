###########################################################################################################
#                                                                                                         #
#    This file is part of the Locus Game Engine                                                           #
#                                                                                                         #
#    Copyright (c) 2014 Shachar Avni. All rights reserved.                                                #
#                                                                                                         #
#    Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    #
#                                                                                                         #
###########################################################################################################

cmake_minimum_required(VERSION 2.8)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

find_package(OpenGL REQUIRED)

if (MSVC)
    option(USE_MSVC_RUNTIME_LIBRARY_DLL "Use MSVC runtime library DLL" OFF)
endif()

add_definitions(-DGLEW_MX)
add_definitions(-DGLEW_NO_GLU)

if (BUILD_SHARED_LIBS)
	add_definitions(-DGLEW_BUILD)
else()
	add_definitions(-DGLEW_STATIC)
endif()

#--------------------------------------------------------------------
# Set compiler specific flags
#--------------------------------------------------------------------
include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/UnixOptions.cmake)

SetUnixOptions(OFF ON)

if (MSVC)
    if (NOT USE_MSVC_RUNTIME_LIBRARY_DLL)
        foreach (flag CMAKE_C_FLAGS
                      CMAKE_C_FLAGS_DEBUG
                      CMAKE_C_FLAGS_RELEASE
                      CMAKE_C_FLAGS_MINSIZEREL
                      CMAKE_C_FLAGS_RELWITHDEBINFO)

            if (${flag} MATCHES "/MD")
                string(REGEX REPLACE "/MD" "/MT" ${flag} "${${flag}}")
            endif()
            if (${flag} MATCHES "/MDd")
                string(REGEX REPLACE "/MDd" "/MTd" ${flag} "${${flag}}")
            endif()

        endforeach()
    endif()
endif()

set(GLEW_INCLUDE ${PROJECT_SOURCE_DIR}/third-party/GLEW/include)

include_directories(${GLEW_INCLUDE})

add_library(glew src/glew.c ${GLEW_INCLUDE}/GL/glew.h ${GLEW_INCLUDE}/GL/wglew.h)

target_link_libraries(glew ${OPENGL_LIBRARIES})