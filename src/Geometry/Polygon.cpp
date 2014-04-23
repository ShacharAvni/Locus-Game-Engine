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

#include "Locus/Geometry/Polygon.h"
#include "Locus/Geometry/Line.h"
#include "Locus/Geometry/Geometry.h"
#include "Locus/Geometry/Model.h"

#include "Locus/Common/Float.h"
#include "Locus/Common/Util.h"

#include <algorithm>
#include <unordered_map>

namespace Locus
{

template <class PointType>
Polygon<PointType>::Polygon()
   : numPoints(0)
{
}

template <class PointType>
bool Polygon<PointType>::IsWellDefined() const
{
   return (numPoints >= 3) && (normal != Vector3::ZeroVector());
}

template <class PointType>
bool Polygon<PointType>::AddPoint(const PointType& point)
{
   if ((numPoints < 4) || GetPlane().pointIsOnPlane(point))
   {
      points.push_back(point);
      ++numPoints;

      UpdateNormal();

      return true;
   }

   return false;
}

template <class PointType>
std::size_t Polygon<PointType>::NumPoints() const
{
   return numPoints;
}

template <class PointType>
const PointType& Polygon<PointType>::operator[](std::size_t i) const
{
   return points[i];
}

template <class PointType>
PointType& Polygon<PointType>::operator[](std::size_t i)
{
   return points[i];
}

template <class PointType>
PointType Polygon<PointType>::MidpointOfEdge(std::size_t i) const
{
   return points[i] + (points[(i + 1) % numPoints] - points[i])/2;
}

template <class PointType>
PointType Polygon<PointType>::Centroid() const
{
   PointType centroid;

   if (numPoints > 0)
   {
      for (const PointType& point : points)
      {
         centroid += point;
      }

      centroid = (centroid / static_cast<float>(numPoints));
   }

   return centroid;
}

template <class PointType>
void Polygon<PointType>::UpdateNormal()
{
   normal.set(0.0f, 0.0f, 0.0f);

   if (numPoints >= 3)
   {
      for (std::size_t pointIndex = 0; pointIndex < numPoints; ++pointIndex)
      {
         normal = (points[(pointIndex + 1) % numPoints] - points[pointIndex]).cross(points[(pointIndex + 2) % numPoints] - points[(pointIndex + 1) % numPoints]);

         if (normal != Vector3::ZeroVector())
         {
            break;
         }
      }
   }
}

template <class PointType>
const Vector3& Polygon<PointType>::Normal() const
{
   return normal;
}

template <class PointType>
Plane Polygon<PointType>::GetPlane() const
{
   PointType point = (numPoints > 0) ? points[0] : PointType::ZeroVector();

   return Plane(point, normal);
}

template <class PointType>
LineSegment<PointType> Polygon<PointType>::GetEdge(std::size_t i) const
{
   return LineSegment<PointType>(points[i], points[(i + 1) % numPoints]);
}

template <class PointType>
bool Polygon<PointType>::PointIsOnPolygonCommonPath(const PointType& point, bool allowPointOnEdges, float toleranceFactor) const
{
   std::size_t numHits = 0;

   Line<PointType> ray(point, points[1] - points[0], true);

   PointType intersectionPoint1, intersectionPoint2;

   for (std::size_t pointIndex = 0; pointIndex < numPoints; ++pointIndex)
   {
      LineSegment<PointType> lineSegmentOfEdge(points[pointIndex], points[(pointIndex + 1) % numPoints]);

      IntersectionType rayLineSegmentIntersectionType = ray.LineSegmentIntersection(lineSegmentOfEdge, intersectionPoint1, intersectionPoint2, toleranceFactor);

      if (rayLineSegmentIntersectionType != IntersectionType::None)
      {
         if (rayLineSegmentIntersectionType == IntersectionType::Point)
         {
            if (intersectionPoint1.ApproximatelyEqualTo(point, toleranceFactor))
            {
               return allowPointOnEdges;
               //point is on the line segment
            }

            if (intersectionPoint1.ApproximatelyEqualTo(lineSegmentOfEdge.P1, toleranceFactor))
            {
               //ray hit the line segment at the endpoint lineSegmentOfEdge.P1

               if (ray.V.cross(lineSegmentOfEdge.P2 - ray.P).goesTheSameWayAs(normal))
               {
                  //only count hits for a vertex if the other vertex is on a
                  //particular side of the ray (this side is determined
                  //by the normal of the polygon)

                  ++numHits;
               }
            }
            else if (intersectionPoint1.ApproximatelyEqualTo(lineSegmentOfEdge.P2, toleranceFactor))
            {
               //ray hit the line segment at the endpoint lineSegmentOfEdge.P2

               if (ray.V.cross(lineSegmentOfEdge.P1 - ray.P).goesTheSameWayAs(normal))
               {
                  //only count hits for a vertex if the other vertex is on a
                  //particular side of the ray (this side is determined
                  //by the normal of the polygon)

                  ++numHits;
               }
            }
            else
            {
               //ray hit the line segment at a point that's not one of its endpoints
               ++numHits;
            }
         }
         else //if (rayLineSegmentIntersectionType == IntersectionType::LineSegment)
         {
            if ((intersectionPoint1.ApproximatelyEqualTo(point, toleranceFactor)) || (intersectionPoint2.ApproximatelyEqualTo(point, toleranceFactor)))
            {
               //point is on the line segment
               return allowPointOnEdges;
            }
         }

      }
   }

   return ((numHits % 2) != 0);
}

template <>
bool Polygon<Vector3>::PointIsOnPolygon(const Vector3& point, bool allowPointOnEdges, float toleranceFactor) const
{
   if (GetPlane().pointIsOnPlane(point))
   {
      return PointIsOnPolygonCommonPath(point, allowPointOnEdges, toleranceFactor);
   }
   else
   {
      return false;
   }
}

template <>
bool Polygon<Vector2>::PointIsOnPolygon(const Vector2& point, bool allowPointOnEdges, float toleranceFactor) const
{
   float minX = points[0].x;
   float maxX = minX;

   float minY = points[0].y;
   float maxY = minY;

   for (std::size_t pointIndex = 1; pointIndex < numPoints; ++pointIndex)
   {
      if (points[pointIndex].x < minX)
      {
         minX = points[pointIndex].x;
      }

      if (points[pointIndex].x > maxX)
      {
         maxX = points[pointIndex].x;
      }

      if (points[pointIndex].y < minY)
      {
         minY = points[pointIndex].y;
      }

      if (points[pointIndex].y > maxY)
      {
         maxY = points[pointIndex].y;
      }
   }

   if ((point.x < minX) || (point.x > maxX) || (point.y < minY) || (point.y > maxY))
   {
      return false;
   }

   return PointIsOnPolygonCommonPath(point, allowPointOnEdges, toleranceFactor);
}

template <class PointType>
bool Polygon<PointType>::PointIsOnPolygon(const PointType& point, float toleranceFactor) const
{
   if (IsWellDefined())
   {
      return PointIsOnPolygon(point, true, toleranceFactor);
   }
   else
   {
      return false;
   }
}

template <class PointType>
bool Polygon<PointType>::PointIsWithinPolygon(const PointType& point, float toleranceFactor) const
{
   if (IsWellDefined())
   {
      return PointIsOnPolygon(point, false, toleranceFactor);
   }
   else
   {
      return false;
   }
}

template <class PointType>
void Polygon<PointType>::Reverse()
{
   std::reverse(points.begin(), points.end());
   normal = -normal;
}

template <class PointType>
Vector3 Polygon<PointType>::CrossAtVertex(std::size_t vertexIndex) const
{
   Vector3 cross;

   if (vertexIndex == 0)
   {
      cross = (points[0] - points[numPoints - 1]).cross(points[1] - points[0]);
   }
   else
   {
      cross = (points[vertexIndex] - points[vertexIndex - 1]).cross(points[(vertexIndex + 1) % numPoints] - points[vertexIndex]);
   }

   return cross;
}

template <class PointType>
PolygonWinding Polygon<PointType>::GetWinding(const Vector3& normalVector) const
{
   if (!IsWellDefined() || (normalVector.cross(normal) != Vector3::ZeroVector()))
   {
      return PolygonWinding::Undefined;
   }

   std::size_t indexOfPointWithMaxX = 0;
   float maxX = points[0].x;

   for (std::size_t pointIndex = 1; pointIndex < numPoints; ++pointIndex)
   {
      if (points[pointIndex].x > maxX)
      {
         maxX = points[pointIndex].x;
         indexOfPointWithMaxX = pointIndex;
      }
   }

   Vector3 cross;

   std::size_t indexOfCrossVertex = indexOfPointWithMaxX;

   do
   {
      cross = CrossAtVertex(indexOfCrossVertex);

      if (indexOfCrossVertex == 0)
      {
         indexOfCrossVertex = numPoints - 1;
      }
      else
      {
         --indexOfCrossVertex;
      }
   } while (cross.ApproximatelyEqualTo(Vector3::ZeroVector()));

   if (cross.goesTheSameWayAs(normalVector))
   {
      return PolygonWinding::CounterClockwise;
   }
   else
   {
      return PolygonWinding::Clockwise;
   }
}

template <>
PolygonWinding Polygon<Vector2>::GetWinding(const Vector3& normalVector) const
{
   if (!IsWellDefined() || (normalVector.cross(normal) != Vector3::ZeroVector()))
   {
      return PolygonWinding::Undefined;
   }

   float sum = 0.0f;
   Vector2 p = Vector2::ZeroVector();

   for (std::size_t pointIndex = 0; pointIndex < numPoints; ++pointIndex)
   {
      sum += ( (points[pointIndex] - p).cross(points[(pointIndex + 1) % numPoints] - points[pointIndex]) ).z;
   }

   if (sum > 0.0f)
   {
      return (normalVector.z > 0.0f) ? PolygonWinding::CounterClockwise : PolygonWinding::Clockwise;
   }
   else
   {
      return (normalVector.z > 0.0f) ? PolygonWinding::Clockwise : PolygonWinding::CounterClockwise;
   }
}

template class LOCUS_GEOMETRY_API_AT_DEFINITION Polygon<Vector3>;
template class LOCUS_GEOMETRY_API_AT_DEFINITION Polygon<Vector2>;

} //namespace Geometry
