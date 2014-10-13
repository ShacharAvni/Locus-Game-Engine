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

include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/UnixOptions.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/MSVCOptions.cmake)

SetUnixOptions(FALSE FALSE)
SetMSVCRuntimeLibrarySettings(FALSE)

set(OGG_INCLUDE ${PROJECT_SOURCE_DIR}/third-party/OGG/include)

include_directories("${OGG_INCLUDE}")

set(OGG_SRC
    src/bitwise.c
    src/framing.c
    ${OGG_INCLUDE}/ogg/ogg.h
    ${OGG_INCLUDE}/ogg/os_types.h)

if(WINDOWS)
	set(OGG_SRC ${OGG_SRC} win32/ogg.def)
endif()

add_library(ogg ${OGG_SRC})