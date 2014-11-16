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

#include "LocusRenderingAPI.h"

#include "Locus/Geometry/Vector3.h"
#include "Locus/Geometry/Transformation.h"

namespace Locus
{

class TransformationStack;

class LOCUS_RENDERING_API ConstrainedViewpoint
{
public:
   ConstrainedViewpoint();

   const Vector3& GetPosition() const;
   const Vector3& GetRight() const;
   const Vector3& GetUp() const;
   const Vector3& GetForward() const;
   const Transformation& GetRotation() const;

   Vector3 ToEyePosition(const Vector3& worldPosition) const;

   void TranslateBy(const Vector3& translation);
   void RotateBy(const Vector3& rotation);

   void Activate(TransformationStack& transformationStack) const;
   void Deactivate(TransformationStack& transformationStack) const;

private:
   Transformation rotations;
   Transformation inverseTransform;

   Vector3 position;

   Vector3 forward;
   Vector3 up;
   Vector3 right;

   float angleAroundRight;
   float angleAroundUp;

   void UpdateTransformation();
};

}