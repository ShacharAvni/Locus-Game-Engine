/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Geometry/Frustum.h"
#include "Locus/Geometry/Vector3Geometry.h"

#include "Locus/Common/Float.h"

namespace Locus
{

Frustum::Frustum(const FVector3& point, const FVector3& forward, const FVector3& up, float horizField, float vertField, float distNear, float distFar)
   : viewPoint(point), forwardVector(forward), upVector(up), horizontalField(horizField), verticalField(vertField), nearDistance(distNear), farDistance(distFar)
{
   FVector3 forwardUnitVector = NormVector(forwardVector);
   FVector3 upUnitVector = NormVector(upVector);
   FVector3 rightUnitVector = NormVector(Cross(forwardVector, upVector));

   planes[Frustum::Near].set(viewPoint + nearDistance*forwardUnitVector, forwardUnitVector);
   planes[Frustum::Far].set(viewPoint + farDistance*forwardUnitVector, -forwardUnitVector);

   //TODO: Fix fudging going on here

   FVector3 pointLeft = viewPoint + nearDistance*forwardUnitVector - rightUnitVector*10.0f;
   FVector3 normalLeft = rightUnitVector;
   RotateAroundDegrees(normalLeft, upUnitVector, horizontalField/2);
   planes[Frustum::Left].set(pointLeft, normalLeft);

   FVector3 pointRight = viewPoint + nearDistance*forwardUnitVector + rightUnitVector*10.0f;
   FVector3 normalRight = -rightUnitVector;
   RotateAroundDegrees(normalRight, upUnitVector, -horizontalField/2);
   planes[Frustum::Right].set(pointRight, normalRight);

   FVector3 pointBottom = viewPoint + nearDistance*forwardUnitVector - upUnitVector*10.0f;
   FVector3 normalBottom = upUnitVector;
   RotateAroundDegrees(normalBottom, rightUnitVector, -verticalField/2);
   planes[Frustum::Bottom].set(pointBottom, normalBottom);

   FVector3 pointTop = viewPoint + nearDistance*forwardUnitVector + upUnitVector*10.0f;
   FVector3 normalTop = -upUnitVector;
   RotateAroundDegrees(normalTop, rightUnitVector, verticalField/2);
   planes[Frustum::Top].set(pointTop, normalTop);
}

bool Frustum::Within(const FVector3& point) const
{
   for (const Plane& plane : planes)
   {
      if (FLess<float>(plane.signedDistanceTo(point), 0.0f))
      {
         //point is on the negative side of one of the planes
         //therefore, it must be outside the frustum
         return false;
      }
   }

   return true;
}

bool Frustum::Within(const FVector3& centerOfSphere, float sphereRadius) const
{
   float radiusSquared = sphereRadius * sphereRadius;

   for (const Plane& plane : planes)
   {
      float signedDistance = plane.signedDistanceTo(centerOfSphere);

      if (FLess<float>(signedDistance, 0.0f))
      {
         //since the Frustum's planes normals are normalized, the squared distance
         //is the same as the squared signed distance
         float squaredDistance = signedDistance * signedDistance;

         if (FGreater<float>(squaredDistance, radiusSquared))
         {
            //the sphere's center is on the negative side of one of the planes
            //and it doesn't intersect that plane. The sphere must be outside
            //the frustum
            return false;
         }
      }
   }

   return true;
}

}