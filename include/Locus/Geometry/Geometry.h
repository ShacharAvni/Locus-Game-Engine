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

#include "Locus/Math/VectorsFwd.h"

namespace Locus
{

LOCUS_GEOMETRY_API extern const float PI;
LOCUS_GEOMETRY_API extern const float TWO_PI;
LOCUS_GEOMETRY_API extern const float HALF_PI;
LOCUS_GEOMETRY_API extern const float TO_RADIANS;
LOCUS_GEOMETRY_API extern const float TO_DEGREES;

class Sphere;
struct MotionProperties;

/*!
 * \brief Determine the new MotionProperties of two colliding rigid
 * body objects given their current MotionProperties.
 *
 * \param[in] coefficientOfRestitution The coefficient of restitution
 * between the two objects.
 *
 * \param[in] sphere1 The bounding sphere of the first object.
 *
 * \param[in] sphere1 The bounding sphere of the second object.
 *
 * \param[in] collisionPoint The point of contact. Contact manifolds are
 * not supported.
 *
 * \param[in] impulseDirection The impulse direction of the collision. This
 * vector should be normalized. This vector should correspond to the impulse
 * direction of the first object.
 *
 * \param[in,out] motionProperties1 The MotionProperties of the first object.
 *
 * \param[in,out] motionProperties2 The MotionProperties of the second object.
 */
LOCUS_GEOMETRY_API void ResolveCollision(float coefficientOfRestitution, const Sphere& sphere1, const Sphere& sphere2, const FVector3& collisionPoint,
                                         const FVector3& impulseDirection, MotionProperties& motionProperties1, MotionProperties& motionProperties2);
 
}