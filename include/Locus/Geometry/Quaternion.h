 /*********************************************************************************************************\
 *                                                                                                        *
 *   This file is part of the Locus Game Engine                                                           *
 *                                                                                                        *
 *   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
 *                                                                                                        *
 *   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
 *                                                                                                        *
 \*********************************************************************************************************/

#pragma once

#include "LocusGeometryAPI.h"

#include "Transformation.h"

namespace Locus
{

class Vector3;

class LOCUS_GEOMETRY_API Quaternion
{
public:
   Quaternion(float a, float b, float c, float d);
   Quaternion(const Vector3& rotationAxis, float angleRadians);

   float a;
   float b;
   float c;
   float d;

   Quaternion& operator+=(const Quaternion& q);
   Quaternion& operator*=(float k);

   Transformation ToTransformation() const;

   Quaternion Conjugate() const;
   float Norm() const;
   Quaternion Reciprocal() const;
   Quaternion NormalizedQuaternion() const;
   void Normalize();
};

LOCUS_GEOMETRY_API Quaternion operator+(const Quaternion& q1, const Quaternion& q2);
LOCUS_GEOMETRY_API Quaternion operator*(float k, const Quaternion& q);
LOCUS_GEOMETRY_API Quaternion operator*(const Quaternion& q, float k);
LOCUS_GEOMETRY_API Quaternion operator*(const Quaternion& q1, const Quaternion& q2); //Hamilton product
LOCUS_GEOMETRY_API Quaternion operator/(const Quaternion& q, float k);

}