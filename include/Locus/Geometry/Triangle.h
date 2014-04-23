 /********************************************************************************************************************************************************\
 *                                                                                                                                                        *
 *   This file is part of the Locus Game Engine                                                                                                           *
 *                                                                                                                                                        *
 *   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                                                                *
 *                                                                                                                                                        *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),   *
 *   to deal in the Software without restriction, including without limitation the rights to use, modify, distribute, and to permit persons to whom the   *
 *   Software is furnished to do so, subject to the following conditions:                                                                                 *
 *                                                                                                                                                        *
 *   a) Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.                      *
 *   b) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the             *
 *      documentation and/or other materials provided with the distribution.                                                                              *
 *                                                                                                                                                        *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,    *
 *   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR              *
 *   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,            *
 *   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF            *
 *   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,    *
 *   EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                                                                                   *
 *                                                                                                                                                        *
 \********************************************************************************************************************************************************/

#pragma once

#include "LocusGeometryAPI.h"

#include "Polygon.h"
#include "LineFwd.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Plane.h"
#include "LineSegmentFwd.h"

#include <vector>
#include <type_traits>

namespace Locus
{

template <class PointType>
class LOCUS_GEOMETRY_API Triangle : public Polygon<PointType>
{
public:
   static_assert(std::is_same<PointType, Vector2>::value || std::is_same<PointType, Vector3>::value, "PointType must be Vector3 or Vector2");

   Triangle();
   Triangle(const PointType& p1, const PointType& p2, const PointType& p3);

   static const std::size_t NumPointsOnATriangle = 3;

   virtual bool IsWellDefined() const override;
   virtual bool AddPoint(const PointType& point) override;

   Vector3 ComputeBarycentricCoordinates(const PointType& targetPoint) const;

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

extern template class LOCUS_SHARED_IMPORTS Triangle<Vector3>;
extern template class LOCUS_SHARED_IMPORTS Triangle<Vector2>;

#endif

} //namespace Geometry