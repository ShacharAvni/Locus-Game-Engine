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
	add_definitions(-DLOCUS_COMMON_DLL)
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
set(LOCUS_COMMON_INCLUDE ${LOCUS_INCLUDE}/Locus/Common)
set(LOCUS_PREPROCESSOR_INCLUDE ${LOCUS_INCLUDE}/Locus/Preprocessor)

include_directories(${LOCUS_INCLUDE} ${PROJECT_SOURCE_DIR}/third-party/RapidXML/include)

add_library(Locus_Common
			Endian.cpp
			Exception.cpp
			Float.cpp
			IDGenerator.cpp
			Parsing.cpp
			Random.cpp
			ScopeFinalizer.cpp
			SequentialIDGenerator.cpp
			${LOCUS_COMMON_INCLUDE}/Casts.h
			${LOCUS_COMMON_INCLUDE}/CircularList.h
			${LOCUS_COMMON_INCLUDE}/Exception.h
			${LOCUS_COMMON_INCLUDE}/Endian.h
			${LOCUS_COMMON_INCLUDE}/Float.h
			${LOCUS_COMMON_INCLUDE}/IDGenerator.h
			${LOCUS_COMMON_INCLUDE}/Parsing.h
			${LOCUS_COMMON_INCLUDE}/Random.h
			${LOCUS_COMMON_INCLUDE}/ScopeFinalizer.h
			${LOCUS_COMMON_INCLUDE}/SequentialIDGenerator.h
			${LOCUS_COMMON_INCLUDE}/StaticAssertFalse.h
			${LOCUS_COMMON_INCLUDE}/Util.h
			${LOCUS_COMMON_INCLUDE}/LocusCommonAPI.h
			${LOCUS_PREPROCESSOR_INCLUDE}/BeginSilenceDLLInterfaceWarnings
			${LOCUS_PREPROCESSOR_INCLUDE}/EndSilenceDLLInterfaceWarnings
			${LOCUS_PREPROCESSOR_INCLUDE}/CompilerDefinitions.h)