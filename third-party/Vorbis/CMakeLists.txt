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

#--------------------------------------------------------------------
# Set compiler specific flags
#--------------------------------------------------------------------
include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/UnixOptions.cmake)

SetUnixOptions(OFF OFF)

if (MSVC)
    if (NOT USE_MSVC_RUNTIME_LIBRARY_DLL)
        foreach (flag CMAKE_C_FLAGS
                      CMAKE_C_FLAGS_DEBUG
                      CMAKE_C_FLAGS_RELEASE
                      CMAKE_C_FLAGS_MINSIZEREL
                      CMAKE_C_FLAGS_RELWITHDEBINFO)

            if (${flag} MATCHES "/MD")
                string(REGEX REPLACE "/MD" "/MT" ${flag} "${${flag}}")
            endif()
            if (${flag} MATCHES "/MDd")
                string(REGEX REPLACE "/MDd" "/MTd" ${flag} "${${flag}}")
            endif()

        endforeach()
    endif()
endif()

set(VORBIS_INCLUDE ${PROJECT_SOURCE_DIR}/third-party/Vorbis/include)
set(OGG_INCLUDE ${PROJECT_SOURCE_DIR}/third-party/OGG/include)

include_directories(${VORBIS_INCLUDE} ${PROJECT_SOURCE_DIR}/third-party/Vorbis/lib ${OGG_INCLUDE})

set(VORBIS_SRC
    lib/analysis.c
	lib/backends.h
	lib/bitrate.c
	lib/bitrate.h
	lib/block.c
	lib/codebook.c
	lib/codebook.h
	lib/codec_internal.h
    lib/envelope.c
    lib/envelope.h
    lib/floor0.c 
    lib/floor1.c
    lib/highlevel.h
    lib/info.c
    lib/lookup.c
    lib/lookup.h
    lib/lookup_data.h
	lib/lpc.c
	lib/lpc.h
	lib/lsp.c 
	lib/lsp.h
	lib/mapping0.c
	lib/masking.h
	lib/mdct.c
	lib/mdct.h
	lib/misc.h
	lib/os.h
	lib/psy.c
	lib/psy.h
	lib/registry.c
	lib/registry.h
	lib/res0.c
	lib/scales.h
	lib/sharedbook.c
	lib/smallft.c
	lib/smallft.h
	lib/synthesis.c
	lib/vorbisenc.c
	lib/window.c
	lib/window.h
	lib/books/coupled/res_books_51.h
	lib/books/coupled/res_books_stereo.h
	lib/books/floor/floor_books.h
	lib/books/uncoupled/res_books_uncoupled.h
	lib/modes/floor_all.h
	lib/modes/psych_8.h
	lib/modes/psych_11.h
	lib/modes/psych_16.h
	lib/modes/psych_44.h
	lib/modes/residue_8.h
	lib/modes/residue_16.h
	lib/modes/residue_44.h
	lib/modes/residue_44p51.h
	lib/modes/residue_44u.h
	lib/modes/setup_8.h
	lib/modes/setup_11.h
	lib/modes/setup_16.h
	lib/modes/setup_22.h
	lib/modes/setup_32.h
	lib/modes/setup_44.h
	lib/modes/setup_44p51.h
	lib/modes/setup_44u.h
	lib/modes/setup_X.h
	${VORBIS_INCLUDE}/vorbis/codec.h
    ${VORBIS_INCLUDE}/vorbis/vorbisenc.h
	${VORBIS_INCLUDE}/vorbis/vorbisfile.h)

if(WINDOWS)
	set(VORBIS_SRC ${VORBIS_SRC} win32/vorbis.def)
endif()

add_library(vorbis ${VORBIS_SRC})

target_link_libraries(vorbis ogg)

set(VORBIS_FILE_SRC
    lib/vorbisfile.c
	${VORBIS_INCLUDE}/vorbis/codec.h
	${VORBIS_INCLUDE}/vorbis/vorbisenc.h
	${VORBIS_INCLUDE}/vorbis/vorbisfile.h)

if(WINDOWS)
	set(VORBIS_FILE_SRC ${VORBIS_FILE_SRC} win32/vorbisfile.def)
endif()

add_library(vorbisfile ${VORBIS_FILE_SRC})

target_link_libraries(vorbisfile ogg)
target_link_libraries(vorbisfile vorbis)