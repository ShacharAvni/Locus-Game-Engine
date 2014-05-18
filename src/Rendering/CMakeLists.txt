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
	add_definitions(-DLOCUS_RENDERING_DLL)
	add_definitions(-DLOCUS_DLL)
endif()

add_definitions(-DGLEW_MX)
add_definitions(-DGLEW_NO_GLU)

if (BUILD_SHARED_LIBS)
	add_definitions(-DSTBI_DLL)
	add_definitions(-DFREETYPE_SHARED)
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
set(LOCUS_RENDERING_INCLUDE ${LOCUS_INCLUDE}/Locus/Rendering)
set(THIRD_PARTY_DIR ${PROJECT_SOURCE_DIR}/third-party)

include_directories(${LOCUS_INCLUDE}
                    ${THIRD_PARTY_DIR}/GLEW/include
                    ${THIRD_PARTY_DIR}/stb_image/include
					${THIRD_PARTY_DIR}/RapidXML/include
					${THIRD_PARTY_DIR}/FreeType/include)

add_library(Locus_Rendering
			Color.cpp
			Drawable.cpp
			DrawablePointCloud.cpp
			DrawUtility.cpp
			GLInfo.cpp
			GPUVertexData.cpp
			Image.cpp
			LineCollection.cpp
			Locus_glew.cpp
			Mesh.cpp
			MeshUtility.cpp
			Quad.cpp
			RenderingState.cpp
			SAPReading.cpp
			Shader.cpp
			ShaderController.cpp
			ShaderLinkException.cpp
			ShaderProgram.cpp
			ShaderSourceStore.cpp
			ShaderVariables.cpp
			SingleDrawable.cpp
			SkyBox.cpp
			Texture.cpp
			TextureCoordinate.cpp
			TextureManager.cpp
			TransformationStack.cpp
			Viewpoint.cpp
			${LOCUS_RENDERING_INCLUDE}/Color.h
			${LOCUS_RENDERING_INCLUDE}/Drawable.h
			${LOCUS_RENDERING_INCLUDE}/DrawablePointCloud.h
			${LOCUS_RENDERING_INCLUDE}/DrawUtility.h
			${LOCUS_RENDERING_INCLUDE}/GLCommonTypes.h
			${LOCUS_RENDERING_INCLUDE}/GLInfo.h
			${LOCUS_RENDERING_INCLUDE}/GPUVertexData.h
			${LOCUS_RENDERING_INCLUDE}/GPUVertexDataStorage.h
			${LOCUS_RENDERING_INCLUDE}/Image.h
			${LOCUS_RENDERING_INCLUDE}/Light.h
			${LOCUS_RENDERING_INCLUDE}/LineCollection.h
			${LOCUS_RENDERING_INCLUDE}/Locus_glew.h
			${LOCUS_RENDERING_INCLUDE}/Mesh.h
			${LOCUS_RENDERING_INCLUDE}/MeshUtility.h
			${LOCUS_RENDERING_INCLUDE}/Quad.h
			${LOCUS_RENDERING_INCLUDE}/RenderingState.h
			${LOCUS_RENDERING_INCLUDE}/SAPReading.h
			${LOCUS_RENDERING_INCLUDE}/Shader.h
			${LOCUS_RENDERING_INCLUDE}/ShaderController.h
			${LOCUS_RENDERING_INCLUDE}/ShaderLinkException.h
			${LOCUS_RENDERING_INCLUDE}/ShaderProgram.h
			${LOCUS_RENDERING_INCLUDE}/ShaderSourceStore.h
			${LOCUS_RENDERING_INCLUDE}/ShaderVariables.h
			${LOCUS_RENDERING_INCLUDE}/SingleDrawable.h
			${LOCUS_RENDERING_INCLUDE}/SkyBox.h
			${LOCUS_RENDERING_INCLUDE}/Texture.h
			${LOCUS_RENDERING_INCLUDE}/TextureCoordinate.h
			${LOCUS_RENDERING_INCLUDE}/TextureManager.h
			${LOCUS_RENDERING_INCLUDE}/TransformationStack.h
			${LOCUS_RENDERING_INCLUDE}/Viewpoint.h
			${LOCUS_RENDERING_INCLUDE}/LocusRenderingAPI.h)

target_link_libraries(Locus_Rendering ${OPENGL_LIBRARIES})
target_link_libraries(Locus_Rendering glew)
target_link_libraries(Locus_Rendering stb_image)
target_link_libraries(Locus_Rendering freetype)
target_link_libraries(Locus_Rendering Locus_Common)
target_link_libraries(Locus_Rendering Locus_Math)
target_link_libraries(Locus_Rendering Locus_Geometry)
target_link_libraries(Locus_Rendering Locus_FileSystem)
