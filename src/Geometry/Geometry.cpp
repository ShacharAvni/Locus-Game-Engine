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

#include "Locus/Geometry/Geometry.h"
#include "Locus/Geometry/MotionProperties.h"
#include "Locus/Geometry/Sphere.h"

#include "Locus/Common/Float.h"

#include "Locus/Math/SquareMatrix.h"

namespace Locus
{

const float PI = 3.141592653589793238f;
const float TO_RADIANS = 0.017453292519943295766666666666667f;
const float TO_DEGREES = 57.295779513082320885235758349777f;

void ResolveCollision(float coefficientOfRestitution,
                        const Sphere& sphere1, const Sphere& sphere2, const Vector3& collisionPoint,
                        const Vector3& impulseDirection, MotionProperties& motionProperties1, MotionProperties& motionProperties2)
{
#define CLAMP_RESOLVED_SPEEDS

   Sphere boundingSpheres[2] = {sphere1, sphere2};

   //static const float_t coefficientOfRestitution = -0.3f; // completely elastic

   //a) compute velocities at the point of contact

   bool hasAngularComponent[2] = { ( Float::FNotZero<float>(motionProperties1.speed) && (motionProperties1.rotation != Vector3::ZeroVector()) ),
                                   ( Float::FNotZero<float>(motionProperties2.speed) && (motionProperties2.rotation != Vector3::ZeroVector()) ) };

   Vector3 contactVectors[2] = { collisionPoint - boundingSpheres[0].center, collisionPoint - boundingSpheres[1].center };

   Vector3 velocities[2] = { motionProperties1.direction * motionProperties1.speed, motionProperties2.direction * motionProperties2.speed };

   Vector3 angularVelocities[2] = { motionProperties1.rotation * motionProperties1.angularSpeed, motionProperties2.rotation * motionProperties2.angularSpeed};

   Vector3 contactVelocities[2] = { velocities[0] + (hasAngularComponent[0] ? angularVelocities[0].cross(contactVectors[0]) : Vector3::ZeroVector()),
                                     velocities[1] + (hasAngularComponent[1] ? angularVelocities[1].cross(contactVectors[1]) : Vector3::ZeroVector()) };

   //b) compute inverses of inertia tensors

   //For now, use spheres as approximations. The inertia tensor for a sphere is:
   //
   //[ (2/5)mr^2       0          0     ]
   //[      0     (2/5)mr^2       0     ]
   //[      0          0     (2/5)mr^2  ]
   //
   //where the inverse is:
   //
   //[ 5 / (2 * mr^2)        0               0         ]
   //[       0         5 / (2 * mr^2)        0         ]
   //[       0               0         5 / (2 * mr^2)  ]

   float masses[2] = { boundingSpheres[0].Volume(), boundingSpheres[1].Volume() }; //assume density of asteroid is 1 Kg per cubic metre

   SquareMatrix<float> inertiaTensorInverses[2] = {SquareMatrix<float>(3), SquareMatrix<float>(3)};

   for (int asteroidIndex = 0; asteroidIndex < 2; ++asteroidIndex)
   {
      float inertiaTensorInverseVal = 5.0f / (2.0f * masses[asteroidIndex] * boundingSpheres[asteroidIndex].radius * boundingSpheres[asteroidIndex].radius);

      inertiaTensorInverses[asteroidIndex](0, 0) = inertiaTensorInverseVal;
      inertiaTensorInverses[asteroidIndex](1, 1) = inertiaTensorInverseVal;
      inertiaTensorInverses[asteroidIndex](2, 2) = inertiaTensorInverseVal;
   }

   //c) determine direction of impulse

   //TODO: improve this

   //Vector3 impulseDirection = -contactVectors[1];
   
   //Vector3 impulseDirection = boundingSpheres[1].center - boundingSpheres[0].center;

   //impulseDirection.normalize();

   //NOTE: For the subsequent steps to work, the impulse direction must be from asteroid1 towards asteroid2

   //d) determine the impulse magnitude

   Vector3 crossA = contactVectors[0].cross(impulseDirection);
   Vector3 multA = inertiaTensorInverses[0] * std::vector<float>{crossA.x, crossA.y, crossA.z};

   Vector3 crossB = contactVectors[1].cross(impulseDirection);
   Vector3 multB = inertiaTensorInverses[1] * std::vector<float>{crossB.x, crossB.y, crossB.z};

   float impulseMagnitudeDenominator = (1.0f / masses[0]) + (1.0f / masses[1]) + ( ( multA.cross(contactVectors[0]) ) +  ( multB.cross(contactVectors[1]) ) ).dot(impulseDirection);

   float impulseMagnitude =  ( (-(1 + coefficientOfRestitution) * (contactVelocities[1] - contactVelocities[0])).dot(impulseDirection) ) / impulseMagnitudeDenominator;

   //e) compute reaction impulse vector

   Vector3 impulseVector = impulseMagnitude * impulseDirection;

   //f) compute new linear velocities

#ifdef CLAMP_RESOLVED_SPEEDS
   float minSpeed = Float::FMin<float>(motionProperties1.speed, motionProperties2.speed);
   float maxSpeed = Float::FMax<float>(motionProperties1.speed, motionProperties2.speed);// + 1.0f;
   //float_t maxSpeed = maxResolvedSpeed;
   float minRotationSpeed = Float::FMin<float>(motionProperties1.angularSpeed, motionProperties2.angularSpeed);
   float maxRotationSpeed = Float::FMax<float>(motionProperties1.angularSpeed, motionProperties2.angularSpeed);// + 0.3f;
#endif

   motionProperties1.direction = velocities[0] - (impulseVector / masses[0]);
   motionProperties1.speed = motionProperties1.direction.norm();

#ifdef CLAMP_RESOLVED_SPEEDS
   if (Float::FGreater<float>(motionProperties1.speed, maxSpeed))
   {
      motionProperties1.speed = maxSpeed;
   }
   else if (Float::FLess<float>(motionProperties1.speed, minSpeed))
   {
      motionProperties1.speed = minSpeed;
   }
#endif

   motionProperties1.direction.normalize();

   motionProperties2.direction = velocities[1] + (impulseVector / masses[1]);
   motionProperties2.speed = motionProperties2.direction.norm();

#ifdef CLAMP_RESOLVED_SPEEDS
   if (Float::FGreater<float>(motionProperties2.speed, maxSpeed))
   {
      motionProperties2.speed = maxSpeed;
   }
   else if (Float::FLess<float>(motionProperties2.speed, minSpeed))
   {
      motionProperties2.speed = minSpeed;
   }
#endif
      
   motionProperties2.direction.normalize();

   //g) compute new angular velocities

   motionProperties1.rotation = angularVelocities[0] - impulseMagnitude * ( multA );
   motionProperties1.angularSpeed = motionProperties1.rotation.norm();
 
#ifdef CLAMP_RESOLVED_SPEEDS
   if (Float::FGreater<float>(motionProperties1.angularSpeed, maxRotationSpeed))
   {
      motionProperties1.angularSpeed = maxRotationSpeed;
   }
   else if (Float::FLess<float>(motionProperties1.angularSpeed, minRotationSpeed))
   {
      motionProperties1.angularSpeed = minRotationSpeed;
   }
#endif

   motionProperties1.rotation.normalize();

   motionProperties2.rotation = angularVelocities[1] + impulseMagnitude * ( multB );
   motionProperties2.angularSpeed = motionProperties2.rotation.norm();

#ifdef CLAMP_RESOLVED_SPEEDS
   if (Float::FGreater<float>(motionProperties2.angularSpeed, maxRotationSpeed))
   {
      motionProperties2.angularSpeed = maxRotationSpeed;
   }
   else if (Float::FLess<float>(motionProperties2.angularSpeed, minRotationSpeed))
   {
      motionProperties2.angularSpeed = minRotationSpeed;
   }
#endif

   motionProperties2.rotation.normalize();
}

}