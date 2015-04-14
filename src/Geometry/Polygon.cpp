/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

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
   return ( (numPoints >= 3) && !ApproximatelyEqual(normal, Vec3D::ZeroVector()) );
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
   return points[i] + (points[(i + 1) % numPoints] - points[i])/2.0f;
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
   normal.Set(0.0f, 0.0f, 0.0f);

   if (numPoints >= 3)
   {
      for (std::size_t pointIndex = 0; pointIndex < numPoints; ++pointIndex)
      {
         normal = Cross(points[(pointIndex + 1) % numPoints] - points[pointIndex], points[(pointIndex + 2) % numPoints] - points[(pointIndex + 1) % numPoints]);

         if (!ApproximatelyEqual(normal, Vec3D::ZeroVector()))
         {
            break;
         }
      }
   }
}

template <class PointType>
const FVector3& Polygon<PointType>::Normal() const
{
   return normal;
}

template <class PointType>
Plane Polygon<PointType>::GetPlane() const
{
   PointType point = (numPoints > 0) ? points[0] : PointType();

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
            if (ApproximatelyEqual(intersectionPoint1, point, toleranceFactor))
            {
               return allowPointOnEdges;
               //point is on the line segment
            }

            if (ApproximatelyEqual(intersectionPoint1, lineSegmentOfEdge.P1, toleranceFactor))
            {
               //ray hit the line segment at the endpoint lineSegmentOfEdge.P1

               if (GoTheSameWay(Cross(ray.V, lineSegmentOfEdge.P2 - ray.P), normal))
               {
                  //only count hits for a vertex if the other vertex is on a
                  //particular side of the ray (this side is determined
                  //by the normal of the polygon)

                  ++numHits;
               }
            }
            else if (ApproximatelyEqual(intersectionPoint1, lineSegmentOfEdge.P2, toleranceFactor))
            {
               //ray hit the line segment at the endpoint lineSegmentOfEdge.P2

               if (GoTheSameWay(Cross(ray.V, lineSegmentOfEdge.P1 - ray.P), normal))
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
            if (ApproximatelyEqual(intersectionPoint1, point, toleranceFactor) || ApproximatelyEqual(intersectionPoint2, point, toleranceFactor))
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
bool Polygon<FVector3>::PointIsOnPolygon(const FVector3& point, bool allowPointOnEdges, float toleranceFactor) const
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
bool Polygon<FVector2>::PointIsOnPolygon(const FVector2& point, bool allowPointOnEdges, float toleranceFactor) const
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
FVector3 Polygon<PointType>::CrossAtVertex(std::size_t vertexIndex) const
{
   FVector3 cross;

   if (vertexIndex == 0)
   {
      cross = Cross(points[0] - points[numPoints - 1], points[1] - points[0]);
   }
   else
   {
      cross = Cross(points[vertexIndex] - points[vertexIndex - 1], points[(vertexIndex + 1) % numPoints] - points[vertexIndex]);
   }

   return cross;
}

template <class PointType>
PolygonWinding Polygon<PointType>::GetWinding(const FVector3& normalVector) const
{
   if (!IsWellDefined() || !ApproximatelyEqual(Cross(normalVector, normal), Vec3D::ZeroVector()))
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

   FVector3 cross;

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
   } while (ApproximatelyEqual(cross, Vec3D::ZeroVector()));

   if (GoTheSameWay(cross, normalVector))
   {
      return PolygonWinding::CounterClockwise;
   }
   else
   {
      return PolygonWinding::Clockwise;
   }
}

template <>
PolygonWinding Polygon<FVector2>::GetWinding(const FVector3& normalVector) const
{
   if (!IsWellDefined() || !ApproximatelyEqual(Cross(normalVector, normal), Vec3D::ZeroVector()))
   {
      return PolygonWinding::Undefined;
   }

   float sum = 0.0f;
   FVector2 p = Vec2D::ZeroVector();

   for (std::size_t pointIndex = 0; pointIndex < numPoints; ++pointIndex)
   {
      sum += Cross(points[pointIndex] - p, points[(pointIndex + 1) % numPoints] - points[pointIndex]).z;
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

template <class PointType>
bool Polygon<PointType>::IsSelfIntersecting() const
{
   if (!IsWellDefined())
   {
      return false;
   }

   //brute force all pairs check

   LineSegment<PointType> lineSegment1, lineSegment2;

   PointType intersectionPoint1, intersectionPoint2;

   for (std::size_t segmentIndex = 0; segmentIndex < (numPoints - 1); ++segmentIndex)
   {
      for (std::size_t segmentIndex2 = (segmentIndex + 1); segmentIndex2 < numPoints; ++segmentIndex2)
      {
         lineSegment1.P1 = points[segmentIndex];
         lineSegment1.P2 = points[(segmentIndex + 1) % numPoints];

         if (lineSegment1.P1 == lineSegment1.P2)
         {
            continue;
         }

         lineSegment2.P1 = points[segmentIndex2];
         lineSegment2.P2 = points[(segmentIndex2 + 1) % numPoints];

         if (lineSegment2.P1 == lineSegment2.P2)
         {
            continue;
         }

         IntersectionType intersectionType = lineSegment1.GetLineSegmentIntersection(lineSegment2, intersectionPoint1, intersectionPoint2);

         if (intersectionType != IntersectionType::None)
         {
            if (intersectionType == IntersectionType::Point)
            {
               bool onSegment1EndPoint = (ApproximatelyEqual(intersectionPoint1, lineSegment1.P1) || ApproximatelyEqual(intersectionPoint1, lineSegment1.P2));
               bool onSegment2EndPoint = (ApproximatelyEqual(intersectionPoint1, lineSegment2.P1) || ApproximatelyEqual(intersectionPoint1, lineSegment2.P2));

               if (!onSegment1EndPoint || !onSegment2EndPoint)
               {
                  return true;
               }

               //here the segments intersect each other at precisely one of their endpoints
               //this is only valid if the segments are neighbours

               bool segmentsAreNeighbours = ((segmentIndex2 == (segmentIndex + 1) % numPoints) || (segmentIndex == (segmentIndex2 + 1) % numPoints));

               if (!segmentsAreNeighbours)
               {
                  return true;
               }
            }
            else // if (intersectionType == IntersectionType::LineSegment)
            {
               return true;
            }
         }
      }
   }

   return false;
}

template <class PointType>
bool Polygon<PointType>::Intersects(const Polygon<PointType>& other) const
{
   std::size_t numPointsOther = other.NumPoints();

   LineSegment<PointType> lineSegment1, lineSegment2;

   //brute force all pairs check

   for (std::size_t segmentIndex = 0; segmentIndex < numPoints; ++segmentIndex)
   {
      lineSegment1.P1 = points[segmentIndex];
      lineSegment1.P2 = points[(segmentIndex + 1) % numPoints];

      for (std::size_t segmentIndex2 = 0; segmentIndex2 < numPointsOther; ++segmentIndex2)
      {
         lineSegment2.P1 = other.points[segmentIndex2];
         lineSegment2.P2 = other.points[(segmentIndex2 + 1) % numPointsOther];

         if (lineSegment1.GetLineSegmentIntersection(lineSegment2))
         {
            return true;
         }
      }
   }

   return false;
}

template class LOCUS_GEOMETRY_API_AT_DEFINITION Polygon<FVector3>;
template class LOCUS_GEOMETRY_API_AT_DEFINITION Polygon<FVector2>;

}