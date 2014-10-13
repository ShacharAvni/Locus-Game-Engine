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

include(${CMAKE_CURRENT_SOURCE_DIR}/../../../cmake/GlobalProjectOptions.cmake)

if(BUILD_SHARED_LIBS)
	add_definitions(-DSTBI_BUILD_DLL)
endif()

include(${CMAKE_CURRENT_SOURCE_DIR}/../../../cmake/UnixOptions.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/../../../cmake/MSVCOptions.cmake)

SetUnixOptions(FALSE FALSE)
SetMSVCRuntimeLibrarySettings(FALSE)

set(STB_IMAGE_INCLUDE ${PROJECT_SOURCE_DIR}/third-party/stb_image/include)

include_directories(${STB_IMAGE_INCLUDE})

add_library(stb_image stb_image.c ${STB_IMAGE_INCLUDE}/stb_image/stb_image.h)