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

#include "LineSegmentFwd.h"
#include "IntersectionTypes.h"

#include "Locus/Math/Vectors.h"

#include <type_traits>

namespace Locus
{

/*!
 * \brief A line or ray.
 *
 * \note Supported template parameters are Vector2
 * and Vector3. If Vector2 is passed then this is
 * a 2D line (or ray). If Vector3 is passed then this
 * is a 3D line (or ray).
 */
template <class PointType>
class LOCUS_GEOMETRY_API Line
{
public:
   static_assert(std::is_same<PointType, FVector2>::value || std::is_same<PointType, FVector3>::value, "PointType must be FVector3 or FVector2");

   /// Initialized as the X axis.
   Line();

   /*!
    * \param[in] pointOnLine A point on the line (or the starting
    * point of the ray).
    *
    * \param[in] vectorInDirectionOfLine A vector in the direction
    * of the line (or ray). It does not need to be normalized.
    *
    * \param[in] isRay If true, then this is a ray. Otherwise, this
    * is a line.
    */
   Line(const PointType& pointOnLine, const PointType& vectorInDirectionOfLine, bool isRay);

   /// \return true if the vector in the direction of the line (or ray) is the zero vector.
   bool IsDegenerate() const;

   /*!
    * \return The result of P + sV where P is a point on the line
    * (or the starting point of the ray) and V is a vector in the
    * direction of the line (or ray).
    *
    * \param[in] s The factor s in P + sV. If this is a ray, then
    * a negative s will result in the starting point of the ray.
    */
   PointType GetPointOnLine(float s) const;

   /*!
    * \return true if the given point is on the line (or ray).
    *
    * \param[in] checkPoint The point we're querying if it is on
    * the line (or ray).
    *
    * \param[in] toleranceFactor The tolerance factor used for any
    * underlying floating point comparisons.
    *
    * \sa Tolerance
    */
   bool IsPointOnLine(const PointType& checkPoint, float toleranceFactor = 1) const;

   /*!
    * \brief Query for intersection between this line (or ray) and another
    * line (or ray).
    *
    * \param[in] otherLine The line (or ray) we're querying for intersection
    * with this line (or ray).
    *
    * \param[in] intersectionPoint1 If the return value is Point, then this is
    * the point of intersection. If the return value is LineSegment, then this is
    * one endpoint of the line segment of intersection.
    *
    * \param[in] intersectionPoint2 If the return value is LineSegment, then this
    * is one endpoint of the line segment of intersection.
    *
    * \param[in] toleranceFactor The tolerance factor used for any
    * underlying floating point comparisons.
    *
    * \return The result of the intersection. The result is either None, Point,
    * Line, LineSegment, or Ray. If this is Point or LineSegment, then intersectionPoint1
    * and intersectionPoint2 will contain the intersecting geometry. If the result is Line
    * then this line and the given line must be coincident. If the result is Ray, then this
    * ray and the given ray were concident or either this object or the parameter were a line
    * and the other was a ray and it was coincident on the line.
    *
    * \sa Tolerance
    */
   IntersectionType GetLineIntersection(const Line<PointType>& otherLine, PointType& intersectionPoint1, PointType& intersectionPoint2, float toleranceFactor = 1) const;

   /*!
    * \brief Query for intersection between this line (or ray) and another
    * line (or ray) parallel to this one.
    *
    * \param[in] otherLine The line (or ray) we're querying for intersection
    * with this line (or ray). It must be parallel to this line (or ray).
    *
    * \param[in] intersectionPoint1 If the return value is LineSegment, then
    * this is one endpoint of the line segment of intersection.
    *
    * \param[in] intersectionPoint2 If the return value is LineSegment, then
    * this is one endpoint of the line segment of intersection.
    *
    * \param[in] toleranceFactor The tolerance factor used for any
    * underlying floating point comparisons.
    *
    * \return The result of the intersection. The result is either None, Line, LineSegment,
    * or Ray. If this is LineSegment, then intersectionPoint1 and intersectionPoint2 will
    * contain the line segment of intersection. If the result is Line then this line and the
    * given line must be coincident. If the result is Ray, then this ray and the given ray
    * were concident or either this object or the parameter were a line and the other was a
    * ray and it was coincident on the line.
    *
    * \sa Tolerance
    */
   IntersectionType GetParallelLineIntersection(const Line<PointType>& otherLine, PointType& intersectionPoint1, PointType& intersectionPoint2, float toleranceFactor = 1) const;

   /*!
    * \brief Query for intersection between this line (or ray) and a
    * line segment.
    *
    * \param[in] lineSegment The line segment we're querying for intersection
    * with this line (or ray).
    *
    * \param[in] intersectionPoint1 If the return value is Point, then this is
    * the point of intersection. If the return value is LineSegment, then this is
    * one endpoint of the line segment of intersection.
    *
    * \param[in] intersectionPoint2 If the return value is LineSegment, then
    * this is one endpoint of the line segment of intersection.
    *
    * \param[in] toleranceFactor The tolerance factor used for any
    * underlying floating point comparisons.
    *
    * \return The result of the intersection. The result is either None, Point,
    * or LineSegment. If this is Point or LineSegment, then intersectionPoint1
    * and intersectionPoint2 will contain the intersecting geometry.
    *
    * \sa Tolerance
    */
   IntersectionType LineSegmentIntersection(const LineSegment<PointType>& lineSegment, PointType& intersectionPoint1, PointType& intersectionPoint2, float toleranceFactor = 1) const;

   /*!
    * \brief Query for intersection between this line (or ray) and a
    * line segment that is parallel to this line (or ray).
    *
    * \param[in] lineSegment The line segment we're querying for intersection
    * with this line (or ray). It must be parallel to this line (or ray).
    *
    * \sa LineSegmentIntersection
    */
   IntersectionType GetParallelLineSegmentIntersection(const LineSegment<PointType>& lineSegment, PointType& intersectionPoint1, PointType& intersectionPoint2, float toleranceFactor = 1) const;

   /// A point on the line (or the starting point of the ray).
   PointType P;

   /// A vector in the direction of the (or ray). It does not need to be normalized.
   PointType V;

   /// If true then this is a ray. Otherwise, it is a line.
   bool isRay;
};

#ifdef LOCUS_GEOMETRY_SHARED_IMPORTS

extern template class LOCUS_SHARED_IMPORTS Line<FVector3>;
extern template class LOCUS_SHARED_IMPORTS Line<FVector2>;

#endif

}