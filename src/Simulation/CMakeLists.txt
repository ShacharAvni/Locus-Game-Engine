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

find_package(OpenGL REQUIRED)

if(BUILD_SHARED_LIBS)
	add_definitions(-DLOCUS_SIMULATION_SHARED)
	add_definitions(-DLOCUS_SHARED)
endif()

add_definitions(-DGLEW_MX)
add_definitions(-DGLEW_NO_GLU)

if(BUILD_SHARED_LIBS)
	add_definitions(-DGLFW_DLL)
else()
	add_definitions(-DGLEW_STATIC)
endif()

include(${PROJECT_SOURCE_DIR}/cmake/UnixOptions.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/MSVCOptions.cmake)

SetUnixOptions(TRUE TRUE)
SetMSVCRuntimeLibrarySettings(TRUE)
SetMSVCWarningLevel4()

set(LOCUS_INCLUDE ${PROJECT_SOURCE_DIR}/include)
set(LOCUS_SIMULATION_INCLUDE ${LOCUS_INCLUDE}/Locus/Simulation)
set(THIRD_PARTY_DIR ${PROJECT_SOURCE_DIR}/third-party)

include_directories(${LOCUS_INCLUDE}
                    ${THIRD_PARTY_DIR}/GLEW/include
                    ${THIRD_PARTY_DIR}/GLFW/include)

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