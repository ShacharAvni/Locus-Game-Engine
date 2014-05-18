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

namespace Locus
{

LOCUS_GEOMETRY_API extern const float PI;
LOCUS_GEOMETRY_API extern const float TO_RADIANS;
LOCUS_GEOMETRY_API extern const float TO_DEGREES;

class Sphere;
class Vector3;
struct MotionProperties;

LOCUS_GEOMETRY_API void ResolveCollision(float coefficientOfRestitution, const Sphere& sphere1, const Sphere& sphere2, const Vector3& collisionPoint,
                                         const Vector3& impulseDirection, MotionProperties& motionProperties1, MotionProperties& motionProperties2);
 
}