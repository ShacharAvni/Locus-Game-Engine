 /*********************************************************************************************************\
 *                                                                                                        *
 *   This file is part of the Locus Game Engine                                                           *
 *                                                                                                        *
 *   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
 *                                                                                                        *
 *   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
 *                                                                                                        *
 \*********************************************************************************************************/

#include "Locus/Geometry/Quaternion.h"
#include "Locus/Geometry/Vector3.h"

#include <math.h>

namespace Locus
{

Quaternion::Quaternion(float a, float b, float c, float d)
   : a(a), b(b), c(c), d(d)
{
}

Quaternion::Quaternion(const Vector3& rotationAxis, float angleRadians)
{
   Vector3 rotationAxisNormalized = rotationAxis.normVector();

   float halfSine = sin(angleRadians / 2);

   a = cos(angleRadians / 2);
   b = halfSine * rotationAxisNormalized.x;
   c = halfSine * rotationAxisNormalized.y;
   d = halfSine * rotationAxisNormalized.z;
}

Quaternion& Quaternion::operator+=(const Quaternion& q)
{
   a += q.a;
   b += q.b;
   c += q.c;
   d += q.d;

   return *this;
}

Quaternion& Quaternion::operator*=(float k)
{
   a *= k;
   b *= k;
   c *= k;
   d *= k;

   return *this;
}

Transformation Quaternion::ToTransformation() const
{
   Quaternion n = NormalizedQuaternion();

   return Transformation(1.0f - 2.0f*n.c*n.c - 2.0f*n.d*n.d,            2.0f*n.b*n.c - 2.0f*n.d*n.a,            2.0f*n.b*n.d + 2.0f*n.c*n.a,       0.0f,
                                 2.0f*n.b*n.c + 2.0f*n.d*n.a,     1.0f - 2.0f*n.b*n.b - 2.0f*n.d*n.d,            2.0f*n.c*n.d - 2.0f*n.b*n.a,       0.0f,
                                 2.0f*n.b*n.d - 2.0f*n.c*n.a,            2.0f*n.c*n.d + 2.0f*n.b*n.a,     1.0f - 2.0f*n.b*n.b - 2.0f*n.c*n.c,       0.0f,
                                                         0.0f,                                   0.0f,                                   0.0f,       1.0f);
}

Quaternion Quaternion::Conjugate() const
{
   return Quaternion(a, -b, -c, -d);
}

float Quaternion::Norm() const
{
   return sqrt((a * a) + (b * b) + (c * c) + (d * d));
}

Quaternion Quaternion::Reciprocal() const
{
   float n = Norm();

   return Quaternion(Conjugate() / (n * n));
}

Quaternion Quaternion::NormalizedQuaternion() const
{
   float k = 1.0f / Norm();

   return Quaternion(a * k, b * k, c * k, d * k);
}

void Quaternion::Normalize()
{
   operator*=( 1.0f / Norm() );
}

Quaternion operator+(const Quaternion& q1, const Quaternion& q2)
{
   return Quaternion(q1.a + q2.a, q1.b + q2.b, q1.c + q2.c, q1.d + q2.d);
}

Quaternion operator*(float k, const Quaternion& q)
{
   return Quaternion(k * q.a, k * q.b, k * q.c, k * q.d);
}

Quaternion operator*(const Quaternion& q, float k)
{
   return Quaternion(k * q.a, k * q.b, k * q.c, k * q.d);
}

Quaternion operator*(const Quaternion& q1, const Quaternion& q2) //Hamilton product
{
   return Quaternion( (q1.a * q2.a) - (q1.b * q2.b) - (q1.c * q2.c) - (q1.d * q2.d),
                        (q1.a * q2.b) + (q1.b * q2.a) + (q1.c * q2.d) - (q1.d * q2.c),
                        (q1.a * q2.c) - (q1.b * q2.d) + (q1.c * q2.a) + (q1.d * q2.b),
                        (q1.a * q2.d) + (q1.b * q2.c) - (q1.c * q2.b) + (q1.d * q2.a) );
}

Quaternion operator/(const Quaternion& q, float k)
{
   return Quaternion(q.a/k, q.b/k, q.c/k, q.d/k);
}

}