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

if(BUILD_SHARED_LIBS)
	add_definitions(-DLOCUS_DLL)
endif()

add_definitions(-DGLEW_MX)
add_definitions(-DGLEW_NO_GLU)

if (NOT BUILD_SHARED_LIBS)
	add_definitions(-DGLEW_STATIC)
endif()

#--------------------------------------------------------------------
# Set compiler specific flags
#--------------------------------------------------------------------
if (UNIX)
    add_definitions(-Wall)
    add_definitions(-std=c++1y)

    if (BUILD_SHARED_LIBS)
        add_definitions(-fvisibility=hidden)
    endif()
endif()

set(LOCUS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../")

include(${LOCUS_DIR}/cmake/SetMSVCRuntimeLibrarySettings.cmake)
include(${LOCUS_DIR}/cmake/SetMSVCWarningLevel4.cmake)

set(LOCUS_INCLUDE ${LOCUS_DIR}/include)
set(THIRD_PARTY_DIR ${LOCUS_DIR}/third-party)

include_directories(${LOCUS_INCLUDE}
                    ${THIRD_PARTY_DIR}/GLEW/include)

add_executable(Locus_Example_Collisions
               CollidableMesh.h
               CollidableMesh.cpp
               CollisionScene.h
               CollisionScene.cpp
               Main.cpp)

if(WIN32)
	if(MSVC)
		set_target_properties(Locus_Example_Collisions PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
	elseif(CMAKE_COMPILER_IS_GNUCXX)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows")
	else()
		message("Warning: Unable to set Locus_Example_Collisions targets subsystem as Windows. Locus_Example_Collisions may not link due to WinMain entrypoint.")
	endif()
endif()

target_link_libraries(Locus_Example_Collisions ${OPENGL_LIBRARIES})
target_link_libraries(Locus_Example_Collisions glew)
target_link_libraries(Locus_Example_Collisions Locus_Common)
target_link_libraries(Locus_Example_Collisions Locus_Math)
target_link_libraries(Locus_Example_Collisions Locus_Geometry)
target_link_libraries(Locus_Example_Collisions Locus_Rendering)
target_link_libraries(Locus_Example_Collisions Locus_Simulation)

if(WIN32)
	if(BUILD_SHARED_LIBS)
      add_custom_target(Locus_Example_Collisions_Copy_DLL_Files)
      add_custom_target(Locus_Example_Collisions_Copy_PDB_Files)

      get_target_property(ThisExampleTargetLocation Locus_Example_Collisions LOCATION)
      get_filename_component(ThisExampleTargetDir ${ThisExampleTargetLocation} PATH)

      list(APPEND DLL_ORIGIN_PATHS "${PROJECT_BINARY_DIR}/third-party/FreeType/$<CONFIGURATION>")
		list(APPEND DLL_NAMES "freetype")

		list(APPEND DLL_ORIGIN_PATHS "${PROJECT_BINARY_DIR}/third-party/GLEW/$<CONFIGURATION>")
		list(APPEND DLL_NAMES "glew")

		list(APPEND DLL_ORIGIN_PATHS "${PROJECT_BINARY_DIR}/third-party/GLFW/src/$<CONFIGURATION>")
		list(APPEND DLL_NAMES "glfw3")

		list(APPEND DLL_ORIGIN_PATHS "${PROJECT_BINARY_DIR}/third-party/PHYSFS/$<CONFIGURATION>")
		list(APPEND DLL_NAMES "physfs")

		list(APPEND DLL_ORIGIN_PATHS "${PROJECT_BINARY_DIR}/third-party/stb_image/src/$<CONFIGURATION>")
		list(APPEND DLL_NAMES "stb_image")

		list(APPEND DLL_ORIGIN_PATHS "${PROJECT_BINARY_DIR}/src/Common/$<CONFIGURATION>")
		list(APPEND DLL_NAMES "Locus_Common")

		list(APPEND DLL_ORIGIN_PATHS "${PROJECT_BINARY_DIR}/src/FileSystem/$<CONFIGURATION>")
		list(APPEND DLL_NAMES "Locus_FileSystem")

		list(APPEND DLL_ORIGIN_PATHS "${PROJECT_BINARY_DIR}/src/Geometry/$<CONFIGURATION>")
		list(APPEND DLL_NAMES "Locus_Geometry")

		list(APPEND DLL_ORIGIN_PATHS "${PROJECT_BINARY_DIR}/src/Math/$<CONFIGURATION>")
		list(APPEND DLL_NAMES "Locus_Math")

		list(APPEND DLL_ORIGIN_PATHS "${PROJECT_BINARY_DIR}/src/Rendering/$<CONFIGURATION>")
		list(APPEND DLL_NAMES "Locus_Rendering")

		list(APPEND DLL_ORIGIN_PATHS "${PROJECT_BINARY_DIR}/src/Simulation/$<CONFIGURATION>")
		list(APPEND DLL_NAMES "Locus_Simulation")

		list(LENGTH DLL_ORIGIN_PATHS NUM_DLLS)
		math(EXPR NUM_DLLS "${NUM_DLLS}-1")
		foreach(i RANGE ${NUM_DLLS})
			list(GET DLL_ORIGIN_PATHS ${i} DLL_PATH)
			list(GET DLL_NAMES ${i} DLL_NAME)

			add_custom_command(TARGET Locus_Example_Collisions_Copy_DLL_Files POST_BUILD
								COMMAND ${CMAKE_COMMAND} -E copy_if_different
								"${DLL_PATH}/${DLL_NAME}.dll"
								"${ThisExampleTargetDir}/${DLL_NAME}.dll")

			add_custom_command(TARGET Locus_Example_Collisions_Copy_PDB_Files POST_BUILD
								COMMAND ${CMAKE_COMMAND} -E copy_if_different
								"${DLL_PATH}/${DLL_NAME}.pdb"
								"${ThisExampleTargetDir}/${DLL_NAME}.pdb")
		endforeach()

      set(ALL_DEPENDENCIES
          glew
          glfw
          Locus_Common
          Locus_Math
          Locus_Geometry
          Locus_Rendering
          Locus_Simulation)

      foreach(DEPENDENCY ${ALL_DEPENDENCIES})
         add_dependencies(Locus_Example_Collisions_Copy_DLL_Files ${DEPENDENCY})
      endforeach()

      add_dependencies(Locus_Example_Collisions Locus_Example_Collisions_Copy_DLL_Files)

      add_dependencies(Locus_Example_Collisions_Copy_PDB_Files Locus_Example_Collisions)
	endif()
endif()