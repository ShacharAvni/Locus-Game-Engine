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

include(${PROJECT_SOURCE_DIR}/cmake/GlobalProjectOptions.cmake)

option(LOCUS_BUILD_EXAMPLES "Build Examples" ON)

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

if(LOCUS_BUILD_EXAMPLES)
   add_subdirectory(examples/Collisions)
   add_subdirectory(examples/Triangulation)
endif()