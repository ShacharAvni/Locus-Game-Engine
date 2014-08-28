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
	add_definitions(-DLOCUS_AUDIO_DLL)
	add_definitions(-DLOCUS_DLL)
endif()

include(${PROJECT_SOURCE_DIR}/cmake/UnixOptions.cmake)

#--------------------------------------------------------------------
# Set compiler specific flags
#--------------------------------------------------------------------
SetUnixOptions(ON ON)

include(${PROJECT_SOURCE_DIR}/cmake/SetMSVCRuntimeLibrarySettings.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/SetMSVCWarningLevel4.cmake)

find_package(OpenAL REQUIRED)

set(LOCUS_INCLUDE ${PROJECT_SOURCE_DIR}/include)
set(THIRD_PARTY_DIR ${PROJECT_SOURCE_DIR}/third-party)
set(LOCUS_AUDIO_INCLUDE ${LOCUS_INCLUDE}/Locus/Audio)

include_directories(${LOCUS_INCLUDE} ${THIRD_PARTY_DIR}/OpenAL/include ${THIRD_PARTY_DIR}/OGG/include ${THIRD_PARTY_DIR}/Vorbis/include)

add_library(Locus_Audio
            OGGLoading.h
            OGGLoading.cpp
            OpenALUtil.h
            OpenALUtil.cpp
            SoundData.h
            SoundEffect.cpp
            SoundState.cpp
            WAVLoading.h
            WAVLoading.cpp
            ${LOCUS_AUDIO_INCLUDE}/LocusAudioAPI.h
            ${LOCUS_AUDIO_INCLUDE}/SoundEffect.h
            ${LOCUS_AUDIO_INCLUDE}/SoundState.h)

target_link_libraries(Locus_Audio ${OPENAL_LIBRARY})
target_link_libraries(Locus_Audio ogg)
target_link_libraries(Locus_Audio vorbisfile)
target_link_libraries(Locus_Audio vorbis)
target_link_libraries(Locus_Audio Locus_Common)
target_link_libraries(Locus_Audio Locus_FileSystem)