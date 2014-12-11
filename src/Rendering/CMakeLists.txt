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
	add_definitions(-DLOCUS_RENDERING_SHARED)
	add_definitions(-DLOCUS_SHARED)
endif()

add_definitions(-DGLEW_MX)
add_definitions(-DGLEW_NO_GLU)

if(BUILD_SHARED_LIBS)
	add_definitions(-DSTBI_DLL)
	add_definitions(-DFREETYPE_SHARED)
else()
	add_definitions(-DGLEW_STATIC)
endif()

include(${PROJECT_SOURCE_DIR}/cmake/UnixOptions.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/MSVCOptions.cmake)

SetUnixOptions(TRUE TRUE)
SetMSVCRuntimeLibrarySettings(TRUE)
SetMSVCWarningLevel4()

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
            ConstrainedViewpoint.cpp
            DefaultGPUVertexData.cpp
            DefaultSingleDrawable.cpp
            Drawable.cpp
            DrawablePointCloud.cpp
            DrawUtility.cpp
            GLInfo.cpp
            GPUVertexData.cpp
            Image.cpp
            LineSegmentCollection.cpp
            Locus_glew.cpp
            Mesh.cpp
            MeshUtility.cpp
            OffscreenBuffer.cpp
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
            TextureArray.cpp
            TextureCoordinate.cpp
            TextureManager.cpp
            TransformationStack.cpp
            Viewpoint.cpp
            ${LOCUS_RENDERING_INCLUDE}/Color.h
            ${LOCUS_RENDERING_INCLUDE}/ConstrainedViewpoint.h
            ${LOCUS_RENDERING_INCLUDE}/DefaultGPUVertexData.h
            ${LOCUS_RENDERING_INCLUDE}/DefaultSingleDrawable.h
            ${LOCUS_RENDERING_INCLUDE}/Drawable.h
            ${LOCUS_RENDERING_INCLUDE}/DrawablePointCloud.h
            ${LOCUS_RENDERING_INCLUDE}/DrawUtility.h
            ${LOCUS_RENDERING_INCLUDE}/GLCommonTypes.h
            ${LOCUS_RENDERING_INCLUDE}/GLInfo.h
            ${LOCUS_RENDERING_INCLUDE}/GPUVertexData.h
            ${LOCUS_RENDERING_INCLUDE}/GPUVertexDataStorage.h
            ${LOCUS_RENDERING_INCLUDE}/Image.h
            ${LOCUS_RENDERING_INCLUDE}/Light.h
            ${LOCUS_RENDERING_INCLUDE}/LineSegmentCollection.h
            ${LOCUS_RENDERING_INCLUDE}/Locus_glew.h
            ${LOCUS_RENDERING_INCLUDE}/Mesh.h
            ${LOCUS_RENDERING_INCLUDE}/MeshUtility.h
            ${LOCUS_RENDERING_INCLUDE}/OffscreenBuffer.h
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
            ${LOCUS_RENDERING_INCLUDE}/TextureArray.h
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