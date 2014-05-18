/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Geometry/Sphere.h"
#include "Locus/Geometry/Moveable.h"
#include "Locus/Geometry/Geometry.h"

#include "Locus/Common/Util.h"

#include <unordered_set>
#include <algorithm>

#include <math.h>

namespace Locus
{

Sphere::Sphere()
   : radius(0.01f)
{
}

Sphere::Sphere(const Vector3& center, float radius)
   : center(center), radius(radius)
{
}

Sphere::Sphere(const std::vector<Vector3>& points)
   : radius(0.01f)
{
   std::size_t numPoints = points.size();

   if (numPoints > 0)
   {
      center = Vector3::ZeroVector();

      for (const Vector3& singlePoint : points)
      {
         center += singlePoint;
      }

      center /= static_cast<float>(numPoints);

      radius = 0;

      for (const Vector3& singlePoint : points)
      {
         radius = std::max(radius, (singlePoint - center).squaredNorm());
      }

      radius = sqrt(radius);
   }
}

bool Sphere::Intersects(const Moveable& thisMoveable, const Sphere& other, const Moveable& otherMoveable) const
{
   float radiiSum = (radius * thisMoveable.CurrentScale().x) + (other.radius * otherMoveable.CurrentScale().x);

   return (otherMoveable.CurrentModelTransformation().MultVertex(other.center) - thisMoveable.CurrentModelTransformation().MultVertex(center)).squaredNorm() <= (radiiSum * radiiSum);
}

float Sphere::Volume() const
{
   return ( (4.0f / 3.0f) * PI * radius * radius * radius );
}

}