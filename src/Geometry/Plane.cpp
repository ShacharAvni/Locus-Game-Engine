/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Geometry/Plane.h"
#include "Locus/Geometry/Triangle.h"
#include "Locus/Geometry/LineSegment.h"

#include "Locus/Common/Float.h"

namespace Locus
{

Plane::Plane()
{
   setNormal( FVector3(0.0, 0.0, 1.0) );
}

Plane::Plane(const FVector3& p, const FVector3& n)
   : P(p)
{
   setNormal(n);
}

Plane::Plane(const FVector3& p, const FVector3& v1, const FVector3& v2)
   : P(p)
{
   setNormal(Cross(v1, v2));
}

FVector3 Plane::getNormal() const
{
   return N;
}

void Plane::setNormal(const FVector3& n)
{
   N = n;

   //if (N.PreciselyEqualTo(Vector3::ZeroVector()))
   //{
      //throw std::runtime_error("Invalid plane normal");
   //}
}

void Plane::set(const FVector3& p, const FVector3& n)
{
   P = p;
   setNormal(n);
}

//////////////////////////////////////Plane Logic Functions//////////////////////////////////////////

float Plane::signedDistanceTo(const FVector3& v) const
{
   //computes signed distance to a particular Vector3 (with the Vector3
   //assumed to be in the same coordinate system as the plane's point P)
   return Dot(N, v - P);
}

float Plane::signedDistanceTo(const FVector3& v, const FVector3& offset) const
{
   //computes signed distance to a particular Vector3. The offset value
   //places the plane in the same coordinate system as the Vector3, v
   return Dot(N, v - (P + offset));
}

float Plane::distanceTo(const FVector3& v) const
{
   return signedDistanceTo(v) / Norm(N);
}

bool Plane::pointIsOnPlane(const FVector3& p) const
{
   return FIsZero<float>( signedDistanceTo(p) );
}

FVector3 Plane::getProjection(const FVector3& p) const
{
   FVector3 projection = p - (signedDistanceTo(p) * N);
   return projection;
}

bool Plane::intersectsLineAtOnePoint(const Line3D_t& line, float& s) const
{
   float denominator = Dot(N, line.V);

   if (FNotZero<float>(denominator))
   {
      s = Dot(N, P - line.P)/denominator;

      if (line.isRay)
      {
         return FGreaterOrEqual<float>(s, 0.0f);
      }
      else
      {
         return true;
      }
   }
   else
   {
      return false;
   }
}

bool Plane::isParallelTo(const Plane& otherPlane) const
{
   return ApproximatelyEqual(Cross(N, otherPlane.N), Vec3D::ZeroVector());
}

bool Plane::isCoplanarTo(const Plane& otherPlane) const
{
   return isParallelTo(otherPlane) && pointIsOnPlane(otherPlane.P);
}

Plane::IntersectionQuery Plane::getLineSegmentIntersection(const LineSegment3D_t& lineSegment, FVector3& intersectionPoint) const
{
   float s = 0.0f;

   Line3D_t line = lineSegment.MakeLine(false);

   if (intersectsLineAtOnePoint(line, s))
   {
      if (FGreaterOrEqual<float>(s, 0.0f) && FLessOrEqual<float>(s, 1.0f))
      {
         intersectionPoint = line.GetPointOnLine(s);
         return IntersectionQuery::Intersects;
      }
   }
   else if (pointIsOnPlane(lineSegment.P1))
   {
      return IntersectionQuery::Infinite;
   }

   return IntersectionQuery::None;
}

Plane::IntersectionQuery Plane::getLineIntersection(const Line3D_t& line, FVector3& intersectionPoint) const
{
   float s = 0.0f;

   if (intersectsLineAtOnePoint(line, s))
   {
      intersectionPoint = line.GetPointOnLine(s);
      return IntersectionQuery::Intersects;
   }
   else if (pointIsOnPlane(line.P))
   {
      return IntersectionQuery::Infinite;
   }
   else
   {
      return IntersectionQuery::None;
   }
}

Plane::IntersectionQuery Plane::triangleIntersectionTest(const Triangle3D_t& triangle) const
{
   //Intersection_None means at least one point is on the positive side of the plane and at least
   //one point is on the negative side of the plane
   //Intersection_Positive means all points are on the positive side of the plane
   //Intersection_Negative means all points are on the negative side of the plane
   //Intersection_Intersects means all points are on the plane

   bool allOnPositiveSide = true;
   bool allOnNegativeSide = true;

   for (std::size_t pointIndex = 0; pointIndex < Triangle3D_t::NumPointsOnATriangle; ++pointIndex)
   {
      float distance = signedDistanceTo(triangle[pointIndex]);

      if (FGreater<float>(distance, 0.0f))
      {
         allOnNegativeSide = false;
         if (!allOnPositiveSide)
         {
            return IntersectionQuery::None;
         }
      }
      else if (FLess<float>(distance, 0.0f))
      {
         allOnPositiveSide = false;
         if (!allOnNegativeSide)
         {
            return IntersectionQuery::None;
         }
      }
   }

   if (allOnPositiveSide && allOnNegativeSide)
   {
      return IntersectionQuery::Intersects;
   }
   else if (allOnPositiveSide)
   {
      return IntersectionQuery::Positive;
   }
   else
   {
      return IntersectionQuery::Negative;
   }
}

IntersectionType Plane::getPlaneIntersection(const Plane& otherPlane, FVector3& pointOnLine, FVector3& lineVector) const
{
   if (isParallelTo(otherPlane))
   {
      if (pointIsOnPlane(otherPlane.P))
      {
         return IntersectionType::Plane;
      }
      else
      {
         return IntersectionType::None;
      }
   }
   else
   {
      lineVector = Cross(N, otherPlane.N);

      //Find D1 in the equation a1x + b1y + c1z - D1 = 0
      //where (a1, b1, c1) = (N.x, N.y, N.z)

      FVector3 origin;

      float D1 = distanceTo(origin);

      //Find D2 in the equation a2x + b2y + c2z - D2 = 0
      //where (a2, b2, c2) = (other.N.x, other.N.y, other.N.z)

      float D2 = otherPlane.distanceTo(origin);

      //now we need to find a point on both planes, i.e. a point that satisfies the following two equations
      //a1x + b1y + c1z - D1 = 0 (1)
      //a2x + b2y + c2z - D2 = 0 (2)

      //if we set z = 0,
      //then, from (1) x = (D1 - b1*y) / a1
      //and substituting into (2),
      //y = (D2 * a1 - D1 * a2) / (b2 * a1 - a2 * b1)
      //but this will only work if (b2 * a1 - a2 * b1) is not zero

      float denominator = (otherPlane.N.y * N.x - otherPlane.N.x * N.y);

      if (FNotZero<float>(denominator))
      {
         pointOnLine.z = 0.0f;

         pointOnLine.y = (D2 * N.x - D1 * otherPlane.N.x) / denominator;

         if (FIsZero<float>(N.x))
         {
            //there are infinite possibilities for x in this case
            pointOnLine.x = 0.0f;
         }
         else
         {
            pointOnLine.x = (D1 - N.y * pointOnLine.y) / N.x;
         }
      }
      else
      {
         //if we set y = 0,
         //then, from (1) x = (D1 - c1*z) / a1
         //and substituting into (2),
         //z = (D2 * a1 - a2 * D1) / (c2 * a1 - a2 * c1)
         //but this will only work if (c2 * a1 - a2 * c1) is not zero

         denominator = (otherPlane.N.z * N.x - otherPlane.N.x * N.z);

         if (FNotZero<float>(denominator))
         {
            pointOnLine.y = 0.0f;

            pointOnLine.z = (D2 * N.x - otherPlane.N.x * D1) / denominator;

            if (FIsZero<float>(N.x))
            {
               //there are infinite possibilities for x in this case
               pointOnLine.x = 0.0f;
            }
            else
            {
               pointOnLine.x = (D1 - N.z * pointOnLine.z) / N.x;
            }
         }
         else
         {
            //if we set x = 0,
            //then, from (1) y = (D1 - c1*z) / b1
            //and substituting into (2),
            //z = (D2 * b1 - b2 * D1) / (c2 * b1 - b2 * c1)
            //but this will only work if (c2 * b1 - b2 * c1) is not zero
            //
            //This is the last possible substitution and we have controlled
            //for the case where a plane's normal is the zero vector (the
            //case that would make this last substitution impossible). Thus,
            //this substitution is guaranteed to work

            pointOnLine.x = 0.0f;

            denominator = (otherPlane.N.z * N.y - otherPlane.N.y * N.z);

            pointOnLine.z = (D2 * N.y - otherPlane.N.y * D1) / denominator;

            if (FIsZero<float>(N.y))
            {
               //there are infinite possibilities for y in this case
               pointOnLine.y = 0.0f;
            }
            else
            {
               pointOnLine.y = (D1 - N.z * pointOnLine.z) / N.y;
            }
         }
      }
         
      return IntersectionType::Line;
   }
}

IntersectionType Plane::getPlaneIntersection(const Plane& otherPlane) const
{
   if (isParallelTo(otherPlane))
   {
      if (pointIsOnPlane(otherPlane.P))
      {
         return IntersectionType::Plane;
      }
      else
      {
         return IntersectionType::None;
      }
   }
   else
   {
      return IntersectionType::Line;
   }
}

IntersectionType Plane::TriangleIntersection(const Triangle3D_t& triangle, FVector3& p1, FVector3& p2) const
{
   IntersectionType planeIntersectionType = getPlaneIntersection(triangle.GetPlane());

   if (planeIntersectionType == IntersectionType::Plane)
   {
      //triangle is on the plane. Intersection is the triangle itself
      return IntersectionType::Area;
   }
   else if (planeIntersectionType == IntersectionType::Line)
   {
      bool hasIntersection = false;
      bool p1Set = false;

      p1 = Vec3D::ZeroVector();
      p2 = Vec3D::ZeroVector();

      for (std::size_t triangleEdgeIndex = 0; triangleEdgeIndex < Triangle3D_t::NumPointsOnATriangle; ++triangleEdgeIndex)
      {
         LineSegment3D_t triangleEdge = triangle.GetEdge(triangleEdgeIndex);

         FVector3 intersectionPoint;
         Plane::IntersectionQuery intersectionQuery = getLineSegmentIntersection(triangleEdge, intersectionPoint);

         if (intersectionQuery == Plane::IntersectionQuery::Intersects)
         {
            hasIntersection = true;

            if (p1Set)
            {
               p2 = intersectionPoint;

               if (!ApproximatelyEqual(p2, p1))
               {
                  break;
               }
            }
            else
            {
               p1 = intersectionPoint;
               p1Set = true;
            }
         }
         else if (intersectionQuery == Plane::IntersectionQuery::Infinite)
         {
            hasIntersection = true;

            p1 = triangleEdge.P1;
            p2 = triangleEdge.P2;

            break;
         }
      }

      if (hasIntersection)
      {
         if (ApproximatelyEqual(p1, p2))
         {
            return IntersectionType::Point;
         }
         else
         {
            return IntersectionType::LineSegment;
         }
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

}