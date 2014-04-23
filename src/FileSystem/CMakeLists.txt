############################################################################################################################################################
#                                                                                                                                                          #
#    This file is part of the Locus Game Engine                                                                                                            #
#                                                                                                                                                          #
#    Copyright (c) 2014 Shachar Avni. All rights reserved.                                                                                                 #
#                                                                                                                                                          #
#    Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),    #
#    to deal in the Software without restriction, including without limitation the rights to use, modify, distribute, and to permit persons to whom the    #
#    Software is furnished to do so, subject to the following conditions:                                                                                  #
#                                                                                                                                                          #
#    a) Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.                       #
#    b) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the              #
#       documentation and/or other materials provided with the distribution.                                                                               #
#                                                                                                                                                          #
#    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,     #
#    THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR               #
#    CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,             #
#    PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF             #
#    LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,     #
#    EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                                                                                    #
#                                                                                                                                                          #
############################################################################################################################################################

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
