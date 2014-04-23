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

#include "Locus/Geometry/Frustum.h"

#include "Locus/Common/Float.h"

namespace Locus
{

Frustum::Frustum(const Vector3& point, const Vector3& forward, const Vector3& up, float horizField, float vertField, float distNear, float distFar)
   : viewPoint(point), forwardVector(forward), upVector(up), horizontalField(horizField), verticalField(vertField), nearDistance(distNear), farDistance(distFar)
{
   Vector3 forwardUnitVector = forwardVector.normVector();
   Vector3 upUnitVector = upVector.normVector();
   Vector3 rightUnitVector = forwardVector.cross(upVector).normVector();

   planes[Frustum::Near].set(viewPoint + nearDistance*forwardUnitVector, forwardUnitVector);
   planes[Frustum::Far].set(viewPoint + farDistance*forwardUnitVector, -forwardUnitVector);

   Vector3 pointLeft = viewPoint + nearDistance*forwardUnitVector - rightUnitVector*10;
   Vector3 normalLeft = rightUnitVector;
   normalLeft.rotate(horizontalField/2, upUnitVector);
   planes[Frustum::Left].set(pointLeft, normalLeft);

   Vector3 pointRight = viewPoint + nearDistance*forwardUnitVector + rightUnitVector*10;
   Vector3 normalRight = -rightUnitVector;
   normalRight.rotate(-horizontalField/2, upUnitVector);
   planes[Frustum::Right].set(pointRight, normalRight);

   Vector3 pointBottom = viewPoint + nearDistance*forwardUnitVector - upUnitVector*10;
   Vector3 normalBottom = upUnitVector;
   normalBottom.rotate(-verticalField/2, rightUnitVector);
   planes[Frustum::Bottom].set(pointBottom, normalBottom);

   Vector3 pointTop = viewPoint + nearDistance*forwardUnitVector + upUnitVector*10;
   Vector3 normalTop = -upUnitVector;
   normalTop.rotate(verticalField/2, rightUnitVector);
   planes[Frustum::Top].set(pointTop, normalTop);
}

bool Frustum::Within(const Vector3& point) const
{
   for (const Plane& plane : planes)
   {
      if (Float::FLess<float>(plane.signedDistanceTo(point), 0.0f))
      {
         //point is on the negative side of one of the planes
         //therefore, it must be outside the frustum
         return false;
      }
   }

   return true;
}

bool Frustum::Within(const Vector3& centerOfSphere, float sphereRadius) const
{
   float radiusSquared = sphereRadius * sphereRadius;

   for (const Plane& plane : planes)
   {
      float signedDistance = plane.signedDistanceTo(centerOfSphere);

      if (Float::FLess<float>(signedDistance, 0.0f))
      {
         //since the Frustum's planes normals are normalized, the squared distance
         //is the same as the squared signed distance
         float squaredDistance = signedDistance * signedDistance;

         if (Float::Greater<float>(squaredDistance, radiusSquared))
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