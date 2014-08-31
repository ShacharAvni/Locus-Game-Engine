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

if (MSVC)
    option(USE_MSVC_RUNTIME_LIBRARY_DLL "Use MSVC runtime library DLL" OFF)
endif()

if(BUILD_SHARED_LIBS)
	add_definitions(-DLOCUS_MATH_DLL)
	add_definitions(-DLOCUS_DLL)
endif()

#--------------------------------------------------------------------
# Set compiler specific flags
#--------------------------------------------------------------------
include(${PROJECT_SOURCE_DIR}/cmake/UnixOptions.cmake)

SetUnixOptions(ON ON)

include(${PROJECT_SOURCE_DIR}/cmake/SetMSVCRuntimeLibrarySettings.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/SetMSVCWarningLevel4.cmake)

set(LOCUS_INCLUDE ${PROJECT_SOURCE_DIR}/include)
set(LOCUS_MATH_INCLUDE "${LOCUS_INCLUDE}/Locus/Math")

include_directories(${LOCUS_INCLUDE})

add_library(Locus_Math
			   Matrix.cpp
			   MByNIterations.cpp
			   Polynomial.cpp
			   SJTPermutations.cpp
			   ${LOCUS_MATH_INCLUDE}/ComplexUtil.h
			   ${LOCUS_MATH_INCLUDE}/Matrix.h
			   ${LOCUS_MATH_INCLUDE}/MByNIterations.h
			   ${LOCUS_MATH_INCLUDE}/Polynomial.h
			   ${LOCUS_MATH_INCLUDE}/SJTPermutations.h
			   ${LOCUS_MATH_INCLUDE}/LocusMathAPI.h)

target_link_libraries(Locus_Math Locus_Common)