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

namespace Locus
{

/// Possible results of object intersection queries.
enum class IntersectionType
{
   None = 0,

   Point, ///< The objects intersect at a point.

   Line, ///< The object intersect on a line. For instance,
         ///< two non parallel planes.

   LineSegment, ///< The objects intersect on a line segment. For instance,
                ///< the ray (R = P + sT) R1 with P = (0, 0, 0) and T = (1, 0, 0),
                ///< and the ray R2 with P = (1, 0, 0) and T = (-1, 0, 0).

   Ray, ///< The objects intersect on a ray. For instance, a line on the
        ///< X axis and the ray (R = P + sT) with P = (0, 0, 0) and T =
        ///< (1, 0, 0) intersect on the ray itself.

   Plane, ///< The objects intersect on a plane. For instance, two identical
          ///< planes.

   Area ///< The objects intersect on an area. For instance, a triangle
        ///< intersecting with the plane that it is on.
};

}