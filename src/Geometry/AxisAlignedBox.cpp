/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Geometry/AxisAlignedBox.h"
#include "Locus/Geometry/Triangle.h"
#include "Locus/Geometry/Sphere.h"
#include "Locus/Geometry/Moveable.h"

#include "Locus/Common/Float.h"

#include <limits>
#include <algorithm>

#include <cassert>

namespace Locus
{

AxisAlignedBox::AxisAlignedBox()
   : min(-1, -1, -1), max(1, 1, 1) //avoiding malformed box
{
}

AxisAlignedBox::AxisAlignedBox(const FVector3& min, const FVector3& max)
   : min(min), max(max)
{
   assert((min.x <= max.x) && (min.y <= max.y) && (min.z <= max.z));
}

AxisAlignedBox::AxisAlignedBox(const FVector3& center, float diagonalLength)
{
   min = center - (diagonalLength / 2) * Vec3D::DiagonalNormalized();
   max = center + (diagonalLength / 2) * Vec3D::DiagonalNormalized();
}

AxisAlignedBox::AxisAlignedBox(const std::vector<FVector3>& points, bool tight)
{
   if (tight)
   {
      const float reallyBigNumber = std::numeric_limits<float>::max();
      const float reallySmallNumber = -reallyBigNumber;

      min.Set(reallyBigNumber, reallyBigNumber, reallyBigNumber);
      max.Set(reallySmallNumber, reallySmallNumber, reallySmallNumber);

      for (const FVector3& singlePoint : points)
      {
         min.x = std::min(min.x, singlePoint.x);
         min.y = std::min(min.y, singlePoint.y);
         min.z = std::min(min.x, singlePoint.z);

         max.x = std::max(max.x, singlePoint.x);
         max.y = std::max(max.y, singlePoint.y);
         max.z = std::max(max.x, singlePoint.z);
      }
   }
   else
   {
      Sphere sphere(points);

      min = sphere.center - (sphere.radius * Vec3D::Diagonal());
      max = sphere.center + (sphere.radius * Vec3D::Diagonal());
   }
}

bool AxisAlignedBox::Contains(const FVector3& point) const
{
   return ( FGreaterOrEqual<float>(point.x, min.x) && FLessOrEqual<float>(point.x, max.x) &&
            FGreaterOrEqual<float>(point.y, min.y) && FLessOrEqual<float>(point.y, max.y) &&
            FGreaterOrEqual<float>(point.z, min.z) && FLessOrEqual<float>(point.z, max.z) );
}

bool AxisAlignedBox::IsOutside(const Triangle3D_t& triangle) const
{
   FVector3 normal(-1, 0, 0);

   Plane plane(min, normal);
   if (plane.triangleIntersectionTest(triangle) == Plane::IntersectionQuery::Positive)
   {
      return true;
   }

   normal.x = 0;
   normal.y = -1;
   plane.setNormal(normal);
   if (plane.triangleIntersectionTest(triangle) == Plane::IntersectionQuery::Positive)
   {
      return true;
   }

   normal.y = 0;
   normal.z = -1;
   plane.setNormal(normal);
   if (plane.triangleIntersectionTest(triangle) == Plane::IntersectionQuery::Positive)
   {
      return true;
   }

   plane.P = max;

   normal.y = 1;
   normal.z = 0;
   plane.setNormal(normal);
   if (plane.triangleIntersectionTest(triangle) == Plane::IntersectionQuery::Positive)
   {
      return true;
   }

   normal.x = 1;
   normal.y = 0;
   plane.setNormal(normal);
   if (plane.triangleIntersectionTest(triangle) == Plane::IntersectionQuery::Positive)
   {
      return true;
   }

   normal.x = 0;
   normal.z = 1;
   plane.setNormal(normal);
   if (plane.triangleIntersectionTest(triangle) == Plane::IntersectionQuery::Positive)
   {
      return true;
   }

   return false;
}

bool AxisAlignedBox::Intersects(const Triangle3D_t& triangle) const
{
   return !IsOutside(triangle);
}

bool AxisAlignedBox::Intersects(const AxisAlignedBox& box) const
{
   return ( (max.x >= box.min.x) && (min.x <= box.max.x) &&
            (max.y >= box.min.y) && (min.y <= box.max.y) &&
            (max.z >= box.min.z) && (min.z <= box.max.z) );
}

bool AxisAlignedBox::Intersects(const Moveable& thisMoveable, const AxisAlignedBox& other, const Moveable& otherMoveable) const
{
   AxisAlignedBox thisTransformed(*this);
   thisTransformed.TransformBy(thisMoveable);

   AxisAlignedBox otherTransformed(other);
   otherTransformed.TransformBy(otherMoveable);

   return thisTransformed.Intersects(otherTransformed);
}

bool AxisAlignedBox::Intersects(const Sphere& sphere) const
{
   //On Faster Sphere-Box Overlap Testing. J. Graphics Tools 12(1): 3-8 (2007)
   //Thomas Larsson, Tomas Akenine-M�ller, Eric Lengyel

   float d = 0.0f;

   for (unsigned int coordinate = 0; coordinate < 3; ++coordinate)
   {
      float e = sphere.center[coordinate] - min[coordinate];

      if (e < 0.0f)
      {
         if (e < -sphere.radius)
         {
            return false;
         }

         d += e * e;
      }

      e = sphere.center[coordinate] - max[coordinate];

      if (e > 0.0f)
      {
         if (e > sphere.radius)
         {
            return false;
         }

         d += e * e;
      }
   }

   return FLessOrEqual<float>(d, sphere.radius * sphere.radius);
}

void AxisAlignedBox::TransformBy(const Moveable& moveable)
{
   const Transformation& modelTransformation = moveable.CurrentModelTransformation();

   FVector3 centroid = Centroid();

   min -= centroid;
   max -= centroid;

   centroid = modelTransformation.MultVertex(centroid);

   float scale = moveable.CurrentScale().x;

   min *= scale;
   max *= scale;

   min += centroid;
   max += centroid;
}

void AxisAlignedBox::Split(AxisAlignedBox& minBox, AxisAlignedBox& maxBox, unsigned int whichCoordinate) const
{
   minBox = *this;
   maxBox = *this;

   float mid = min[whichCoordinate] + (max[whichCoordinate] - min[whichCoordinate]) / 2;

   minBox.max[whichCoordinate] = mid;
   maxBox.min[whichCoordinate] = mid;
}

void AxisAlignedBox::OctantSplit(std::array<AxisAlignedBox, 8>& octants) const
{
   //Split this box into two halves along X
   //Store halves temporarily

   Split(octants[0], octants[4], 0);

   //Split the two halves computed above into two halves along Y each,
   //storing the results temporarily

   octants[0].Split(octants[1], octants[2], 1);
   octants[4].Split(octants[5], octants[6], 1);

   //Split the quarters computed above such that we have the following
   //configuration:
   //
   //octants[0] == (min half x, min half y, min half z)
   //octants[1] == (min half x, min half y, max half z)
   //octants[2] == (min half x, max half y, min half z)
   //octants[3] == (min half x, max half y, max half z)
   //
   //octants[4] == (max half x, min half y, min half z)
   //octants[5] == (max half x, min half y, max half z)
   //octants[6] == (max half x, max half y, min half z)
   //octants[7] == (max half x, max half y, max half z)

   octants[1].Split(octants[0], octants[1], 2);
   octants[2].Split(octants[2], octants[3], 2);
   octants[5].Split(octants[4], octants[5], 2);
   octants[6].Split(octants[6], octants[7], 2);
}

void AxisAlignedBox::Extents(std::array<float, 3>& extents) const
{
   extents[0] = max.x - min.x;
   extents[1] = max.y - min.y;
   extents[2] = max.z - min.z;
}

float AxisAlignedBox::DiagonalLength() const
{
   return Norm(max - min);
}

FVector3 AxisAlignedBox::Centroid() const
{
   return min + ((max - min) / 2.0f);
}

}