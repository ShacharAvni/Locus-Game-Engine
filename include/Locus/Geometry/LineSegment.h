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

#include "Line.h"
#include "IntersectionTypes.h"

#include "Locus/Math/Vectors.h"

#include <type_traits>

namespace Locus
{

template <class PointType>
class LOCUS_GEOMETRY_API LineSegment
{
public:
   static_assert(std::is_same<PointType, FVector2>::value || std::is_same<PointType, FVector3>::value, "PointType must be FVector3 or FVector2");

   LineSegment();
   LineSegment(const PointType& P1, const PointType& P2);

   bool IsDegenerate() const;

   Line<PointType> MakeLine(bool isRay) const;

   PointType Vector() const;

   bool PointIsOnLineSegment(const PointType& checkPoint, float toleranceFactor = 1) const;
   IntersectionType GetCollinearLineSegmentIntersection(const LineSegment<PointType>& otherLineSegment, PointType& pIntersection1, PointType& pIntersection2) const;
   bool GetCollinearLineSegmentIntersection(const LineSegment<PointType>& otherLineSegment) const;
   IntersectionType GetLineSegmentIntersection(const LineSegment<PointType>& otherLineSegment, PointType& pIntersection1, PointType& pIntersection2) const;
   bool GetLineSegmentIntersection(const LineSegment<PointType>& otherLineSegment) const;

   PointType P1;
   PointType P2;
};

#ifdef LOCUS_GEOMETRY_SHARED_IMPORTS

template class LOCUS_SHARED_IMPORTS LineSegment<FVector3>;
template class LOCUS_SHARED_IMPORTS LineSegment<FVector2>;

#endif

}