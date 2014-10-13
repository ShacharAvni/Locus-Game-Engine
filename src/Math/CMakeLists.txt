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

include(${PROJECT_SOURCE_DIR}/cmake/GlobalProjectOptions.cmake)

if(BUILD_SHARED_LIBS)
	add_definitions(-DLOCUS_MATH_SHARED)
	add_definitions(-DLOCUS_SHARED)
endif()

include(${PROJECT_SOURCE_DIR}/cmake/UnixOptions.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/MSVCOptions.cmake)

SetUnixOptions(TRUE TRUE)
SetMSVCRuntimeLibrarySettings(TRUE)
SetMSVCWarningLevel4()

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