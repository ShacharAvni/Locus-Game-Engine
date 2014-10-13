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

include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/GlobalProjectOptions.cmake)

find_package(OpenGL REQUIRED)

add_definitions(-DGLEW_MX)
add_definitions(-DGLEW_NO_GLU)

if(BUILD_SHARED_LIBS)
	add_definitions(-DGLEW_BUILD)
else()
	add_definitions(-DGLEW_STATIC)
endif()

include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/UnixOptions.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/MSVCOptions.cmake)

SetUnixOptions(FALSE TRUE)
SetMSVCRuntimeLibrarySettings(FALSE)

set(GLEW_INCLUDE ${PROJECT_SOURCE_DIR}/third-party/GLEW/include)

include_directories(${GLEW_INCLUDE})

add_library(glew src/glew.c ${GLEW_INCLUDE}/GL/glew.h ${GLEW_INCLUDE}/GL/wglew.h)

target_link_libraries(glew ${OPENGL_LIBRARIES})