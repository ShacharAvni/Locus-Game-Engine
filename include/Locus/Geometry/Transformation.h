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
#include "Locus/Math/Vectors.h"

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

   static const FVector3& IdentityScale();

   static const Transformation& Identity();

   static Transformation Translation(const FVector3& t);
   static Transformation XRotation(float radians);
   static Transformation YRotation(float radians);
   static Transformation ZRotation(float radians);
   static Transformation Scale(const FVector3& scale);
   static Transformation Perspective(float fovy, float aspect, float zNear, float zFar);
   static Transformation Orthographic(float left, float right, float bottom, float top, float nearVal, float farVal);

   FVector3 MultVector(const FVector3& v) const;
   FVector3 MultVertex(const FVector3& v) const;

   void TranslateBy(const FVector3& t);
   void InverseTranslateBy(const FVector3& t);

   void RotateBy(const FVector3& rotation);
   void InverseRotateBy(const FVector3& rotation);

   void ScaleBy(const FVector3& scale);
   void InverseScaleBy(const FVector3& scale);
};

#include "Locus/Preprocessor/EndSilenceDLLInterfaceWarnings"

}