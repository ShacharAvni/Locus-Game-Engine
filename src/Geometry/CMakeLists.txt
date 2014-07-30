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

if (MSVC)
    option(USE_MSVC_RUNTIME_LIBRARY_DLL "Use MSVC runtime library DLL" OFF)
endif()

if (BUILD_SHARED_LIBS)
	add_definitions(-DLOCUS_GEOMETRY_DLL)
	add_definitions(-DLOCUS_DLL)
endif()

#--------------------------------------------------------------------
# Set compiler specific flags
#--------------------------------------------------------------------
include(${PROJECT_SOURCE_DIR}/cmake/UnixOptions.cmake)

SetUnixOptions(ON ON)

include(${PROJECT_SOURCE_DIR}/cmake/SetMSVCRuntimeLibrarySettings.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/SetMSVCWarningLevel4.cmake)

set(LOCUS_INCLUDE ${PROJECT_SOURCE_DIR}/include)
set(LOCUS_GEOMETRY_INCLUDE ${LOCUS_INCLUDE}/Locus/Geometry)

include_directories(${LOCUS_INCLUDE})

add_library(Locus_Geometry
			AxisAlignedBox.cpp
			BoundingVolumeHierarchy.cpp
			Collidable.cpp
			CollisionManager.cpp
			DualTransformation.cpp
			EarClipping.cpp
			EarClippingImpl.cpp
			Frustum.cpp
			Geometry.cpp
			Line.cpp
			LineSegment.cpp
			ModelUtility.cpp
			Moveable.cpp
			Octree.cpp
			OrientedBox.cpp
			Plane.cpp
			PointCloud.cpp
			Polygon.cpp
			PolygonHierarchy.cpp
			PolygonWinding.cpp
			Quaternion.cpp
			Sphere.cpp
			Transformation.cpp
			Triangle.cpp
			Triangulation.cpp
			Vector2.cpp
			Vector3.cpp
			${LOCUS_GEOMETRY_INCLUDE}/AxisAlignedBox.h
			${LOCUS_GEOMETRY_INCLUDE}/BoundingVolumeHierarchy.h
			${LOCUS_GEOMETRY_INCLUDE}/Collidable.h
			${LOCUS_GEOMETRY_INCLUDE}/CollisionManager.h
			${LOCUS_GEOMETRY_INCLUDE}/DualTransformation.h
			EarClipping.h
			EarClippingImpl.h
			${LOCUS_GEOMETRY_INCLUDE}/Frustum.h
			${LOCUS_GEOMETRY_INCLUDE}/Geometry.h
			${LOCUS_GEOMETRY_INCLUDE}/IntersectionTypes.h
			${LOCUS_GEOMETRY_INCLUDE}/Line.h
			${LOCUS_GEOMETRY_INCLUDE}/LineFwd.h
			${LOCUS_GEOMETRY_INCLUDE}/LineSegment.h
			${LOCUS_GEOMETRY_INCLUDE}/LineSegmentFwd.h
			${LOCUS_GEOMETRY_INCLUDE}/LocusGeometryAPI.h
			${LOCUS_GEOMETRY_INCLUDE}/Model.h
			${LOCUS_GEOMETRY_INCLUDE}/ModelUtility.h
			${LOCUS_GEOMETRY_INCLUDE}/MotionProperties.h
			${LOCUS_GEOMETRY_INCLUDE}/Moveable.h
			${LOCUS_GEOMETRY_INCLUDE}/Octree.h
			${LOCUS_GEOMETRY_INCLUDE}/OrientedBox.h
			${LOCUS_GEOMETRY_INCLUDE}/Plane.h
			${LOCUS_GEOMETRY_INCLUDE}/PointCloud.h
			${LOCUS_GEOMETRY_INCLUDE}/Polygon.h
			${LOCUS_GEOMETRY_INCLUDE}/PolygonFwd.h
			${LOCUS_GEOMETRY_INCLUDE}/PolygonHierarchy.h
			${LOCUS_GEOMETRY_INCLUDE}/PolygonWinding.h
			${LOCUS_GEOMETRY_INCLUDE}/Quaternion.h
			${LOCUS_GEOMETRY_INCLUDE}/Sphere.h
			${LOCUS_GEOMETRY_INCLUDE}/Transformation.h
			${LOCUS_GEOMETRY_INCLUDE}/Triangle.h
			${LOCUS_GEOMETRY_INCLUDE}/TriangleFwd.h
			${LOCUS_GEOMETRY_INCLUDE}/Triangulation.h
			${LOCUS_GEOMETRY_INCLUDE}/Vector2.h
			${LOCUS_GEOMETRY_INCLUDE}/Vector3.h)

target_link_libraries(Locus_Geometry Locus_Common)
target_link_libraries(Locus_Geometry Locus_Math)