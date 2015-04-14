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

#include "IntersectionTypes.h"
#include "TriangleFwd.h"
#include "LineFwd.h"
#include "LineSegmentFwd.h"

#include "Locus/Math/Vectors.h"

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
   Plane(const FVector3& p, const FVector3& n);
   Plane(const FVector3& p, const FVector3& v1, const FVector3& v2);

   FVector3 getNormal() const;

   void setNormal(const FVector3& n);
   void set(const FVector3& p, const FVector3& n);

   float signedDistanceTo(const FVector3& v) const; //only the sign of the result is relevant (positive, negative, or zero)
   float signedDistanceTo(const FVector3& v, const FVector3& offset) const; //only the sign of the result is relevant (positive, negative, or zero)
   float distanceTo(const FVector3& v) const;
   bool pointIsOnPlane(const FVector3& p) const;

   FVector3 getProjection(const FVector3& p) const;

   bool isParallelTo(const Plane& otherPlane) const;
   bool isCoplanarTo(const Plane& otherPlane) const;

   bool intersectsLineAtOnePoint(const Line3D_t& line, float& s) const;

   IntersectionQuery getLineSegmentIntersection(const LineSegment3D_t& lineSegment, FVector3& intersectionPoint) const;
   IntersectionQuery getLineIntersection(const Line3D_t& line, FVector3& intersectionPoint) const;

   IntersectionQuery triangleIntersectionTest(const Triangle3D_t& triangle) const;

   IntersectionType getPlaneIntersection(const Plane& otherPlane, FVector3& pointOnLine, FVector3& lineVector) const;
   IntersectionType getPlaneIntersection(const Plane& otherPlane) const;
   IntersectionType TriangleIntersection(const Triangle3D_t& triangle, FVector3& p1, FVector3& p2) const;

   FVector3 P;

private:
   FVector3 N; //normal (not necessarily unit length)
};

}