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

find_package(OpenGL REQUIRED)

if (MSVC)
    option(USE_MSVC_RUNTIME_LIBRARY_DLL "Use MSVC runtime library DLL" OFF)
endif()

if (BUILD_SHARED_LIBS)
	add_definitions(-DLOCUS_SIMULATION_SHARED)
	add_definitions(-DLOCUS_SHARED)
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