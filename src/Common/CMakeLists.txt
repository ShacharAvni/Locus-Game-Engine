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
	add_definitions(-DLOCUS_COMMON_SHARED)
	add_definitions(-DLOCUS_SHARED)
endif()

include(${PROJECT_SOURCE_DIR}/cmake/UnixOptions.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/MSVCOptions.cmake)

SetUnixOptions(TRUE TRUE)
SetMSVCRuntimeLibrarySettings(TRUE)
SetMSVCWarningLevel4()

set(LOCUS_INCLUDE ${PROJECT_SOURCE_DIR}/include)
set(LOCUS_COMMON_INCLUDE ${LOCUS_INCLUDE}/Locus/Common)
set(LOCUS_PREPROCESSOR_INCLUDE ${LOCUS_INCLUDE}/Locus/Preprocessor)

include_directories(${LOCUS_INCLUDE} ${PROJECT_SOURCE_DIR}/third-party/RapidXML/include)

add_library(Locus_Common
            Endian.cpp
            Exception.cpp
            Float.cpp
            IDType.cpp
            Parsing.cpp
            Random.cpp
            ScopeFinalizer.cpp
            SequentialIDGenerator.cpp
            Util.cpp
            ${LOCUS_COMMON_INCLUDE}/Casts.h
            ${LOCUS_COMMON_INCLUDE}/CircularList.h
            ${LOCUS_COMMON_INCLUDE}/Exception.h
            ${LOCUS_COMMON_INCLUDE}/Endian.h
            ${LOCUS_COMMON_INCLUDE}/Float.h
            ${LOCUS_COMMON_INCLUDE}/IDType.h
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