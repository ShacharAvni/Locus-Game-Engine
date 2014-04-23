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

#include "Locus/Geometry/Plane.h"
#include "Locus/Geometry/Triangle.h"
#include "Locus/Geometry/LineSegment.h"

#include "Locus/Common/Float.h"

namespace Locus
{

Plane::Plane()
{
   setNormal( Vector3(0.0, 0.0, 1.0) );
}

Plane::Plane(const Vector3& p, const Vector3& n)
   : P(p)
{
   setNormal(n);
}

Plane::Plane(const Vector3& p, const Vector3& v1, const Vector3& v2)
   : P(p)
{
   setNormal(v1.cross(v2));
}

Vector3 Plane::getNormal() const
{
   return N;
}

void Plane::setNormal(const Vector3& n)
{
   N = n;

   //if (N.PreciselyEqualTo(Vector3::ZeroVector()))
   //{
      //throw std::runtime_error("Invalid plane normal");
   //}
}

void Plane::set(const Vector3& p, const Vector3& n)
{
   P = p;
   setNormal(n);
}

//////////////////////////////////////Plane Logic Functions//////////////////////////////////////////

float Plane::signedDistanceTo(const Vector3& v) const
{
   //computes signed distance to a particular Vector3 (with the Vector3
   //assumed to be in the same coordinate system as the plane's point P)
   return N.dot(v - P);
}

float Plane::signedDistanceTo(const Vector3& v, const Vector3& offset) const
{
   //computes signed distance to a particular Vector3. The offset value
   //places the plane in the same coordinate system as the Vector3, v
   return N.dot(v - (P + offset));
}

float Plane::distanceTo(const Vector3& v) const
{
   return signedDistanceTo(v) / N.norm();
}

bool Plane::pointIsOnPlane(const Vector3& p) const
{
   return Float::FIsZero<float>( signedDistanceTo(p) );
}

Vector3 Plane::getProjection(const Vector3& p) const
{
   Vector3 projection = p - (signedDistanceTo(p) * N);
   return projection;
}

bool Plane::intersectsLineAtOnePoint(const Line3D_t& line, float& s) const
{
   float denominator = N.dot(line.V);

   if (Float::FNotZero<float>(denominator))
   {
      s = N.dot(P - line.P)/denominator;

      if (line.isRay)
      {
         return Float::FGreaterOrEqual<float>(s, 0.0f);
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
   return N.cross(otherPlane.N) == Vector3::ZeroVector();
}

bool Plane::isCoplanarTo(const Plane& otherPlane) const
{
   return isParallelTo(otherPlane) && pointIsOnPlane(otherPlane.P);
}

Plane::IntersectionQuery Plane::getLineSegmentIntersection(const LineSegment3D_t& lineSegment, Vector3& intersectionPoint) const
{
   float s = 0.0f;

   Line3D_t line = lineSegment.MakeLine(false);

   if (intersectsLineAtOnePoint(line, s))
   {
      if (Float::FGreaterOrEqual<float>(s, 0.0f) && Float::FLessOrEqual<float>(s, 1.0f))
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

Plane::IntersectionQuery Plane::getLineIntersection(const Line3D_t& line, Vector3& intersectionPoint) const
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

      if (Float::FGreater<float>(distance, 0.0f))
      {
         allOnNegativeSide = false;
         if (!allOnPositiveSide)
         {
            return IntersectionQuery::None;
         }
      }
      else if (Float::FLess<float>(distance, 0.0f))
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

IntersectionType Plane::getPlaneIntersection(const Plane& otherPlane, Vector3& pointOnLine, Vector3& lineVector) const
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
      lineVector = N.cross(otherPlane.N);

      //Find D1 in the equation a1x + b1y + c1z - D1 = 0
      //where (a1, b1, c1) = (N.x, N.y, N.z)

      Vector3 origin;

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

      if (Float::FNotZero<float>(denominator))
      {
         pointOnLine.z = 0.0f;

         pointOnLine.y = (D2 * N.x - D1 * otherPlane.N.x) / denominator;

         if (Float::FIsZero<float>(N.x))
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

         if (Float::FNotZero<float>(denominator))
         {
            pointOnLine.y = 0.0f;

            pointOnLine.z = (D2 * N.x - otherPlane.N.x * D1) / denominator;

            if (Float::FIsZero<float>(N.x))
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

            if (Float::FIsZero<float>(N.y))
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

IntersectionType Plane::TriangleIntersection(const Triangle3D_t& triangle, Vector3& p1, Vector3& p2) const
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

      p1 = Vector3::ZeroVector();
      p2 = Vector3::ZeroVector();

      for (std::size_t triangleEdgeIndex = 0; triangleEdgeIndex < Triangle3D_t::NumPointsOnATriangle; ++triangleEdgeIndex)
      {
         LineSegment3D_t triangleEdge = triangle.GetEdge(triangleEdgeIndex);

         Vector3 intersectionPoint;
         Plane::IntersectionQuery intersectionQuery = getLineSegmentIntersection(triangleEdge, intersectionPoint);

         if (intersectionQuery == Plane::IntersectionQuery::Intersects)
         {
            hasIntersection = true;

            if (p1Set)
            {
               p2 = intersectionPoint;

               if (p2 != p1)
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
         if (p1 == p2)
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