/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Geometry/Triangle.h"
#include "Locus/Geometry/Line.h"
#include "Locus/Geometry/Geometry.h"
#include "Locus/Geometry/Model.h"

#include "Locus/Common/Float.h"
#include "Locus/Common/Util.h"

namespace Locus
{

template <class PointType>
Triangle<PointType>::Triangle()
{
}

template <class PointType>
Triangle<PointType>::Triangle(const PointType& p1, const PointType& p2, const PointType& p3)
{
   this->points.resize(3);
   this->points[0] = p1;
   this->points[1] = p2;
   this->points[2] = p3;

   this->numPoints = 3;

   this->normal = Cross(p2 - p1, p3 - p2);
}

template <class PointType>
bool Triangle<PointType>::IsWellDefined() const
{
   return true;
}

template <class PointType>
bool Triangle<PointType>::AddPoint(const PointType& /*point*/)
{
   return false;
}

template <class PointType>
FVector3 Triangle<PointType>::ComputeBarycentricCoordinates(const PointType& targetPoint) const
{
   FVector3 barycentric;

   FVector3 u = this->points[1] - this->points[0];
   FVector3 v = this->points[2] - this->points[0];
   FVector3 w = targetPoint - this->points[0];

   FVector3 vCrossW = Cross(v, w);
 
   FVector3 uCrossW = Cross(u, w);
   FVector3 uCrossV = Cross(u, v);
 
   float denom = Norm(uCrossV);
   barycentric.y = Norm(vCrossW) / denom;
   barycentric.z = Norm(uCrossW) / denom;

   barycentric.x = 1 - barycentric.y - barycentric.z;

   return barycentric;
}

template <class PointType>
PointType Triangle<PointType>::ComputeCentroid(const Triangle<PointType>& triangle1, const Triangle<PointType>& triangle2)
{
   PointType centroid;

   for (std::size_t triangleVertexIndex = 0; triangleVertexIndex < Triangle<PointType>::NumPointsOnATriangle; ++triangleVertexIndex)
   {
      centroid += triangle1[triangleVertexIndex];
      centroid += triangle2[triangleVertexIndex];
   }

   centroid /= static_cast<float>(2 * Triangle<PointType>::NumPointsOnATriangle);

   return centroid;
}

template <class PointType>
bool Triangle<PointType>::IsValidTriangle(const PointType& point1, const PointType& point2, const PointType& point3, float toleranceFactor)
{
   return Triangle<FVector3>::IsValidTriangle(point1, point2, point3, toleranceFactor);
}

template <>
bool Triangle<FVector3>::IsValidTriangle(const FVector3& point1, const FVector3& point2, const FVector3& point3, float toleranceFactor)
{
   return !( ApproximatelyEqual(Cross(point2 - point1, point3 - point2), Vec3D::ZeroVector(), toleranceFactor) );
}

template <class PointType>
bool Triangle<PointType>::PointIsOnPolygonCommonPath(const PointType& point, bool allowPointOnEdges, float toleranceFactor) const
{
   //Point is on the triangle if all the cross products
   //between the edge vectors and the vector from the
   //edge to the point "go the same way"

   FVector3 checkCross;
   bool crossSet = false;

   for (std::size_t i = 0; i < this->numPoints; ++i)
   {
      FVector3 edgeCross = Cross(this->points[(i + 1) % this->numPoints] - this->points[i], point - this->points[(i + 1) % this->numPoints]);

      if (!ApproximatelyEqual(edgeCross, Vec3D::ZeroVector(), toleranceFactor))
      {
         if (!crossSet)
         {
            checkCross = edgeCross;
            crossSet = true;
         }
         else
         {
            if (!GoTheSameWay(checkCross, edgeCross))
            {
               return false;
            }
         }
      }
      else
      {
         if (!allowPointOnEdges)
         {
            return false;
         }
         //else
         //{
         // if the cross product from the edge vector to the point is
         // the zero vector, that means the point is on the line going
         // through the edge. There are two cases: (1) the point is
         // on the line segment of the edge and thus on the triangle or
         // (2) the point is not on the line segment of the edge and thus
         // it is not on the polygon. The code as it is written will
         // differentiate between these two cases as it will be Case (1)
         // if the cross products of the remaining edges "go the same way"
         // and it will be Case (2) otherwise
         //}
      }
   }

   return true;
}

template <class PointType>
IntersectionType Triangle<PointType>::CoplanarTriangleIntersection(const Triangle<PointType>& otherTriangle, std::vector<PointType>& intersectionPoints) const
{
   std::vector<PointType> possibleIntersectionPoints;
   possibleIntersectionPoints.reserve(12);

   PointType intersectionPoint1, intersectionPoint2;

   for (std::size_t thisTriangleEdgeIndex = 0; thisTriangleEdgeIndex < NumPointsOnATriangle; ++thisTriangleEdgeIndex)
   {
      LineSegment<PointType> thisLineSegment(this->points[thisTriangleEdgeIndex], this->points[(thisTriangleEdgeIndex + 1) % NumPointsOnATriangle]);

      for (std::size_t otherTriangleEdgeIndex = 0; otherTriangleEdgeIndex < NumPointsOnATriangle; ++otherTriangleEdgeIndex)
      {
         LineSegment<PointType> otherLineSegment(otherTriangle.points[otherTriangleEdgeIndex], otherTriangle.points[(otherTriangleEdgeIndex + 1) % NumPointsOnATriangle]);

         IntersectionType intersectionType = thisLineSegment.GetLineSegmentIntersection(otherLineSegment, intersectionPoint1, intersectionPoint2);

         if (intersectionType == IntersectionType::Point)
         {
            possibleIntersectionPoints.push_back(intersectionPoint1);
         }
         else if (intersectionType == IntersectionType::LineSegment)
         {
            possibleIntersectionPoints.push_back(intersectionPoint1);
            possibleIntersectionPoints.push_back(intersectionPoint2);
         }
      }
   }

   if (possibleIntersectionPoints.size() > 0)
   {
      for (const PointType& point : this->points)
      {
         if (otherTriangle.PointIsOnPolygon(point))
         {
            possibleIntersectionPoints.push_back(point);
         }
      }

      for (const PointType& otherPoint : otherTriangle.points)
      {
         if (this->PointIsOnPolygon(otherPoint))
         {
            possibleIntersectionPoints.push_back(otherPoint);
         }
      }

      if (possibleIntersectionPoints.size() > 1)
      {
         std::vector<PointType> uniqueIntersectionPoints;
         std::vector<std::size_t> uniqueIndices;

         GetUniqueItems<PointType>(possibleIntersectionPoints, uniqueIntersectionPoints, [](const PointType& first, const PointType& second)->bool{ return ApproximatelyEqual(first, second); }, uniqueIndices);

         ReorderCoplanarVertices<PointType>(uniqueIntersectionPoints, this->Normal(), PolygonWinding::CounterClockwise,
            [](const PointType& v)->FVector3
            {
               return v;
            });

         std::size_t numUniqueIntersectionPoints = uniqueIntersectionPoints.size();
         intersectionPoints.resize(numUniqueIntersectionPoints);

         std::copy(uniqueIntersectionPoints.begin(), uniqueIntersectionPoints.end(), intersectionPoints.begin());

         if (numUniqueIntersectionPoints == 1)
         {
            return IntersectionType::Point;
         }
         else if (numUniqueIntersectionPoints == 2)
         {
            return IntersectionType::LineSegment;
         }
         else
         {
            return IntersectionType::Area;
         }
      }
      else
      {
         intersectionPoints.resize(1);
         intersectionPoints[0] = possibleIntersectionPoints[0];

         return IntersectionType::Point;
      }
   }
   else
   {
      //three cases are possible here. Either,
      //a) these triangles don't intersect, or
      //b) this triangle is completely within the other triangle
      //c) the other triangle is completely within this triangle

      if (otherTriangle.PointIsOnPolygon(this->points[0]))
      {
         intersectionPoints.resize(NumPointsOnATriangle);
         std::copy(this->points.begin(), this->points.end(), intersectionPoints.begin());

         return IntersectionType::Area;
      }
      else if (this->PointIsOnPolygon(otherTriangle.points[0]))
      {
         intersectionPoints.resize(NumPointsOnATriangle);
         std::copy(otherTriangle.points.begin(), otherTriangle.points.end(), intersectionPoints.begin());

         return IntersectionType::Area;
      }
      else
      {
         return IntersectionType::None;
      }
   }
}

template <class PointType>
bool Triangle<PointType>::CoplanarTriangleIntersection(const Triangle<PointType>& otherTriangle) const
{
   for (std::size_t thisTriangleEdgeIndex = 0; thisTriangleEdgeIndex < NumPointsOnATriangle; ++thisTriangleEdgeIndex)
   {
      LineSegment<PointType> thisLineSegment(this->points[thisTriangleEdgeIndex], this->points[(thisTriangleEdgeIndex + 1) % NumPointsOnATriangle]);

      for (std::size_t otherTriangleEdgeIndex = 0; otherTriangleEdgeIndex < NumPointsOnATriangle; ++otherTriangleEdgeIndex)
      {
         LineSegment<PointType> otherLineSegment(otherTriangle.points[otherTriangleEdgeIndex], otherTriangle.points[(otherTriangleEdgeIndex + 1) % NumPointsOnATriangle]);

         if (thisLineSegment.GetLineSegmentIntersection(otherLineSegment))
         {
            return true;
         }
      }
   }

   return (otherTriangle.PointIsOnPolygon(this->points[0]) || this->PointIsOnPolygon(otherTriangle.points[0]));
}

template <>
IntersectionType Triangle<FVector3>::TriangleIntersection(const Triangle<FVector3>& otherTriangle, std::vector<FVector3>& intersectionPoints) const
{
   //p1 and p2 describe the line segment on the otherTriangle that intersects the plane of this triangle if such
   //an intersection occurs. Otherwise, if the plane of this triangle intersects the other triangle at one point,
   //then p1 stores this point. If there is no intersection with the plane of this triangle and the other triangle
   //or if the intersection is the other triangle itself, then p1 and p2 are undefined.

   FVector3 p1;
   FVector3 p2;

   IntersectionType thisPlaneWithOtherTriangleIntersectionType = GetPlane().TriangleIntersection(otherTriangle, p1, p2);

   if (thisPlaneWithOtherTriangleIntersectionType == IntersectionType::LineSegment)
   {
      LineSegment<FVector3> lineSegmentThisPlaneWithOtherTriangle(p1, p2);

      //p3 and p4 describe the line segment on this triangle that intersects the plane of the other triangle if such
      //an intersection occurs. Otherwise, if the plane of the other triangle intersects this triangle at one point,
      //then p3 stores this point. If there is no intersection with the plane of the other triangle and this triangle
      //then p3 and p4 are undefined.
      //
      //Note that an "Area" intersection is not possible in this case because that would
      //imply that the previous intersection test (this plane with other triangle) would
      //have been an "Area" intersection as well.

      FVector3 p3;
      FVector3 p4;

      IntersectionType otherPlaneWithThisTriangleIntersectionType = otherTriangle.GetPlane().TriangleIntersection(*this, p3, p4);

      if (otherPlaneWithThisTriangleIntersectionType == IntersectionType::LineSegment)
      {
         LineSegment<FVector3> lineSegmentOtherPlaneWithThisTriangle(p3, p4);

         FVector3 pIntersection1;
         FVector3 pIntersection2;

         IntersectionType lineSegmentsIntersectionType = lineSegmentThisPlaneWithOtherTriangle.GetLineSegmentIntersection(lineSegmentOtherPlaneWithThisTriangle, pIntersection1, pIntersection2);

         if (lineSegmentsIntersectionType == IntersectionType::LineSegment)
         {
            intersectionPoints.resize(2);
            intersectionPoints[0] = pIntersection1;
            intersectionPoints[1] = pIntersection2;

            return IntersectionType::LineSegment;
         }
         else if (lineSegmentsIntersectionType == IntersectionType::Point)
         {
            intersectionPoints.resize(1);
            intersectionPoints[0] = pIntersection1;

            return IntersectionType::Point;
         }
         else
         {
            return IntersectionType::None;
         }
      }
      else if (otherPlaneWithThisTriangleIntersectionType == IntersectionType::Point)
      {
         if (lineSegmentThisPlaneWithOtherTriangle.PointIsOnLineSegment(p3))
         {
            intersectionPoints.resize(1);
            intersectionPoints[0] = p3;

            return IntersectionType::Point;
         }
         else
         {
            return IntersectionType::None;
         }
      }
      else
      {
         return IntersectionType::None;
      }
   }
   else if (thisPlaneWithOtherTriangleIntersectionType == IntersectionType::Area)
   {
      return CoplanarTriangleIntersection(otherTriangle, intersectionPoints);
   }
   else if (thisPlaneWithOtherTriangleIntersectionType == IntersectionType::Point)
   {
      if (this->PointIsOnPolygon(p1))
      {
         intersectionPoints.resize(1);
         intersectionPoints[0] = p1;

         return IntersectionType::Point;
      }
      else
      {
         return IntersectionType::None;
      }
   }
   else
   {
      return IntersectionType::None;
   }
}

template <>
IntersectionType Triangle<FVector2>::TriangleIntersection(const Triangle<FVector2>& otherTriangle, std::vector<FVector2>& intersectionPoints) const
{
   return CoplanarTriangleIntersection(otherTriangle, intersectionPoints);
}

template <>
bool Triangle<FVector3>::TriangleIntersection(const Triangle<FVector3>& otherTriangle) const
{
   Plane otherPlane = otherTriangle.GetPlane();
   IntersectionType planeIntersection = GetPlane().getPlaneIntersection(otherPlane);

   if (planeIntersection == IntersectionType::Line)
   {
      FVector3 lineSegmentIntersectionPoint;
      for (int lineSegmentIndex = 0; lineSegmentIndex < 3; ++lineSegmentIndex)
      {
         if (otherPlane.getLineSegmentIntersection(GetEdge(lineSegmentIndex), lineSegmentIntersectionPoint) == Plane::IntersectionQuery::Intersects)
         {
            if (otherTriangle.PointIsOnPolygon(lineSegmentIntersectionPoint))
            {
               return true;
            }
         }
      }

      return false;
   }
   else if (planeIntersection == IntersectionType::None)
   {
      return false;
   }
   else //if (planeIntersection == IntersectionType::Plane)
   {
      return CoplanarTriangleIntersection(otherTriangle);
   }
}

template <>
bool Triangle<FVector2>::TriangleIntersection(const Triangle<FVector2>& otherTriangle) const
{
   return CoplanarTriangleIntersection(otherTriangle);
}

template <class PointType>
IntersectionType Triangle<PointType>::CoplanarLineIntersection(const Line<PointType>& line, std::vector<PointType>& intersectionPoints) const
{
   PointType p1;
   PointType p2;
   bool hasIntersection = false;
   bool hasSegmentIntersection = false;

   PointType lineLineSegmentIntersectionPoint1, lineLineSegmentIntersectionPoint2;

   for (std::size_t triangleEdgeIndex = 0; triangleEdgeIndex < Triangle::NumPointsOnATriangle; ++triangleEdgeIndex)
   {
      LineSegment<PointType> triangleEdge = this->GetEdge(triangleEdgeIndex);

      IntersectionType lineLineSegmentIntersectionType = line.LineSegmentIntersection(triangleEdge, lineLineSegmentIntersectionPoint1, lineLineSegmentIntersectionPoint2);

      if (lineLineSegmentIntersectionType == IntersectionType::Point)
      {
         if (hasIntersection)
         {
            if (!ApproximatelyEqual(lineLineSegmentIntersectionPoint1, p1))
            {
               p2 = lineLineSegmentIntersectionPoint1;

               hasSegmentIntersection = true;
               break;
            }
         }
         else
         {
            p1 = lineLineSegmentIntersectionPoint1;
            hasIntersection = true;
         }
      }
      else if (lineLineSegmentIntersectionType == IntersectionType::LineSegment)
      {
         intersectionPoints.resize(2);
         intersectionPoints[0] = lineLineSegmentIntersectionPoint1; 
         intersectionPoints[1] = lineLineSegmentIntersectionPoint2;

         return IntersectionType::LineSegment;
      }
   }

   if (hasIntersection)
   {
      if (hasSegmentIntersection)
      {
         intersectionPoints.resize(2);
         intersectionPoints[0] = p1; 
         intersectionPoints[1] = p2;

         return IntersectionType::LineSegment;
      }
      else
      {
         intersectionPoints.resize(1);
         intersectionPoints[0] = p1;

         return IntersectionType::Point;
      }
   }
   else
   {
      return IntersectionType::None;
   }
}

template <>
IntersectionType Triangle<FVector3>::LineIntersection(const Line<FVector3>& line, std::vector<FVector3>& intersectionPoints) const
{
   FVector3 intersectionPoint;
   Plane::IntersectionQuery planeIntersectionQuery = GetPlane().getLineIntersection(line, intersectionPoint);

   if (planeIntersectionQuery == Plane::IntersectionQuery::Infinite)
   {
      return CoplanarLineIntersection(line, intersectionPoints);
   }
   else if (planeIntersectionQuery == Plane::IntersectionQuery::Intersects)
   {
      if (PointIsOnPolygon(intersectionPoint))
      {
         intersectionPoints.resize(1);
         intersectionPoints[0] = intersectionPoint;

         return IntersectionType::Point;
      }
      else
      {
         return IntersectionType::None;
      }
   }
   else
   {
      return IntersectionType::None;
   }
}

template <>
IntersectionType Triangle<FVector2>::LineIntersection(const Line<FVector2>& line, std::vector<FVector2>& intersectionPoints) const
{
   return CoplanarLineIntersection(line, intersectionPoints);
}

template <class PointType>
IntersectionType Triangle<PointType>::CoplanarLineSegmentIntersection(const LineSegment<PointType>& lineSegment, std::vector<PointType>& intersectionPoints) const
{
   PointType p1;
   PointType p2;
   bool hasIntersection = false;
   bool hasSegmentIntersection = false;

   for (std::size_t triangleEdgeIndex = 0; triangleEdgeIndex < Triangle::NumPointsOnATriangle; ++triangleEdgeIndex)
   {
      LineSegment<PointType> triangleEdge = this->GetEdge(triangleEdgeIndex);

      PointType lineSegmentLineSegmentIntersectionPoint1;
      PointType lineSegmentLineSegmentIntersectionPoint2;
      IntersectionType lineSegmentLineSegmentIntersectionType = lineSegment.GetLineSegmentIntersection(triangleEdge, lineSegmentLineSegmentIntersectionPoint1, lineSegmentLineSegmentIntersectionPoint2);

      if (lineSegmentLineSegmentIntersectionType == IntersectionType::Point)
      {
         if (hasIntersection)
         {
            if (!ApproximatelyEqual(lineSegmentLineSegmentIntersectionPoint1, p1))
            {
               p2 = lineSegmentLineSegmentIntersectionPoint1;

               hasSegmentIntersection = true;
               break;
            }
         }
         else
         {
            p1 = lineSegmentLineSegmentIntersectionPoint1;
            hasIntersection = true;
         }
      }
      else if (lineSegmentLineSegmentIntersectionType == IntersectionType::LineSegment)
      {
         intersectionPoints.resize(2);
         intersectionPoints[0] = lineSegmentLineSegmentIntersectionPoint1; 
         intersectionPoints[1] = lineSegmentLineSegmentIntersectionPoint2;

         return IntersectionType::LineSegment;
      }
   }

   if (hasIntersection)
   {
      if (hasSegmentIntersection)
      {
         intersectionPoints.resize(2);
         intersectionPoints[0] = p1; 
         intersectionPoints[1] = p2;

         return IntersectionType::LineSegment;
      }
      else
      {
         intersectionPoints.resize(1);
         intersectionPoints[0] = p1;

         return IntersectionType::Point;
      }
   }
   else
   {
      return IntersectionType::None;
   }
}

template <>
IntersectionType Triangle<FVector3>::LineSegmentIntersection(const LineSegment<FVector3>& lineSegment, std::vector<FVector3>& intersectionPoints) const
{
   FVector3 intersectionPoint;
   Plane::IntersectionQuery planeIntersectionQuery = GetPlane().getLineSegmentIntersection(lineSegment, intersectionPoint);

   if (planeIntersectionQuery == Plane::IntersectionQuery::Intersects)
   {
      if (PointIsOnPolygon(intersectionPoint))
      {
         intersectionPoints.resize(1);
         intersectionPoints[0] = intersectionPoint;

         return IntersectionType::Point;
      }
   }
   else if (planeIntersectionQuery == Plane::IntersectionQuery::Infinite)
   {
      return CoplanarLineSegmentIntersection(lineSegment, intersectionPoints);
   }

   return IntersectionType::None;
}

template <>
IntersectionType Triangle<FVector2>::LineSegmentIntersection(const LineSegment<FVector2>& lineSegment, std::vector<FVector2>& intersectionPoints) const
{
   return CoplanarLineSegmentIntersection(lineSegment, intersectionPoints);
}

template class LOCUS_GEOMETRY_API_AT_DEFINITION Triangle<FVector3>;
template class LOCUS_GEOMETRY_API_AT_DEFINITION Triangle<FVector2>;

}