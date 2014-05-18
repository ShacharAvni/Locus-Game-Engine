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
	add_definitions(-DLOCUS_FILE_SYSTEM_DLL)
	add_definitions(-DLOCUS_DLL)
	add_definitions(-DPHYSFS_SHARED)
endif()

#--------------------------------------------------------------------
# Set compiler specific flags
#--------------------------------------------------------------------
include(${PROJECT_SOURCE_DIR}/cmake/UnixOptions.cmake)

SetUnixOptions(ON ON)

include(${PROJECT_SOURCE_DIR}/cmake/SetMSVCRuntimeLibrarySettings.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/SetMSVCWarningLevel4.cmake)

set(LOCUS_INCLUDE ${PROJECT_SOURCE_DIR}/include)
set(LOCUS_FILE_SYSTEM_INCLUDE ${LOCUS_INCLUDE}/Locus/FileSystem)

include_directories(${LOCUS_INCLUDE} ${PROJECT_SOURCE_DIR}/third-party/PHYSFS)

add_library(Locus_FileSystem
			${LOCUS_FILE_SYSTEM_INCLUDE}/DataStream.h
			${LOCUS_FILE_SYSTEM_INCLUDE}/File.h
			${LOCUS_FILE_SYSTEM_INCLUDE}/FileOnDisk.h
            ${LOCUS_FILE_SYSTEM_INCLUDE}/FileSystem.h
			${LOCUS_FILE_SYSTEM_INCLUDE}/FileSystemUtil.h
			${LOCUS_FILE_SYSTEM_INCLUDE}/InMemoryDataStream.h
			${LOCUS_FILE_SYSTEM_INCLUDE}/LocusFileSystemAPI.h
			${LOCUS_FILE_SYSTEM_INCLUDE}/MountedFilePath.h
			DataStream.cpp
			File.cpp
			FileOnDisk.cpp
			FileSystem.cpp
			FileSystemUtil.cpp
			InMemoryDataStream.cpp
			MountedFilePath.cpp)

if(BUILD_SHARED_LIBS)
	target_link_libraries(Locus_FileSystem physfs)
else()
	target_link_libraries(Locus_FileSystem physfs-static)
endif()

target_link_libraries(Locus_FileSystem Locus_Common)
