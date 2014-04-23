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

#include "Vector3.h"
#include "Vector2.h"
#include "Line.h"
#include "IntersectionTypes.h"

#include <type_traits>

namespace Locus
{

template <class PointType>
class LOCUS_GEOMETRY_API LineSegment
{
public:
   static_assert(std::is_same<PointType, Vector2>::value || std::is_same<PointType, Vector3>::value, "PointType must be Vector3 or Vector2");

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

template class LOCUS_SHARED_IMPORTS LineSegment<Vector3>;
template class LOCUS_SHARED_IMPORTS LineSegment<Vector2>;

#endif

}