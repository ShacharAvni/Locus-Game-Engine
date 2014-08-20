/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Geometry/LineSegment.h"

#include "Locus/Common/Float.h"

#include <algorithm>

namespace Locus
{

template <>
LineSegment<Vector3>::LineSegment()
   : P1(0, 0, 0), P2(1, 0, 0) //avoiding degenerate case
{
}

template <>
LineSegment<Vector2>::LineSegment()
   : P1(0, 0), P2(1, 0) //avoiding degenerate case
{
}

template <class PointType>
LineSegment<PointType>::LineSegment(const PointType& P1, const PointType& P2)
   : P1(P1), P2(P2)
{
}

template <class PointType>
bool LineSegment<PointType>::IsDegenerate() const
{
   return P1 == P2;
}

template <class PointType>
Line<PointType> LineSegment<PointType>::MakeLine(bool isRay) const
{
   return Line<PointType>(P1, Vector(), isRay);
}

template <class PointType>
PointType LineSegment<PointType>::Vector() const
{
   return P2 - P1;
}

template <class PointType>
bool LineSegment<PointType>::PointIsOnLineSegment(const PointType& checkPoint, float toleranceFactor) const
{
   Vector3 checkVector = checkPoint - P1;
   Vector3 segmentVector = Vector();

   Vector3 checkCross = segmentVector.cross(checkVector);

   if (checkCross.ApproximatelyEqualTo(Vector3::ZeroVector(), toleranceFactor))
   {
      float norm = segmentVector.norm();
      segmentVector /= norm;

      float dot = checkVector.dot(segmentVector);

      return ( FGreaterOrEqual<float>(dot, 0.0f, toleranceFactor) && FLessOrEqual<float>(dot, norm, toleranceFactor) );
   }
   else
   {
      return false;
   }
}

template <class PointType>
IntersectionType LineSegment<PointType>::GetCollinearLineSegmentIntersection(const LineSegment<PointType>& otherLineSegment, PointType& pIntersection1, PointType& pIntersection2) const
{
   //get scalar projections of each point onto lineVector1 with p1Line1 as
   //the origin of projection. We can then simply check how the projections
   //overlap.

   PointType lineSegmentVector = Vector();
   PointType normVector = lineSegmentVector.normVector();

   float projP1Line1 = 0.0f;
   float projP2Line1 = lineSegmentVector.dot(normVector);

   float line2Projections[2];
   line2Projections[0] = (otherLineSegment.P1 - P1).dot(normVector);
   line2Projections[1] = (otherLineSegment.P2 - P1).dot(normVector);

   PointType lineSegment2Points[2] = {otherLineSegment.P1, otherLineSegment.P2};

   if (FGreater<float>(line2Projections[0], line2Projections[1]))
   {
      //other line segment is ordered in the opposite direction as this line segment

      std::swap(line2Projections[0], line2Projections[1]);
      std::swap(lineSegment2Points[0], lineSegment2Points[1]);
   }

   if (FLess<float>(line2Projections[1], projP1Line1) || FGreater<float>(line2Projections[0], projP2Line1))
   {
      return IntersectionType::None;
   }
   else
   {
      if (FLess<float>(line2Projections[0], projP1Line1))
      {
         if (FLessOrEqual<float>(line2Projections[1], projP2Line1))
         {
            pIntersection1 = P1;
            pIntersection2 = lineSegment2Points[1];
         }
         else
         {
            pIntersection1 = P1;
            pIntersection2 = P2;
         }
      }
      else
      {
         pIntersection1 = lineSegment2Points[0];

         if (FLessOrEqual<float>(line2Projections[1], projP2Line1))
         {
            pIntersection2 = lineSegment2Points[1];
         }
         else
         {
            pIntersection2 = P2;
         }
      }

      if (pIntersection1 == pIntersection2)
      {
         return IntersectionType::Point;
      }
      else
      {
         return IntersectionType::LineSegment;
      }
   }
}

template <class PointType>
bool LineSegment<PointType>::GetCollinearLineSegmentIntersection(const LineSegment<PointType>& otherLineSegment) const
{
   //get scalar projections of each point onto lineVector1 with p1Line1 as
   //the origin of projection. We can then simply check how the projections
   //overlap.

   PointType lineSegmentVector = Vector();
   PointType normVector = lineSegmentVector.normVector();

   float projP2Line1 = lineSegmentVector.dot(normVector);

   float projP1Line2 = (otherLineSegment.P1 - P1).dot(normVector);
   float projP2Line2 = (otherLineSegment.P2 - P1).dot(normVector);

   return ( (FGreaterOrEqual<float>(projP1Line2, 0.0f) && FLessOrEqual<float>(projP1Line2, projP2Line1)) ||
            (FGreaterOrEqual<float>(projP2Line2, 0.0f) && FLessOrEqual<float>(projP2Line2, projP2Line1)) );
}

template <class PointType>
IntersectionType LineSegment<PointType>::GetLineSegmentIntersection(const LineSegment<PointType>& otherLineSegment, PointType& pIntersection1, PointType& pIntersection2) const
{
   Line<PointType> L1(P1, Vector(), false);
   Line<PointType> L2(otherLineSegment.P1, otherLineSegment.Vector(), false);

   PointType intersectionPoint1, intersectionPoint2;
   IntersectionType lineLineIntersectionType = L1.GetLineIntersection(L2, intersectionPoint1, intersectionPoint2);

   if (lineLineIntersectionType == IntersectionType::Line)
   {
      return GetCollinearLineSegmentIntersection(otherLineSegment, pIntersection1, pIntersection2);
   }
   else if (lineLineIntersectionType == IntersectionType::Point)
   {
      if (PointIsOnLineSegment(intersectionPoint1) && otherLineSegment.PointIsOnLineSegment(intersectionPoint1))
      {
         pIntersection1 = intersectionPoint1;
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

template <class PointType>
bool LineSegment<PointType>::GetLineSegmentIntersection(const LineSegment<PointType>& otherLineSegment) const
{
   Line<PointType> L1(P1, Vector(), false);
   Line<PointType> L2(otherLineSegment.P1, otherLineSegment.Vector(), false);

   PointType intersectionPoint1, intersectionPoint2;
   IntersectionType lineLineIntersectionType = L1.GetLineIntersection(L2, intersectionPoint1, intersectionPoint2);

   if (lineLineIntersectionType == IntersectionType::Line)
   {
      return GetCollinearLineSegmentIntersection(otherLineSegment);
   }
   else if (lineLineIntersectionType == IntersectionType::Point)
   {
      if (PointIsOnLineSegment(intersectionPoint1) && otherLineSegment.PointIsOnLineSegment(intersectionPoint1))
      {
         return true;
      }
      else
      {
         return false;
      }
   }
   else
   {
      return false;
   }
}

template class LOCUS_GEOMETRY_API_AT_DEFINITION LineSegment<Vector3>;
template class LOCUS_GEOMETRY_API_AT_DEFINITION LineSegment<Vector2>;

}