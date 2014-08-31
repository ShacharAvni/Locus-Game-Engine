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

#include "Locus/Math/Matrix.h"

#include "Vector3.h"

namespace Locus
{

#include "Locus/Preprocessor/BeginSilenceDLLInterfaceWarnings"

class LOCUS_GEOMETRY_API Transformation : public Matrix<float>
{
public:
   Transformation();
   Transformation(float m0, float m4, float m8,  float m12,
                  float m1, float m5, float m9,  float m13,
                  float m2, float m6, float m10, float m14,
                  float m3, float m7, float m11, float m15);

   Transformation& operator=(const Matrix<float>& matrix);

   static const Vector3& IdentityScale();

   static const Transformation& Identity();

   static Transformation Translation(const Vector3& t);
   static Transformation XRotation(float radians);
   static Transformation YRotation(float radians);
   static Transformation ZRotation(float radians);
   static Transformation Scale(const Vector3& scale);
   static Transformation Perspective(float fovy, float aspect, float zNear, float zFar);
   static Transformation Orthographic(float left, float right, float bottom, float top, float nearVal, float farVal);

   Vector3 MultVector(const Vector3& v) const;
   Vector3 MultVertex(const Vector3& v) const;

   void TranslateBy(const Vector3& t);
   void InverseTranslateBy(const Vector3& t);

   void RotateBy(const Vector3& rotation);
   void InverseRotateBy(const Vector3& rotation);

   void ScaleBy(const Vector3& scale);
   void InverseScaleBy(const Vector3& scale);
};

#include "Locus/Preprocessor/EndSilenceDLLInterfaceWarnings"

}