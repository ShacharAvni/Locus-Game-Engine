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

#pragma once

#include "LocusGeometryAPI.h"

#include "Locus/Math/SquareMatrix.h"

#include "Vector3.h"

namespace Locus
{

#include "Locus/Preprocessor/BeginSilenceDLLInterfaceWarnings"

class LOCUS_GEOMETRY_API Transformation : public SquareMatrix<float>
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