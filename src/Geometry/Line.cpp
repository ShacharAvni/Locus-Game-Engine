/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Geometry/Line.h"
#include "Locus/Geometry/LineSegment.h"
#include "Locus/Geometry/Vector3Geometry.h"

#include "Locus/Common/Float.h"

#include <cassert>

namespace Locus
{

template <>
Line<FVector3>::Line()
   : V(1, 0, 0), isRay(false) //avoiding degenerate case by setting the line initially to the X axis.
{
}

template <>
Line<FVector2>::Line()
   : V(1, 0), isRay(false)  //avoiding degenerate case by setting the line initially to the X axis.
{
}

template <class PointType>
Line<PointType>::Line(const PointType& pointOnLine, const PointType& vectorInDirectionOfLine, bool isRay)
   : P(pointOnLine), V(vectorInDirectionOfLine), isRay(isRay)
{
}

template <>
bool Line<FVector2>::IsDegenerate() const
{
   return ( (V.x == 0.0f) && (V.y == 0.0f) );
}

template <>
bool Line<FVector3>::IsDegenerate() const
{
   return ( (V.x == 0.0f) && (V.y == 0.0f) && (V.z == 0.0f) );
}

template <class PointType>
PointType Line<PointType>::GetPointOnLine(float s) const
{
   if (isRay)
   {
      if (FLess<float>(s, 0.0f))
      {
         return P;
      }
   }

   return P + V * s;
}

template <class PointType>
bool Line<PointType>::IsPointOnLine(const PointType& checkPoint, float toleranceFactor) const
{
   bool isOnLine = ApproximatelyEqual(Cross(V, checkPoint - P), Vec3D::ZeroVector(), toleranceFactor);

   if (isRay)
   {
      isOnLine = isOnLine && FGreaterOrEqual<float>( Dot(checkPoint - P, V), 0.0f, toleranceFactor );
   }

   return isOnLine;
}

template <>
IntersectionType Line<FVector3>::GetLineIntersection(const Line<FVector3>& otherLine, FVector3& intersectionPoint1, FVector3& intersectionPoint2, float toleranceFactor) const
{
   if (IsDegenerate() || otherLine.IsDegenerate())
   {
      return IntersectionType::None;
   }

   //from http://geomalgorithms.com/a07-_distance.html

   FVector3 wZero = P - otherLine.P;

   float a = Dot(V, V);
   float b = Dot(V, otherLine.V);
   float c = Dot(otherLine.V, otherLine.V);
   float d = Dot(V, wZero);
   float e = Dot(otherLine.V, wZero);

   float ACMinusBSquared = a * c - b * b;

   if (ACMinusBSquared == 0.0f)
   {
      return GetParallelLineIntersection(otherLine, intersectionPoint1, intersectionPoint2, toleranceFactor);
   }

   FVector3 closestPointOnThisLine = P + ( (b * e - c * d) / ACMinusBSquared ) * V;
   FVector3 closestPointOnOtherLine = otherLine.P + ( (a * e - b * d) / ACMinusBSquared ) * otherLine.V;

   if (FEqual<float>(DistanceBetween(closestPointOnThisLine, closestPointOnOtherLine), 0.0f, toleranceFactor))
   {
      if (isRay || otherLine.isRay)
      {
         if (isRay && otherLine.isRay)
         {
            if (!IsPointOnLine(closestPointOnThisLine, toleranceFactor) || !otherLine.IsPointOnLine(closestPointOnThisLine, toleranceFactor))
            {
               return IntersectionType::None;
            }
         }
         else if (isRay)
         {
            if (!IsPointOnLine(closestPointOnThisLine, toleranceFactor))
            {
               return IntersectionType::None;
            }
         }
         else
         {
            if (!otherLine.IsPointOnLine(closestPointOnThisLine, toleranceFactor))
            {
               return IntersectionType::None;
            }
         }
      }

      intersectionPoint1 = closestPointOnThisLine;
      return IntersectionType::Point;
   }
   
   return IntersectionType::None;
}

template <>
IntersectionType Line<FVector2>::GetLineIntersection(const Line<FVector2>& otherLine, FVector2& intersectionPoint1, FVector2& intersectionPoint2, float toleranceFactor) const
{
   //re-use 3D solution

   Line<FVector3> thisLine3D(P, V, isRay);
   Line<FVector3> otherLine3D(otherLine.P, otherLine.V, otherLine.isRay);

   FVector3 intersectionPoint3D1, intersectionPoint3D2;
   IntersectionType lineIntersection = thisLine3D.GetLineIntersection(otherLine3D, intersectionPoint3D1, intersectionPoint3D2, toleranceFactor);

   if (lineIntersection != IntersectionType::None)
   {
      intersectionPoint1.x = intersectionPoint3D1.x;
      intersectionPoint1.y = intersectionPoint3D1.y;

      if (lineIntersection == IntersectionType::LineSegment)
      {
         intersectionPoint2.x = intersectionPoint3D2.x;
         intersectionPoint2.y = intersectionPoint3D2.y;
      }
   }
      
   return lineIntersection;
}

template <class PointType>
IntersectionType Line<PointType>::GetParallelLineIntersection(const Line<PointType>& otherLine, PointType& intersectionPoint1, PointType& intersectionPoint2, float toleranceFactor) const
{
   if (isRay)
   {
      if (otherLine.isRay)
      {
         bool thisPointIsOnOther = otherLine.IsPointOnLine(P, toleranceFactor);
         bool otherPointIsOnThis = IsPointOnLine(otherLine.P, toleranceFactor);

         if (thisPointIsOnOther || otherPointIsOnThis)
         {
            if (ApproximatelyEqual(P, otherLine.P, toleranceFactor))
            {
               if (GoTheSameWay(V, otherLine.V))
               {
                  return IntersectionType::Ray;
               }
               else
               {
                  intersectionPoint1 = P;

                  return IntersectionType::Point;
               }
            }
            else
            {
               if (thisPointIsOnOther && otherPointIsOnThis)
               {
                  intersectionPoint1 = P;
                  intersectionPoint2 = otherLine.P;

                  return IntersectionType::LineSegment;
               }
               else if (thisPointIsOnOther)
               {
                  //TODO: which ray?
                  //this ray
                  return IntersectionType::Ray;
               }
               else
               {
                  //TODO: which ray?
                  //other ray
                  return IntersectionType::Ray;
               }
            }
         }
         else
         {
            return IntersectionType::None;
         }
      }
      else
      {
         if (otherLine.IsPointOnLine(P, toleranceFactor))
         {
            //TODO: which ray?
            //this ray
            return IntersectionType::Ray;
         }
         else
         {
            return IntersectionType::None;
         }
      }
   }
   else if (otherLine.isRay)
   {
      if (IsPointOnLine(otherLine.P, toleranceFactor))
      {
         //TODO: which ray?
         //other ray
         return IntersectionType::Ray;
      }
      else
      {
         return IntersectionType::None;
      }
   }
   else
   {
      if (otherLine.IsPointOnLine(P, toleranceFactor))
      {
         //lines are coincident
         return IntersectionType::Line;
      }
      else
      {
         return IntersectionType::None;
      }
   }
}

template <class PointType>
IntersectionType Line<PointType>::LineSegmentIntersection(const LineSegment<PointType>& lineSegment, PointType& intersectionPoint1, PointType& intersectionPoint2, float toleranceFactor) const
{
   Line<PointType> lineSegmentAsCompleteLine = lineSegment.MakeLine(false);

   PointType lineLineIntersectionPoint1, lineLineIntersectionPoint2;
   IntersectionType lineLineIntersectionType = GetLineIntersection(lineSegmentAsCompleteLine, lineLineIntersectionPoint1, lineLineIntersectionPoint2, toleranceFactor);

   if (lineLineIntersectionType == IntersectionType::Point)
   {
      if (lineSegment.PointIsOnLineSegment(lineLineIntersectionPoint1, toleranceFactor))
      {
         intersectionPoint1 = lineLineIntersectionPoint1;
         return IntersectionType::Point;
      }
      else
      {
         return IntersectionType::None;
      }
   }
   else if ( (lineLineIntersectionType == IntersectionType::Line) || (lineLineIntersectionType == IntersectionType::Ray) )
   {
      return GetParallelLineSegmentIntersection(lineSegment, intersectionPoint1, intersectionPoint2, toleranceFactor);
   }
   else if (lineLineIntersectionType == IntersectionType::LineSegment)
   {
      intersectionPoint1 = lineLineIntersectionPoint1;
      intersectionPoint2 = lineLineIntersectionPoint2;

      return IntersectionType::LineSegment;
   }
   else
   {
      return IntersectionType::None;
   }
}

template <class PointType>
IntersectionType Line<PointType>::GetParallelLineSegmentIntersection(const LineSegment<PointType>& lineSegment, PointType& intersectionPoint1, PointType& intersectionPoint2, float toleranceFactor) const
{
   if (isRay)
   {
      bool firstPointIsOn = IsPointOnLine(lineSegment.P1, toleranceFactor);
      bool secondPointIsOn = IsPointOnLine(lineSegment.P2, toleranceFactor);

      if (firstPointIsOn || secondPointIsOn)
      {
         if (firstPointIsOn && secondPointIsOn)
         {
            intersectionPoint1 = lineSegment.P1;
            intersectionPoint2 = lineSegment.P2;
         }
         else if (firstPointIsOn)
         {
            intersectionPoint1 = lineSegment.P1;
            intersectionPoint2 = P;
         }
         else
         {
            intersectionPoint1 = lineSegment.P2;
            intersectionPoint2 = P;
         }

         return IntersectionType::LineSegment;
      }
      else
      {
         return IntersectionType::None;
      }
   }
   else
   {
      intersectionPoint1 = lineSegment.P1;
      intersectionPoint2 = lineSegment.P2;

      return IntersectionType::LineSegment;
   } 
}

template class LOCUS_GEOMETRY_API_AT_DEFINITION Line<FVector3>;
template class LOCUS_GEOMETRY_API_AT_DEFINITION Line<FVector2>;

}
