###########################################################################################################
#                                                                                                         #
#    This file is part of the Locus Game Engine                                                           #
#                                                                                                         #
#    Copyright (c) 2014 Shachar Avni. All rights reserved.                                                #
#                                                                                                         #
#    Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    #
#                                                                                                         #
###########################################################################################################

project(Locus)

cmake_minimum_required(VERSION 2.8)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

option(LOCUS_BUILD_EXAMPLES "Build Examples" ON)

if (MSVC)
    option(USE_MSVC_RUNTIME_LIBRARY_DLL "Use MSVC runtime library DLL" OFF)
endif()

include(${PROJECT_SOURCE_DIR}/cmake/Config.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/FindOpenAL.cmake)

find_package(OpenAL REQUIRED)
find_package(OpenGL REQUIRED)

add_subdirectory(src)
add_subdirectory(third-party/FreeType)
add_subdirectory(third-party/GLEW)
add_subdirectory(third-party/GLFW)
add_subdirectory(third-party/OGG)
add_subdirectory(third-party/PHYSFS)
add_subdirectory(third-party/stb_image/src)
add_subdirectory(third-party/Vorbis)

if (LOCUS_BUILD_EXAMPLES)
   add_subdirectory(examples/Collisions)
   add_subdirectory(examples/Triangulation)
endif()