/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#pragma once

#include "LocusGeometryAPI.h"

#include "Vector3.h"
#include "IntersectionTypes.h"
#include "TriangleFwd.h"
#include "LineFwd.h"
#include "LineSegmentFwd.h"

#include <vector>

namespace Locus
{

class LOCUS_GEOMETRY_API Plane
{
public:
   enum class IntersectionQuery
   {
      None = 0,
      Intersects,
      Positive,
      Negative,
      Infinite
   };

   Plane();
   Plane(const Vector3& p, const Vector3& n);
   Plane(const Vector3& p, const Vector3& v1, const Vector3& v2);

   Vector3 getNormal() const;

   void setNormal(const Vector3& n);
   void set(const Vector3& p, const Vector3& n);

   float signedDistanceTo(const Vector3& v) const; //only the sign of the result is relevant (positive, negative, or zero)
   float signedDistanceTo(const Vector3& v, const Vector3& offset) const; //only the sign of the result is relevant (positive, negative, or zero)
   float distanceTo(const Vector3& v) const;
   bool pointIsOnPlane(const Vector3& p) const;

   Vector3 getProjection(const Vector3& p) const;

   bool isParallelTo(const Plane& otherPlane) const;
   bool isCoplanarTo(const Plane& otherPlane) const;

   bool intersectsLineAtOnePoint(const Line3D_t& line, float& s) const;

   IntersectionQuery getLineSegmentIntersection(const LineSegment3D_t& lineSegment, Vector3& intersectionPoint) const;
   IntersectionQuery getLineIntersection(const Line3D_t& line, Vector3& intersectionPoint) const;

   IntersectionQuery triangleIntersectionTest(const Triangle3D_t& triangle) const;

   IntersectionType getPlaneIntersection(const Plane& otherPlane, Vector3& pointOnLine, Vector3& lineVector) const;
   IntersectionType getPlaneIntersection(const Plane& otherPlane) const;
   IntersectionType TriangleIntersection(const Triangle3D_t& triangle, Vector3& p1, Vector3& p2) const;

   Vector3 P;

private:
   Vector3 N; //normal (not necessarily unit length)
};

}