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

find_package(OpenGL REQUIRED)

if (MSVC)
    option(USE_MSVC_RUNTIME_LIBRARY_DLL "Use MSVC runtime library DLL" OFF)
endif()

if (BUILD_SHARED_LIBS)
	add_definitions(-DLOCUS_SIMULATION_DLL)
	add_definitions(-DLOCUS_DLL)
endif()

add_definitions(-DGLEW_MX)
add_definitions(-DGLEW_NO_GLU)

if (BUILD_SHARED_LIBS)
	add_definitions(-DGLFW_DLL)
else ()
	add_definitions(-DGLEW_STATIC)
endif()

#--------------------------------------------------------------------
# Set compiler specific flags
#--------------------------------------------------------------------
include(${PROJECT_SOURCE_DIR}/cmake/UnixOptions.cmake)

SetUnixOptions(ON ON)

include(${PROJECT_SOURCE_DIR}/cmake/SetMSVCRuntimeLibrarySettings.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/SetMSVCWarningLevel4.cmake)

set(LOCUS_INCLUDE ${PROJECT_SOURCE_DIR}/include)
set(LOCUS_SIMULATION_INCLUDE ${LOCUS_INCLUDE}/Locus/Simulation)
set(THIRD_PARTY_DIR ${PROJECT_SOURCE_DIR}/third-party)

include_directories(${LOCUS_INCLUDE} ${THIRD_PARTY_DIR}/GLEW/include ${THIRD_PARTY_DIR}/GLFW/include)

add_library(Locus_Simulation
            Scene.cpp
			SceneManager.cpp
			Window.cpp
			WindowContext.cpp
			${LOCUS_SIMULATION_INCLUDE}/Scene.h
			${LOCUS_SIMULATION_INCLUDE}/SceneManager.h
			${LOCUS_SIMULATION_INCLUDE}/UserEvents.h
			${LOCUS_SIMULATION_INCLUDE}/Window.h
			${LOCUS_SIMULATION_INCLUDE}/WindowEventListener.h
			${LOCUS_SIMULATION_INCLUDE}/WindowContext.h
			${LOCUS_SIMULATION_INCLUDE}/LocusSimulationAPI.h)

target_link_libraries(Locus_Simulation ${OPENGL_LIBRARIES})
target_link_libraries(Locus_Simulation glew)
target_link_libraries(Locus_Simulation glfw ${GLFW_LIBRARIES})
target_link_libraries(Locus_Simulation Locus_Common)
target_link_libraries(Locus_Simulation Locus_Rendering)