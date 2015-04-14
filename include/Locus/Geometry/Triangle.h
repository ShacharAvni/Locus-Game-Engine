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

#include "Polygon.h"
#include "LineFwd.h"
#include "Plane.h"
#include "LineSegmentFwd.h"

#include "Locus/Math/Vectors.h"

#include <vector>
#include <type_traits>

namespace Locus
{

template <class PointType>
class LOCUS_GEOMETRY_API Triangle : public Polygon<PointType>
{
public:
   static_assert(std::is_same<PointType, FVector2>::value || std::is_same<PointType, FVector3>::value, "PointType must be FVector3 or FVector2");

   Triangle();
   Triangle(const PointType& p1, const PointType& p2, const PointType& p3);

   static const std::size_t NumPointsOnATriangle = 3;

   virtual bool IsWellDefined() const override;
   virtual bool AddPoint(const PointType& point) override;

   FVector3 ComputeBarycentricCoordinates(const PointType& targetPoint) const;

   static PointType ComputeCentroid(const Triangle<PointType>& triangle1, const Triangle<PointType>& triangle2);

   static bool IsValidTriangle(const PointType& point1, const PointType& point2, const PointType& point3, float toleranceFactor = 1);

   IntersectionType CoplanarTriangleIntersection(const Triangle<PointType>& otherTriangle, std::vector<PointType>& intersectionPoints) const;
   bool CoplanarTriangleIntersection(const Triangle<PointType>& otherTriangle) const;
   IntersectionType TriangleIntersection(const Triangle<PointType>& otherTriangle, std::vector<PointType>& intersectionPoints) const;
   bool TriangleIntersection(const Triangle<PointType>& otherTriangle) const;

   IntersectionType LineIntersection(const Line<PointType>& line, std::vector<PointType>& intersectionPoints) const;
   IntersectionType LineSegmentIntersection(const LineSegment<PointType>& lineSegment, std::vector<PointType>& intersectionPoints) const;

private:
   IntersectionType CoplanarLineIntersection(const Line<PointType>& line, std::vector<PointType>& intersectionPoints) const;
   IntersectionType CoplanarLineSegmentIntersection(const LineSegment<PointType>& lineSegment, std::vector<PointType>& intersectionPoints) const;

protected:
   virtual bool PointIsOnPolygonCommonPath(const PointType& point, bool allowPointOnEdges, float toleranceFactor = 1) const override;
};

#ifdef LOCUS_GEOMETRY_SHARED_IMPORTS

extern template class LOCUS_SHARED_IMPORTS Triangle<FVector3>;
extern template class LOCUS_SHARED_IMPORTS Triangle<FVector2>;

#endif

}