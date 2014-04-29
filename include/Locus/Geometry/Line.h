 /*********************************************************************************************************\
 *                                                                                                        *
 *   This file is part of the Locus Game Engine                                                           *
 *                                                                                                        *
 *   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
 *                                                                                                        *
 *   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
 *                                                                                                        *
 \*********************************************************************************************************/

#pragma once

#include "LocusGeometryAPI.h"

#include "LineSegmentFwd.h"
#include "Vector3.h"
#include "Vector2.h"
#include "IntersectionTypes.h"

#include <type_traits>

namespace Locus
{

template <class PointType>
class LOCUS_GEOMETRY_API Line
{
public:
   static_assert(std::is_same<PointType, Vector2>::value || std::is_same<PointType, Vector3>::value, "PointType must be Vector3 or Vector2");

   Line();
   Line(const PointType& pointOnLine, const PointType& vectorInDirectionOfLine, bool isRay);

   bool IsDegenerate() const;

   PointType GetPointOnLine(float s) const;

   bool IsPointOnLine(const PointType& checkPoint, float toleranceFactor = 1) const;

   IntersectionType GetLineIntersection(const Line<PointType>& otherLine, PointType& intersectionPoint1, PointType& intersectionPoint2, float toleranceFactor = 1) const;
   IntersectionType GetParallelLineIntersection(const Line<PointType>& otherLine, PointType& intersectionPoint1, PointType& intersectionPoint2, float toleranceFactor = 1) const;
   IntersectionType LineSegmentIntersection(const LineSegment<PointType>& lineSegment, PointType& intersectionPoint1, PointType& intersectionPoint2, float toleranceFactor = 1) const;
   IntersectionType GetParallelLineSegmentIntersection(const LineSegment<PointType>& lineSegment, PointType& intersectionPoint1, PointType& intersectionPoint2, float toleranceFactor = 1) const;

   PointType P;
   PointType V;
   bool isRay;
};

#ifdef LOCUS_GEOMETRY_SHARED_IMPORTS

extern template class LOCUS_SHARED_IMPORTS Line<Vector3>;
extern template class LOCUS_SHARED_IMPORTS Line<Vector2>;

#endif

}