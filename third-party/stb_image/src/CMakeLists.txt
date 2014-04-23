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

if (MSVC)
    option(USE_MSVC_RUNTIME_LIBRARY_DLL "Use MSVC runtime library DLL" OFF)
endif()

if (BUILD_SHARED_LIBS)
	add_definitions(-DSTBI_BUILD_DLL)
endif()

#--------------------------------------------------------------------
# Set compiler specific flags
#--------------------------------------------------------------------
include(${CMAKE_CURRENT_SOURCE_DIR}/../../../cmake/UnixOptions.cmake)

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

set(STB_IMAGE_INCLUDE ${PROJECT_SOURCE_DIR}/third-party/stb_image/include)

include_directories(${STB_IMAGE_INCLUDE})

add_library(stb_image stb_image.c ${STB_IMAGE_INCLUDE}/stb_image/stb_image.h)