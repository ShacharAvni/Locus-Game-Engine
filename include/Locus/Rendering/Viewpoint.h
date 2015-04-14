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

#include "Locus/Geometry/DualTransformation.h"
#include "Locus/Geometry/Transformation.h"

namespace Locus
{

class TransformationStack;

class LOCUS_RENDERING_API Viewpoint
{
public:
   Viewpoint();

   const FVector3& GetPosition() const;
   const FVector3& GetRight() const;
   const FVector3& GetUp() const;
   const FVector3& GetForward() const;
   const Transformation& GetRotation() const;
   const DualTransformation& GetTransformation() const;

   FVector3 ToEyePosition(const FVector3& worldPosition) const;

   void TranslateBy(const FVector3& translation);
   void RotateBy(const FVector3& rotation);

   void Activate(TransformationStack& transformationStack) const;
   void Deactivate(TransformationStack& transformationStack) const;

private:
   DualTransformation transformation;
   Transformation rotations;

   FVector3 position;

   FVector3 forward;
   FVector3 up;
   FVector3 right;
};

}