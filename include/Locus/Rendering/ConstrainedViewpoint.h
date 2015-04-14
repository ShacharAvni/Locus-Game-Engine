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

#include "Locus/Math/Vectors.h"

#include "Locus/Geometry/Transformation.h"

namespace Locus
{

class TransformationStack;

class LOCUS_RENDERING_API ConstrainedViewpoint
{
public:
   ConstrainedViewpoint();

   const FVector3& GetPosition() const;
   const FVector3& GetRight() const;
   const FVector3& GetUp() const;
   const FVector3& GetForward() const;
   const Transformation& GetRotation() const;

   void GetPositionAndOrientation(FVector3& position, float& angleAroundRight, float& angleAroundUp) const;
   void SetPositionAndOrientation(const FVector3& position, float angleAroundRight, float angleAroundUp);

   FVector3 ToEyePosition(const FVector3& worldPosition) const;

   void TranslateBy(const FVector3& translation);
   void RotateBy(const FVector3& rotation);

   void Activate(TransformationStack& transformationStack) const;
   void Deactivate(TransformationStack& transformationStack) const;

private:
   Transformation rotations;
   Transformation inverseTransform;

   FVector3 position;

   FVector3 forward;
   FVector3 up;
   FVector3 right;

   float angleAroundRight;
   float angleAroundUp;

   void UpdateTransformation();
};

}