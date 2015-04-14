/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Geometry/Transformation.h"
#include "Locus/Geometry/Geometry.h"

#include "Locus/Common/Float.h"

#include <cmath>
#include <cassert>

namespace Locus
{

Transformation::Transformation()
   : Matrix<float>(4, 4)
{
   SetMainDiagonal(1.0f);
}

Transformation::Transformation(float m0, float m4, float m8,  float m12,
                               float m1, float m5, float m9,  float m13,
                               float m2, float m6, float m10, float m14,
                               float m3, float m7, float m11, float m15)
   : Matrix<float>(4, 4, { m0, m4, m8,  m12,
                           m1, m5, m9,  m13,
                           m2, m6, m10, m14,
                           m3, m7, m11, m15 })
{
}

Transformation& Transformation::operator=(const Matrix<float>& matrix)
{
   assert((matrix.Rows() == 4) && (matrix.Columns() == 4));

   Matrix<float>::operator=(matrix);

   return *this;
}

const FVector3& Transformation::IdentityScale()
{
   static FVector3 identityScale(1.0f, 1.0f, 1.0f);
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

Transformation Transformation::Translation(const FVector3& t)
{
   return Transformation(1,  0,  0,  t.x,
                         0,  1,  0,  t.y,
                         0,  0,  1,  t.z,
                         0,  0,  0,   1 );
}

Transformation Transformation::XRotation(float radians)
{
   return Transformation(1,                   0,                  0,     0,
                         0,   std::cos(radians), -std::sin(radians),     0,
                         0,   std::sin(radians),  std::cos(radians),     0,
                         0,                   0,                  0,     1);
}

Transformation Transformation::YRotation(float radians)
{
   return Transformation(std::cos(radians),    0,  std::sin(radians),   0,
                                         0,    1,                  0,   0,
                        -std::sin(radians),    0,  std::cos(radians),   0,
                                         0,    0,                  0,   1);
}

Transformation Transformation::ZRotation(float radians)
{
   return Transformation(std::cos(radians), -std::sin(radians),   0,   0,
                         std::sin(radians),  std::cos(radians),   0,   0,
                                         0,                  0,   1,   0,
                                         0,                  0,   0,   1);
}

Transformation Transformation::Scale(const FVector3& scale)
{
   //scale vectors can't have zeroes
   FVector3 properScale(FIsZero<float>(scale.x) ? 1.0f : scale.x,
                        FIsZero<float>(scale.y) ? 1.0f : scale.y,
                        FIsZero<float>(scale.z) ? 1.0f : scale.z);

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

FVector3 Transformation::MultVector(const FVector3& v) const
{
   const std::vector<float>& columnMajorValues = GetElements();

   return FVector3(columnMajorValues[0] * v.x + columnMajorValues[4] * v.y + columnMajorValues[8]  * v.z,
                   columnMajorValues[1] * v.x + columnMajorValues[5] * v.y + columnMajorValues[9]  * v.z,
                   columnMajorValues[2] * v.x + columnMajorValues[6] * v.y + columnMajorValues[10] * v.z);
}

FVector3 Transformation::MultVertex(const FVector3& v) const
{
   const std::vector<float>& columnMajorValues = GetElements();

   return FVector3(columnMajorValues[0] * v.x + columnMajorValues[4] * v.y + columnMajorValues[8]  * v.z + columnMajorValues[12],
                   columnMajorValues[1] * v.x + columnMajorValues[5] * v.y + columnMajorValues[9]  * v.z + columnMajorValues[13],
                   columnMajorValues[2] * v.x + columnMajorValues[6] * v.y + columnMajorValues[10] * v.z + columnMajorValues[14]);
}

void Transformation::TranslateBy(const FVector3& t)
{
   MultMatrix( Transformation::Translation(t) );
}

void Transformation::InverseTranslateBy(const FVector3& t)
{
   Transformation translationInverseResult = Transformation::Translation(-t);
   translationInverseResult.MultMatrix(*this);

   *this = translationInverseResult;
}

void Transformation::RotateBy(const FVector3& rotation)
{
   if (FNotZero<float>(rotation.y))
   {
      MultMatrix( Transformation::YRotation(rotation.y) );
   }

   if (FNotZero<float>(rotation.x))
   {
      MultMatrix( Transformation::XRotation(rotation.x) );
   }

   if (FNotZero<float>(rotation.z))
   {
      MultMatrix( Transformation::ZRotation(rotation.z) );
   }
}

void Transformation::InverseRotateBy(const FVector3& rotation)
{
   Transformation rotationInverseResult;

   if (FNotZero<float>(rotation.z))
   {
      rotationInverseResult.MultMatrix( Transformation::ZRotation(-rotation.z) );
   }

   if (FNotZero<float>(rotation.x))
   {
      rotationInverseResult.MultMatrix( Transformation::XRotation(-rotation.x) );
   }

   if (FNotZero<float>(rotation.y))
   {
      rotationInverseResult.MultMatrix( Transformation::YRotation(-rotation.y) );
   }

   rotationInverseResult.MultMatrix(*this);

   *this = rotationInverseResult;
}

void Transformation::ScaleBy(const FVector3& scale)
{
   MultMatrix( Transformation::Scale(scale) );
}

void Transformation::InverseScaleBy(const FVector3& scale)
{
   Transformation scaleInverseResult = Transformation::Scale( FVector3(FIsZero<float>(scale.x) ? 1.0f : 1/scale.x,
                                                                       FIsZero<float>(scale.y) ? 1.0f : 1/scale.y,
                                                                       FIsZero<float>(scale.z) ? 1.0f : 1/scale.z) );

   scaleInverseResult.MultMatrix(*this);

   *this = scaleInverseResult;
}

}