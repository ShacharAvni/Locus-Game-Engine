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

#include "Locus/Geometry/Transformation.h"
#include "Locus/Geometry/Vector3.h"
#include "Locus/Geometry/Geometry.h"

#include "Locus/Common/Float.h"

namespace Locus
{

Transformation::Transformation()
   : SquareMatrix<float>(4)
{
}

Transformation::Transformation(float m0, float m4, float m8,  float m12,
                               float m1, float m5, float m9,  float m13,
                               float m2, float m6, float m10, float m14,
                               float m3, float m7, float m11, float m15)
   : SquareMatrix<float>(4, { m0, m4, m8,  m12,
                              m1, m5, m9,  m13,
                              m2, m6, m10, m14,
                              m3, m7, m11, m15 })
{
}

Transformation& Transformation::operator=(const Matrix<float>& matrix)
{
   if ((matrix.NumColumns() == 4) && (matrix.NumRows() == 4))
   {
      values = matrix.GetElements();
   }

   return *this;
}

const Vector3& Transformation::IdentityScale()
{
   static Vector3 identityScale(1.0f, 1.0f, 1.0f);
   return identityScale;
}

const Transformation& Transformation::Identity()
{
   static Transformation identity(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  0, 0, 0, 1);

   return identity;
}

Transformation Transformation::Translation(const Vector3& t)
{
   return Transformation(1,  0,  0,  t.x,
                         0,  1,  0,  t.y,
                         0,  0,  1,  t.z,
                         0,  0,  0,   1 );
}

Transformation Transformation::XRotation(float radians)
{
   return Transformation(1,            0,             0,     0,
                         0, cos(radians), -sin(radians),     0,
                         0, sin(radians),  cos(radians),     0,
                         0,            0,             0,     1);
}

Transformation Transformation::YRotation(float radians)
{
   return Transformation(cos(radians),    0,  sin(radians),   0,
                                    0,    1,             0,   0,
                        -sin(radians),    0,  cos(radians),   0,
                                    0,    0,             0,   1);
}

Transformation Transformation::ZRotation(float radians)
{
   return Transformation(cos(radians), -sin(radians),   0,   0,
                         sin(radians),  cos(radians),   0,   0,
                                    0,             0,   1,   0,
                                    0,             0,   0,   1);
}

Transformation Transformation::Scale(const Vector3& scale)
{
   //scale vectors can't have zeroes
   Vector3 properScale(Float::FIsZero<float>(scale.x) ? 1.0f : scale.x,
                        Float::FIsZero<float>(scale.y) ? 1.0f : scale.y,
                        Float::FIsZero<float>(scale.z) ? 1.0f : scale.z);

   return Transformation(properScale.x,               0,               0,        0,
                                     0,   properScale.y,               0,        0,
                                     0,               0,   properScale.z,        0,
                                     0,               0,               0,        1);
}

Transformation Transformation::Perspective(float fovy, float aspect, float zNear, float zFar)
{
   float yMax = zNear * tan(fovy * TO_RADIANS);
   float yMin = -yMax;
   float xMin = yMin * aspect;
   float xMax = yMax * aspect;

   return Transformation((2 * zNear) / (xMax - xMin),                              0,       (xMax + xMin) / (xMax - xMin) ,                                     0,
                                                   0,    (2 * zNear) / (yMax - yMin),       (yMax + yMin) / (yMax - yMin) ,                                     0,
                                                   0,                              0,     -(zFar + zNear) / (zFar - zNear),  -(2 * zFar * zNear) / (zFar - zNear),
                                                   0,                              0,                                   -1,                                     0);

}

Transformation Transformation::Orthographic(float left, float right, float bottom, float top, float nearVal, float farVal)
{
   return Transformation(2/(right - left),                  0,                       0,           -(right + left) / (right - left),
                                        0,   2/(top - bottom),                       0,           -(top + bottom) / (top - bottom),
                                        0,                  0,   -2/(farVal - nearVal),   -(farVal + nearVal) / (farVal - nearVal),
                                        0,                  0,                       0,                                         1);

}

Vector3 Transformation::MultVector(const Vector3& v) const
{
   return Vector3(values[0] * v.x + values[4] * v.y + values[8]  * v.z,
                   values[1] * v.x + values[5] * v.y + values[9]  * v.z,
                   values[2] * v.x + values[6] * v.y + values[10] * v.z);
}

Vector3 Transformation::MultVertex(const Vector3& v) const
{
   return Vector3(values[0] * v.x + values[4] * v.y + values[8]  * v.z + values[12],
                   values[1] * v.x + values[5] * v.y + values[9]  * v.z + values[13],
                   values[2] * v.x + values[6] * v.y + values[10] * v.z + values[14]);
}

void Transformation::TranslateBy(const Vector3& t)
{
   MultMatrix( Transformation::Translation(t) );
}

void Transformation::InverseTranslateBy(const Vector3& t)
{
   Transformation translationInverseResult = Transformation::Translation(-t);
   translationInverseResult.MultMatrix(*this);

   *this = translationInverseResult;
}

void Transformation::RotateBy(const Vector3& rotation)
{
   if (!Float::FIsZero<float>(rotation.y)) MultMatrix( Transformation::YRotation(rotation.y) );
   if (!Float::FIsZero<float>(rotation.x)) MultMatrix( Transformation::XRotation(rotation.x) );
   if (!Float::FIsZero<float>(rotation.z)) MultMatrix( Transformation::ZRotation(rotation.z) );
}

void Transformation::InverseRotateBy(const Vector3& rotation)
{
   Transformation rotationInverseResult;

   if (!Float::FIsZero<float>(rotation.z)) rotationInverseResult.MultMatrix( Transformation::ZRotation(-rotation.z) );
   if (!Float::FIsZero<float>(rotation.x)) rotationInverseResult.MultMatrix( Transformation::XRotation(-rotation.x) );
   if (!Float::FIsZero<float>(rotation.y)) rotationInverseResult.MultMatrix( Transformation::YRotation(-rotation.y) );

   rotationInverseResult.MultMatrix(*this);

   *this = rotationInverseResult;
}

void Transformation::ScaleBy(const Vector3& scale)
{
   MultMatrix( Transformation::Scale(scale) );
}

void Transformation::InverseScaleBy(const Vector3& scale)
{
   Transformation scaleInverseResult = Transformation::Scale( Vector3(Float::FIsZero<float>(scale.x) ? 1.0f : 1/scale.x,
                                                                       Float::FIsZero<float>(scale.y) ? 1.0f : 1/scale.y,
                                                                       Float::FIsZero<float>(scale.z) ? 1.0f : 1/scale.z) );

   scaleInverseResult.MultMatrix(*this);

   *this = scaleInverseResult;
}

} //namespace Geometry